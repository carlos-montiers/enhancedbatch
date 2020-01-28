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
	int args;			// getters use this as a help flag for setter, too
	LPVOID fn;
	LPCWSTR brief, help;
};

#define HELPSTR(ext) ext##BriefStr, ext##HelpStr

const struct sExt getExtensionList[] = {
	{ L"@attr",             1, GetAttributes, HELPSTR(Attr) },
	{ L"@batchfile",        1, GetBatchFile, HELPSTR(BatchFile) },
	{ L"@batchline",        0, GetBatchLine, HELPSTR(BatchLine) },
	{ L"@bg",               1, GetBackground, HELPSTR(Background) },
	{ L"@chhit",            0, Chhit, HELPSTR(Readchar) },
	{ L"@cmdversion",       0, GetCmdVersion, HELPSTR(CmdVersion) },
	{ L"@color",            1, GetColor, HELPSTR(Color) },
	{ L"@column",           1, GetColumn, HELPSTR(Column) },
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
	{ L"@echo",             1, GetEcho, HELPSTR(Echo) },
	{ L"@english",          1, GetEnglish, HELPSTR(English) },
	{ L"@enhancedbatch",    0, GetEnhancedBatch, HELPSTR(EnhancedBatch) },
	{ L"@extensions",       1, GetExtensions, HELPSTR(Extensions) },
	{ L"@fg",               1, GetForeground, HELPSTR(Foreground) },
	{ L"@forline",          0, GetForLine, HELPSTR(ForLine) },
	{ L"@forlines",         0, GetForLines, HELPSTR(ForLines) },
	{ L"@getch",            0, Getch, HELPSTR(Getchar) },
	{ L"@getkb",            0, Getkb, HELPSTR(Getkey) },
	{ L"@height",           0, GetHeight, HELPSTR(Height) },
	{ L"@hour",             0, GetHour, HELPSTR(Hour) },
	{ L"@inputcp",          1, GetInputCodePage, HELPSTR(InputCodePage) },
	{ L"@iselevated",       0, GetElevated, HELPSTR(Elevated) },
	{ L"@kbhit",            0, Kbhit, HELPSTR(Readkey) },
	{ L"@milliseconds",     0, GetMilliseconds, HELPSTR(Milliseconds) },
	{ L"@minute",           0, GetMinute, HELPSTR(Minute) },
	{ L"@month",            0, GetMonth, HELPSTR(Month) },
	{ L"@monthname",        0, GetMonthName, HELPSTR(MonthName) },
	{ L"@monthshort",       0, GetMonthShort, HELPSTR(MonthShort) },
	{ L"@opacity",          1, GetOpacity, HELPSTR(Opacity) },
	{ L"@osbuild",          0, GetOSBuild, HELPSTR(OSBuild) },
	{ L"@osmajor",          0, GetOSMajor, HELPSTR(OSMajor) },
	{ L"@osminor",          0, GetOSMinor, HELPSTR(OSMinor) },
	{ L"@osrevision",       0, GetOSRevision, HELPSTR(OSRevision) },
	{ L"@osversion",        0, GetOSVersion, HELPSTR(OSVersion) },
	{ L"@outputcp",         1, GetOutputCodePage, HELPSTR(OutputCodePage) },
	{ L"@position",         1, GetPosition, HELPSTR(Position) },
	{ L"@row",              1, GetRow, HELPSTR(Row) },
	{ L"@second",           0, GetSecond, HELPSTR(Second) },
	{ L"@size",             0, GetSize, HELPSTR(Size) },
	{ L"@stderr",           0, GetStderr, HELPSTR(Stderr) },
	{ L"@stdin",            0, GetStdin, HELPSTR(Stdin) },
	{ L"@stdout",           0, GetStdout, HELPSTR(Stdout) },
	{ L"@tempdir",          0, GetTempDir, HELPSTR(TempDir) },
	{ L"@tempfile",         0, GetTempFile, HELPSTR(TempFile) },
	{ L"@time",             0, GetTime, HELPSTR(Time) },
	{ L"@timems",           0, GetTimems, HELPSTR(Timems) },
	{ L"@timer",            0, GetTimer, HELPSTR(TimerVar) },
	{ L"@timerhi",          0, GetHiTimer, HELPSTR(HiTimer) },
	{ L"@title",            0, GetTitle, HELPSTR(Title) },
	{ L"@transient",        0, GetTransient, HELPSTR(Transient) },
	{ L"@ul",               1, GetUnderline, HELPSTR(Underline) },
	{ L"@unicode",          1, GetUnicode, HELPSTR(Unicode) },
	{ L"@unique",           0, GetUnique, HELPSTR(Unique) },
	{ L"@unixtime",         0, GetUnixTime, HELPSTR(UnixTime) },
	{ L"@version",          0, GetEBVersion, HELPSTR(EBVersion) },
	{ L"@voice",            1, GetVoice, HELPSTR(Voice) },
	{ L"@width",            0, GetWidth, HELPSTR(Width) },
	{ L"@year",             0, GetYear, HELPSTR(Year) },
};

