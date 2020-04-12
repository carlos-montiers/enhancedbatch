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

/*
 * 3 September, 2013 : Jason Hood. Code for inject dll code.
 * 28 October, 2017  : Carlos Montiers A. Revisited for work with rundll32.
 * 9 February, 2019  : Begin of the project.
 * 24 February, 2019 : Jason Hood. Add unload
 * 24 February, 2019 : Carlos Montiers A. Add code for save the variables in heap using linked list.
 * 25 February, 2019 : Jason Hood. Replaced the linked list with generic hash table.
 * 3 March, 2019     : Carlos Montiers A. Add extensions: reply, position, color.
 * April, 2019       : Jason Hood. Bunch of extensions, filters and low level behaviors.
 * 9 April, 2019     : Jason Hood. Add cool behavior: for range.
 * 12 May, 2019      : Jason Hood and Carlos Montiers A. Extension: opacity.
 * 13 July, 2019     : Carlos Montiers A. Add behavior: for infinite.
 */

#include "dll_enhancedbatch.h"
#include "extensions.h"
#include <shellapi.h>
#include <tlhelp32.h>

BOOL WINAPI
CmdBatNotification(BOOL start);

#include "khash.h"
KHASH_MAP_INIT_WSTR(wstr, LPWSTR)
KHASH_MAP_INIT_WSTR(line, DWORD)


WCHAR enh_dll[MAX_PATH];

khash_t(wstr) *variables;
khash_t(line) *batch_lnums;

HMODULE hDllInstance;
BOOL global;			// launched outside of a batch
LPSTR AnsiBuf;			// batch file buffer
LPVOID cmd_end; 		// end of the CMD.EXE image
int winMajor;			// major version of Windows
BOOL onWindowsTerminal; 		// running on Windows Terminal
HWND consoleHwnd; 		// Hwnd of the console
HANDLE consoleOutput;
HANDLE hSpeaking;		// speech thread

WCHAR stringBuffer[STRINGBUFFERMAX]; // For hold conversion of values
WCHAR varBuffer[STRINGBUFFERMAX];

int CallHelp(int argc, LPCWSTR argv[]);

LPWIN32_FIND_DATA findForStack[FINDFOR_STACKSIZE];
int findForStackTop = -1;

enum { forRange, forInfinite };

struct sFor {
	// The size of this struct cannot be higher than MAX_PATH,
	// that is the size for the field cFileName of struct WIN32_FIND_DATA.
	// The value field must be first, and null terminated.
	WCHAR szValue[12];	 // longest value for a 32-bit number + null
	int type;
	int start, stop, step;
};

BOOL Next(int argc, LPCWSTR argv[]);

void unhook(void);
int CallUnload(int argc, LPCWSTR argv[]);
void handleElevation(void);

fnCmdFunc *peCall, eCall;
int *pLastRetCode;
DWORD cmdFileVersionMS, cmdFileVersionLS, cmdDebug;
#ifdef _WIN64
fnPutMsg pPutMsg;
#else
LPVOID pPutMsg;
#endif
int batchfile = 1;
DWORD last_lnum;
LPWSTR *last_bat;

struct sExt {
	LPCWSTR name;
	int args;			// getters use this as a flag
	LPVOID fn;
	LPCWSTR brief, help;
};

#define HELPSTR(ext) ext##BriefStr, ext##HelpStr
#define CONSTANT(str) str, str, str
#define CONSTHELP(str, help) str, help, help

// args is:
//	0: getter only
//	1: getter with corresponding setter
//	2: constant, defined in fn
const struct sExt getExtensionList[] = {
	{ L"@amp",              2, CONSTANT(L"&") },
	{ L"@apos",             2, CONSTANT(L"'") },
	{ L"@at",               2, CONSTANT(L"@") },
	{ L"@attr",             1, GetAttributes, HELPSTR(Attr) },
	{ L"@bar",              2, CONSTANT(L"|") },
	{ L"@batchfile",        1, GetBatchFile, HELPSTR(BatchFile) },
	{ L"@batchline",        0, GetBatchLine, HELPSTR(BatchLine) },
	{ L"@bell",             2, CONSTHELP(L"\a", L"Bell (U+0007)") },
	{ L"@bg",               1, GetBackground, HELPSTR(Background) },
	{ L"@bquot",            2, CONSTANT(L"`") },
	{ L"@bs",               2, CONSTHELP(L"\b", L"Backspace (U+0008)") },
	{ L"@btick",            2, CONSTANT(L"`") },
	{ L"@caret",            2, CONSTANT(L"^") },
	{ L"@cbrc",             2, CONSTANT(L"}") },
	{ L"@cbrs",             2, CONSTANT(L"]") },
	{ L"@chhit",            0, Chhit, HELPSTR(Readchar) },
	{ L"@cmdversion",       0, GetCmdVersion, HELPSTR(CmdVersion) },
	{ L"@colon",            2, CONSTANT(L":") },
	{ L"@color",            1, GetColor, HELPSTR(Color) },
	{ L"@column",           1, GetColumn, HELPSTR(Column) },
	{ L"@comma",            2, CONSTANT(L",") },
	{ L"@cpar",             2, CONSTANT(L")") },
	{ L"@cr",               2, CONSTHELP(L"\r", L"Carriage return (U+000D)") },
	{ L"@crlf",             2, CONSTHELP(L"\r\n", L"CR+LF (U+000D U+000A)") },
	{ L"@ctrl+?",           2, NULL, HELPSTR(Ctrl) },
	{ L"@ctrlcaborts",      1, GetCtrlCAborts, HELPSTR(CtrlCAborts) },
	{ L"@cursor",           1, GetConsoleCursor, HELPSTR(ConsoleCursor) },
	{ L"@date",             0, GetDate, HELPSTR(Date) },
	{ L"@datetime",         0, GetDateTime, HELPSTR(DateTime) },
	{ L"@datetimems",       0, GetDateTimems, HELPSTR(DateTimems) },
	{ L"@day",              0, GetDay, HELPSTR(Day) },
	{ L"@dayname",          0, GetDayName, HELPSTR(DayName) },
	{ L"@dayofweek",        0, GetDayOfWeek, HELPSTR(DayOfWeek) },
	{ L"@dayshort",         0, GetDayShort, HELPSTR(DayShort) },
	{ L"@decsep",           1, GetDecSep, HELPSTR(DecSep) },
	{ L"@delayedexpansion", 1, GetDelayedExpansion, HELPSTR(DelayedExpansion) },
	{ L"@dollar",           2, CONSTANT(L"$") },
	{ L"@dquot",            2, CONSTANT(L"\"") },
	{ L"@echo",             1, GetEcho, HELPSTR(Echo) },
	{ L"@english",          1, GetEnglish, HELPSTR(English) },
	{ L"@enhancedbatch",    0, GetEnhancedBatch, HELPSTR(EnhancedBatch) },
	{ L"@eq",               2, CONSTANT(L"=") },
	{ L"@esc",              2, CONSTHELP(L"\33", L"Escape (U+001B)") },
	{ L"@excl",             2, CONSTANT(L"!") },
	{ L"@extensions",       1, GetExtensions, HELPSTR(Extensions) },
	{ L"@fg",               1, GetForeground, HELPSTR(Foreground) },
	{ L"@forline",          0, GetForLine, HELPSTR(ForLine) },
	{ L"@forlines",         0, GetForLines, HELPSTR(ForLines) },
	{ L"@getch",            0, Getch, HELPSTR(Getchar) },
	{ L"@getkb",            0, Getkb, HELPSTR(Getkey) },
	{ L"@gt",               2, CONSTANT(L">") },
	{ L"@height",           0, GetHeight, HELPSTR(Height) },
	{ L"@hour",             0, GetHour, HELPSTR(Hour) },
	{ L"@inputcp",          1, GetInputCodePage, HELPSTR(InputCodePage) },
	{ L"@iselevated",       0, GetElevated, HELPSTR(Elevated) },
	{ L"@kbhit",            0, Kbhit, HELPSTR(Readkey) },
	{ L"@lf",               2, CONSTHELP(L"\n", L"Line feed (U+000A)") },
	{ L"@lt",               2, CONSTANT(L"<") },
	{ L"@milliseconds",     0, GetMilliseconds, HELPSTR(Milliseconds) },
	{ L"@minute",           0, GetMinute, HELPSTR(Minute) },
	{ L"@month",            0, GetMonth, HELPSTR(Month) },
	{ L"@monthname",        0, GetMonthName, HELPSTR(MonthName) },
	{ L"@monthshort",       0, GetMonthShort, HELPSTR(MonthShort) },
	{ L"@nbsp",             2, CONSTHELP(L"\xA0", L"No-break space (U+00A0)") },
	{ L"@no" ,              0, GetNo, HELPSTR(No) },
	{ L"@obrc",             2, CONSTANT(L"{") },
	{ L"@obrs",             2, CONSTANT(L"[") },
	{ L"@opacity",          1, GetOpacity, HELPSTR(Opacity) },
	{ L"@opar",             2, CONSTANT(L"(") },
	{ L"@osbuild",          0, GetOSBuild, HELPSTR(OSBuild) },
	{ L"@osmajor",          0, GetOSMajor, HELPSTR(OSMajor) },
	{ L"@osminor",          0, GetOSMinor, HELPSTR(OSMinor) },
	{ L"@osrevision",       0, GetOSRevision, HELPSTR(OSRevision) },
	{ L"@osversion",        0, GetOSVersion, HELPSTR(OSVersion) },
	{ L"@outputcp",         1, GetOutputCodePage, HELPSTR(OutputCodePage) },
	{ L"@perc",             2, CONSTANT(L"%") },
	{ L"@position",         1, GetPosition, HELPSTR(Position) },
	{ L"@ques",             2, CONSTANT(L"?") },
	{ L"@quot",             2, CONSTANT(L"\"") },
	{ L"@row",              1, GetRow, HELPSTR(Row) },
	{ L"@second",           0, GetSecond, HELPSTR(Second) },
	{ L"@semi",             2, CONSTANT(L";") },
	{ L"@size",             0, GetSize, HELPSTR(Size) },
	{ L"@squot",            2, CONSTANT(L"'") },
	{ L"@star",             2, CONSTANT(L"*") },
	{ L"@stderr",           0, GetStderr, HELPSTR(Stderr) },
	{ L"@stdin",            0, GetStdin, HELPSTR(Stdin) },
	{ L"@stdout",           0, GetStdout, HELPSTR(Stdout) },
	{ L"@tab",              2, CONSTHELP(L"\t", L"Tab (U+0009)") },
	{ L"@tempdir",          0, GetTempDir, HELPSTR(TempDir) },
	{ L"@tempfile",         0, GetTempFile, HELPSTR(TempFile) },
	{ L"@tilde",            2, CONSTANT(L"~") },
	{ L"@time",             0, GetTime, HELPSTR(Time) },
	{ L"@timems",           0, GetTimems, HELPSTR(Timems) },
	{ L"@timer",            0, GetTimer, HELPSTR(TimerVar) },
	{ L"@timerhi",          0, GetHiTimer, HELPSTR(HiTimer) },
	{ L"@title",            0, GetTitle, HELPSTR(Title) },
	{ L"@trace",            1, GetTrace, HELPSTR(Trace) },
	{ L"@transient",        0, GetTransient, HELPSTR(Transient) },
	{ L"@ul",               1, GetUnderline, HELPSTR(Underline) },
	{ L"@unicode",          1, GetUnicode, HELPSTR(Unicode) },
	{ L"@unique",           0, GetUnique, HELPSTR(Unique) },
	{ L"@unixtime",         0, GetUnixTime, HELPSTR(UnixTime) },
	{ L"@version",          0, GetEBVersion, HELPSTR(EBVersion) },
	{ L"@voice",            1, GetVoice, HELPSTR(Voice) },
	{ L"@width",            0, GetWidth, HELPSTR(Width) },
	{ L"@year",             0, GetYear, HELPSTR(Year) },
	{ L"@yes",              0, GetYes, HELPSTR(Yes) },
};

