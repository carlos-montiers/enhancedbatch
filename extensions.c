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
#include "extensions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <shellapi.h>


#define CHECK_CONSOLE_AND_ARG(argc) \
	if (argc > 1 || !haveOutputHandle()) return FALSE

WORD original_attributes;
CONSOLE_CURSOR_INFO original_cci;


static DWORD toString(int num, LPWSTR buffer, DWORD size)
{
	return wsnprintf(buffer, size, L"%d", num);
}

static void toNumber(int *num, int argc, LPCWSTR argv[])
{
	while (argc-- > 0) {
		*num++ = (int) wcstol(*argv++, NULL, 10);
	}
}

static BOOL setBoolean(LPBYTE var, LPCWSTR arg)
{
	if (arg == NULL
		|| (arg[0] == L'0' && arg[1] == L'\0')
		|| WCSIEQ(arg, L"no")
		|| WCSIEQ(arg, L"off")
		|| WCSIEQ(arg, L"false")) {
		*var = FALSE;
		return TRUE;
	}
	if ((arg[0] == L'1' && arg[1] == L'\0')
		|| WCSIEQ(arg, L"yes")
		|| WCSIEQ(arg, L"on")
		|| WCSIEQ(arg, L"true")) {
		*var = TRUE;
		return TRUE;
	}
	return FALSE;
}

inline BOOL isExtendedKeyCode(int code)
{
	switch (code) {
		case 0:
		case 0xE0:
			return TRUE;
		default:
			return FALSE;
	}
}

inline int getKeyCode(void)
{
	int code = _getwch();
	return isExtendedKeyCode(code) ? -_getwch() : code;
}

DWORD Getch(LPWSTR buffer, DWORD size)
{
	int code;
	while ((code = getKeyCode()) < 0) {
		// do nothing
	}

	*buffer = code;
	buffer[1] = L'\0';
	return 1;
}

DWORD Chhit(LPWSTR buffer, DWORD size)
{
	if (_kbhit()) {
		int code = getKeyCode();
		if (code > 0) {
			*buffer = code;
			buffer[1] = L'\0';
			return 1;
		}
	}
	return toString(-1, buffer, size);
}

DWORD Getkb(LPWSTR buffer, DWORD size)
{
	int code = getKeyCode();
	extern LPCWSTR const getch_normal[], getch_enhanced[];
	if (code > 0 && code < 128 && getch_normal[code] != NULL) {
		return wsnprintf(buffer, size, L"%s", getch_normal[code]);
	}
	if (-code > 0 && -code < 167 && getch_enhanced[-code] != NULL) {
		return wsnprintf(buffer, size, L"%s", getch_enhanced[-code]);
	}
	return toString(code, buffer, size);
}

DWORD Kbhit(LPWSTR buffer, DWORD size)
{
	if (_kbhit()) {
		return Getkb(buffer, size);
	} else {
		return wsnprintf(buffer, size, L"VK_NONE");
	}
}

int CallWaitkey(int argc, LPCWSTR argv[])
{
	BOOL flush = FALSE, close = FALSE;
	for (int i = 0; i < argc; ++i) {
		if (WCSIEQ(argv[i], L"/c")) {
			close = flush = TRUE;
		} else if (WCSIEQ(argv[i], L"/f")) {
			flush = TRUE;
		}
	}
	if (flush) {
		while (_kbhit()) {
			_getch();
		}
	}
	if (close) {
		if (!haveWindowHandle()) {
			return 0;
		}
		// This is a trick from Stack Overflow to determine if this process is
		// the one that started the console.  See:
		//		https://stackoverflow.com/questions/8610489/distinguish-if-program-runs-by-clicking-on-the-icon-typing-its-name-in-the-cons
		//		https://stackoverflow.com/questions/1482604/how-to-tell-if-a-delphi-app-owns-its-console
		// XP and later could use GetConsoleProcessList, but we still support
		// 2000, so I need this anyway and it's just as good.
		DWORD pid;
		GetWindowThreadProcessId(consoleHwnd, &pid);
		if (pid != GetCurrentProcessId()) {
			return 0;
		}
		WCHAR buf[4];
		GetTransient(buf, 4);
		if (*buf == L'0') {
			return 0;
		}
		cmd_printf(CloseWindowStr);
	}
	getKeyCode();
	return 0;
}

int CallCheckkey(int argc, LPCWSTR argv[])
{
	if (_kbhit()) {
		getKeyCode();
		return 0;
	}
	return 1;
}

