/*
 Copyright (c) 2019 Carlos Montiers Aguilera
 Copyright (c) 2019 Jason Hood

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.

 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.

 3. This notice may not be removed or altered from any source distribution.

 Carlos Montiers Aguilera   cmontiers@gmail.com
 Jason Hood                 jadoxa@yahoo.com.au

*/

#include "dll_enhancedbatch.h"
#include <stdarg.h>

static BOOL CALLBACK FindTabWindow(HWND hwnd, LPARAM lParam);
static HWND GetConsoleWindowInTab(void);


// If the string is bigger than the buffer snwprintf fills it and returns -1.
// If the string is the same size as the buffer snwprintf fills it.
// These do not add the terminator.  TODO: Rectify that.
static int my_vsnwprintf(LPWSTR buf, size_t size, LPCWSTR fmt, va_list args)
{
	if (size == 0) {
		return 0;
	}
	if (size == 1) {
		*buf = L'\0';
		return 0;
	}

	size_t len = vsnwprintf(buf, size - 1, fmt, args);

	if (len == -1u) {
		len = size - 1;
	}
	if (len == size - 1) {
		buf[len] = L'\0';
	}

	return (int) len;
}


int wsnprintf(LPWSTR buf, size_t size, LPCWSTR fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	int len = my_vsnwprintf(buf, size, fmt, args);
	va_end(args);

	return len;
}


// Determine the size of our string buffers automatically.
int sbprintf(LPWSTR buf, LPCWSTR fmt, ...)
{
	LPWSTR base;
	va_list args;

	if (buf >= stringBuffer && buf < stringBuffer + STRINGBUFFERMAX) {
		base = stringBuffer;
	} else if (buf >= varBuffer && buf < varBuffer + STRINGBUFFERMAX) {
		base = varBuffer;
	} else if (buf >= sayBuffer && buf < sayBuffer + STRINGBUFFERMAX) {
		base = sayBuffer;
	} else {
		return 0;
	}

	va_start(args, fmt);
	int len = my_vsnwprintf(buf, STRINGBUFFERMAX - (buf - base), fmt, args);
	va_end(args);

	return len;
}


LPSTR readBatchFile(DWORD size, LPSTR buf, DWORD buf_size)
{
	HANDLE hFile;
	LPSTR mem;
	DWORD len;

	if (size > buf_size) {
		mem = malloc(size);
		if (mem == NULL) {
			return NULL;
		}
	} else {
		mem = buf;
	}

	hFile = CreateFile(**pCurrentBatchFile, GENERIC_READ,
					   FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
					   0, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		ReadFile(hFile, mem, size, &len, NULL);
		CloseHandle(hFile);
		if (len == size) {
			return mem;
		}
	}
	if (mem != buf) {
		free(mem);
	}
	return NULL;
}


BOOL SafeCloseHandle(HANDLE handle)
{
	if (handle != NULL && handle != INVALID_HANDLE_VALUE) {
		return CloseHandle(handle);
	} else {
		return FALSE;
	}
}


BOOL haveOutputHandle(void)
{
	// If is not initialized
	if (consoleOutput == NULL) {
		// If this fails nothing will change to make it succeed, so there's no
		// need to keep trying.
		consoleOutput = CreateFile(L"CONOUT$", (GENERIC_READ | GENERIC_WRITE),
			(FILE_SHARE_READ | FILE_SHARE_WRITE), NULL, OPEN_EXISTING, 0, NULL);

		if (consoleOutput != INVALID_HANDLE_VALUE) {
			extern WORD original_attributes;
			extern CONSOLE_CURSOR_INFO original_cci;
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(consoleOutput, &csbi);
			original_attributes = csbi.wAttributes;
			GetConsoleCursorInfo(consoleOutput, &original_cci);
		}
	}

	return consoleOutput != INVALID_HANDLE_VALUE;
}


BOOL haveWindowHandle(void)
{
	if (consoleHwnd == NULL) {
		if (onWindowsTerminal) {
			consoleHwnd = GetConsoleWindowInTab();
		} else {
			consoleHwnd = GetConsoleWindow();
		}
	}
	return consoleHwnd != NULL;
}

// Based on old method for retrieve console window handle:
// https://web.archive.org/web/20070116020857/http://support.microsoft.com/kb/124103
static HWND GetConsoleWindowInTab(void)
{
	#define MY_BUFSIZE 1016
	#define MY_STAMPSIZE 7	// digits required for 2**32 in base 32
	HWND hwndFound;
	WCHAR pszWindowTitle[MY_BUFSIZE + MY_STAMPSIZE + 1];
	DWORD oldlen, newlen, pid;

	newlen = oldlen = GetConsoleTitle(pszWindowTitle, MY_BUFSIZE);
	pid = GetCurrentProcessId();
	// Generate a unique title by converting the PID to base 32,
	// using characters U+0080..U+009F, which are not displayed.
	do {
		pszWindowTitle[newlen++] = (pid % 32) + 0x80;
		pid /= 32;
	} while (pid != 0);
	pszWindowTitle[newlen] = L'\0';
	SetConsoleTitle(pszWindowTitle);
	Sleep(40);	// Ensure window title has been updated.
	hwndFound = FindWindow(NULL, pszWindowTitle);
	pszWindowTitle[oldlen] = L'\0';
	SetConsoleTitle(pszWindowTitle);
	if (hwndFound != NULL) {
		EnumChildWindows(hwndFound, FindTabWindow, (LPARAM) &hwndFound);
	}
	return hwndFound;
}

// Search the Terminal windows for the tab window itself. This lets us make
// CMD transparent, but not Terminal as a whole. It does, unfortunately (or
// fortunately, depending on your point of view), apply to all tabs, though.
static BOOL CALLBACK FindTabWindow(HWND hwnd, LPARAM lParam)
{
	WCHAR buf[MAX_PATH];
	GetClassName(hwnd, buf, MAX_PATH);
	if (WCSEQ(buf, L"Windows.UI.Composition.DesktopWindowContentBridge")) {
		*(HWND *) lParam = hwnd;
		return FALSE;
	}
	return TRUE;
}


void WriteMemory(LPVOID dst, LPCVOID src, int size)
{
	DWORD protect;
	VirtualProtect(dst, size, PAGE_READWRITE, &protect);
	if ((DWORD_PTR) src < 256) {
		*(LPBYTE) dst = (BYTE) (DWORD_PTR) src;
	} else {
		memcpy(dst, src, size);
	}
	VirtualProtect(dst, size, protect, &protect);
}