// If args is 0 the entire value is passed to the function as the sole argument,
// otherwise the specified number of arguments are required.
const struct sExt setExtensionList[] = {
	{ L"@attr",             0, SetAttributes, AttrBriefStr, NULL },
	{ L"@batchfile",        1, SetBatchFile, BatchFileBriefStr, NULL },
	{ L"@bg",               0, SetBackground, BackgroundBriefStr, NULL },
	{ L"@codepage",         0, SetCodePage, HELPSTR(CodePage) },
	{ L"@color",            0, SetColor, ColorBriefStr, NULL },
	{ L"@column",           1, SetColumn, ColumnBriefStr, NULL },
	{ L"@ctrlcaborts",      0, SetCtrlCAborts, CtrlCAbortsBriefStr, NULL },
	{ L"@cursor",           0, SetConsoleCursor, ConsoleCursorBriefStr, NULL },
	{ L"@decsep",           0, SetDecSep, DecSepBriefStr, NULL },
	{ L"@delayedexpansion", 0, SetDelayedExpansion, DelayedExpansionBriefStr, NULL },
	{ L"@dumpparse",		0, SetDumpParse, HELPSTR(DumpParse) },
	{ L"@dumptokens",		0, SetDumpTokens, HELPSTR(DumpTokens) },
	{ L"@echo",             0, SetEcho, EchoBriefStr, NULL },
	{ L"@english",          0, SetEnglish, EnglishBriefStr, NULL },
	{ L"@extensions",       0, SetExtensions, ExtensionsBriefStr, NULL },
	{ L"@fg",               0, SetForeground, ForegroundBriefStr, NULL },
	{ L"@inputcp",          0, SetInputCodePage, InputCodePageBriefStr, NULL },
	{ L"@next",             0, Next, HELPSTR(Next) },
	{ L"@opacity",          1, SetOpacity, OpacityBriefStr, NULL },
	{ L"@outputcp",         0, SetOutputCodePage, OutputCodePageBriefStr, NULL },
	{ L"@position",         2, SetPosition, PositionBriefStr, NULL },
	{ L"@row",              1, SetRow, RowBriefStr, NULL },
	{ L"@trace",            0, SetTrace, TraceBriefStr, NULL },
	{ L"@ul",               0, SetUnderline, UnderlineBriefStr, NULL },
	{ L"@unicode",          0, SetUnicode, UnicodeBriefStr, NULL },
	{ L"@voice",            0, SetVoice, VoiceBriefStr, NULL },
};

// If args is 0 the entire command line is passed to the function, stripping
// leading and trailing spaces and tabs.  If it's inverted (binary not) at
// least that many arguments are required.
const struct sExt callExtensionList[] = {
	{ L"@checkkey",	0, CallCheckkey, HELPSTR(Checkkey) },
	{ L"@clear",   ~0, CallClear, HELPSTR(Clear) },
	{ L"@elevate",  0, CallElevate, HELPSTR(Elevate) },
	{ L"@help",    ~0, CallHelp, HELPSTR(Help) },
	{ L"@image",   ~1, CallImage, HELPSTR(Image) },
	{ L"@img",	   ~1, CallImg, HELPSTR(Img) },
	{ L"@say",	   ~1, CallSay, HELPSTR(Say) },
	{ L"@sleep",	1, CallSleep, HELPSTR(Sleep) },
	{ L"@text",    ~1, CallText, HELPSTR(Text) },
	{ L"@timer",	0, CallTimer, HELPSTR(Timer) },
	{ L"@timerhi",	0, CallTimerHi, HELPSTR(TimerHi) },
	{ L"@unload",	0, CallUnload, HELPSTR(Unload) },
	{ L"@waitkey", ~0, CallWaitkey, HELPSTR(Waitkey) },
	{ L"@write",   ~0, CallWrite, HELPSTR(Write) },
};

// Modifiers, only for the help.
const struct sExt modExtensionList[] = {
	{ L"\"...\"",       0, NULL, HELPSTR(Default) },
	{ L"'...'",         0, NULL, HELPSTR(Default) },
	{ L"`...`",         0, NULL, HELPSTR(Default) },
	{ L"alt",           0, NULL, HELPSTR(Alt) },
	{ L"capital",       0, NULL, HELPSTR(Capital) },
	{ L"capital[LIST]", 0, NULL, HELPSTR(CapitalList) },
	{ L"ctrl",          0, NULL, HELPSTR(CtrlMod) },
	{ L"hexify",        0, NULL, HELPSTR(Hexify) },
	{ L"key",           0, NULL, HELPSTR(Key) },
	{ L"length",        0, NULL, HELPSTR(Length) },
	{ L"lower",         0, NULL, HELPSTR(Lower) },
	{ L"ltrim",         0, NULL, HELPSTR(Ltrim) },
	{ L"ltrim[LIST]",   0, NULL, HELPSTR(LtrimList) },
	{ L"NUMBER",        0, NULL, HELPSTR(Number) },
	{ L"rtrim",         0, NULL, HELPSTR(Rtrim) },
	{ L"rtrim[LIST]",   0, NULL, HELPSTR(RtrimList) },
	{ L"shift",         0, NULL, HELPSTR(Shift) },
	{ L"trim",          0, NULL, HELPSTR(Trim) },
	{ L"trim[LIST]",    0, NULL, HELPSTR(TrimList) },
	{ L"unhexify",      0, NULL, HELPSTR(Unhexify) },
	{ L"upper",         0, NULL, HELPSTR(Upper) },
	{ L"~",             0, NULL, HELPSTR(Tilde) },
};

void setVar(LPCWSTR var, LPCWSTR val)
{
	khint_t k;
	int absent;

	k = kh_put(wstr, variables, var, &absent);
	if (absent) {
		kh_key(variables, k) = _wcsdup(var);
	} else {
		free((void *) kh_val(variables, k));
	}
	kh_val(variables, k) = _wcsdup(val);
}

int sortcmp(const void *a, const void *b)
{
	return _wcsicmp(*(LPCWSTR *) a, *(LPCWSTR *) b);
}

enum { EV_DISPLAY, EV_ADD, EV_REMOVE };

int enumVars(LPCWSTR prefix, int op)
{
	LPCWSTR *vars;
	DWORD count;
	DWORD len;
	khint_t k;

	vars = malloc(kh_size(variables) * sizeof(LPCWSTR) * 2);
	if (vars == NULL) {
		return EXIT_FAILURE;
	}
	if (prefix != NULL) {
		len = wcslen(prefix);
		if (len > 0 && prefix[len-1] == L' ') {
			--len;
		}
	}
	count = 0;
	for (k = kh_begin(variables); k != kh_end(variables); ++k) {
		if (kh_exist(variables, k)
			&& (prefix == NULL ||
				_wcsnicmp(kh_key(variables, k), prefix, len) == 0)) {
			vars[count++] = kh_key(variables, k);
			vars[count++] = kh_val(variables, k);
		}
	}
	qsort(vars, count / 2, sizeof(LPCWSTR) * 2, sortcmp);

	for (k = 0; k < count; k += 2) {
		if (op == EV_DISPLAY) {
			cmd_printf(L"%s=%s\r\n", vars[k], vars[k+1]);
		} else if (op == EV_ADD) {
			SetEnvironmentVariable(vars[k], vars[k+1]);
		} else if (op == EV_REMOVE) {
			SetEnvironmentVariable(vars[k], NULL);
		}
	}

	free(vars);

	return count == 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}

int __fastcall displayVars(LPCWSTR prefix)
{
	return enumVars(prefix, EV_DISPLAY);
}

int extcmp(const void *a, const void *b)
{
	return _wcsicmp(a, *(LPCWSTR *) b);
}