BOOL setPosition(int row, int column)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD screen_max;
	COORD coord;
	SHORT x, y;

	if (!haveOutputHandle()) {
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

	if (haveOutputHandle()) {
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
	return wsnprintf(buffer, size, L"%d %d", position.Y, position.X);
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

	if (haveOutputHandle()) {
		GetConsoleScreenBufferInfo(consoleOutput, &csbi);
		size.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		size.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	} else {
		size.X = size.Y = 0;
	}

	return size;
}

DWORD GetSize(LPWSTR buffer, DWORD size)
{
	COORD csize;

	csize = getSize();
	return wsnprintf(buffer, size, L"%d %d", csize.Y, csize.X);
}

DWORD GetHeight(LPWSTR buffer, DWORD size)
{
	return toString(getSize().Y, buffer, size);
}

DWORD GetWidth(LPWSTR buffer, DWORD size)
{
	return toString(getSize().X, buffer, size);
}

BOOL SetAttributes(int argc, LPCWSTR argv[])
{
	DWORD value;

	CHECK_CONSOLE_AND_ARG(argc);

	if (argc == 0) {
		value = original_attributes;
	} else {
		value = wcstol(argv[0], NULL, 16);
		if (value > 0xFFFF) {
			return FALSE;
		}
	}

	return SetConsoleTextAttribute(consoleOutput, value);
}

DWORD GetAttributes(LPWSTR buffer, DWORD size)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!haveOutputHandle()) {
		return toString(-1, buffer, size);
	}

	GetConsoleScreenBufferInfo(consoleOutput, &csbi);

	return wsnprintf(buffer, size, L"%.4X", csbi.wAttributes);
}

BOOL SetColor(int argc, LPCWSTR argv[])
{
	DWORD value;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	CHECK_CONSOLE_AND_ARG(argc);

	if (argc == 0) {
		value = original_attributes & 0xFF;
	} else {
		value = wcstol(argv[0], NULL, 16);
		if (value > 0xFF) {
			return FALSE;
		}
	}

	GetConsoleScreenBufferInfo(consoleOutput, &csbi);
	value |= csbi.wAttributes & 0xFF00;

	return SetConsoleTextAttribute(consoleOutput, value);
}

DWORD GetColor(LPWSTR buffer, DWORD size)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!haveOutputHandle()) {
		return toString(-1, buffer, size);
	}

	GetConsoleScreenBufferInfo(consoleOutput, &csbi);

	return wsnprintf(buffer, size, L"%.2X", csbi.wAttributes & 0xFF);
}

BOOL SetForeground(int argc, LPCWSTR argv[])
{
	DWORD value;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	CHECK_CONSOLE_AND_ARG(argc);

	if (argc == 0) {
		value = original_attributes & 0xF;
	} else {
		value = wcstol(argv[0], NULL, 16);
		if (value > 0xF) {
			return FALSE;
		}
	}

	GetConsoleScreenBufferInfo(consoleOutput, &csbi);
	value |= csbi.wAttributes & 0xFFF0;

	return SetConsoleTextAttribute(consoleOutput, value);
}

DWORD GetForeground(LPWSTR buffer, DWORD size)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!haveOutputHandle()) {
		return toString(-1, buffer, size);
	}

	GetConsoleScreenBufferInfo(consoleOutput, &csbi);

	return wsnprintf(buffer, size, L"%X", csbi.wAttributes & 0xF);
}

BOOL SetBackground(int argc, LPCWSTR argv[])
{
	DWORD value;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	CHECK_CONSOLE_AND_ARG(argc);

	if (argc == 0) {
		value = (original_attributes >> 4) & 0xF;
	} else {
		value = wcstol(argv[0], NULL, 16);
		if (value > 0xF) {
			return FALSE;
		}
	}

	GetConsoleScreenBufferInfo(consoleOutput, &csbi);
	value = (csbi.wAttributes & 0xFF0F) | (value << 4);

	return SetConsoleTextAttribute(consoleOutput, value);
}

DWORD GetBackground(LPWSTR buffer, DWORD size)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!haveOutputHandle()) {
		return toString(-1, buffer, size);
	}

	GetConsoleScreenBufferInfo(consoleOutput, &csbi);

	return wsnprintf(buffer, size, L"%X", (csbi.wAttributes & 0xF0) >> 4);
}

BOOL SetUnderline(int argc, LPCWSTR argv[])
{
	BYTE ul;
	WORD value;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	CHECK_CONSOLE_AND_ARG(argc);

	if (argc == 0) {
		ul = original_attributes >> 15;
	} else {
		if (!setBoolean(&ul, *argv)) {
			return FALSE;
		}
	}

	GetConsoleScreenBufferInfo(consoleOutput, &csbi);
	value = (csbi.wAttributes & 0x7FFF) | (ul << 15);

	return SetConsoleTextAttribute(consoleOutput, value);
}

DWORD GetUnderline(LPWSTR buffer, DWORD size)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!haveOutputHandle()) {
		return toString(-1, buffer, size);
	}

	GetConsoleScreenBufferInfo(consoleOutput, &csbi);

	return toString(csbi.wAttributes >> 15, buffer, size);
}

int CallSleep(int argc, LPCWSTR argv[])
{
	int milliseconds;

	if (argc != 1) {
		return EXIT_FAILURE;
	}

	toNumber(&milliseconds, 1, argv);
	if (milliseconds < 0) {
		return EXIT_FAILURE;
	}

	Sleep(milliseconds);

	return EXIT_SUCCESS;
}

static DWORD lo_timer_begin, lo_timer_end;
static LARGE_INTEGER hi_timer_begin, hi_timer_end, hi_frequency;
static BOOL lo_timer_running, hi_timer_running;
static BOOL lo_timer_started, hi_timer_started;