const struct sExt setExtensionList[] = {
	{ L"@attr",             0, SetAttributes, NULL, NULL },
	{ L"@batchfile",        1, SetBatchFile, NULL, NULL },
	{ L"@bg",               0, SetBackground, NULL, NULL },
	{ L"@codepage",         0, SetCodePage, HELPSTR(CodePage) },
	{ L"@color",            0, SetColor, NULL, NULL },
	{ L"@column",           1, SetColumn, NULL, NULL },
	{ L"@ctrlcaborts",      0, SetCtrlCAborts, NULL, NULL },
	{ L"@cursor",           0, SetConsoleCursor, NULL, NULL },
	{ L"@decsep",           0, SetDecSep, NULL, NULL },
	{ L"@delayedexpansion", 0, SetDelayedExpansion, NULL, NULL },
	{ L"@dumpparse",		0, SetDumpParse, HELPSTR(DumpParse) },
	{ L"@dumptokens",		0, SetDumpTokens, HELPSTR(DumpTokens) },
	{ L"@echo",             0, SetEcho, NULL, NULL },
	{ L"@english",          0, SetEnglish, NULL, NULL },
	{ L"@extensions",       0, SetExtensions, NULL, NULL },
	{ L"@fg",               0, SetForeground, NULL, NULL },
	{ L"@inputcp",          0, SetInputCodePage, NULL, NULL },
	{ L"@next",             0, Next, HELPSTR(Next) },
	{ L"@opacity",          1, SetOpacity, NULL, NULL },
	{ L"@outputcp",         0, SetOutputCodePage, NULL, NULL },
	{ L"@position",         2, SetPosition, NULL, NULL },
	{ L"@row",              1, SetRow, NULL, NULL },
	{ L"@ul",               0, SetUnderline, NULL, NULL },
	{ L"@unicode",          0, SetUnicode, NULL, NULL },
	{ L"@voice",            0, SetVoice, NULL, NULL },
};