DWORD getVar(LPCWSTR lpName)
{
	if (lpName == NULL) {
		SetLastError(ERROR_ENVVAR_NOT_FOUND);
		return 0;
	}

	if (*lpName == L'U' && lpName[1] == L'+') {
		LPWSTR end;
		DWORD wch = (DWORD) wcstoul(lpName+2, &end, 16);
		if (*end == L'\0' && end != lpName+2 && wch < 0x110000) {
			if (wch < 0x10000) {
				*stringBuffer = wch;
				stringBuffer[1] = L'\0';
				return 1;
			}
			wch -= 0x10000;
			stringBuffer[0] = HIGH_SURROGATE_START + (wch >> 10);
			stringBuffer[1] = LOW_SURROGATE_START + (wch & 0x3ff);
			stringBuffer[2] = L'\0';
			return 2;
		}
	}

	if (WCSIBEG(lpName, L"@ctrl+") && lpName[6] != L'\0' && lpName[7] == L'\0') {
		WCHAR ctrlch = lpName[6];
		if (ctrlch >= L'a' && ctrlch <= L'z') {
			ctrlch &= ~0x20;
		}
		ctrlch -= L'@';
		if (ctrlch > 0 && ctrlch < 32) {
			*stringBuffer = ctrlch;
			stringBuffer[1] = L'\0';
			return 1;
		}
	}

	if ((*lpName == L'$') || (*lpName == L'@')) {

		khint_t k;

		if (*lpName == L'@') {

			if (lpName[1] == L'@') {
				return GetRun(lpName+2, stringBuffer, STRINGBUFFERMAX);
			} else {
				const struct sExt *ext = bsearch(lpName, getExtensionList,
					lenof(getExtensionList), sizeof(struct sExt), extcmp);
				if (ext != NULL && ext->fn != NULL) {
					if (ext->args == 2) {
						return sbcpy(stringBuffer, ext->fn);
					}
					return ((fnGetExt) ext->fn)(stringBuffer, STRINGBUFFERMAX);
				}
			}
		}

		if (WCSEQ(lpName, L"$#")) {
			return GetArgCount(stringBuffer, STRINGBUFFERMAX);
		}
		if (lpName[1] == L'-') {
			DWORD arg;
			LPWSTR end;
			if (lpName[2] == L'\0') {
				arg = -1;
			} else {
				arg = (DWORD) wcstoul(lpName + 2, &end, 10);
				if (*end != L'\0' || arg == 0) {
					goto dovar;
				}
			}
			return GetArgs(1, arg, stringBuffer, STRINGBUFFERMAX);
		}
		if (lpName[1] >= L'0' && lpName[1] <= L'9') {
			DWORD arg1, arg2;
			LPWSTR end;
			arg1 = (DWORD) wcstoul(lpName + 1, &end, 10);
			if (*end == L'\0') {
				arg2 = arg1;
			} else if (*end == L'-') {
				if (end[1] == L'\0') {
					arg2 = -1;
				} else {
					arg2 = (DWORD) wcstoul(end + 1, &end, 10);
					if (*end != L'\0' || arg2 < arg1) {
						goto dovar;
					}
				}
			} else {
				goto dovar;
			}
			return GetArgs(arg1, arg2, stringBuffer, STRINGBUFFERMAX);
		}

	dovar:
		k = kh_get(wstr, variables, lpName);
		if (k == kh_end(variables)) {
			SetLastError(ERROR_ENVVAR_NOT_FOUND);
			return 0;
		}

		wcscpy(stringBuffer, kh_val(variables, k));
		return (DWORD) wcslen(stringBuffer);
	}

	DWORD len = GetEnvironmentVariable(lpName, stringBuffer, STRINGBUFFERMAX);
	if (len == 0 && *pfEnableExtensions && WCSIEQ(lpName, L"cmdcmdline")) {
		len = sbcpy(stringBuffer, GetCommandLine());
	}
	return len;
}

DWORD ltrim(DWORD length, LPCWSTR delim)
{
	if (length != 0) {
		LPWSTR p = stringBuffer, end = p + length;
		while (wcschr(delim, *p)) {
			if (++p == end) {
				break;
			}
		}
		if (p != stringBuffer) {
			stringBuffer[length] = L'\0';
			wcscpy(stringBuffer, p);
			length -= (DWORD)(p - stringBuffer);
		}
	}
	return length;
}

DWORD rtrim(DWORD length, LPCWSTR delim)
{
	LPWSTR p = stringBuffer;
	while (length > 0 && wcschr(delim, p[length-1])) {
		--length;
	}
	p[length] = L'\0';
	return length;
}

DWORD hexify(DWORD length)
{
	LPWSTR src = stringBuffer, dst = varBuffer;
	for (; length > 0; ++src, --length) {
		if (*src < 0x100) {
			dst += sbprintf(dst, L"%.2X ", *src);
		}
	}
	if (dst == varBuffer) {
		*stringBuffer = L'\0';
		length = 0;
	} else {
		dst[-1] = L'\0';
		length = sbcpy(stringBuffer, varBuffer);
	}
	return length;
}

DWORD unhexify(DWORD length)
{
	LPWSTR src = stringBuffer, dst = varBuffer;
	while (length > 0) {
		if (iswxdigit(src[0]) && iswxdigit(src[1])) {
			swscanf(src, L"%2X", (unsigned int *) dst);
			++dst;
			src += 2;
			length -= 2;
		} else if (src[0] == L'0' && (src[1] == L'x' || src[1] == L'X')
				   && iswxdigit(src[2]) && iswxdigit(src[3])) {
			swscanf(src+2, L"%2X", (unsigned int *) dst);
			++dst;
			src += 4;
			length -= 4;
		} else if (iswspace(*src) || wcschr(L",-.:;", *src) != NULL) {
			++src;
			--length;
		} else {
			*stringBuffer = L'\0';
			return 0;
		}
	}
	*dst = L'\0';
	return sbcpy(stringBuffer, varBuffer);
}

DWORD WINAPI
MyGetEnvironmentVariableW(LPCWSTR lpName, LPWSTR lpBuffer, DWORD nSize)
{
	DWORD length, pad, size;
	BOOL zero, right;
	int extract_key, extract_shift, extract_ctrl, extract_alt;
	LPWSTR mod, end, var, varcpy, spad, padend;

	pad = 0;
	zero = right = FALSE;
	extract_key = extract_shift = extract_ctrl = extract_alt = 0;
	varcpy = NULL;

	var = (LPWSTR) lpName;
	if (lpName != NULL && *lpName != L'\0'
		&& (*lpName != L'@' || lpName[1] != L'@')) {
		var = wcschr(lpName + 1, L'$');
		if (var != NULL && var[1] != L'\0') {
			sbcpy(varBuffer, lpName);
			var += varBuffer - lpName;
			end = varBuffer;
			LPWSTR p = stringBuffer;
			do {
				if (var[1] == L'$') {
					p += sbprintf(p, L"%.*s", var+1 - end, end);
					end = var + 2;
				} else {
					p += sbprintf(p, L"%.*s", var - end, end);
					end = var;
					while (iswalnum(*++end)) {
						// do nothing
					}
					WCHAR tmp = *end;
					*end = L'\0';
					khint_t k = kh_get(wstr, variables, var);
					if (k != kh_end(variables)) {
						p += sbcpy(p, kh_val(variables, k));
					}
					if (tmp == L'/') {
						++end;
					} else {
						*end = tmp;
					}
				}
				var = wcschr(end, L'$');
			} while (var != NULL);
			sbcpy(p, end);
			var = varcpy = _wcsdup(stringBuffer);
		} else {
			var = (LPWSTR) lpName;
		}
		mod = wcschr(var, L';');
		if (mod != NULL) {
			if (varcpy == NULL) {
				var = varcpy = _wcsdup(var);
				mod += var - lpName;
			}
			*mod++ = L'\0';
		}
	} else {
		mod = NULL;
	}

	*stringBuffer = L'\0';
	length = getVar(var);

	while (mod != NULL) {
		if (*mod == L'\'' || *mod == L'"' || *mod == L'`') {
			for (end = mod + 1; *end != L'\0' && *end != *mod; ++end) {
				// do nothing
			}
			if (*end == L'\0') {
				break;
			}
			*end++ = L'\0';
			if (length == 0) {
				length = sbcpy(stringBuffer, mod+1);
			}
			if (*end == L'\0') {
				mod = NULL;
				break;
			} else if (*end != L';') {
				break;
			}
			mod = end + 1;
			continue;
		}
		if (WCSIBEG(mod, L"trim[") ||
			WCSIBEG(mod, L"ltrim[") ||
			WCSIBEG(mod, L"rtrim[") ||
			WCSIBEG(mod, L"capital[")) {
			end = wcschr(mod, L'[');
			if (end[1] == L'\0') {
				break;
			}
			end = wcschr(end + 2, L']');
			if (end == NULL) {
				break;
			}
			*end++ = L'\0';
			if (*end == L'\0') {
				end = NULL;
			} else if (*end != L';') {
				break;
			}
		} else {
			end = wcschr(mod, L';');
		}
		if (end != NULL) {
			*end++ = L'\0';
		}
		if (WCSIEQ(mod, L"upper")) {
			CharUpper(stringBuffer);
		} else if (WCSIEQ(mod, L"lower")) {
			CharLower(stringBuffer);
		} else if (WCSIEQ(mod, L"capital") ||
				   WCSIBEG(mod, L"capital[")) {
			mod += mod[7] ? 6 : 5;
			mod[0] = L' ';
			mod[1] = L'\t';
			CharLower(stringBuffer);
			for (var = stringBuffer; *var; ++var) {
				if (var == stringBuffer || wcschr(mod, var[-1]) != NULL) {
					CharUpperBuff(var, 1);
				}
			}
		} else if (WCSIEQ(mod, L"trim")) {
			length = rtrim(length, L" \t");
			length = ltrim(length, L" \t");
		} else if (WCSIEQ(mod, L"ltrim")) {
			length = ltrim(length, L" \t");
		} else if (WCSIEQ(mod, L"rtrim")) {
			length = rtrim(length, L" \t");
		} else if (WCSIBEG(mod, L"trim[")) {
			length = rtrim(length, mod+5);
			length = ltrim(length, mod+5);
		} else if (WCSIBEG(mod, L"ltrim[")) {
			length = ltrim(length, mod+6);
		} else if (WCSIBEG(mod, L"rtrim[")) {
			length = rtrim(length, mod+6);
		} else if (WCSIEQ(mod, L"length")) {
			length = sbprintf(stringBuffer, L"%d", length);
		} else if (WCSIEQ(mod, L"hexify")) {
			length = hexify(length);
		} else if (WCSIEQ(mod, L"unhexify")) {
			length = unhexify(length);
		} else if (WCSIEQ(mod, L"key")) {
			extract_key = 1;
		} else if (WCSIEQ(mod, L"shift")) {
			extract_shift = 1;
		} else if (WCSIEQ(mod, L"ctrl")) {
			extract_ctrl = 2;
		} else if (WCSIEQ(mod, L"alt")) {
			extract_alt = 4;
		} else if (*mod == L'~') {
			int unused;
			DWORD ext;
			var = (end != NULL) ? end - 1 : mod + wcslen(mod);
			*var = L'?';
			var = stringBuffer;
			if (WCSBEG(lpName, L"$0;~")) {
				wcscpy(var, **pCurrentBatchFile);
			}
			ext = *pfEnableExtensions;
			*pfEnableExtensions = TRUE;
#ifdef _WIN64
			var = pMSCmdVar(NULL, mod, &unused, L"?", &var);
#else
			if CMD_MAJOR_MINOR(>, 6,2) {
				var = ((fastMSCmdVar) pMSCmdVar)(NULL, mod, &unused, L"?", &var);
			} else if CMD_MAJOR_MINOR(==, 6,2) {
				var = ((fastMSCmdVar62) pMSCmdVar)(L"?", NULL, mod, &unused, &var);
			} else {
				var = ((stdMSCmdVar) pMSCmdVar)(NULL, mod, &unused, L"?", &var);
			}
#endif
			*pfEnableExtensions = ext;
			if (var == NULL) {
				break;
			}
			length = sbcpy(stringBuffer, var);
		} else if (*mod == L'-' || (*mod >= L'0' && *mod <= L'9')) {
			spad = mod;
			if (*spad == L'0') {
				zero = TRUE;
				++spad;
			} else if (*spad == L'-') {
				right = TRUE;
				++spad;
			}
			pad = (DWORD) wcstoul(spad, &padend, 10);
			if (*padend != L'\0' || padend == spad) {
				pad = 0;
				break;
			}
		} else {
			break;
		}
		mod = end;
	}
	if (varcpy != NULL) {
		free(varcpy);
	}
	if (mod != NULL) {
		// Unknown modifier: return nothing on the command line; the variable
		// itself in a batch (easier to diagnose).
		if (*pCurrentBatchFile == NULL) {
			return 0;
		}
		pad = 0;
		length = sbcpy(stringBuffer, lpName);
	}

	if (extract_key || extract_shift || extract_ctrl || extract_alt) {
		int is_key, is_shift, is_ctrl, is_alt;
		is_key = is_shift = is_ctrl = is_alt = 0;
		LPWSTR key = stringBuffer;
		if (WCSIBEG(key, L"S+")) {
			key += 2;
			is_shift = 1;
		}
		if (WCSIBEG(key, L"C+")) {
			key += 2;
			is_ctrl = 2;
		}
		if (WCSIBEG(key, L"A+")) {
			key += 2;
			is_alt = 4;
		}
		if (WCSIBEG(key, L"VK_")) {
			is_key = 1;
		}
		if (is_key) {
			int flag = extract_shift | extract_ctrl | extract_alt;
			flag = (flag & (is_shift | is_ctrl | is_alt)) == flag;
			if (flag == 0 || !extract_key) {
				*stringBuffer = flag + L'0';
				stringBuffer[1] = L'\0';
				length = 1;
			} else {
				length = sbprintf(stringBuffer, L"%s%s%s%s",
								  extract_shift ? L"S+" : L"",
								  extract_ctrl	? L"C+" : L"",
								  extract_alt	? L"A+" : L"", key);
			}
		} else {
			*stringBuffer = L'0';
			stringBuffer[1] = L'\0';
			length = 1;
		}
	}

	size = (pad > length) ? pad : length;
	if (size > nSize || lpBuffer == NULL) {
		return size + 1;
	}
	if (pad > length) {
		if (right) {
			wsnprintf(lpBuffer, nSize, L"%-*s", pad, stringBuffer);
		} else {
			wsnprintf(lpBuffer, nSize, L"%*s", pad, stringBuffer);
			if (zero) {
				LPWSTR s = lpBuffer;
				while (*s == L' ') {
					*s++ = L'0';
				}
				if (*s == L'-') {
					*s = L'0';
					*lpBuffer = L'-';
				}
			}
		}
	} else {
		wcscpy(lpBuffer, stringBuffer);
	}
	return size;
}

