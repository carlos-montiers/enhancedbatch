/*
 Copyright (c) 2019 Carlos Montiers Aguilera
 Copyright (c) 2019 Jason Hood

 This software is provided 'as-is', without any express or implied
 warranty.  In no event will the authors be held liable for any damages
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
#include "extensions.h"
#include <stdio.h>
#include <stdlib.h>

static DWORD toString(DWORD num, LPWSTR buffer, DWORD size)
{
	return snwprintf(buffer, size, L"%d", num);
}

static void toNumber(int *num, int argc, LPCWSTR argv[])
{
	while (argc-- > 0) {
		*num++ = (int) wcstol(*argv++, NULL, 10);
	}
}

DWORD Getch(LPWSTR buffer, DWORD size)
{
	int code;

	while (!(code = _getwch()) || (0xE0 == code)) {
		_getwch();
	}

	*buffer = code;
	buffer[1] = L'\0';
	return 1;
}

DWORD Chhit(LPWSTR buffer, DWORD size)
{
	int code;

	if (_kbhit()) {
		if (!(code = _getwch()) || (0xE0 == code)) {
			_getwch();
			return toString(-1, buffer, size);
		}
		*buffer = code;
		buffer[1] = L'\0';
		return 1;
	} else {
		return toString(-1, buffer, size);
	}
}

DWORD Getkb(LPWSTR buffer, DWORD size)
{
	int code;

	code = _getwch();
	if ((!code) || (0xE0 == code)) {
		code = _getwch();
		code = -code;
	}

	return toString(code, buffer, size);
}

DWORD Kbhit(LPWSTR buffer, DWORD size)
{
	if (_kbhit()) {
		return Getkb(buffer, size);
	} else {
		return toString(-1, buffer, size);
	}
}

BOOL setPosition(int row, int column)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD screen_max;
	COORD coord;
	SHORT x, y;

	if (!getOutputHandle()) {
		return FALSE;
	}

	GetConsoleScreenBufferInfo(consoleOutput, &csbi);
	screen_max.X = csbi.srWindow.Right - csbi.srWindow.Left;
	screen_max.Y = csbi.srWindow.Bottom - csbi.srWindow.Top;

	x = column;
	y = row;

	if (x < 0) {
		x = 0;
	}

	if (y < 0) {
		y = 0;
	}

	if (x > screen_max.X) {
		x = screen_max.X;
	}

	if (y > screen_max.Y) {
		y = screen_max.Y;
	}

	coord.X = x + csbi.srWindow.Left;
	coord.Y = y + csbi.srWindow.Top;

	return SetConsoleCursorPosition(consoleOutput, coord);
}

BOOL SetPosition(int argc, LPCWSTR argv[])
{
	int num[2];

	if (argc != 2) {
		return FALSE;
	}

	toNumber(num, 2, argv);
	return setPosition(num[0], num[1]);
}

COORD getPosition(void)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (getOutputHandle()) {
		GetConsoleScreenBufferInfo(consoleOutput, &csbi);
		csbi.dwCursorPosition.X -= csbi.srWindow.Left;
		csbi.dwCursorPosition.Y -= csbi.srWindow.Top;
	} else {
		csbi.dwCursorPosition.X = csbi.dwCursorPosition.Y = -1;
	}

	return csbi.dwCursorPosition;
}

DWORD GetPosition(LPWSTR buffer, DWORD size)
{
	COORD position;

	position = getPosition();
	return snwprintf(buffer, size, L"%d %d", position.Y, position.X);
}

BOOL SetRow(int argc, LPCWSTR argv[])
{
	COORD cur;
	int row;

	if (argc != 1) {
		return FALSE;
	}

	toNumber(&row, 1, argv);
	cur = getPosition();
	return setPosition(row, cur.X);
}

DWORD GetRow(LPWSTR buffer, DWORD size)
{
	return toString(getPosition().Y, buffer, size);
}

BOOL SetColumn(int argc, LPCWSTR argv[])
{
	COORD cur;
	int column;

	if (argc != 1) {
		return FALSE;
	}

	toNumber(&column, 1, argv);
	cur = getPosition();
	return setPosition(cur.Y, column);
}

DWORD GetColumn(LPWSTR buffer, DWORD size)
{
	return toString(getPosition().X, buffer, size);
}

COORD getSize(void)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD size;

	if (getOutputHandle()) {
		GetConsoleScreenBufferInfo(consoleOutput, &csbi);
		size.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		size.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	} else {
		csbi.dwCursorPosition.X = csbi.dwCursorPosition.Y = -1;
	}

	return size;
}

DWORD GetSize(LPWSTR buffer, DWORD size)
{
	COORD csize;

	csize = getSize();
	return snwprintf(buffer, size, L"%d %d", csize.Y, csize.X);
}

DWORD GetHeight(LPWSTR buffer, DWORD size)
{
	return toString(getSize().Y, buffer, size);
}

DWORD GetWidth(LPWSTR buffer, DWORD size)
{
	return toString(getSize().X, buffer, size);
}

BOOL SetColor(int argc, LPCWSTR argv[])
{
	WORD value;

	if (argc != 1 || !getOutputHandle()) {
		return FALSE;
	}

	value = (WORD) wcstol(argv[0], NULL, 16);

	return SetConsoleTextAttribute(consoleOutput, value);
}

DWORD GetColor(LPWSTR buffer, DWORD size)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!getOutputHandle()) {
		return toString(-1, buffer, size);
	}

	GetConsoleScreenBufferInfo(consoleOutput, &csbi);

	return snwprintf(buffer, size, L"%X", csbi.wAttributes);
}

BOOL WaitMilliseconds(int argc, LPCWSTR argv[])
{
	int milliseconds;

	if (argc != 1) {
		return FALSE;
	}

	toNumber(&milliseconds, 1, argv);
	if (milliseconds < 0) {
		return FALSE;
	}

	Sleep(milliseconds);

	return TRUE;
}

static DWORD lo_timer_begin, lo_timer_end;
static LARGE_INTEGER hi_timer_begin, hi_timer_end, hi_frequency;
static BOOL lo_timer_running, hi_timer_running;
static BOOL lo_timer_started, hi_timer_started;

BOOL SetLoTimer(int argc, LPCWSTR argv[])
{
	if (argc > 1) {
		return FALSE;
	}

	if (argc == 0 || _wcsicmp(*argv, L"stop") == 0) {
		if (lo_timer_running) {
			lo_timer_end = GetTickCount();
			lo_timer_running = FALSE;
			return TRUE;
		}
	} else if (_wcsicmp(*argv, L"start" ) == 0) {
		lo_timer_running = lo_timer_started = TRUE;
		lo_timer_begin = GetTickCount();
		return TRUE;
	}

	return FALSE;
}

BOOL SetHiTimer(int argc, LPCWSTR argv[])
{
	if (argc > 1) {
		return FALSE;
	}

	if (argc == 0 || _wcsicmp(*argv, L"stop") == 0) {
		if (hi_timer_running) {
			QueryPerformanceCounter(&hi_timer_end);
			hi_timer_running = FALSE;
			return TRUE;
		}
	} else if (_wcsicmp(*argv, L"start" ) == 0) {
		if (hi_frequency.QuadPart == -1) {
			return FALSE;
		}
		if (!hi_frequency.QuadPart) {
			if (!QueryPerformanceFrequency(&hi_frequency)) {
				hi_frequency.QuadPart = -1;
				return FALSE;
			}
		}
		hi_timer_running = hi_timer_started = TRUE;
		QueryPerformanceCounter(&hi_timer_begin);
		return TRUE;
	}

	return FALSE;
}

DWORD GetTimer(LPWSTR buffer, DWORD size)
{
	if (lo_timer_started) {
		DWORD end = lo_timer_running ? GetTickCount() : lo_timer_end;
		return toString(end - lo_timer_begin, buffer, size);
	}
	return toString(-1, buffer, size);
}

DWORD GetHiTimer(LPWSTR buffer, DWORD size)
{
	if (hi_timer_started) {
		LARGE_INTEGER end;
		if (hi_timer_running) {
			QueryPerformanceCounter(&end);
		} else {
			end = hi_timer_end;
		}
		return toString((int)((end.QuadPart - hi_timer_begin.QuadPart)
							  * 1000000 / hi_frequency.QuadPart),
						buffer, size);
	}
	return toString(-1, buffer, size);
}

// Search the Terminal windows for the tab window itself.  This lets us make
// CMD transparent, but not Terminal as a whole.  It does, unfortunately (or
// fortunately, depending on your point of view), apply to all tabs, though.
BOOL CALLBACK FindTabWindow(HWND hwnd, LPARAM lParam)
{
	WCHAR buf[MAX_PATH];
	GetClassName(hwnd, buf, MAX_PATH);
	if (wcscmp(buf, L"Windows.UI.Composition.DesktopWindowContentBridge") == 0) {
		*(HWND *) lParam = hwnd;
		return FALSE;
	}
	return TRUE;
}

// Based on old method for retrieve console window handle:
// https://web.archive.org/web/20070116020857/http://support.microsoft.com/kb/124103
HWND GetConsoleWindowInTab(void)
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
	pszWindowTitle[newlen++] = L'\0';
	SetConsoleTitle(pszWindowTitle);
	Sleep(40);	// Ensure window title has been updated.
	hwndFound = FindWindow(NULL, pszWindowTitle);
	pszWindowTitle[oldlen++] = L'\0';
	SetConsoleTitle(pszWindowTitle);
	if (hwndFound) {
		EnumChildWindows(hwndFound, FindTabWindow, (LPARAM) &hwndFound);
	}
	return hwndFound;
}

HWND GetConsoleHwnd(void)
{
	if (!consoleHwnd) {
		if (onWindowsTerminal) {
			consoleHwnd = GetConsoleWindowInTab();
		} else {
			consoleHwnd = GetConsoleWindow();
		}
	}
	return consoleHwnd;
}

BOOL SetOpacity(int argc, LPCWSTR argv[])
{
	int pc;
	BYTE alpha;
	HANDLE hwnd;
	LONG_PTR exstyle;

	if (argc != 1) {
		return FALSE;
	}

	hwnd = GetConsoleHwnd();
	if (!hwnd) {
		return FALSE;
	}

	toNumber(&pc, 1, argv);
	if (pc <= MIN_OPACITY_PERCENT) {
		alpha = MIN_OPACITY_ALPHA;
	} else if (pc >= MAX_OPACITY_PERCENT) {
		alpha = MAX_OPACITY_ALPHA;
	} else {
		alpha = (MAX_OPACITY_ALPHA * pc + MAX_OPACITY_PERCENT - 1)
				/ MAX_OPACITY_PERCENT;
	}

	if (LOBYTE(GetVersion()) >= 10) { // is Windows 10 or greater
		// Simulate wheel movements to keep the properties dialog in sync
		// (within 4.7%, anyway).
		BYTE curr_alpha;
		int wheel_movements;
		if (!GetLayeredWindowAttributes(hwnd, NULL, &curr_alpha, NULL)) {
			curr_alpha = MAX_OPACITY_ALPHA;
		}
		wheel_movements = alpha - curr_alpha;
		wheel_movements /= OPACITY_DELTA_INTERVAL;
		if (wheel_movements) {
			PostMessage(
				hwnd,
				WM_MOUSEWHEEL,
				MAKELONG(MK_CONTROL | MK_SHIFT, WHEEL_DELTA * wheel_movements),
				0
			);
		}
	}

	exstyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
	if (!(exstyle & WS_EX_LAYERED)) {
		exstyle |= WS_EX_LAYERED;
		SetWindowLongPtr(hwnd, GWL_EXSTYLE, exstyle);
	}

	return SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
}

DWORD GetOpacity(LPWSTR buffer, DWORD size)
{
	HANDLE hwnd = GetConsoleHwnd();
	DWORD pc = MAX_OPACITY_PERCENT;
	BYTE alpha;

	if (hwnd && GetLayeredWindowAttributes(hwnd, NULL, &alpha, NULL)) {
		pc = MAX_OPACITY_PERCENT * alpha / MAX_OPACITY_ALPHA;
	}

	return toString(pc, buffer, size);
}

BOOL SetConsoleCursor(int argc, LPCWSTR argv[])
{
	CONSOLE_CURSOR_INFO cci;
	int iValue;

	if (argc != 1 || !getOutputHandle()) {
		return FALSE;
	}

	GetConsoleCursorInfo(consoleOutput, &cci);

	toNumber(&iValue, 1, argv);
	if (iValue < 0) {
		iValue = 0;
	} else if (iValue > 100) {
		iValue = 100;
	}
	if (iValue == 0 || iValue == 1) {
		cci.bVisible = iValue;
	} else if (iValue >= 2 && iValue <= 100) {
		cci.dwSize = iValue;
		cci.bVisible = TRUE;
	}

	return SetConsoleCursorInfo(consoleOutput, &cci);
}

DWORD GetConsoleCursor(LPWSTR buffer, DWORD size)
{
	CONSOLE_CURSOR_INFO cci;

	if (!getOutputHandle()) {
		return toString(-1, buffer, size);
	}

	GetConsoleCursorInfo(consoleOutput, &cci);

	if (!cci.bVisible) {
		return toString(0, buffer, size);
	} else {
		return toString(cci.dwSize, buffer, size);
	}
}

DWORD GetOutputCodePage(LPWSTR buffer, DWORD size)
{
	return toString(GetConsoleOutputCP(), buffer, size);
}

DWORD GetInputCodePage(LPWSTR buffer, DWORD size)
{
	return toString(GetConsoleCP(), buffer, size);
}

int cpFromCommandLine(int argc, LPCWSTR argv[])
{
	int cp;

	if (argc == 0 || _wcsicmp(*argv, L"oem") == 0) {
		cp = GetOEMCP();
	} else if (_wcsicmp(*argv, L"utf-8") == 0 ||
			   _wcsicmp(*argv, L"utf8") == 0 ||
			   _wcsicmp(*argv, L"utf_8") == 0) {
		cp = CP_UTF8;
	} else if (_wcsicmp(*argv, L"ansi") == 0) {
		cp = GetACP();
	} else {
		toNumber(&cp, 1, argv);
	}

	return cp;
}

BOOL SetOutputCodePage(int argc, LPCWSTR argv[])
{
	int cp;

	if (argc > 1) {
		return FALSE;
	}

	cp = cpFromCommandLine(argc, argv);

	return SetConsoleOutputCP(cp);
}

BOOL SetInputCodePage(int argc, LPCWSTR argv[])
{
	int cp;

	if (argc > 1) {
		return FALSE;
	}

	cp = cpFromCommandLine(argc, argv);

	return SetConsoleCP(cp);
}

BOOL SetCodePage(int argc, LPCWSTR argv[])
{
	int cp;

	if (argc > 1) {
		return FALSE;
	}

	cp = cpFromCommandLine(argc, argv);

	if (!SetConsoleOutputCP(cp)) {
		return FALSE;
	}
	return SetConsoleCP(cp);
}

DWORD GetArgCount(LPWSTR buffer, DWORD size)
{
	LPWSTR *argv, rest;
	DWORD argc;

	if (!*pCurrentBatchFile) {
		return toString(-1, buffer, size);
	}

	argv = *pCurrentBatchFile;
#ifdef _WIN64
	if (cmdFileVersionMS >= 0x60000) {
		argv += 10;
	} else {
		argv += 6;
	}
#else
	if (cmdFileVersionMS >= 0x60000) {
		argv += 15;
	} else if (cmdFileVersionMS > 0x50000) {
		argv += 8;
	} else {
		argv += 7;
	}
#endif
	rest = argv[-1];
	if (rest && *rest) {
		argc = 10;
		for (;; ++rest) {
			if (!*rest) {
				++argc;
				if (!rest[1]) {
					break;
				}
			}
		}
	}
	else {
		argc = 0;
		while (*argv) {
			if (++argc == 10) {
				break;
			}
			++argv;
		}
	}
	return toString(argc - 1, buffer, size);
}

DWORD GetArgs(DWORD first, DWORD last, LPWSTR buffer, DWORD size)
{
	LPWSTR *argv, rest;
	DWORD argc, *arglen;

	if (!*pCurrentBatchFile) {
		return 0;
	}

	argv = *pCurrentBatchFile;
#ifdef _WIN64
	if (cmdFileVersionMS >= 0x60000) {
		argv += 10;
	} else {
		argv += 6;
	}
#else
	if (cmdFileVersionMS >= 0x60000) {
		argv += 15;
	} else if (cmdFileVersionMS > 0x50000) {
		argv += 8;
	} else {
		argv += 7;
	}
#endif
	arglen = (DWORD *) (argv + 10);
	rest = argv[-1];

	// I'll take a shortcut here: the supplied buffer is 32Ki, which is as big
	// as the command line can be, so no need to test size.
	size = 0;
	argc = 0;
	while (*argv) {
		if (argc >= first) {
			if (argc > first) {
				*buffer++ = L' ';
				++size;
			}
			memcpy(buffer, *argv, *arglen * 2);
			buffer += *arglen;
			size += *arglen;
		}
		if (argc == last) {
			*buffer = L'\0';
			return size;
		}
		if (++argc == 10) {
			if (rest && *rest) {
				if (argc > first) {
					*buffer++ = L' ';
					++size;
				}
				for (;; ++rest) {
					if (*rest) {
						if (argc >= first) {
							*buffer++ = *rest;
							++size;
						}
					} else {
						if (argc++ == last || !rest[1]) {
							*buffer = L'\0';
							return size;
						}
						if (argc > first) {
							*buffer++ = L' ';
							++size;
						}
					}
				}
			}
			break;
		}
		++arglen;
		++argv;
	}
	*buffer = L'\0';
	return size;
}

static BOOL setBoolean(LPBYTE var, LPCWSTR arg)
{
	if (arg == NULL
		|| (arg[0] == L'0' && arg[1] == L'\0')
		|| _wcsicmp(arg, L"off") == 0
		|| _wcsicmp(arg, L"false") == 0) {
		*var = FALSE;
		return TRUE;
	}
	if ((arg[0] == L'1' && arg[1] == L'\0')
		|| _wcsicmp(arg, L"on") == 0
		|| _wcsicmp(arg, L"true") == 0) {
		*var = TRUE;
		return TRUE;
	}
	return FALSE;
}

BOOL SetEcho(int argc, LPCWSTR argv[])
{
	return setBoolean(pEchoFlag, *argv);
}

DWORD GetEcho(LPWSTR buffer, DWORD size)
{
	return toString(*pEchoFlag, buffer, size);
}

BOOL SetUnicode(int argc, LPCWSTR argv[])
{
	return setBoolean(pfOutputUnicode, *argv);
}

DWORD GetUnicode(LPWSTR buffer, DWORD size)
{
	return toString(*pfOutputUnicode, buffer, size);
}

BOOL SetDelayedExpansion(int argc, LPCWSTR argv[])
{
	return setBoolean(pfDelayedExpansion, *argv);
}

DWORD GetDelayedExpansion(LPWSTR buffer, DWORD size)
{
	return toString(*pfDelayedExpansion, buffer, size);
}

BOOL SetExtensions(int argc, LPCWSTR argv[])
{
	return setBoolean(pfEnableExtensions, *argv);
}

DWORD GetExtensions(LPWSTR buffer, DWORD size)
{
	return toString(*pfEnableExtensions, buffer, size);
}

DWORD GetTransient(LPWSTR buffer, DWORD size)
{
	static int transient = -1;

	if (transient == -1) {
		LPWSTR cmd;
		for (cmd = GetCommandLine();; ++cmd) {
			cmd = wcschr(cmd, L'/');
			if (cmd == NULL || cmd[1] == L'k' || cmd[1] == L'K') {
				transient = 0;
				break;
			}
			if (cmd[1] == L'c' || cmd[1] == L'C') {
				transient = 1;
				break;
			}
		}
	}

	return toString(transient, buffer, size);
}

DWORD getStd(DWORD handle, LPWSTR buffer, DWORD size)
{
	DWORD mode;
	return toString(GetConsoleMode(GetStdHandle(handle), &mode), buffer, size);
}

DWORD GetStdin(LPWSTR buffer, DWORD size)
{
	return getStd(STD_INPUT_HANDLE, buffer, size);
}

DWORD GetStdout(LPWSTR buffer, DWORD size)
{
	return getStd(STD_OUTPUT_HANDLE, buffer, size);
}

DWORD GetStderr(LPWSTR buffer, DWORD size)
{
	return getStd(STD_ERROR_HANDLE, buffer, size);
}

DWORD GetUnique(LPWSTR buffer, DWORD size)
{
	*buffer = L'\0';
	GetTempFileName(L".", L"eb-", 0, buffer);
	return wcslen(buffer);
}

DWORD GetTempFile(LPWSTR buffer, DWORD size)
{
	WCHAR temp[MAX_PATH];

	GetTempPath(MAX_PATH, temp);
	*buffer = L'\0';
	GetTempFileName(temp, L"eb-", 0, buffer);
	return wcslen(buffer);
}

DWORD GetTempDir(LPWSTR buffer, DWORD size)
{
	size = GetTempFile(buffer, size);
	if (size) {
		if (!DeleteFile(buffer) || !CreateDirectory(buffer, NULL)) {
			size = 0;
		}
	}
	return size;
}

static SYSTEMTIME st;
static DWORD time_retrieved;
static WCHAR point;
static int	 english;
static const LPCWSTR MonthNames[] = {
	L"January", L"February", L"March", L"April", L"May", L"June",
	L"July", L"August", L"September", L"October", L"November", L"December"
};
static const LPCWSTR DayNames[] = {
	L"Sunday", L"Monday", L"Tuesday", L"Wednesday",
	L"Thursday", L"Friay", L"Saturday"
};

WCHAR getPoint(void)
{
	if (!point) {
		WCHAR buf[5];
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, buf, 5);
		point = *buf;
	}
	return point;
}

void getDate(void)
{
	DWORD ticks = GetTickCount();
	if (ticks - time_retrieved >= 1000) {
		GetLocalTime(&st);
		time_retrieved = ticks;
	}
}

DWORD GetDate(LPWSTR buffer, DWORD size)
{
	getDate();
	return snwprintf(buffer, size, L"%d-%02d-%02d",
					 st.wYear, st.wMonth, st.wDay);
}

DWORD GetTime(LPWSTR buffer, DWORD size)
{
	getDate();
	return snwprintf(buffer, size, L"%02d:%02d:%02d",
					 st.wHour, st.wMinute, st.wSecond);
}

DWORD GetTimems(LPWSTR buffer, DWORD size)
{
	DWORD len = GetTime(buffer, size);
	return len + snwprintf(buffer + len, size - len, L"%c%03d",
						   getPoint(), st.wMilliseconds);
}

DWORD getDateTime(fnGetExt tfn, LPWSTR buffer, DWORD size)
{
	DWORD len = GetDate(buffer, size);
	buffer[len++] = L' ';
	return len + tfn(buffer + len, size - len);
}

DWORD GetDateTime(LPWSTR buffer, DWORD size)
{
	return getDateTime(GetTime, buffer, size);
}

DWORD GetDateTimems(LPWSTR buffer, DWORD size)
{
	return getDateTime(GetTimems, buffer, size);
}

DWORD GetYear(LPWSTR buffer, DWORD size)
{
	getDate();
	return toString(st.wYear, buffer, size);
}

DWORD GetMonth(LPWSTR buffer, DWORD size)
{
	getDate();
	return toString(st.wMonth, buffer, size);
}

DWORD GetMonthName(LPWSTR buffer, DWORD size)
{
	getDate();
	if (english) {
		return snwprintf(buffer, size, L"%s", MonthNames[st.wMonth-1]);
	}
	return GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME1-1+st.wMonth,
						 buffer, size);
}

DWORD GetMonthShort(LPWSTR buffer, DWORD size)
{
	getDate();
	if (english) {
		return snwprintf(buffer, size, L"%.3s", MonthNames[st.wMonth-1]);
	}
	return GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME1-1+st.wMonth,
						 buffer, size);
}

DWORD GetDay(LPWSTR buffer, DWORD size)
{
	getDate();
	return toString(st.wDay, buffer, size);
}

DWORD GetDayName(LPWSTR buffer, DWORD size)
{
	int day;
	getDate();
	if (english) {
		return snwprintf(buffer, size, L"%s", DayNames[st.wDayOfWeek]);
	}
	day = (st.wDayOfWeek == 0) ? 7 : st.wDayOfWeek;
	return GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDAYNAME1-1+day,
						 buffer, size);
}

DWORD GetDayShort(LPWSTR buffer, DWORD size)
{
	int day;
	getDate();
	if (english) {
		return snwprintf(buffer, size, L"%.3s", DayNames[st.wDayOfWeek]);
	}
	day = (st.wDayOfWeek == 0) ? 7 : st.wDayOfWeek;
	return GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVDAYNAME1-1+day,
						 buffer, size);
}

DWORD GetDayOfWeek(LPWSTR buffer, DWORD size)
{
	getDate();
	return toString(st.wDayOfWeek, buffer, size);
}

DWORD GetHour(LPWSTR buffer, DWORD size)
{
	getDate();
	return toString(st.wHour, buffer, size);
}

DWORD GetMinute(LPWSTR buffer, DWORD size)
{
	getDate();
	return toString(st.wMinute, buffer, size);
}

DWORD GetSecond(LPWSTR buffer, DWORD size)
{
	getDate();
	return toString(st.wSecond, buffer, size);
}

DWORD GetMilliseconds(LPWSTR buffer, DWORD size)
{
	getDate();
	return toString(st.wMilliseconds, buffer, size);
}

DWORD GetEnglish(LPWSTR buffer, DWORD size)
{
	return toString(english, buffer, size);
}

BOOL SetEnglish(int argc, LPCWSTR argv[])
{
	if (argc != 1) {
		return FALSE;
	}
	toNumber(&english, 1, argv);
	return TRUE;
}

DWORD GetDecSep(LPWSTR buffer, DWORD size)
{
	*buffer = getPoint();
	buffer[1] = L'\0';
	return 1;
}

BOOL SetDecSep(int argc, LPCWSTR argv[])
{
	if (argc == 0) {
		point = L'\0';
	} else {
		point = **argv;
	}
	return TRUE;
}

DWORD GetBatchLine(LPWSTR buffer, DWORD size)
{
	return toString(getBatchLine(), buffer, size);
}

DWORD GetBatchFile(LPWSTR buffer, DWORD size)
{
	return toString(batchfile, buffer, size);
}

BOOL SetBatchFile(int argc, LPCWSTR argv[])
{
	if (argc != 1) {
		return FALSE;
	}
	toNumber(&batchfile, 1, argv);
	return TRUE;
}

DWORD getVersionRevision(void)
{
	HKEY curver;
	DWORD revision = 0;
	LONG rc;
	rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					  L"Software\\Microsoft\\Windows NT\\CurrentVersion",
					  0, MAXIMUM_ALLOWED | KEY_WOW64_64KEY, &curver);
#ifndef _WIN64
	if (rc == ERROR_ACCESS_DENIED) {
		rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
						  L"Software\\Microsoft\\Windows NT\\CurrentVersion",
						  0, MAXIMUM_ALLOWED, &curver);
		revision = 1;
	}
#endif
	if (rc == ERROR_SUCCESS) {
		DWORD type, rev, size = sizeof(rev);
		if (ERROR_SUCCESS == RegQueryValueEx(curver, L"UBR", NULL, &type,
											 (LPBYTE) &rev, &size)
			&& REG_DWORD == type) {
			revision = rev;
		}
		RegCloseKey(curver);
	}
	return revision;
}

DWORD GetEBVersion(LPWSTR buffer, DWORD size)
{
	return snwprintf(buffer, size, L"%S", __DATE__);
}

DWORD GetEnhancedBatch(LPWSTR buffer, DWORD size)
{
	return toString(EBVERSION, buffer, size);
}

DWORD GetOSVersion(LPWSTR buffer, DWORD size)
{
	DWORD ver = GetVersion();
	return snwprintf(buffer, size, L"%d.%d.%d.%d",
					 LOBYTE(LOWORD(ver)), HIBYTE(LOWORD(ver)), HIWORD(ver),
					 getVersionRevision());
}

DWORD GetOSMajor(LPWSTR buffer, DWORD size)
{
	return toString(LOBYTE(LOWORD(GetVersion())), buffer, size);
}

DWORD GetOSMinor(LPWSTR buffer, DWORD size)
{
	return toString(HIBYTE(LOWORD(GetVersion())), buffer, size);
}

DWORD GetOSBuild(LPWSTR buffer, DWORD size)
{
	return toString(HIWORD(GetVersion()), buffer, size);
}

DWORD GetOSRevision(LPWSTR buffer, DWORD size)
{
	return toString(getVersionRevision(), buffer, size);
}

DWORD GetCmdVersion(LPWSTR buffer, DWORD size)
{
	return snwprintf(buffer, size, L"%d.%d.%d.%d%s",
					 HIWORD(cmdFileVersionMS), LOWORD(cmdFileVersionMS),
					 HIWORD(cmdFileVersionLS), LOWORD(cmdFileVersionLS),
					 cmdDebug ? L" [debug]" : L"");
}

DWORD GetTitle(LPWSTR buffer, DWORD size)
{
	WCHAR admin[128];
	DWORD tsize, asize;

	for (;;) {
		tsize = GetConsoleTitle(buffer, size);
		if (tsize) {
			break;
		}
		// This happened running an XP virtual machine.
		if (GetLastError() == ERROR_NOT_ENOUGH_MEMORY) {
			size /= 2;
		} else if (GetLastError() == ERROR_SUCCESS) {
			size += size / 2;
		} else {
			break;
		}
	}

	// Strip off "Administrator: " and an extra space.
	asize = FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE,
						  NULL, 0x40002748, 0, admin, lenof(admin), NULL);
	if (asize) {
		if (wcsncmp(buffer, admin, asize) == 0) {
			if (buffer[asize] == L' ') {
				++asize;
			}
			wcscpy(buffer, buffer+asize);
			tsize -= asize;
		}
	}

	return tsize;
}


DWORD GetElevated(LPWSTR buffer, DWORD size)
{
	static int isElevated = -1;

	if (isElevated == -1) {
		HKEY hKey;
		LONG rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software", 0, KEY_WRITE, &hKey);
		isElevated = (rc == ERROR_SUCCESS) ? 1 : 0;
		if (isElevated) {
			RegCloseKey(hKey);
		}
	}
	return toString(isElevated, buffer, size);
}

DWORD GetRun(LPCWSTR cmd, LPWSTR buffer, DWORD size)
{
	FILE *pipe;
	DWORD len, r;
	char *buf, *pos;

	pipe = _wpopen(cmd, L"rb");
	if (pipe == NULL) {
		return 0;
	}

	buf = malloc(size);
	if (buf == NULL) {
		_pclose(pipe);
		return 0;
	}

	pos = buf;
	len = size - 1;
	while (len > 0) {
		r = fread(pos, 1, len, pipe);
		if (r == 0) {
			break;
		}
		pos += r;
		len -= r;
	}

	_pclose(pipe);

	len = (DWORD) (pos - buf);
	len = MultiByteToWideChar(GetConsoleOutputCP(), 0, buf, len, buffer, size);
	free(buf);

	if (len > 0 && buffer[len-1] == '\n') {
		--len;
		if (len > 0 && buffer[len-1] == '\r') {
			--len;
		}
	}
	buffer[len] = '\0';
	return len;
}

BOOL SetDumpTokens(int argc, LPCWSTR argv[])
{
	return setBoolean(pfDumpTokens, *argv);
}

BOOL SetDumpParse(int argc, LPCWSTR argv[])
{
	return setBoolean(pfDumpParse, *argv);
}

BOOL getOutputHandle(void)
{
	if (!consoleOutput) {
		// If this fails nothing will change to make it succeed, so there's no
		// need to keep trying.
		consoleOutput = CreateFile(L"CONOUT$", (GENERIC_READ | GENERIC_WRITE),
			(FILE_SHARE_READ | FILE_SHARE_WRITE), NULL, OPEN_EXISTING, 0, NULL);
	}

	return consoleOutput != INVALID_HANDLE_VALUE;
}