const struct sExt callExtensionList[] = {
	{ L"@checkkey",	0, CallCheckkey, HELPSTR(Checkkey) },
	{ L"@clear",   ~0, CallClear, HELPSTR(Clear) },
	{ L"@getkb",	0, CallGetkb, HELPSTR(Getkb) },
	{ L"@help",    ~0, CallHelp, HELPSTR(Help) },
	{ L"@image",   ~1, CallImage, HELPSTR(Image) },
	{ L"@img",	   ~1, CallImg, HELPSTR(Img) },
	{ L"@say",	   ~1, CallSay, HELPSTR(Say) },
	{ L"@sleep",	1, CallSleep, HELPSTR(Sleep) },
	{ L"@text",    ~1, CallText, HELPSTR(Text) },
	{ L"@timer",	0, CallTimer, HELPSTR(Timer) },
	{ L"@timerhi",	0, CallTimerHi, HELPSTR(TimerHi) },
	{ L"@unload",	0, CallUnload, HELPSTR(Unload) },
	{ L"@write",   ~0, CallWrite, HELPSTR(Write) },
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

void setChars(void)
{
	setVar(L"$LF",     L"\n");
	setVar(L"$CR",     L"\r");
	setVar(L"$CRLF",   L"\r\n");
	setVar(L"$BS",     L"\b");
	setVar(L"$TAB",    L"\t");
	setVar(L"$BELL",   L"\a");
	setVar(L"$ESC",    L"\33");
	setVar(L"$EXCL",   L"!");
	setVar(L"$PERC",   L"%");
	setVar(L"$QUOT",   L"\"");
	setVar(L"$APOS",   L"'");
	setVar(L"$BTICK",  L"`");
	setVar(L"$DQUOT",  L"\"");
	setVar(L"$SQUOT",  L"'");
	setVar(L"$BQUOT",  L"`");
	setVar(L"$AMP",    L"&");
	setVar(L"$BAR",    L"|");
	setVar(L"$GT",     L">");
	setVar(L"$LT",     L"<");
	setVar(L"$CARET",  L"^");
	setVar(L"$TILDE",  L"~");
	setVar(L"$OPAR",   L"(");
	setVar(L"$CPAR",   L")");
	setVar(L"$OBRC",   L"{");
	setVar(L"$CBRC",   L"}");
	setVar(L"$OBRS",   L"[");
	setVar(L"$CBRS",   L"]");
	setVar(L"$STAR",   L"*");
	setVar(L"$QUES",   L"?");
	setVar(L"$DOLLAR", L"$");
	setVar(L"$COLON",  L":");
	setVar(L"$SEMI",   L";");
	setVar(L"$COMMA",  L",");
	setVar(L"$EQ",     L"=");
	setVar(L"$AT",     L"@");
	setVar(L"$NBSP",   L"\xA0");

	setVar(L"$KEY_UP",     L"-72");
	setVar(L"$KEY_DOWN",   L"-80");
	setVar(L"$KEY_LEFT",   L"-75");
	setVar(L"$KEY_RIGHT",  L"-77");
	setVar(L"$KEY_ENTER",  L"13");
	setVar(L"$KEY_ESC",    L"27");
	setVar(L"$KEY_BS",     L"8");
	setVar(L"$KEY_TAB",    L"9");
	setVar(L"$KEY_HOME",   L"-71");
	setVar(L"$KEY_END",    L"-79");
	setVar(L"$KEY_PGUP",   L"-73");
	setVar(L"$KEY_PGDN",   L"-81");
	setVar(L"$KEY_INS",    L"-82");
	setVar(L"$KEY_DEL",    L"-83");
	setVar(L"$KEY_F1",     L"-59");
	setVar(L"$KEY_F2",     L"-60");
	setVar(L"$KEY_F3",     L"-61");
	setVar(L"$KEY_F4",     L"-62");
	setVar(L"$KEY_F5",     L"-63");
	setVar(L"$KEY_F6",     L"-64");
	setVar(L"$KEY_F7",     L"-65");
	setVar(L"$KEY_F8",     L"-66");
	setVar(L"$KEY_F9",     L"-67");
	setVar(L"$KEY_F10",    L"-68");
	setVar(L"$KEY_F11",    L"-133");
	setVar(L"$KEY_F12",    L"-134");

	WCHAR yesno[8] = L"NY";
	FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE,
				  NULL, 0x2328, 0, yesno, lenof(yesno), NULL);
	yesno[2] = L'\0';
	setVar(L"$YES", yesno+1);
	yesno[1] = yesno[0];
	setVar(L"$NO", yesno+1);

	WCHAR ctrlvar[16] = L"$CTRL+A";
	WCHAR ctrlch[4] = L"\x1";
	int c;
	for (c = 1; c < 32; ++c) {
		setVar(ctrlvar, ctrlch);
		++ctrlvar[6];
		++ctrlch[0];
	}
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

	if ((*lpName == L'$') || (*lpName == L'@')) {

		khint_t k;

		if (*lpName == L'@') {

			if (lpName[1] == L'@') {
				return GetRun(lpName+2, stringBuffer, STRINGBUFFERMAX);
			} else {
				const struct sExt *ext = bsearch(lpName, getExtensionList,
					lenof(getExtensionList), sizeof(struct sExt), extcmp);
				if (ext != NULL) {
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

	return GetEnvironmentVariable(lpName, stringBuffer, STRINGBUFFERMAX);
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
	LPWSTR mod, end, var, varcpy, spad, padend;

	pad = 0;
	zero = right = FALSE;
	varcpy = NULL;

	var = (LPWSTR) lpName;
	if (lpName != NULL && *lpName != L'\0'
		&& (*lpName != L'@' || lpName[1] != L'@')) {
		var = wcschr(lpName + 1, L'$');
		if (var != NULL) {
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
					int k = kh_get(wstr, variables, var);
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

		DWORD varmax = STRINGBUFFERMAX;

		if (lpValue != NULL && *lpValue == L' ' && *lpName != L'@') {
			DWORD len = wcslen(lpName);
			if (len > 1 && lpName[len-1] == L' ') {
				lpName = name = _wcsdup(lpName);
				name[len-1] = L'\0';
				++lpValue;
			} else if (len > 2 && lpName[len-1] == L'+' && lpName[len-2] == L' ') {
				lpName = name = _wcsdup(lpName);
				name[len-2] = L'\0';
				len = MyGetEnvironmentVariableW(name, varBuffer, STRINGBUFFERMAX);
				varbuf += len;
				varmax -= len;
				++lpValue;
				append = TRUE;
			}
		}

		if (lpValue != NULL && *lpValue == L'@') {
			if (MyGetEnvironmentVariableW(lpValue, varbuf, varmax) != 0
				|| lpValue[1] == L'@') {
				lpValue = varbuf;
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

	if (argc == 0) {
		int width = 0;

		void findMaxLen(const struct sExt *ext, int count)
		{
			const struct sExt *end = ext + count;
			for (; ext < end; ++ext) {
				if (ext->brief != NULL) {
					int len = wcslen(ext->name);
					if (len > width) {
						width = len;
					}
				}
			}
		}

		findMaxLen(getExtensionList, lenof(getExtensionList));
		findMaxLen(setExtensionList, lenof(setExtensionList));
		findMaxLen(callExtensionList, lenof(callExtensionList));

		width += 3;
		if (width < 12) {
			width = 12;
		}

		void printBriefHelp(const struct sExt *ext, int count, int flag)
		{
			const struct sExt *end = ext + count;
			for (; ext < end; ++ext) {
				if (ext->brief != NULL && (flag < 0 || flag == ext->args)) {
					cmd_printf(L"%-*s%s\r\n", width, ext->name, ext->brief);
				}
			}
		}

		printTitle(GetSetStr);
		printBriefHelp(getExtensionList, lenof(getExtensionList), 1);

		cmd_printf(L"\r\n");
		printTitle(GetterStr);
		printBriefHelp(getExtensionList, lenof(getExtensionList), 0);

		cmd_printf(L"\r\n");
		printTitle(SetterStr);
		printBriefHelp(setExtensionList, lenof(setExtensionList), -1);

		cmd_printf(L"\r\n");
		printTitle(CallStr);
		printBriefHelp(callExtensionList, lenof(callExtensionList), -1);

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

		return EXIT_SUCCESS;
	}

	const struct sExt *ext = bsearch(argv[0], getExtensionList,
					lenof(getExtensionList), sizeof(struct sExt), extcmp);
	if (ext == NULL) {
		ext = bsearch(argv[0], setExtensionList,
					lenof(setExtensionList), sizeof(struct sExt), extcmp);
	}
	BOOL var = ext != NULL;
	if (var) {
		cmd_printf(L"%s\r\n", ext->help);
	}
	ext = bsearch(argv[0], callExtensionList,
			lenof(callExtensionList), sizeof(struct sExt), extcmp);
	if (ext == NULL) {
		return var ? EXIT_SUCCESS : EXIT_FAILURE;
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

DWORD WINAPI
FreeLibraryThread(LPVOID param)
{
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
	{ NULL, 0, 0 },
},

DelayedHooks[] = {
	{ "CmdBatNotificationStub",  (DWORD_PTR) MyCmdBatNotification,      0 },
	{ NULL, 0, 0 },
};

void hook(HMODULE hInstance)
{
	hDllInstance = hInstance;

	hookCmd();

	Hooks = AllHooks;
	if (LOBYTE(GetVersion()) > 5) {
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
	setChars();
	onWindowsTerminal = GetEnvironmentVariable(L"WT_SESSION", NULL, 0) != 0;
}

void unhook(void)
{
	khint_t k;

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
		// RunDLL & RegSvr32 are GUI, CMD is console.
		PBYTE base = GetModuleHandle(NULL);
		PIMAGE_NT_HEADERS phdr = (PIMAGE_NT_HEADERS)(base + *(LPDWORD)(base + 0x3C));
		if (phdr->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI) {
			GetModuleFileName(hDll, enh_dll, lenof(enh_dll));
		} else {
			DisableThreadLibraryCalls(hDll);
			hook(hDll);
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
	} else if (cmdpid == -1) {
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