BOOL WINAPI
MySetEnvironmentVariableW(LPCWSTR lpName, LPCWSTR lpValue)
{
	LPWSTR name = NULL;
	LPWSTR varbuf = varBuffer;
	BOOL append = FALSE;

	if (lpName != NULL) {

		if (lpValue != NULL && *lpValue == L' ') {
			DWORD len = wcslen(lpName);
			if (len > 1 && lpName[len-1] == L' ') {
				lpName = name = _wcsdup(lpName);
				name[len-1] = L'\0';
				++lpValue;
			} else if (len > 2 && lpName[len-2] == L' ') {
				if (lpName[len-1] == L'+' && (*lpName != L'@' ||
					bsearch(lpName, setExtensionList, lenof(setExtensionList),
							sizeof(struct sExt), extcmp) == NULL)) {
					lpName = name = _wcsdup(lpName);
					name[len-2] = L'\0';
					len = MyGetEnvironmentVariableW(name, varBuffer, STRINGBUFFERMAX);
					varbuf += len;
					++lpValue;
					append = TRUE;
				} else if (lpName[len-1] == L':') {
					lpName = name = _wcsdup(lpName);
					name[len-2] = L'\0';
					if (MyGetEnvironmentVariableW(++lpValue, varbuf, STRINGBUFFERMAX) != 0) {
						lpValue = varbuf;
					}
				}
			}
		}

		if (append) {
			if (lpValue != varbuf) {
				sbcpy(varbuf, lpValue);
			}
			lpValue = varBuffer;
		}

		if ((*lpName == L'$') || (*lpName == L'@')) {

			if (*lpName == L'@') {

				const struct sExt *ext = bsearch(lpName, setExtensionList,
					lenof(setExtensionList), sizeof(struct sExt), extcmp);
				if (ext != NULL) {
					BOOL ret;
					if (ext->args == 0) {
						ret = ((fnSetExt) ext->fn)(lpValue == NULL ? 0 : 1, &lpValue);
					} else {
						LPWSTR *szArglist = NULL;
						int nArgs = 0;

						if (lpValue != NULL) {
							szArglist = CommandLineToArgvW(lpValue, &nArgs);
							if (NULL == szArglist) {
								fwprintf(stderr, ArgErrorStr);
								return FALSE;
							}
						}
						if (ext->args != nArgs) {
							fwprintf(stderr, WrongArgsStr, ext->args, nArgs);
							ret = FALSE;
						} else {
							ret = ((fnSetExt) ext->fn)(nArgs, (LPCWSTR *) szArglist);
						}
						if (szArglist != NULL) {
							LocalFree(szArglist);
						}
					}
					return ret;
				}
			}

			if (lpValue == NULL) {
				khint_t k = kh_get(wstr, variables, lpName);
				if (k != kh_end(variables)) {
					free((void *) kh_key(variables, k));
					free((void *) kh_val(variables, k));
					kh_del(wstr, variables, k);
				}
				return TRUE;
			}

			setVar(lpName, lpValue);
			if (name != NULL) {
				free(name);
			}
			return TRUE;
		}
	}

	BOOL ret = SetEnvironmentVariable(lpName, lpValue);

	// XP fails if removing an undefined name, make it succeed.
	if (!ret && lpValue == NULL && GetLastError() == ERROR_ENVVAR_NOT_FOUND) {
		ret = TRUE;
	}

	if (name != NULL) {
		free(name);
	}

	return ret;
}

int WINAPI MyCall(struct cmdnode *node)
{
	if (node->arg != NULL) {
		LPWSTR arg = node->arg;
		while (*arg == L' ' || *arg == L'\t') {
			++arg;
		}
		if (*arg == L'@') {
			int nArgs;
			LPWSTR *szArglist = CommandLineToArgvW(arg, &nArgs);
			if (NULL == szArglist) {
				fwprintf(stderr, ArgErrorStr);
				return *pLastRetCode = EXIT_FAILURE;
			}
			const struct sExt *ext = bsearch(szArglist[0], callExtensionList,
				lenof(callExtensionList), sizeof(struct sExt), extcmp);
			if (NULL == ext) {
				LocalFree(szArglist);
				return *pLastRetCode = EXIT_FAILURE;
			}
			if (nArgs == 2 && WCSEQ(szArglist[1], L"/?")) {
				cmd_printf(L"%s\r\n", ext->help);
				LocalFree(szArglist);
				return *pLastRetCode = EXIT_SUCCESS;
			}
			int ret;
			if (ext->args == 0) {
				do {
					++arg;
				} while (*arg != L' ' && *arg != L'\t' && *arg != L'\0');
				if (*arg != L'\0') {
					do {
						++arg;
					} while (*arg == L' ' || *arg == L'\t');
				}
				ret = ((fnCallExt) ext->fn)(*arg == L'\0' ? 0 : 1, (LPCWSTR *) &arg);
			} else {
				--nArgs;
				if (ext->args < 0 && ~ext->args > nArgs) {
					fwprintf(stderr, MoreArgsStr, ~ext->args, nArgs);
					ret = EXIT_FAILURE;
				} else if (ext->args > 0 && ext->args != nArgs) {
					fwprintf(stderr, WrongArgsStr, ext->args, nArgs);
					ret = EXIT_FAILURE;
				} else {
					ret = ((fnCallExt) ext->fn)(nArgs, (LPCWSTR *) szArglist + 1);
				}
				LocalFree(szArglist);
			}
			return *pLastRetCode = ret;
		}
	}

	return eCall(node);
}