int CallTimer(int argc, LPCWSTR argv[])
{
	if (argc > 1) {
		return EXIT_FAILURE;
	}

	// *argv points to null character if argc is 0.
	if ((argc == 0 && lo_timer_running) || WCSIEQ(*argv, L"stop")) {
		if (lo_timer_running) {
			lo_timer_end = GetTickCount();
			lo_timer_running = FALSE;
			return EXIT_SUCCESS;
		}
	} else if ((argc == 0 && !lo_timer_running) || WCSIEQ(*argv, L"start" )) {
		lo_timer_running = lo_timer_started = TRUE;
		lo_timer_begin = GetTickCount();
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
}

int CallTimerHi(int argc, LPCWSTR argv[])
{
	if (argc > 1) {
		return EXIT_FAILURE;
	}

	// *argv points to null character if argc is 0.
	if ((argc == 0 && hi_timer_running) || WCSIEQ(*argv, L"stop")) {
		if (hi_timer_running) {
			QueryPerformanceCounter(&hi_timer_end);
			hi_timer_running = FALSE;
			return EXIT_SUCCESS;
		}
	} else if ((argc == 0 && !hi_timer_running) || WCSIEQ(*argv, L"start" )) {
		if (hi_frequency.QuadPart == -1) {
			return EXIT_FAILURE;
		}
		if (hi_frequency.QuadPart == 0) {
			if (!QueryPerformanceFrequency(&hi_frequency)) {
				hi_frequency.QuadPart = -1;
				return EXIT_FAILURE;
			}
		}
		hi_timer_running = hi_timer_started = TRUE;
		QueryPerformanceCounter(&hi_timer_begin);
		return EXIT_SUCCESS;
	}

	return EXIT_FAILURE;
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

BOOL SetOpacity(int argc, LPCWSTR argv[])
{
	int pc;
	BYTE alpha;
	LONG_PTR exstyle;

	if (argc != 1 || !haveWindowHandle()) {
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

	if (winMajor >= 10) {		// is Windows 10 or greater
		// Simulate wheel movements to keep the properties dialog in sync
		// (within 4.7%, anyway).
		BYTE curr_alpha;
		int wheel_movements;
		if (!GetLayeredWindowAttributes(consoleHwnd, NULL, &curr_alpha, NULL)) {
			curr_alpha = MAX_OPACITY_ALPHA;
		}
		wheel_movements = alpha - curr_alpha;
		wheel_movements /= OPACITY_DELTA_INTERVAL;
		if (wheel_movements != 0) {
			PostMessage(
				consoleHwnd,
				WM_MOUSEWHEEL,
				MAKELONG(MK_CONTROL | MK_SHIFT, WHEEL_DELTA * wheel_movements),
				0
			);
		}
	}

	exstyle = GetWindowLongPtr(consoleHwnd, GWL_EXSTYLE);
	if (!(exstyle & WS_EX_LAYERED)) {
		exstyle |= WS_EX_LAYERED;
		SetWindowLongPtr(consoleHwnd, GWL_EXSTYLE, exstyle);
	}

	return SetLayeredWindowAttributes(consoleHwnd, 0, alpha, LWA_ALPHA);
}

DWORD GetOpacity(LPWSTR buffer, DWORD size)
{
	DWORD pc = MAX_OPACITY_PERCENT;
	BYTE alpha;

	if (haveWindowHandle()
		&& GetLayeredWindowAttributes(consoleHwnd, NULL, &alpha, NULL)) {
		pc = MAX_OPACITY_PERCENT * alpha / MAX_OPACITY_ALPHA;
	}

	return toString(pc, buffer, size);
}

BOOL SetConsoleCursor(int argc, LPCWSTR argv[])
{
	CONSOLE_CURSOR_INFO cci;
	int iValue;

	CHECK_CONSOLE_AND_ARG(argc);

	if (argc == 0) {
		return SetConsoleCursorInfo(consoleOutput, &original_cci);
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
	} else {
		cci.dwSize = iValue;
		cci.bVisible = TRUE;
	}

	return SetConsoleCursorInfo(consoleOutput, &cci);
}

DWORD GetConsoleCursor(LPWSTR buffer, DWORD size)
{
	CONSOLE_CURSOR_INFO cci;

	if (!haveOutputHandle()) {
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

	if (argc == 0 || WCSIEQ(*argv, L"oem")) {
		cp = GetOEMCP();
	} else if (WCSIEQ(*argv, L"utf-8") ||
			   WCSIEQ(*argv, L"utf8") ||
			   WCSIEQ(*argv, L"utf_8")) {
		cp = CP_UTF8;
	} else if (WCSIEQ(*argv, L"ansi")) {
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

	if (*pCurrentBatchFile == NULL) {
		return toString(-1, buffer, size);
	}

	argv = *pCurrentBatchFile;
#ifdef _WIN64
	if CMD_MAJOR(>=, 6) {
		argv += 10;
	} else {
		argv += 6;
	}
#else
	if CMD_MAJOR(>=, 6) {
		argv += 15;
	} else if CMD_MAJOR_MINOR(>, 5,0) {
		argv += 8;
	} else {
		argv += 7;
	}
#endif
	rest = argv[-1];
	if (rest != NULL && *rest != L'\0') {
		argc = 10;
		for (;; ++rest) {
			if (*rest == L'\0') {
				++argc;
				if (rest[1] == L'\0') {
					break;
				}
			}
		}
	}
	else {
		argc = 0;
		while (*argv != NULL) {
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

	if (*pCurrentBatchFile == NULL) {
		return 0;
	}

	argv = *pCurrentBatchFile;
#ifdef _WIN64
	if CMD_MAJOR(>=, 6) {
		argv += 10;
	} else {
		argv += 6;
	}
#else
	if CMD_MAJOR(>=, 6) {
		argv += 15;
	} else if CMD_MAJOR_MINOR(>, 5,0) {
		argv += 8;
	} else {
		argv += 7;
	}
#endif
	arglen = (LPDWORD)(argv + 10);
	rest = argv[-1];

	// I'll take a shortcut here: the supplied buffer is 32Ki, which is as big
	// as the command line can be, so no need to test size.
	size = 0;
	argc = 0;
	while (*argv != NULL) {
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
			if (rest != NULL && *rest != L'\0') {
				if (argc > first) {
					*buffer++ = L' ';
					++size;
				}
				for (;; ++rest) {
					if (*rest != L'\0') {
						if (argc >= first) {
							*buffer++ = *rest;
							++size;
						}
					} else {
						if (argc++ == last || rest[1] == L'\0') {
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


int CallClear(int argc, LPCWSTR argv[])
{
	static BOOL called;
	DWORD dummy;

	if (!haveOutputHandle()) {
		return EXIT_FAILURE;
	}

	SMALL_RECT sr;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(consoleOutput, &csbi);
	CHAR_INFO ci;
	ci.Char.UnicodeChar = L' ';
	ci.Attributes = csbi.wAttributes;
	BOOL fill = FALSE, gotchar = FALSE, gotattr = FALSE;
	BOOL nomove = FALSE;

	int i;
	for (i = 0; i < argc; ++i) {
		if (*argv[i] != L'/') {
			break;
		}
		if (WCSIEQ(argv[i], L"/f")) {
			fill = TRUE;
		} else if (WCSIEQ(argv[i], L"/n")) {
			nomove = TRUE;
		} else {
			if (i+1 == argc) {
				return 1;
			}
			if (WCSIEQ(argv[i], L"/a")) {
				ci.Attributes = wcstol(argv[++i], NULL, 16);
				gotattr = TRUE;
			} else if (WCSIEQ(argv[i], L"/c")) {
				ci.Char.UnicodeChar = *argv[++i];
				gotchar = TRUE;
			}
		}
	}
	if (fill && gotattr && gotchar) {
		fill = FALSE;
	}
	if (i == argc) {
		if (!called) {
			// Scroll in a new window the simple way.
			while (++csbi.srWindow.Top <= csbi.srWindow.Bottom) {
				WriteConsole(consoleOutput, L"\n", 1, &dummy, NULL);
			}
			COORD c;
			c.X = csbi.dwCursorPosition.X;
			GetConsoleScreenBufferInfo(consoleOutput, &csbi);
			c.Y = csbi.srWindow.Top;
			SetConsoleCursorPosition(consoleOutput, c);
		}
		sr = csbi.srWindow;
	} else {
		if (i+4 != argc) {
			return EXIT_FAILURE;
		}
		int rect[4];
		toNumber(rect, 4, argv+i);
		sr.Top = csbi.srWindow.Top + rect[0];
		sr.Left = csbi.srWindow.Left + rect[1];
		sr.Bottom = sr.Top + rect[2] - 1;
		sr.Right = sr.Left + rect[3] - 1;
		if (sr.Top < csbi.srWindow.Top) {
			sr.Top = csbi.srWindow.Top;
		} else if (sr.Top > csbi.srWindow.Bottom) {
			sr.Top = csbi.srWindow.Bottom;
		}
		if (sr.Bottom < csbi.srWindow.Top) {
			sr.Bottom = csbi.srWindow.Top;
		} else if (sr.Bottom > csbi.srWindow.Bottom) {
			sr.Bottom = csbi.srWindow.Bottom;
		}
		if (sr.Left < csbi.srWindow.Left) {
			sr.Left = csbi.srWindow.Left;
		} else if (sr.Left > csbi.srWindow.Right) {
			sr.Left = csbi.srWindow.Right;
		}
		if (sr.Right < csbi.srWindow.Left) {
			sr.Right = csbi.srWindow.Left;
		} else if (sr.Right > csbi.srWindow.Right) {
			sr.Right = csbi.srWindow.Right;
		}

	}
	if (!nomove) {
		COORD c = { sr.Left, sr.Top };
		SetConsoleCursorPosition(consoleOutput, c);
	}
	if (fill) {
		DWORD len = sr.Right - sr.Left + 1;
		COORD c = { sr.Left, sr.Top };
		while (sr.Top++ <= sr.Bottom) {
			if (gotchar) {
				FillConsoleOutputCharacter(consoleOutput, ci.Char.UnicodeChar,
										   len, c, &dummy);
			} else {
				FillConsoleOutputAttribute(consoleOutput, ci.Attributes,
										   len, c, &dummy);
			}
			++c.Y;
		}
	} else {
		COORD c = { csbi.srWindow.Right + 1, csbi.srWindow.Bottom + 1 };
		ScrollConsoleScreenBuffer(consoleOutput, &sr, &sr, c, &ci);
	}

	called = TRUE;
	return EXIT_SUCCESS;
}


struct sWriteOptions {
	BOOL vertical;
	BOOL console;
	BOOL escapes;
	WCHAR esc_ch;
};

static void doWrite(LPCWSTR text, const struct sWriteOptions *options);

int CallWrite(int argc, LPCWSTR argv[])
{
	struct sWriteOptions opts = {
		.vertical = FALSE,
		.console = FALSE,
		.escapes = FALSE,
		.esc_ch = L'\\',
	};
	LPCWSTR ending = L"\r\n";
	BOOL console = FALSE;
	BOOL space = FALSE;
	BOOL opts_done = FALSE;
	int got_org = 0;
	BOOL underline = FALSE;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	LPCWSTR arg;

	void getOrg(void)
	{
		if (got_org == 0) {
			if (haveOutputHandle()) {
				got_org = 1;
				GetConsoleScreenBufferInfo(consoleOutput, &csbi);
				underline = csbi.wAttributes >> 15;
			} else {
				got_org = -1;
			}
		}
	}

	for (int i = 0; i < argc; ++i) {
		if (opts_done || *argv[i] != L'/') {
			if (space) {
				doWrite(L" ", &opts);
			}
			doWrite(argv[i], &opts);
			space = TRUE;
		} else {
			space = FALSE;
			if (WCSIEQ(argv[i], L"/con")) {
				console ^= TRUE;
				if (console) {
					opts.console = haveOutputHandle();
				} else {
					opts.console = FALSE;
				}
			} else if (WCSIBEG(argv[i], L"/f")) {
				getOrg();
				argv[i] += 2;
				SetForeground(*argv[i] == L'\0' ? 0 : 1, argv+i);
			} else if (WCSIBEG(argv[i], L"/b")) {
				getOrg();
				argv[i] += 2;
				SetBackground(*argv[i] == L'\0' ? 0 : 1, argv+i);
			} else if (WCSIBEG(argv[i], L"/a")) {
				getOrg();
				argv[i] += 2;
				SetColor(*argv[i] == L'\0' ? 0 : 1, argv+i);
			} else if (WCSIBEG(argv[i], L"/e")) {
				if (argv[i][2] == L'?') {
					cmd_printf(L"%s\r\n", EscapeHelpStr);
					return EXIT_SUCCESS;
				}
				if (argv[i][2] != L'\0' && !iswalnum(argv[i][2])) {
					opts.esc_ch = argv[i][2];
					opts.escapes = TRUE;
				} else {
					opts.escapes ^= TRUE;
				}
			} else if (WCSIEQ(argv[i], L"/l")) {
				getOrg();
				underline ^= TRUE;
				arg = underline ? L"1" : L"0";
				SetUnderline(1, &arg);
			} else if (WCSIEQ(argv[i], L"/n")) {
				ending = NULL;
			} else if (WCSIEQ(argv[i], L"/u")) {
				ending = L"\n";
			} else if (WCSIEQ(argv[i], L"/v")) {
				opts.vertical ^= TRUE;
			} else if (WCSEQ(argv[i], L"//")) {
				opts_done = TRUE;
			} else {
				if (i == argc-1) {
					return EXIT_FAILURE;
				}
				if (WCSIEQ(argv[i], L"/c")) {
					++i;
					getOrg();
					ending = NULL;
					SetColumn(1, argv+i);
				} else if (WCSIEQ(argv[i], L"/r")) {
					++i;
					getOrg();
					ending = NULL;
					SetRow(1, argv+i);
				} else {
					if (i >= argc-2) {
						return EXIT_FAILURE;
					}
					if (WCSIEQ(argv[i], L"/p")) {
						++i;
						getOrg();
						ending = NULL;
						SetPosition(2, argv+i);
						++i;
					}
				}
			}
		}
	}

	if (got_org == 1) {
		SetConsoleTextAttribute(consoleOutput, csbi.wAttributes);
		SetConsoleCursorPosition(consoleOutput, csbi.dwCursorPosition);
	}

	if (ending != NULL) {
		if (opts.console) {
			DWORD dummy;
			WriteConsole(consoleOutput, L"\n", 1, &dummy, NULL);
		} else {
			cmd_printf(ending);
		}
	}

	return EXIT_SUCCESS;
}

static void doWrite(LPCWSTR text, const struct sWriteOptions *options)
{
	DWORD dummy;

	if (options->escapes) {
		LPWSTR p = stringBuffer, end = p + STRINGBUFFERMAX - 1;
		for (; *text != L'\0' && p < end;) {
			if (*text == options->esc_ch) {
				WCHAR esc = 0;
				LPCWSTR hex = NULL;
				if (text[1] == options->esc_ch) {
					esc = options->esc_ch;
				} else switch (text[1]) {
					case L'a': esc = L'\a'; break;
					case L'b': esc = L'\b'; break;
					case L'e': esc = L'\33'; break;
					case L'f': esc = L'\f'; break;
					case L'n': esc = L'\n'; break;
					case L'r': esc = L'\r'; break;
					case L't': esc = L'\t'; break;
					case L'v': esc = L'\v'; break;
					case L'x': hex = L"%2x%n"; break;
					case L'u': hex = L"%4x%n"; break;
					case L'U': hex = L"%6x%n"; break;
				}
				if (esc != 0) {
					*p++ = esc;
					text += 2;
					continue;
				} else if (hex != NULL) {
					DWORD code;
					int size = 0;
					swscanf(text+2, hex, &code, &size);
					if (size != 0) {
						if (code < 0x10000) {
							*p++ = code;
						} else {
							code -= 0x10000;
							*p++ = HIGH_SURROGATE_START + (code >> 10);
							*p++ = LOW_SURROGATE_START + (code & 0x3ff);
						}
						text += 2 + size;
						continue;
					}
				}
				// else ignore unknown escapes and preserve the escape char
			}
			*p++ = *text++;
		}
		*p = L'\0';
		text = stringBuffer;
	}

	if (!options->vertical) {
		if (options->console) {
			WriteConsole(consoleOutput, text, wcslen(text), &dummy, NULL);
		} else {
			cmd_printf(L"%s", text);
		}
	} else {
		int col = getPosition().X;
		LPWSTR next;
		while (*text != L'\0') {
			// CharNext skips over the high surrogate and stops at the low.
			if (*text >= HIGH_SURROGATE_START && *text <= HIGH_SURROGATE_END
				&& text[1] >= LOW_SURROGATE_START && text[1] <= LOW_SURROGATE_END) {
				next = CharNext(text+1);
			} else {
				next = CharNext(text);
				if (*next >= LOW_SURROGATE_START && *next <= LOW_SURROGATE_END
					&& next[-1] >= HIGH_SURROGATE_START && next[-1] <= HIGH_SURROGATE_END) {
					--next;
				}
			}
			if (options->console) {
				WriteConsole(consoleOutput, text, next - text, &dummy, NULL);
				WriteConsole(consoleOutput, L"\n", 1, &dummy, NULL);
			} else {
				cmd_printf(L"%.*s\n", next - text, text);
			}
			text = next;
			setPosition(getPosition().Y, col);
		}
	}
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

static char ctrlcaborts = -1;

BOOL SetCtrlCAborts(int argc, LPCWSTR argv[])
{
	if (argc == 0) {
		ctrlcaborts = -1;
	} else if (!setBoolean((LPBYTE) &ctrlcaborts, *argv)) {
		return FALSE;
	}
	hookCtrlCAborts(ctrlcaborts);
	return TRUE;
}

DWORD GetCtrlCAborts(LPWSTR buffer, DWORD size)
{
	return toString(ctrlcaborts, buffer, size);
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
	if (size != 0) {
		if (!DeleteFile(buffer) || !CreateDirectory(buffer, NULL)) {
			size = 0;
		}
	}
	return size;
}

static SYSTEMTIME st;
static DWORD time_retrieved;
static WCHAR point;
static BYTE english;
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
	if (point == L'\0') {
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
	return wsnprintf(buffer, size, L"%d-%02d-%02d",
					 st.wYear, st.wMonth, st.wDay);
}

DWORD GetTime(LPWSTR buffer, DWORD size)
{
	getDate();
	return wsnprintf(buffer, size, L"%02d:%02d:%02d",
					 st.wHour, st.wMinute, st.wSecond);
}

DWORD GetTimems(LPWSTR buffer, DWORD size)
{
	DWORD len = GetTime(buffer, size);
	return len + wsnprintf(buffer + len, size - len, L"%c%03d",
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
		return wsncpy(buffer, size, MonthNames[st.wMonth-1]);
	}
	return GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME1-1+st.wMonth,
						 buffer, size);
}

DWORD GetMonthShort(LPWSTR buffer, DWORD size)
{
	getDate();
	if (english) {
		return wsnprintf(buffer, size, L"%.3s", MonthNames[st.wMonth-1]);
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
		return wsncpy(buffer, size, DayNames[st.wDayOfWeek]);
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
		return wsnprintf(buffer, size, L"%.3s", DayNames[st.wDayOfWeek]);
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

DWORD GetUnixTime(LPWSTR buffer, DWORD size)
{
	return wsnprintf(buffer, size, L"%I64d", time(NULL));
}

static WCHAR noyes[8];

static void getNoYes(void) {
	if (*noyes == L'\0') {
		wcscpy(noyes, L"NY");
		FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_HMODULE,
					  NULL, 0x2328, 0, noyes, lenof(noyes), NULL);
	}
}

DWORD GetYes(LPWSTR buffer, DWORD size)
{
	WCHAR yes;
	if (english) {
		yes = L'Y';
	} else {
		getNoYes();
		yes = noyes[1];
	}
	return wsnprintf(buffer, size, L"%c", yes);
}

DWORD GetNo(LPWSTR buffer, DWORD size)
{
	WCHAR no;
	if (english) {
		no = L'N';
	} else {
		getNoYes();
		no = noyes[0];
	}
	return wsnprintf(buffer, size, L"%c", no);
}

DWORD GetEnglish(LPWSTR buffer, DWORD size)
{
	return toString(english, buffer, size);
}

BOOL SetEnglish(int argc, LPCWSTR argv[])
{
	return setBoolean(&english, *argv);
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

DWORD GetTrace(LPWSTR buffer, DWORD size)
{
	return toString(trace, buffer, size);
}

BOOL SetBatchFile(int argc, LPCWSTR argv[])
{
	if (argc != 1) {
		return FALSE;
	}
	toNumber(&batchfile, 1, argv);
	return TRUE;
}

BOOL SetTrace(int argc, LPCWSTR argv[])
{
	return setBoolean(&trace, *argv);
}

extern int ForF_stack[FORF_STACKSIZE], ForF_stacktop;

DWORD GetForLine(LPWSTR buffer, DWORD size)
{
	return toString(ForF_stack[ForF_stacktop], buffer, size);
}

DWORD GetForLines(LPWSTR buffer, DWORD size)
{
	int sp = ForF_stacktop + 1;
	if (sp == FORF_STACKSIZE) {
		sp = 0;
	}
	return toString(ForF_stack[sp], buffer, size);
}

AttrOptSize
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
	return wsnprintf(buffer, size, L"%S", __DATE__);
}

DWORD GetEnhancedBatch(LPWSTR buffer, DWORD size)
{
	return toString(EBVERSION, buffer, size);
}

AttrOptSize
DWORD GetOSVersion(LPWSTR buffer, DWORD size)
{
	DWORD ver = GetVersion();
	return wsnprintf(buffer, size, L"%d.%d.%d.%d",
					 LOBYTE(LOWORD(ver)), HIBYTE(LOWORD(ver)), HIWORD(ver),
					 getVersionRevision());
}

AttrOptSize
DWORD GetOSMajor(LPWSTR buffer, DWORD size)
{
	return toString(winMajor, buffer, size);
}

AttrOptSize
DWORD GetOSMinor(LPWSTR buffer, DWORD size)
{
	return toString(HIBYTE(LOWORD(GetVersion())), buffer, size);
}

AttrOptSize
DWORD GetOSBuild(LPWSTR buffer, DWORD size)
{
	return toString(HIWORD(GetVersion()), buffer, size);
}

AttrOptSize
DWORD GetOSRevision(LPWSTR buffer, DWORD size)
{
	return toString(getVersionRevision(), buffer, size);
}

DWORD GetCmdVersion(LPWSTR buffer, DWORD size)
{
	return wsnprintf(buffer, size, L"%d.%d.%d.%d%s",
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
		if (tsize != 0) {
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
	if (asize != 0) {
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

int CallElevate(int argc, LPCWSTR argv[])
{
	SHELLEXECUTEINFO sei;
	extern WCHAR enh_dll[MAX_PATH];
	BOOL new_window = LOWORD(GetVersion()) == 5;  // attach not supported by 2K
	BOOL transient = TRUE;
	LPCWSTR cmd = NULL;
	WCHAR name[64], key[32];
	static LPVOID keyptr;

	// Quick test if nothing's been elevated.
	if (argc == -1 && keyptr == NULL) {
		return EXIT_SUCCESS;
	}
	if (keyptr == NULL) {
		keyptr = *pCurrentBatchFile;
	}
	wsnprintf(key, lenof(key), keyptr == NULL ? L"%u-" : L"%u-%p",
			  GetCurrentProcessId(), keyptr);
	wsnprintf(name, lenof(name), L"EB-elevate-event-%s", key);
	HANDLE event = CreateEvent(NULL, FALSE, FALSE, name);
	if (event == NULL) {
		return 0xEBEF;
	}
	if (argc == -1) {
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			// Let the elevated process know it should exit.
			SetEvent(event);
		}
		CloseHandle(event);
		keyptr = NULL;
		return EXIT_SUCCESS;
	}
	if (GetLastError() != ERROR_ALREADY_EXISTS) {
		WCHAR title[1024];
		ZeroMemory(&sei, sizeof(sei));
		sei.cbSize = sizeof(sei);
		sei.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_UNICODE;
		sei.lpVerb = L"runas";
		sei.lpFile = L"rundll32";
		GetConsoleTitle(title, lenof(title));
		sbprintf(stringBuffer, L"\"%s\" Elevate [ %s ] %s", enh_dll, title, key);
		sei.lpParameters = stringBuffer;
		if (!ShellExecuteEx(&sei)) {
			goto errexit;
		}
	}

	wsnprintf(name, lenof(name), L"EB-elevate-mutex-%s", key);
	HANDLE mutex = CreateMutex(NULL, FALSE, name);
	if (mutex == NULL) {
		goto errexit;
	}
	WaitForSingleObject(mutex, INFINITE);

	LPWSTR env = GetEnvironmentStrings();
	LPWSTR p = env;
	for (;; p += 2) {
		if (*p == L'\0') {
			if (p[-1] == L'\0') {
				break;
			}
			if (p[1] == L'\0') {
				++p;
				break;
			}
		}
	}
	DWORD envlen = WSZ(p + 1 - env);
	DWORD maplen = sizeof(struct sElevate) + envlen;
	wsnprintf(name, lenof(name), L"EB-elevate-data-%s", key);
	HANDLE map = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
								   0, maplen, name);
	struct sElevate *data = MapViewOfFile(map, FILE_MAP_WRITE, 0, 0, maplen);
	if (data == NULL) {
		SafeCloseHandle(map);
		ReleaseMutex(mutex);
		CloseHandle(mutex);
	errexit:
		CloseHandle(event);
		return 0xEBEF;
	}
	memcpy(data->env, env, envlen);
	FreeEnvironmentStrings(env);

	GetCurrentDirectory(MAX_PATH, data->curdir);

	if (argc == 1) {
		BOOL stop = FALSE;
		cmd = *argv;
		while (*cmd == L'/' && !stop) {
			++cmd;
			if ((*cmd | 0x20) == L'n') {
				new_window = TRUE;
				++cmd;
				if ((*cmd | 0x20) == L'k') {
					transient = FALSE;
					++cmd;
				}
			} else if (*cmd == L'/') {
				stop = TRUE;
				++cmd;
			}
			while (*cmd == L' ' || *cmd == L'\t') {
				++cmd;
			}
		}
		if (*cmd == L'\0') {
			cmd = NULL;
		}
	}

	if (!new_window) {
		data->console_pid = GetCurrentProcessId();
	}

	if (cmd == NULL) {
		wsnprintf(data->cmdline, STRINGBUFFERMAX, L"cmd /k");
	} else {
		wsnprintf(data->cmdline, STRINGBUFFERMAX, L"cmd /%c %s",
				  transient ? L'c' : L'k', cmd);
	}

	SetEvent(event);

	// Wait for the elevated process to finish with the data.
	int count = 50;
	while (*data->env != L'\0' && --count > 0) {
		Sleep(10);
	}

	DWORD pid = data->elevated_pid;

	UnmapViewOfFile(data);
	CloseHandle(map);
	ReleaseMutex(mutex);
	CloseHandle(mutex);
	CloseHandle(event);

	if (pid == 0) {
		return 0xEBEF;
	}

	DWORD rc = EXIT_SUCCESS;

	if (!new_window) {
		if (count == 0) {
			// Something went wrong loading EB, unhide the window.
			BOOL CALLBACK find_window_process(HWND hwnd, LPARAM lParam)
			{
				DWORD elepid = (DWORD) lParam, winpid = 0;
				GetWindowThreadProcessId(hwnd, &winpid);
				if (winpid == elepid) {
					ShowWindow(hwnd, SW_SHOW);
					SetForegroundWindow(hwnd);
					return FALSE;
				}
				return TRUE;
			}
			EnumWindows(find_window_process, (LPARAM) pid);
		} else {
			DWORD info = winMajor >= 6 ? PROCESS_QUERY_LIMITED_INFORMATION
									   : PROCESS_QUERY_INFORMATION;
			HANDLE hProcess = OpenProcess(info | SYNCHRONIZE, FALSE, pid);
			if (hProcess != NULL) {
				WaitForSingleObject(hProcess, INFINITE);
				if (!GetExitCodeProcess(hProcess, &rc)) {
					rc = 0xDEAD;
				}
				CloseHandle(hProcess);
			}
			// It shouldn't fail, but it's a problem if it does - both
			// processes are running simultaneously in the same console.
		}
	}

	return rc;
}

DWORD GetRun(LPCWSTR cmd, LPWSTR buffer, DWORD size)
{
	FILE *pipe = _wpopen(cmd, L"rb");
	if (pipe == NULL) {
		return 0;
	}

	char *buf = malloc(size);
	if (buf == NULL) {
		_pclose(pipe);
		return 0;
	}

	char * pos = buf;
	DWORD len = size - 1;
	DWORD r;
	while (len > 0) {
		r = fread(pos, 1, len, pipe);
		if (r == 0) {
			break;
		}
		pos += r;
		len -= r;
	}

	*pLastRetCode = _pclose(pipe);

	len = (DWORD)(pos - buf);
	len = MultiByteToWideChar(GetConsoleOutputCP(), 0, buf, len, buffer, size);
	free(buf);

	if (len > 0 && buffer[len-1] == L'\n') {
		--len;
		if (len > 0 && buffer[len-1] == L'\r') {
			--len;
		}
	}
	buffer[len] = L'\0';
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