int CallHelp(int argc, LPCWSTR argv[])
{
	void printTitle(LPCWSTR title)
	{
		cmd_printf(L"%s\r\n", title);
		int len = wcslen(title);
		while (len-- > 0) {
			cmd_printf(L"%c", L'=');
		}
		cmd_printf(L"\r\n\r\n");
	}

	BOOL show_all, show_const, show_get, show_set, show_call, show_mod;
	show_all = TRUE;
	show_const = show_get = show_set = show_call = show_mod = FALSE;
	if (argc == 1) {
		if (WCSIEQ(argv[0], L"const")) {
			show_const = TRUE;
			show_all = FALSE;
			argc = 0;
		} else if (WCSIEQ(argv[0], L"get")) {
			show_get = TRUE;
			show_all = FALSE;
			argc = 0;
		} else if (WCSIEQ(argv[0], L"set")) {
			show_set = TRUE;
			show_all = FALSE;
			argc = 0;
		} else if (WCSIEQ(argv[0], L"call")) {
			show_call = TRUE;
			show_all = FALSE;
			argc = 0;
		} else if (WCSIEQ(argv[0], L"mod")) {
			show_mod = TRUE;
			show_all = FALSE;
			argc = 0;
		}
	}

	if (argc == 0) {
		int width = 0;

		void findMaxLen(const struct sExt *ext, int count, int flag)
		{
			const struct sExt *end = ext + count;
			for (; ext < end; ++ext) {
				if (flag == -1 || flag == ext->args
					|| (flag == ~2 && ext->args != 2)) {
					int len = wcslen(ext->name);
					if (len > width) {
						width = len;
					}
				}
			}
		}

		if (show_all) {
			findMaxLen(getExtensionList, lenof(getExtensionList), -1);
			findMaxLen(setExtensionList, lenof(setExtensionList), -1);
			findMaxLen(callExtensionList, lenof(callExtensionList), -1);
			findMaxLen(modExtensionList, lenof(modExtensionList), -1);
		} else if (show_const) {
			findMaxLen(getExtensionList, lenof(getExtensionList), 2);
		} else if (show_get) {
			findMaxLen(getExtensionList, lenof(getExtensionList), ~2);
		} else if (show_set) {
			findMaxLen(setExtensionList, lenof(setExtensionList), -1);
		} else if (show_call) {
			findMaxLen(callExtensionList, lenof(callExtensionList), -1);
		} else if (show_mod) {
			findMaxLen(modExtensionList, lenof(modExtensionList), -1);
		}

		width += 3;
		if (width < 12) {
			width = 12;
		}

		void printBriefHelp(const struct sExt *ext, int count, int flag)
		{
			const struct sExt *end = ext + count;
			for (; ext < end; ++ext) {
				if (flag == -1 || flag == ext->args
					|| (flag == -2 && ext->help != NULL)
					|| (flag == ~2 && ext->args != 2)) {
					cmd_printf(L"%-*s%s\r\n", width, ext->name, ext->brief);
				}
			}
		}

		if (show_all) {
			printTitle(ConstStr);
			printBriefHelp(getExtensionList, lenof(getExtensionList), 2);

			cmd_printf(L"\r\n");
			printTitle(GetSetStr);
			printBriefHelp(getExtensionList, lenof(getExtensionList), 1);

			cmd_printf(L"\r\n");
			printTitle(GetterStr);
			printBriefHelp(getExtensionList, lenof(getExtensionList), 0);

			cmd_printf(L"\r\n");
			printTitle(SetterStr);
			printBriefHelp(setExtensionList, lenof(setExtensionList), -2);

			cmd_printf(L"\r\n");
			printTitle(CallStr);
			printBriefHelp(callExtensionList, lenof(callExtensionList), -1);

			cmd_printf(L"\r\n");
			printTitle(ModStr);
			printBriefHelp(modExtensionList, lenof(modExtensionList), -1);
		} else if (show_const) {
			printBriefHelp(getExtensionList, lenof(getExtensionList), 2);
		} else if (show_get) {
			printBriefHelp(getExtensionList, lenof(getExtensionList), ~2);
		} else if (show_set) {
			printBriefHelp(setExtensionList, lenof(setExtensionList), -1);
		} else if (show_call) {
			printBriefHelp(callExtensionList, lenof(callExtensionList), -1);
		} else if (show_mod) {
			printBriefHelp(modExtensionList, lenof(modExtensionList), -1);
		}

		return EXIT_SUCCESS;
	}

	if (WCSIEQ(argv[0], L"all")) {
		void printHelp(const struct sExt *ext, int count, int flag)
		{
			const struct sExt *end = ext + count;
			for (; ext < end; ++ext) {
				if (ext->help != NULL && (flag < 0 || flag == ext->args)) {
					printTitle(ext->name);
					cmd_printf(L"%s\r\n\r\n", ext->help);
				}
			}
		}

		printHelp(getExtensionList, lenof(getExtensionList), 1);
		printHelp(getExtensionList, lenof(getExtensionList), 0);
		printHelp(setExtensionList, lenof(setExtensionList), -1);
		printHelp(callExtensionList, lenof(callExtensionList), -1);
		printHelp(modExtensionList, lenof(modExtensionList), -1);

		return EXIT_SUCCESS;
	}

	const struct sExt *ext = bsearch(argv[0], getExtensionList,
					lenof(getExtensionList), sizeof(struct sExt), extcmp);
	if (ext == NULL) {
		ext = bsearch(argv[0], setExtensionList,
					lenof(setExtensionList), sizeof(struct sExt), extcmp);
	}
	if (ext == NULL) {
		ext = bsearch(argv[0], modExtensionList,
					lenof(modExtensionList), sizeof(struct sExt), extcmp);
	}
	BOOL var = ext != NULL;
	if (var) {
		cmd_printf(L"%s\r\n", ext->help);
	}
	ext = bsearch(argv[0], callExtensionList,
			lenof(callExtensionList), sizeof(struct sExt), extcmp);
	if (ext == NULL) {
		if (!var) {
			fwprintf(stderr, *argv[0] == L'@' ? NotExtensionStr : NotOptionStr,
					 argv[0]);
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
	if (var) {
		cmd_printf(L"\r\n--------------------\r\n\r\n");
	}
	cmd_printf(L"%s\r\n", ext->help);

	return EXIT_SUCCESS;
}

BOOL DwFlagsForCodepageMustBeZero(UINT CodePage)
{
	if (CodePage == 0) {
		CodePage = GetACP();
	}

	switch (CodePage) {
	case 65001:
	case 65000:
		return TRUE;
	}

	if (CodePage >= 57002 && CodePage <= 57011) {
		return TRUE;
	}

	switch (CodePage) {
	case 50220:
	case 50221:
	case 50222:
	case 50225:
	case 50227:
	case 50229:
	case 42:
	case 54936:
	case 52936: // Undocumented
		return TRUE;
	}

	return FALSE;
}

int WINAPI
MyMultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr,
	int cbMultiByte, LPWSTR lpWideCharStr, int cchWideChar)
{
	if (dwFlags != 0 && DwFlagsForCodepageMustBeZero(CodePage)) {
		dwFlags = 0;
	}

	return MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte,
		lpWideCharStr, cchWideChar);
}

UINT MyLexText(void)
{
	LPWSTR buf = pTmpBuf;

	for (;;) {
		LPWSTR uc, end;
		ULONG wc;
		uc = wcsstr(buf, L"!U+");
		if (uc == NULL) {
			break;
		}
		wc = wcstoul(uc+3, &end, 16);
		if (*end == L'!' && wc < 0x110000) {
			if (wc < 0x10000) {
				*uc++ = wc;
			} else {
				wc -= 0x10000;
				*uc++ = HIGH_SURROGATE_START + (wc >> 10);
				*uc++ = LOW_SURROGATE_START + (wc & 0x3ff);
			}
			wcscpy(uc, end+1);
		} else {
			uc += 3;
		}
		buf = uc;
	}

	return 0x4000;
}

void setBatchLine(DWORD pos)
{
	khint_t k;
	int absent;

	k = kh_put(line, batch_lnums, **pCurrentBatchFile, &absent);
	kh_val(batch_lnums, k) = pos;
	if (pos == 0) {
		last_lnum = 0;
	}
}

DWORD getBatchLine()
{
	khint_t k;
	DWORD pos, lnum;
	LPSTR mem, p;
	char buf[65536];

	if (*pCurrentBatchFile == NULL) {
		return 0;
	}

	k = kh_get(line, batch_lnums, **pCurrentBatchFile);
	if (k == kh_end(batch_lnums)) {
		return 0;
	}
	pos = kh_val(batch_lnums, k);
	mem = readBatchFile(pos, buf, sizeof(buf));
	if (mem == NULL) {
		return 0;
	}
	lnum = 1;
	for (p = mem;; ++p) {
		p = memchr(p, '\n', pos - (DWORD)(p - mem));
		if (p == NULL) {
			break;
		}
		++lnum;
	}
	if (mem != buf) {
		free(mem);
	}
	return lnum;
}

int MyPutStdErrMsg(UINT a, int b, UINT c, va_list *d)
{
	DWORD lnum = (batchfile != 0) ? getBatchLine() : 0;
	if (lnum != 0 && (lnum != last_lnum || last_bat != *pCurrentBatchFile)) {
		LPVOID args = &stringBuffer;
		LPVOID pargs = &args;
		LPCWSTR file = **pCurrentBatchFile;
		if (batchfile == 1) {
			file = wcsrchr(file, L'\\');
			if (file != NULL) {
				++file;
			} else {
				file = **pCurrentBatchFile;
			}
		}
		sbprintf(stringBuffer, L"%s:%lu: ", file, lnum);
#ifdef _WIN64
		pPutMsg(0x2371, b, 1, (va_list *) &pargs);
#else
		if CMD_MAJOR_MINOR(>, 6,2) {
			((fastPutMsg) pPutMsg)(0x2371, b, 1, (va_list *) &pargs);
		} else if CMD_MAJOR_MINOR(==, 6,2) {
			((fastPutMsg62) pPutMsg)(b, (va_list *) &pargs, 0x2371, 1);
		} else {
			((stdPutMsg) pPutMsg)(0x2371, b, 1, (va_list *) &pargs);
		}
#endif
		last_lnum = lnum;
		last_bat = *pCurrentBatchFile;
	}
#ifdef _WIN64
	return pPutMsg(a, b, c, d);
#else
	if CMD_MAJOR_MINOR(>, 6,2) {
		return ((fastPutMsg) pPutMsg)(a, b, c, d);
	} else if CMD_MAJOR_MINOR(==, 6,2) {
		return ((fastPutMsg62) pPutMsg)(b, d, a, c);
	} else {
		return ((stdPutMsg) pPutMsg)(a, b, c, d);
	}
#endif
}

BOOL WINAPI
MyReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead,
		   LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	BOOL ret, utf8file;

	if (AnsiBuf == NULL) {
		// If the read is into a global variable that should be the buffer.
		if (lpBuffer > (LPVOID) GetModuleHandle(NULL) && lpBuffer < cmd_end) {
			AnsiBuf = lpBuffer;
		}
	}

	if (*pCurrentBatchFile != NULL && lpBuffer == AnsiBuf) {
		setBatchLine(SetFilePointer(hFile, 0, NULL, FILE_CURRENT));
	}

	utf8file = (GetConsoleCP() == CP_UTF8 &&
				GetFileType(hFile) == FILE_TYPE_DISK);
	if (utf8file && SetFilePointer(hFile, 0, NULL, FILE_CURRENT) == 0) {
		BYTE bom[3];
		DWORD read;
		if (ReadFile(hFile, bom, 3, &read, NULL) && read == 3 &&
			bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF) {
			// okay, skip the BOM
		} else {
			SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
		}
	}

	ret = ReadFile(hFile, lpBuffer, nNumberOfBytesToRead,
				   lpNumberOfBytesRead, lpOverlapped);

	// Prevent splitting a UTF-8 sequence.
	if (ret && utf8file) {
		BYTE lead = 0x80, mask = 0xC0, *utf8 = lpBuffer;
		DWORD size = *lpNumberOfBytesRead;
		while (size > 0 && (utf8[--size] & 0xC0) == 0x80) {
			lead = (lead >> 1) | 0x80;
			mask = (mask >> 1) | 0x80;
		}
		if (size > 0) {
			if ((utf8[size] & mask) == lead) {
				// okay, complete sequence
			} else if ((utf8[size] & 0xC0) != 0xC0) {
				// invalid sequence, leave it be
			} else {
				// incomplete sequence, read it next time
				SetFilePointer(hFile, size - *lpNumberOfBytesRead, NULL,
							   FILE_CURRENT);
				*lpNumberOfBytesRead = size;
			}
		}
	}

	// Check for string continuation: if this line ends with '"^+' and the next
	// line starts with '"' (ignoring space and tab) join the lines together by
	// removing the quotes and everything between.	Fill the discarded space
	// with CR so the file offset of the next line remains valid.
	if (lpBuffer == AnsiBuf && ret) {
		DWORD pos, size = *lpNumberOfBytesRead, discarded = 0;
		DWORD end_quote;
		LPSTR buffer = lpBuffer;
		for (pos = 0; pos < size; ++pos) {
			if (buffer[pos] == '\n') {
				end_quote = pos;
				if (end_quote > 0 && buffer[end_quote-1] == '\r') {
					--end_quote;
				}
				end_quote -= 3;
				if ((int) end_quote < 0 || memcmp(buffer+end_quote, "\"^+", 3) != 0) {
					if (discarded != 0) {
						end_quote += 3;
						discarded += pos - end_quote + 1;
						memset(buffer + end_quote, '\r', discarded - 1);
						buffer[end_quote+discarded-1] = '\n';
					}
					break;
				}
				while (++pos < size) {
					if (buffer[pos] == ' ' || buffer[pos] == '\t') {
						continue;
					}
					if (buffer[pos] == '"') {
						++pos;
						memcpy(buffer + end_quote, buffer + pos, size - pos);
						DWORD len = pos - end_quote;
						discarded += len;
						size -= len;
						pos -= len + 1;
					}
					break;
				}
			}
		}

		// Treat a line starting with '::' (igoring space and tab) as a real
		// comment and blank it out (unless it ends with a caret).
		for (pos = 0; pos < size; ++pos) {
			if (buffer[pos] == ' ' || buffer[pos] == '\t') {
				continue;
			}
			if (buffer[pos] == ':' && buffer[pos+1] == ':') {
				LPSTR end = memchr(buffer + pos, '\n', size - pos);
				if (end != NULL) {
					if (end[-1] == '\r') {
						--end;
					}
					if (end[-1] != '^') {
						memset(buffer + pos, '\r', end - (buffer + pos));
					}
				}
			}
			break;
		}
	}

	return ret;
}

BOOL findRange(LPCWSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData)
{
	if (WCSIBEG(lpFileName, L":range*")) {
		LPCWSTR paramsLine = lpFileName + 7;
		int args, arg[3];
		struct sFor *it = (struct sFor *) lpFindFileData->cFileName;
		findForStack[++findForStackTop] = lpFindFileData;
		ZeroMemory(lpFindFileData, sizeof(WIN32_FIND_DATA));
		it->type = forRange;
		it->step = 1;
		args = swscanf(paramsLine, L"%d:%d:%d", &arg[0], &arg[1], &arg[2]);
		if (args == 3) {
			it->start = arg[0];
			it->stop = arg[1];
			it->step = arg[2];
		} else if (args == 2) {
			it->start = arg[0];
			it->stop = arg[1];
			if (it->start > it->stop)
				it->step = -1;
		} else if (args == 1) {
			it->stop = arg[0];
			if (it->stop == 0) {
				it->start = 0;
			} else if (it->stop > 0) {
				it->start = 1;
			} else {
				it->start = -1;
				it->step = -1;
			}
		} else {
			it->start = 1;
			it->stop = 0x7fffffff;
		}
		_itow(it->start, it->szValue, 10);
		return TRUE;
	}
	return FALSE;
}

BOOL findInfinite(LPCWSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData)
{
	if (WCSEQ(lpFileName, L":*")) {
		struct sFor *it = (struct sFor *) lpFindFileData->cFileName;
		findForStack[++findForStackTop] = lpFindFileData;
		ZeroMemory(lpFindFileData, sizeof(WIN32_FIND_DATA));
		it->type = forInfinite;
		it->szValue[0] = L'\u221E';
		it->szValue[1] = L'\0';
		return TRUE;
	}
	return FALSE;
}

BOOL findFor(LPCWSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData)
{
	if (*lpFileName == L':' && findForStackTop < FINDFOR_STACKSIZE - 1) {
		if (findInfinite(lpFileName, lpFindFileData)) {
			return TRUE;
		}
		if (findRange(lpFileName, lpFindFileData)) {
			return TRUE;
		}
	}
	return FALSE;
}

HANDLE WINAPI
MyFindFirstFileExW(LPCWSTR lpFileName,
				   FINDEX_INFO_LEVELS fInfoLevelId,
				   LPVOID lpFindFileData,
				   FINDEX_SEARCH_OPS fSearchOp,
				   LPVOID lpSearchFilter,
				   DWORD dwAdditionalFlags)
{
	if (findFor(lpFileName, lpFindFileData)) {
		return (HANDLE) 1;
	}
	return FindFirstFileExW(lpFileName, fInfoLevelId, lpFindFileData, fSearchOp,
							lpSearchFilter, dwAdditionalFlags);
}

HANDLE WINAPI
MyFindFirstFileW(LPCWSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData)
{
	if (findFor(lpFileName, lpFindFileData)) {
		return (HANDLE) 1;
	}
	return FindFirstFileW(lpFileName, lpFindFileData);
}

BOOL WINAPI
MyFindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData)
{
	if (findForStackTop >= 0 && lpFindFileData == findForStack[findForStackTop]) {
		struct sFor *it = (struct sFor *) lpFindFileData->cFileName;

		if (it->type == forInfinite) {
			return TRUE;
		}
		if (it->type == forRange) {
			int before = it->start;
			it->start += it->step;
			if (it->step < 0) {
				if (it->start < it->stop || it->start > before) {
					--findForStackTop;
					return FALSE;
				}
			} else {
				if (it->start > it->stop || it->start < before) {
					--findForStackTop;
					return FALSE;
				}
			}
			_itow(it->start, it->szValue, 10);
			return TRUE;
		}
	}
	return FindNextFileW(hFindFile, lpFindFileData);
}

BOOL Next(int argc, LPCWSTR argv[])
{
	if (findForStackTop == -1) {
		return FALSE;
	}
	if (argc == 1) {
		struct sFor *it = (struct sFor *) findForStack[findForStackTop]->cFileName;
		if (it->type == forRange) {
			it->start = _wtoi(*argv) - it->step;
		} else {
			return FALSE;
		}
	} else {
		--findForStackTop;
	}
	return TRUE;
}

BOOL fPipeSet;
LPWSTR pipeVar;

FILE *My_popen(LPCSTR command, LPCSTR mode)
{
	fPipeSet = FALSE;
	LPCSTR p = command;
	while (*p == ' ' || *p == '\t') {
		++p;
	}
	if (STRIBEG(p, "set")
		&& (p[3] == '\0' || p[3] == ' ' || p[3] == '\t' || p[3] == '|')) {
		p += 3;
		while (*p == ' ' || *p == '\t') {
			++p;
		}
		if (*p == '\0' || *p == '|') {
			fPipeSet = TRUE;
		} else if (*p == '$' || *p == '@') {
			LPCSTR beg = p;
			while (*p != '\0' && *p != ' ' && *p != '\t' && *p != '|') {
				++p;
			}
			int len = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, beg, p - beg,
										  stringBuffer, STRINGBUFFERMAX);
			stringBuffer[len] = L'\0';
			pipeVar = _wcsdup(stringBuffer);
			fPipeSet = TRUE;
		}
		if (fPipeSet) {
			enumVars(pipeVar, EV_ADD);
		}
	}

	return _popen(command, mode);
}

FILE *My_wpopen(LPCWSTR command, LPCWSTR mode)
{
	fPipeSet = FALSE;
	LPCWSTR p = command;
	while (*p == L' ' || *p == L'\t') {
		++p;
	}
	if (WCSIBEG(p, L"set")
		&& (p[3] == L'\0' || p[3] == L' ' || p[3] == L'\t' || p[3] == L'|')) {
		p += 3;
		while (*p == L' ' || *p == L'\t') {
			++p;
		}
		if (*p == L'\0' || *p == L'|') {
			fPipeSet = TRUE;
		} else if (*p == L'$' || *p == L'@') {
			LPCWSTR beg = p;
			while (*p != L'\0' && *p != L' ' && *p != L'\t' && *p != L'|') {
				++p;
			}
			pipeVar = malloc(WSZ(p - beg + 1));
			wsncpy(pipeVar, p - beg + 1, beg);
			fPipeSet = TRUE;
		}
		if (fPipeSet) {
			enumVars(pipeVar, EV_ADD);
		}
	}

	return _wpopen(command, mode);
}

int My_pclose(FILE *stream)
{
	if (fPipeSet) {
		enumVars(pipeVar, EV_REMOVE);
		if (pipeVar != NULL) {
			free(pipeVar);
			pipeVar = NULL;
		}
	}

	return _pclose(stream);
}

DWORD WINAPI
FreeLibraryThread(LPVOID param)
{
	Sleep(10);	// give CALLs a chance to return
	FreeLibraryAndExitThread(hDllInstance, 0);
	return 0;
}

int CallUnload(int argc, LPCWSTR argv[])
{
	unhook();
	SafeCloseHandle(CreateThread(NULL, 4096, FreeLibraryThread, NULL, 0, NULL));
	return EXIT_SUCCESS;
}

BOOL WINAPI
MyCmdBatNotification(BOOL start)
{
	BOOL rc = CmdBatNotification(start);

	if (start) {
		global = TRUE;
	} else if (!global) {
		unhook();
		SafeCloseHandle(CreateThread(NULL, 4096, FreeLibraryThread, NULL, 0, NULL));
	} else {
		kh_clear(line, batch_lnums);
		doneCmdBat();
		// Batch has exited, stop the elevated process, restoring the prompt.
		CallElevate(-1, NULL);
	}

	return rc;
}

// ========== Hooking API functions
//
// References about API hooking (and dll injection):
// - Matt Pietrek ~ Windows 95 System Programming Secrets.
// - Jeffrey Richter ~ Programming Applications for Microsoft Windows 4th ed.

typedef struct {
	LPCSTR name;
	DWORD_PTR newfunc;
	DWORD_PTR oldfunc;
} HookFn, *PHookFn;

void HookThunks(PHookFn Hooks,
				PIMAGE_DOS_HEADER pDosHeader,
				PIMAGE_THUNK_DATA pThunk,
				PIMAGE_THUNK_DATA pNameThunk)
{
	PHookFn pHook;

	// Blast through the table of import names
	while (pNameThunk->u1.AddressOfData != 0) {
		PIMAGE_IMPORT_BY_NAME pName = MakeVA(PIMAGE_IMPORT_BY_NAME,
				pNameThunk->u1.AddressOfData);
		LPCSTR name = (LPCSTR) pName->Name;
		for (pHook = Hooks; pHook->name; ++pHook) {
			if (STREQ(name, pHook->name)) {		// We found it!
				DWORD flOldProtect, flDummy;

				// Change the access protection on the region of committed pages in the
				// virtual address space of the current process
				VirtualProtect(&pThunk->u1.Function, sizeof(PVOID),
						PAGE_READWRITE, &flOldProtect);

				// Overwrite the original address with the address of the new function
				if (pHook->oldfunc != 0) {
					pThunk->u1.Function = pHook->oldfunc;
				} else {
					pHook->oldfunc = pThunk->u1.Function;
					pThunk->u1.Function = pHook->newfunc;
				}

				// Put the page attributes back the way they were.
				VirtualProtect(&pThunk->u1.Function, sizeof(PVOID),
						flOldProtect, &flDummy);
			}
		}
		pThunk++;	// Advance to next imported function address
		pNameThunk++;
	}
}

//-----------------------------------------------------------------------------
//   HookAPIOneMod
// Substitute a new function in the Import Address Table (IAT) of the
// specified module.
//-----------------------------------------------------------------------------

void HookAPIOneMod(HMODULE hFromModule, // Handle of the module to intercept calls from
				   PHookFn Hooks)		// Functions to replace
{
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNTHeader;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
	PIMAGE_THUNK_DATA pThunk, pNameThunk;
	DWORD rva;

	pDosHeader = (PIMAGE_DOS_HEADER) hFromModule;
	pNTHeader = MakeVA(PIMAGE_NT_HEADERS, pDosHeader->e_lfanew);

	// Get a pointer to the module's imports section
	rva = pNTHeader->OptionalHeader
					 .DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]
					 .VirtualAddress;
	// Bail out if the RVA of the imports section is 0 (it doesn't exist)
	if (rva == 0) {
		return;
	}
	pImportDesc = MakeVA(PIMAGE_IMPORT_DESCRIPTOR, rva);

	// Iterate through the array of imported module descriptors, looking
	// for module names of interest.
	for (; pImportDesc->Name != 0; pImportDesc++) {
		PSTR pszModName = MakeVA(PSTR, pImportDesc->Name);
		if (STRIEQ(pszModName, "kernel32.dll")
			|| STRIEQ(pszModName, "msvcrt.dll")
			|| STRIBEG(pszModName, "API-MS-Win-Core-ProcessEnvironment-")
			|| STRIBEG(pszModName, "API-MS-Win-Core-String-")
			|| STRIBEG(pszModName, "API-MS-Win-Core-File-")) {

			// Get a pointer to the found module's import address table (IAT)
			pThunk = MakeVA(PIMAGE_THUNK_DATA, pImportDesc->FirstThunk);
			pNameThunk = MakeVA(PIMAGE_THUNK_DATA,
					pImportDesc->OriginalFirstThunk);
			HookThunks(Hooks, pDosHeader, pThunk, pNameThunk);
		}
	}
}

void HookAPIDelayMod(HMODULE hFromModule, // Handle of the module to intercept calls from
					 PHookFn Hooks) 	  // Functions to replace
{
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNTHeader;
	PIMAGE_DELAYLOAD_DESCRIPTOR pImportDesc;
	PIMAGE_THUNK_DATA pThunk, pNameThunk;
	DWORD rva;

	pDosHeader = (PIMAGE_DOS_HEADER) hFromModule;
	pNTHeader = MakeVA(PIMAGE_NT_HEADERS, pDosHeader->e_lfanew);

	// Only interested in CmdBatNotification, which is not delayed until 6.2.
	// 5.0 uses virtual addresses (not relative), so avoid testing that by
	// checking the OS version in the header (CMD version hasn't been set yet).
	if (pNTHeader->OptionalHeader.MajorOperatingSystemVersion < 6 ||
		(pNTHeader->OptionalHeader.MajorOperatingSystemVersion == 6 &&
		 pNTHeader->OptionalHeader.MinorOperatingSystemVersion < 2)) {
		return;
	}

	// Get a pointer to the module's imports section
	rva = pNTHeader->OptionalHeader
					 .DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT]
					 .VirtualAddress;
	// Bail out if the RVA of the delayed imports section is 0 (it doesn't exist)
	if (rva == 0) {
		return;
	}
	pImportDesc = MakeVA(PIMAGE_DELAYLOAD_DESCRIPTOR, rva);

	// Iterate through the array of imported module descriptors, looking
	// for module names of interest.
	for (; pImportDesc->DllNameRVA != 0; pImportDesc++) {
		PSTR pszModName = MakeVA(PSTR, pImportDesc->DllNameRVA);
		if (STRIBEG(pszModName, "ext-ms-win-cmd-util-")) {

			// Get a pointer to the found module's import address table (IAT)
			pThunk = MakeVA(PIMAGE_THUNK_DATA,
					pImportDesc->ImportAddressTableRVA);
			pNameThunk = MakeVA(PIMAGE_THUNK_DATA,
					pImportDesc->ImportNameTableRVA);
			HookThunks(Hooks, pDosHeader, pThunk, pNameThunk);
			break;
		}
	}
}

// ========== Initialisation

HookFn *Hooks, AllHooks[] = {
	// This is expected first!
	{ "MultiByteToWideChar",     (DWORD_PTR) MyMultiByteToWideChar,     0 },
	// This is expected second!
	{ "CmdBatNotification",      (DWORD_PTR) MyCmdBatNotification,      0 },
	{ "GetEnvironmentVariableW", (DWORD_PTR) MyGetEnvironmentVariableW, 0 },
	{ "SetEnvironmentVariableW", (DWORD_PTR) MySetEnvironmentVariableW, 0 },
	{ "ReadFile",                (DWORD_PTR) MyReadFile,                0 },
	{ "FindFirstFileExW",        (DWORD_PTR) MyFindFirstFileExW,        0 },
	{ "FindNextFileW",           (DWORD_PTR) MyFindNextFileW,           0 },
	{ "FindFirstFileW",          (DWORD_PTR) MyFindFirstFileW,          0 },
	{ "_popen",                  (DWORD_PTR) My_popen,                  0 },
	{ "_wpopen",                 (DWORD_PTR) My_wpopen,                 0 },
	{ "_pclose",                 (DWORD_PTR) My_pclose,                 0 },
	{ NULL, 0, 0 },
},

DelayedHooks[] = {
	{ "CmdBatNotificationStub",  (DWORD_PTR) MyCmdBatNotification,      0 },
	{ NULL, 0, 0 },
};

void hook(HMODULE hInstance)
{
	hDllInstance = hInstance;
	winMajor = LOBYTE(GetVersion());

	hookCmd();

	Hooks = AllHooks;
	if (winMajor > 5) {
		// No need for the MultiByteToWideChar patch.
		++Hooks;
	}
	HMODULE cmd = GetModuleHandle(NULL);
	HookAPIOneMod(cmd, Hooks);
	if (AllHooks[1].oldfunc != 0) {
		HookAPIOneMod(hInstance, Hooks);
	} else {
		// CmdBatNotification is delay-loaded as CmdBatNotificationStub,
		// so point our own CmdBatNotification to the stub, too.
		HookAPIDelayMod(cmd, DelayedHooks);
		if (DelayedHooks[0].oldfunc >= (DWORD_PTR) cmd &&
			DelayedHooks[0].oldfunc < (DWORD_PTR) cmd_end) {
			// Loading from the command line before running any batch means
			// it hasn't been imported yet, so can't hook, just replace.
			AllHooks[1].oldfunc = (DWORD_PTR) GetProcAddress(GetModuleHandle(L"kernel32.dll"),
														"CmdBatNotification");
		} else {
			AllHooks[1].oldfunc = DelayedHooks[0].oldfunc;
		}
		HookAPIOneMod(hInstance, Hooks);
	}

	variables = kh_init(wstr);
	batch_lnums = kh_init(line);
	onWindowsTerminal = GetEnvironmentVariable(L"WT_SESSION", NULL, 0) != 0;
}

void unhook(void)
{
	khint_t k;

	CallElevate(-1, NULL);	// stop the elevated process

	// Wait for speech to finish before exiting.
	WaitForSingleObject(hSpeaking, INFINITE);
	uninitCo();
	unload_delayed();

	HookAPIOneMod(GetModuleHandle(NULL), Hooks);
	HookAPIDelayMod(GetModuleHandle(NULL), DelayedHooks);

	for (k = 0; k < kh_end(variables); ++k) {
		if (kh_exist(variables, k)) {
			free((void *) kh_key(variables, k));
			free((void *) kh_val(variables, k));
		}
	}
	kh_destroy(wstr, variables);
	variables = NULL;
	kh_destroy(line, batch_lnums);

	unhookCmd();

	SafeCloseHandle(consoleOutput);
}

BOOL WINAPI
_dllstart(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) {
		PBYTE base = (PBYTE) GetModuleHandle(NULL);
		GetModuleFileName(hDll, enh_dll, lenof(enh_dll));
		// RunDLL & RegSvr32 are GUI, CMD is console.
		PIMAGE_NT_HEADERS phdr = (PIMAGE_NT_HEADERS)(base + *(LPDWORD)(base + 0x3C));
		if (phdr->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_CUI) {
			DisableThreadLibraryCalls(hDll);
			hook(hDll);
			handleElevation();
		}
	} else if (dwReason == DLL_PROCESS_DETACH) {
		if (variables != NULL) {
			unhook();
		}
	}

	return TRUE;
}

// Search each process in the snapshot for id.
BOOL find_proc_id(HANDLE snap, DWORD id, LPPROCESSENTRY32 pe,
		LPPROCESSENTRY32 ppe)
{
	BOOL fOk;

	pe->dwSize = sizeof(PROCESSENTRY32);
	for (fOk = Process32First(snap, pe); fOk; fOk = Process32Next(snap, pe)) {
		if (pe->th32ProcessID == id)
			break;
		*ppe = *pe;
	}

	return fOk;
}

// Obtain the process identifier of the parent process; verify the architecture.
DWORD GetParentProcessId()
{
	HANDLE hSnap;
	PROCESSENTRY32 pe, ppe;
	BOOL parent_wow64, me_wow64;
	BOOL (WINAPI *pIsWow64Process)(HANDLE, PBOOL);

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return 0;
	}

	if (!find_proc_id(hSnap, GetCurrentProcessId(), &pe, &ppe)) {
		return 0;
	}
	if (ppe.th32ProcessID == pe.th32ParentProcessID) {
		pe = ppe;
	} else if (!find_proc_id(hSnap, pe.th32ParentProcessID, &pe, &ppe)) {
		return 0;
	}

	CloseHandle(hSnap);

	GETPROC(GetModuleHandle(L"kernel32.dll"), IsWow64Process);
	if (pIsWow64Process != NULL) {
		HANDLE ph = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe.th32ProcessID);
		if (ph == NULL) {
			return 0;
		}
		pIsWow64Process(ph, &parent_wow64);
		pIsWow64Process(GetCurrentProcess(), &me_wow64);
		CloseHandle(ph);

		if (parent_wow64 != me_wow64) {
			return -1;
		}
	}

	return pe.th32ProcessID;
}

// Determine if EB is already installed in the parent.
BOOL IsInstalled(DWORD id, LPWSTR name, PBYTE *base)
{
	HANDLE hModuleSnap;
	MODULEENTRY32 me;
	BOOL fOk;
	LPWSTR enh_name;

	// Take a snapshot of all modules in the current process.
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id);

	if (hModuleSnap == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	// Fill the size of the structure before using it.
	me.dwSize = sizeof(MODULEENTRY32);

	// Get the name of the DLL.
	enh_name = wcsrchr(enh_dll, L'\\') + 1;

	// Walk the module list of the modules.
	fOk = Module32First(hModuleSnap, &me);
	if (fOk) {
		// First module is the process, get its details.
		wcscpy(name, me.szExePath);
		*base = me.modBaseAddr;
		while ((fOk = Module32Next(hModuleSnap, &me))) {
			if (WCSIEQ(me.szModule, enh_name)) {
				break;
			}
		}
	}
	CloseHandle(hModuleSnap);

	return fOk;
}

// Inject code into the target process to load our DLL.
void Inject(HANDLE hProcess)
{
	DWORD len;
	LPVOID mem;
	LPVOID LLW;
	HANDLE thread;

	len = (DWORD) wcslen(enh_dll);

	LLW = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
	mem = VirtualAllocEx(hProcess, NULL, len, MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, mem, enh_dll, WSZ(len), NULL);
	thread = CreateRemoteThread(hProcess, NULL, 4096, LLW, mem, 0, NULL);
	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);
	VirtualFreeEx(hProcess, mem, 0, MEM_RELEASE);
}

BOOL IsSupported(HANDLE ph, LPCWSTR name, PBYTE base)
{
	const struct sCMD *ver;
	WCHAR eol;
	VS_FIXEDFILEINFO *pfi;
	struct LANGANDCODEPAGE {
		WORD language;
		WORD codepage;
	} *lang;
	WCHAR subblock[64];
	LPWSTR originalname;
	char vi[4096];
	UINT len;
	SIZE_T written;

	if (!GetFileVersionInfo(name, 0, sizeof(vi), vi)
		|| !VerQueryValue(vi, L"\\", (LPVOID *) &pfi, &len)) {
		return FALSE;
	}
	for (ver = cmd_versions; ver->offsets; ++ver) {
		if (ver->verMS == pfi->dwFileVersionMS
			&& ver->verLS == pfi->dwFileVersionLS
			&& ReadProcessMemory(ph, base + *ver->offsets, &eol, 2, &written)
			&& eol == L';') {
			return TRUE;
		}
	}
	VerQueryValue(vi, L"\\VarFileInfo\\Translation", (LPVOID *) &lang, &len);
	wsnprintf(subblock, lenof(subblock),
			  L"\\StringFileInfo\\%04x%04x\\OriginalFilename",
			  lang->language, lang->codepage);
	if (VerQueryValue(vi, subblock, (LPVOID *) &originalname, &len)
		&& (WCSEQ(originalname, L"Cmd.Exe") ||
			WCSEQ(originalname, L"Cmd.Exe.MUI"))) {
		cmdFileVersionMS = pfi->dwFileVersionMS;
		cmdFileVersionLS = pfi->dwFileVersionLS;
		cmdDebug = pfi->dwFileFlags & VS_FF_DEBUG;
	}
	return FALSE;
}

static BOOL quiet;

void checkArgs(void)
{
	int nArgs = 0;
	LPWSTR *szArglist = CommandLineToArgvW(GetCommandLine(), &nArgs);
	if (NULL != szArglist) {
		while (--nArgs > 0) {
			if (WCSIEQ(szArglist[nArgs], L"/s")) {
				quiet = TRUE;
				break;
			}
		}
	}
}

void Info(LPCWSTR msg)
{
	if (!quiet) {
		MessageBox(NULL, msg, ProgramNameStr, MB_OK | MB_ICONERROR);
	}
}

HRESULT DllLoad(void)
{
	DWORD cmdpid = GetParentProcessId();
	WCHAR cmdname[MAX_PATH];
	PBYTE cmdbase = NULL;	// remove a gcc warning

	checkArgs();

	if (cmdpid == 0) {
		Info(ParentErrStr);
	} else if (cmdpid == (DWORD)-1) {
		Info(ArchErrStr);
	} else if (!IsInstalled(cmdpid, cmdname, &cmdbase)) {
		HANDLE ph = OpenProcess(PROCESS_ALL_ACCESS, FALSE, cmdpid);
		if (ph != NULL) {
			if (IsSupported(ph, cmdname, cmdbase)) {
				Inject(ph);
			} else {
				if (cmdFileVersionMS == 0) {
					sbprintf(stringBuffer, NotCmdStr, cmdname);
				} else {
					GetCmdVersion(varBuffer, STRINGBUFFERMAX);
					sbprintf(stringBuffer, NotSupportedStr, varBuffer);
				}
				Info(stringBuffer);
			}
			CloseHandle(ph);
		} else {
			Info(ParentErrStr);
		}
	}

	return S_OK;
}

Export_DllLoad_Entrypoint(load);
Export_DllLoad_Entrypoint(Load);
// Default entry point of regsvr32 used only as a method to load the DLL into CMD
Export_DllLoad_Entrypoint(DllRegisterServer);

__declspec(dllexport)
VOID /*CALLBACK*/	// CALLBACK exports ElevateW@16
ElevateW(HWND hwnd, HINSTANCE ModuleHandle, LPCWSTR CmdLineBuffer, INT nCmdShow)
{
	WCHAR name[64];

	LPCWSTR key = wcsrchr(CmdLineBuffer, L' ');
	if (key == NULL) {
		return;
	}
	++key;
	wsnprintf(name, lenof(name), L"EB-elevate-event-%s", key);
	HANDLE event = OpenEvent(EVENT_ALL_ACCESS, FALSE, name);
	if (event == NULL) {
		return;
	}
	for (;;) {
		WaitForSingleObject(event, INFINITE);
		wsnprintf(name, lenof(name), L"EB-elevate-data-%s", key);
		HANDLE map = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, name);
		if (map == NULL) {
			// No map means the batch is finished, prompt again next time.
			return;
		}
		struct sElevate *data = MapViewOfFile(map, FILE_MAP_WRITE, 0, 0, 0);
		if (data != NULL) {
			STARTUPINFO si = { cb: sizeof(si) };

			if (data->console_pid != 0) {
				si.dwFlags = STARTF_USESHOWWINDOW;
				si.wShowWindow = SW_HIDE;
			}
			PROCESS_INFORMATION pi;
			SetEnvironmentVariable(L"~EB-elevate-key", key);
			if (CreateProcess(NULL, data->cmdline, NULL, NULL, TRUE,
							  CREATE_NEW_CONSOLE | CREATE_SUSPENDED
							  | CREATE_UNICODE_ENVIRONMENT,
							  NULL, data->curdir, &si, &pi)) {
				data->elevated_pid = pi.dwProcessId;
				Inject(pi.hProcess);
				ResumeThread(pi.hThread);
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
			}
			UnmapViewOfFile(data);
		}
		CloseHandle(map);
		// If the key ends in '-' it's from the command line, which should
		// always prompt.
		if (key[wcslen(key)-1] == L'-') {
			return;
		}
	}
}

void handleElevation(void)
{
	WCHAR name[64], key[32];

	if (GetEnvironmentVariable(L"~EB-elevate-key", key, lenof(key)) == 0) {
		return;
	}
	SetEnvironmentVariable(L"~EB-elevate-key", NULL);
	wsnprintf(name, lenof(name), L"EB-elevate-data-%s", key);
	HANDLE map = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, name);
	if (map == NULL) {
		return;
	}

	struct sElevate *data = MapViewOfFile(map, FILE_MAP_WRITE, 0, 0, 0);
	if (data == NULL) {
		CloseHandle(map);
		return;
	}

	if (data->console_pid != 0) {
		typedef BOOL (WINAPI *fnAttachConsole)(DWORD);
		fnAttachConsole pAttachConsole = (fnAttachConsole)
			(VOID *)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "AttachConsole");
		if (pAttachConsole != NULL) {
			FreeConsole();
			pAttachConsole(data->console_pid);
		}
	}

	LPWSTR env = data->env;
	while (*env != L'\0') {
		LPWSTR var = wcschr(env + 1, L'=');
		*var++ = L'\0';
		if (GetEnvironmentVariable(env, NULL, 0) == 0) {
			SetEnvironmentVariable(env, var);
		}
		env = var + wcslen(var) + 1;
	}

	// Let the caller know we've finished using it.
	*data->env = L'\0';

	UnmapViewOfFile(data);
	CloseHandle(map);
}
