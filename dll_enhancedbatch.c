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
HANDLE hSpeaking;		// event signalling end of speech

WCHAR stringBuffer[STRINGBUFFERMAX]; // For hold conversion of values
WCHAR varBuffer[STRINGBUFFERMAX];

BOOL Help(int argc, LPCWSTR argv[]);

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
BOOL DllUnload(int argc, LPCWSTR argv[]);

fnCmdFunc *peEcho, eEcho, *peCall, eCall;
LPWSTR Fmt17;
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

struct sGetExt {
	LPCWSTR name;
	fnGetExt fn;
};

const struct sGetExt getExtensionList[] = {
	{ L"@attr", 			GetAttributes },
	{ L"@batchfile",		GetBatchFile },
	{ L"@batchline",		GetBatchLine },
	{ L"@bg",				GetBackground },
	{ L"@chhit",			Chhit },
	{ L"@cmdversion",		GetCmdVersion },
	{ L"@color",			GetColor },
	{ L"@column",			GetColumn },
	{ L"@ctrlcaborts",		GetCtrlCAborts },
	{ L"@cursor",			GetConsoleCursor },
	{ L"@date", 			GetDate },
	{ L"@datetime", 		GetDateTime },
	{ L"@datetimems",		GetDateTimems },
	{ L"@day",				GetDay },
	{ L"@dayname",			GetDayName },
	{ L"@dayofweek",		GetDayOfWeek },
	{ L"@dayshort", 		GetDayShort },
	{ L"@decsep",			GetDecSep },
	{ L"@delayedexpansion", GetDelayedExpansion },
	{ L"@echo", 			GetEcho },
	{ L"@echooptions",		GetEchoOptions },
	{ L"@english",			GetEnglish },
	{ L"@enhancedbatch",	GetEnhancedBatch},
	{ L"@extensions",		GetExtensions },
	{ L"@fg",				GetForeground },
	{ L"@forline",			GetForLine },
	{ L"@forlines", 		GetForLines },
	{ L"@getch",			Getch },
	{ L"@getkb",			Getkb },
	{ L"@height",			GetHeight },
	{ L"@hour", 			GetHour },
	{ L"@inputcp", 		 	GetInputCodePage },
	{ L"@iselevated", 		GetElevated },
	{ L"@kbhit",			Kbhit },
	{ L"@milliseconds", 	GetMilliseconds },
	{ L"@minute",			GetMinute },
	{ L"@month",			GetMonth },
	{ L"@monthname",		GetMonthName },
	{ L"@monthshort",		GetMonthShort },
	{ L"@opacity",			GetOpacity },
	{ L"@osbuild",			GetOSBuild },
	{ L"@osmajor",			GetOSMajor },
	{ L"@osminor",			GetOSMinor },
	{ L"@osrevision",		GetOSRevision },
	{ L"@osversion",		GetOSVersion },
	{ L"@outputcp", 		GetOutputCodePage },
	{ L"@position", 		GetPosition },
	{ L"@row",				GetRow },
	{ L"@second",			GetSecond },
	{ L"@size", 			GetSize },
	{ L"@stderr",			GetStderr },
	{ L"@stdin",			GetStdin },
	{ L"@stdout",			GetStdout },
	{ L"@tempdir",			GetTempDir },
	{ L"@tempfile", 		GetTempFile },
	{ L"@time", 			GetTime },
	{ L"@timems",			GetTimems },
	{ L"@timer",			GetTimer },
	{ L"@timerhi",			GetHiTimer },
	{ L"@title",			GetTitle },
	{ L"@transient",		GetTransient },
	{ L"@ul",				GetUnderline },
	{ L"@unicode",			GetUnicode },
	{ L"@unique",			GetUnique },
	{ L"@unixtime",			GetUnixTime },
	{ L"@version",			GetEBVersion },
	{ L"@voice",			GetVoice },
	{ L"@width",			GetWidth },
	{ L"@year", 			GetYear },
};

struct sSetExt {
	LPCWSTR name;
	fnSetExt fn;
	int args;
};

const struct sSetExt setExtensionList[] = {
	{ L"@attr", 			SetAttributes, 1 },
	{ L"@batchfile",		SetBatchFile, 1 },
	{ L"@bg",				SetBackground, 1 },
	{ L"@codepage", 		SetCodePage, 0 },
	{ L"@color",			SetColor, 1 },
	{ L"@column",			SetColumn, 1 },
	{ L"@ctrlcaborts",		SetCtrlCAborts, 0 },
	{ L"@cursor",			SetConsoleCursor, 1 },
	{ L"@decsep",			SetDecSep, 0 },
	{ L"@delayedexpansion", SetDelayedExpansion, 0 },
	{ L"@dumpparse",		SetDumpParse, 0 },
	{ L"@dumptokens",		SetDumpTokens, 0 },
	{ L"@echo", 			SetEcho, 0 },
	{ L"@echooptions", 		SetEchoOptions, 0 },
	{ L"@english",			SetEnglish, 0 },
	{ L"@extensions",		SetExtensions, 0 },
	{ L"@fg",				SetForeground, 1 },
	{ L"@inputcp", 			SetInputCodePage, 0 },
	{ L"@next", 			Next, 0 },
	{ L"@opacity",			SetOpacity, 1 },
	{ L"@outputcp", 		SetOutputCodePage, 0 },
	{ L"@position", 		SetPosition, 2 },
	{ L"@row",				SetRow, 1 },
	{ L"@ul",				SetUnderline, 0 },
	{ L"@unicode",			SetUnicode, 0 },
	{ L"@voice",			SetVoice, 0 },
};

struct sCallExt {
	LPCWSTR name;
	int args;
	fnSetExt fn;
	LPCWSTR brief, help;
};

const struct sCallExt callExtensionList[] = {
	{ L"@help", 	0, Help, HelpBriefStr, HelpHelpStr },
	{ L"@say",	   ~1, Say, SayBriefStr, SayHelpStr },
	{ L"@sleep",	1, WaitMilliseconds, SleepBriefStr, SleepHelpStr },
	{ L"@timer",	0, SetLoTimer, TimerBriefStr, TimerHelpStr },
	{ L"@timerhi",	0, SetHiTimer, TimerHiBriefStr, TimerHiHelpStr },
	{ L"@unload",	0, DllUnload, UnloadBriefStr, UnloadHelpStr },
};

void setVar(LPCWSTR var, LPCWSTR val)
{
	khint_t k;
	int absent;

	k = kh_put(wstr, variables, var, &absent);
	if (absent) {
		kh_key(variables, k) = _wcsdup(var);
	} else {
		free((void*) kh_val(variables, k));
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

	if (*lpName == L'U' && lpName[1] == '+') {
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
				const struct sGetExt *ext = bsearch(lpName, getExtensionList,
					lenof(getExtensionList), sizeof(struct sGetExt), extcmp);
				if (ext) {
					return ext->fn(stringBuffer, STRINGBUFFERMAX);
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
		if (lpName[1] >= '0' && lpName[1] <= '9') {
			DWORD arg1, arg2;
			LPWSTR end;
			arg1 = (DWORD) wcstoul(lpName + 1, &end, 10);
			if (*end == L'\0') {
				arg2 = arg1;
			} else if (*end == '-') {
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
	if (length) {
		LPWSTR p = stringBuffer, end = p + length;
		while (wcschr(delim, *p)) {
			if (++p == end) {
				break;
			}
		}
		if (p != stringBuffer) {
			stringBuffer[length] = L'\0';
			wcscpy(stringBuffer, p);
			length -= (DWORD) (p - stringBuffer);
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
			dst += snwprintf(dst, STRINGBUFFERMAX - (dst - varBuffer),
							 L"%.2X ", *src);
		}
	}
	if (dst == varBuffer) {
		*stringBuffer = L'\0';
		length = 0;
	} else {
		dst[-1] = L'\0';
		length = snwprintf(stringBuffer, STRINGBUFFERMAX, L"%s", varBuffer);
	}
	return length;
}

DWORD unhexify(DWORD length)
{
	LPWSTR src = stringBuffer, dst = varBuffer;
	while (length > 0) {
		if (iswxdigit(src[0]) && iswxdigit(src[1])) {
			swscanf(src, L"%2X", (int *) dst);
			++dst;
			src += 2;
			length -= 2;
		} else if (src[0] == L'0' && (src[1] == L'x' || src[1] == L'X')
				   && iswxdigit(src[2]) && iswxdigit(src[3])) {
			swscanf(src+2, L"%2X", (int *) dst);
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
	return snwprintf(stringBuffer, STRINGBUFFERMAX, L"%s", varBuffer);
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
	if (lpName && (*lpName != '@' || lpName[1] != '@')) {
		mod = wcschr(lpName, L';');
		if (mod) {
			var = varcpy = _wcsdup(lpName);
			var[mod - lpName] = L'\0';
			mod += var - lpName + 1;
		}
	} else {
		mod = NULL;
	}

	*stringBuffer = L'\0';
	length = getVar(var);

	while (mod) {
		if (*mod == L'\'' || *mod == L'"' || *mod == L'`') {
			for (end = mod + 1; *end && *end != *mod; ++end) {
				// do nothing
			}
			if (*end == L'\0') {
				break;
			}
			*end++ = L'\0';
			if (length == 0) {
				length = snwprintf(stringBuffer, STRINGBUFFERMAX, L"%s", mod+1);
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
			if (!end) {
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
		if (end) {
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
				if (var == stringBuffer || wcschr(mod, var[-1])) {
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
			length = snwprintf(stringBuffer, STRINGBUFFERMAX, L"%d", length);
		} else if (WCSIEQ(mod, L"hexify")) {
			length = hexify(length);
		} else if (WCSIEQ(mod, L"unhexify")) {
			length = unhexify(length);
		} else if (*mod == L'~') {
			int unused;
			DWORD ext;
			var = (end) ? end - 1 : mod + wcslen(mod);
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
			if (cmdFileVersionMS > 0x60002) {
				var = ((fastMSCmdVar) pMSCmdVar)(NULL, mod, &unused, L"?", &var);
			} else if (cmdFileVersionMS == 0x60002) {
				var = ((fastMSCmdVar62) pMSCmdVar)(L"?", NULL, mod, &unused, &var);
			} else {
				var = ((stdMSCmdVar) pMSCmdVar)(NULL, mod, &unused, L"?", &var);
			}
#endif
			*pfEnableExtensions = ext;
			if (!var) {
				break;
			}
			length = snwprintf(stringBuffer, STRINGBUFFERMAX, L"%s", var);
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
	if (mod) {
		pad = 0;
		length = snwprintf(stringBuffer, STRINGBUFFERMAX, L"%s", lpName);
	}
	if (varcpy) {
		free(varcpy);
	}

	size = (pad > length) ? pad : length;
	if (size > nSize || lpBuffer == NULL) {
		return size + 1;
	}
	if (pad > length) {
		if (right) {
			snwprintf(lpBuffer, nSize, L"%-*s", pad, stringBuffer);
		} else {
			snwprintf(lpBuffer, nSize, L"%*s", pad, stringBuffer);
			if (zero) {
				LPWSTR s = lpBuffer;
				while (*s == L' ') {
					*s++ = L'0';
				}
				if (*s == L'-') {
					*s = L'0';
					*lpBuffer = '-';
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
	if (lpName != NULL) {

		if (lpValue && *lpValue == '@') {
			if (MyGetEnvironmentVariableW(lpValue, varBuffer, STRINGBUFFERMAX)
				|| lpValue[1] == '@') {
				lpValue = varBuffer;
			}
		}

		if ((*lpName == L'$') || (*lpName == L'@')) {

			if (*lpName == L'@') {

				const struct sSetExt *ext = bsearch(lpName, setExtensionList,
					lenof(setExtensionList), sizeof(struct sSetExt), extcmp);
				if (ext) {
					BOOL ret;
					if (ext->args == 0) {
						ret = ext->fn(lpValue == NULL ? 0 : 1, &lpValue);
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
							ret = ext->fn(nArgs, (LPCWSTR*) szArglist);
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
					free((void*) kh_key(variables, k));
					free((void*) kh_val(variables, k));
					kh_del(wstr, variables, k);
				}
				return TRUE;
			}

			setVar(lpName, lpValue);
			return TRUE;
		}

	}

	return SetEnvironmentVariable(lpName, lpValue);
}

DWORD WINAPI MyCall(struct cmdnode *node)
{
	if (node->arg) {
		LPWSTR arg = node->arg;
		while (*arg == L' ' || *arg == L'\t') {
			++arg;
		}
		if (*arg == L'@') {
			int nArgs;
			LPWSTR *szArglist = CommandLineToArgvW(arg, &nArgs);
			if (NULL == szArglist) {
				fwprintf(stderr, ArgErrorStr);
				*pLastRetCode = 1;
				return 1;
			}
			const struct sCallExt *ext = bsearch(szArglist[0], callExtensionList,
				lenof(callExtensionList), sizeof(struct sCallExt), extcmp);
			if (NULL == ext) {
				LocalFree(szArglist);
				*pLastRetCode = 1;
				return 1;
			}
			if (nArgs == 2 && WCSEQ(szArglist[1], L"/?")) {
				cmd_printf(L"%s\r\n", ext->help);
				LocalFree(szArglist);
				*pLastRetCode = 0;
				return 0;
			}
			BOOL ret;
			if (ext->args == 0) {
				do {
					++arg;
				} while (*arg != L' ' && *arg != L'\t' && *arg != L'\0');
				if (*arg != L'\0') {
					do {
						++arg;
					} while (*arg == L' ' || *arg == L'\t');
				}
				ret = ext->fn(*arg == L'\0' ? 0 : 1, (LPCWSTR*) &arg);
			} else {
				--nArgs;
				if (ext->args < 0 && ~ext->args > nArgs) {
					fwprintf(stderr, MoreArgsStr, ~ext->args, nArgs);
					ret = FALSE;
				} else if (ext->args > 0 && ext->args != nArgs) {
					fwprintf(stderr, WrongArgsStr, ext->args, nArgs);
					ret = FALSE;
				} else {
					ret = ext->fn(nArgs, (LPCWSTR*) szArglist + 1);
				}
				LocalFree(szArglist);
			}
			*pLastRetCode = ret ? 0 : 1;
			return *pLastRetCode;
		}
	}

	return eCall(node);
}

BOOL Help(int argc, LPCWSTR argv[])
{
	const struct sCallExt *ext;
	const struct sCallExt *end = callExtensionList + lenof(callExtensionList);
	int width = 0, len = 0;
	for (ext = callExtensionList; ext < end; ++ext) {
		len = wcslen(ext->name);
		if (len > width) {
			width = len;
		}
	}
	width += 3;
	if (width < 12) {
		width = 12;
	}
	for (ext = callExtensionList; ext < end; ++ext) {
		cmd_printf(L"%-*s%s\r\n", width, ext->name, ext->brief);
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

DWORD WINAPI MyEcho(struct cmdnode *node)
{
	BOOL modified_newline = FALSE;
	BOOL suppressed_quotes = FALSE;
	int  arg_ofs;
	DWORD ret;

	if (!node->arg) {
		static WCHAR space[] = L" ";
		node->arg = space;
		ret = eEcho(node);
		node->arg = NULL;
		return ret;
	}

	if (*node->arg == L';') {
		WriteByte(Fmt17+2, 0);
		modified_newline = TRUE;
	} else if (*node->arg == L',') {
		WriteMemory(Fmt17+2, L"\n", 4);
		modified_newline = TRUE;
	}

	if ((node->arg[1] == L'~' && node->arg[2] == L'"') ||
		(modified_newline && WCSBEG(node->arg+1, L" ~\""))) {
		DWORD len = (DWORD) wcslen(node->arg);
		if (node->arg[len-1] == '"') {
			node->arg[len-1] = L'\0';
			arg_ofs = (node->arg[2] == '~') ? 3 : 2;
			node->arg += arg_ofs;
			suppressed_quotes = TRUE;
		}
	}

	ret = eEcho(node);

	if (suppressed_quotes) {
		node->arg -= arg_ofs;
	}

	if (modified_newline) {
		WriteMemory(Fmt17+2, L"\r\n", 4);
	}

	return ret;
}

BOOL DwFlagsForCodepageMustBeZero(UINT CodePage)
{
	if (!CodePage) {
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

BOOL SafeCloseHandle(HANDLE handle) {
	if (handle != NULL
		&& handle != INVALID_HANDLE_VALUE
	) {
		return CloseHandle(handle);
	} else {
		return FALSE;
	}
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
		if (!uc) {
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
	HANDLE hFile, hMap;
	LPBYTE mem, p;

	if (!*pCurrentBatchFile) {
		return 0;
	}

	k = kh_get(line, batch_lnums, **pCurrentBatchFile);
	if (k == kh_end(batch_lnums)) {
		return 0;
	}
	pos = kh_val(batch_lnums, k);
	hFile = CreateFile(**pCurrentBatchFile, GENERIC_READ,
					   FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
					   0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return 0;
	}
	hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (hMap == NULL) {
		CloseHandle(hFile);
		return 0;
	}
	mem = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, pos);
	if (mem == NULL) {
		CloseHandle(hMap);
		CloseHandle(hFile);
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
	UnmapViewOfFile(mem);
	CloseHandle(hMap);
	CloseHandle(hFile);
	return lnum;
}

int MyPutStdErrMsg(UINT a, int b, UINT c, va_list *d)
{
	DWORD lnum = (batchfile) ? getBatchLine() : 0;
	if (lnum && (lnum != last_lnum || last_bat != *pCurrentBatchFile)) {
		LPVOID args = &stringBuffer;
		LPVOID pargs = &args;
		LPCWSTR file = **pCurrentBatchFile;
		if (batchfile == 1) {
			file = wcsrchr(file, L'\\');
			if (file) {
				++file;
			} else {
				file = **pCurrentBatchFile;
			}
		}
		snwprintf(stringBuffer, STRINGBUFFERMAX, L"%s:%lu: ", file, lnum);
#ifdef _WIN64
		pPutMsg(0x2371, b, 1, (va_list *) &pargs);
#else
		if (cmdFileVersionMS > 0x60002) {
			((fastPutMsg) (pPutMsg))(0x2371, b, 1, (va_list *) &pargs);
		} else if (cmdFileVersionMS == 0x60002) {
			((fastPutMsg62) (pPutMsg))(b, (va_list *) &pargs, 0x2371, c);
		} else {
			((stdPutMsg) (pPutMsg))(0x2371, b, 1, (va_list *) &pargs);
		}
#endif
		last_lnum = lnum;
		last_bat = *pCurrentBatchFile;
	}
#ifdef _WIN64
	return pPutMsg(a, b, c, d);
#else
	if (cmdFileVersionMS > 0x60002) {
		return ((fastPutMsg) (pPutMsg))(a, b, c, d);
	} else if (cmdFileVersionMS == 0x60002) {
		return ((fastPutMsg62) (pPutMsg))(b, d, a, c);
	} else {
		return ((stdPutMsg) (pPutMsg))(a, b, c, d);
	}
#endif
}

BOOL WINAPI
MyReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead,
		   LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	BOOL ret, utf8file;

	if (!AnsiBuf) {
		// If the read is into a global variable that should be the buffer.
		if (lpBuffer > (LPVOID) GetModuleHandle(NULL) && lpBuffer < cmd_end) {
			AnsiBuf = lpBuffer;
		}
	}

	if (pCurrentBatchFile && *pCurrentBatchFile && lpBuffer == AnsiBuf) {
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

	if (lpBuffer == AnsiBuf && ret && *lpNumberOfBytesRead > 4) {
		DWORD pos, size = *lpNumberOfBytesRead, discarded = 0;
		DWORD end_quote;
		LPSTR buffer = lpBuffer;
		for (pos = 0; pos < size; ++pos) {
			if (buffer[pos] == '\n') {
				end_quote = pos;
				if (end_quote > 0 && buffer[end_quote-1] == '\r') {
					--end_quote;
				}
				if (end_quote > 1 &&
					buffer[end_quote-2] == '"' && buffer[end_quote-1] == '^') {
					end_quote -= 2;
				} else {
					if (discarded) {
						memset(buffer, ' ', discarded);
					}
					break;
				}
				for (; pos < size; ++pos) {
					if (buffer[pos] == ' ' || buffer[pos] == '\t')
						continue;
					if (buffer[pos] == '"') {
						memmove(buffer+pos+1-end_quote, buffer, end_quote);
						discarded += pos + 1 - end_quote;
						break;
					}
				}
			}
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

BOOL DllUnload(int argc, LPCWSTR argv[])
{
	unhook();
	SafeCloseHandle(CreateThread(NULL, 4096, FreeLibraryThread, NULL, 0, NULL));
	return TRUE;
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
	PHookFn hook;

	// Blast through the table of import names
	while (pNameThunk->u1.AddressOfData) {
		PIMAGE_IMPORT_BY_NAME pName = MakeVA(PIMAGE_IMPORT_BY_NAME,
				pNameThunk->u1.AddressOfData);
		LPCSTR name = (LPCSTR) pName->Name;
		for (hook = Hooks; hook->name; ++hook) {
			if (STREQ(name, hook->name)) {		// We found it!
				DWORD flOldProtect, flDummy;

				// Change the access protection on the region of committed pages in the
				// virtual address space of the current process
				VirtualProtect(&pThunk->u1.Function, sizeof(PVOID),
						PAGE_READWRITE, &flOldProtect);

				// Overwrite the original address with the address of the new function
				if (hook->oldfunc) {
					pThunk->u1.Function = hook->oldfunc;
				} else {
					hook->oldfunc = pThunk->u1.Function;
					pThunk->u1.Function = hook->newfunc;
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

	Hooks = AllHooks;
	if (LOBYTE(GetVersion()) > 5) {
		// No need for the MultiByteToWideChar patch.
		++Hooks;
	}
	HookAPIOneMod(GetModuleHandle(NULL), Hooks);
	if (AllHooks[1].oldfunc) {
		HookAPIOneMod(hInstance, Hooks);
	} else {
		// CmdBatNotification is delay-loaded as CmdBatNotificationStub,
		// so point our own CmdBatNotification to the stub, too.
		HookAPIDelayMod(GetModuleHandle(NULL), DelayedHooks);
		AllHooks[1].oldfunc = DelayedHooks[0].oldfunc;
		HookAPIOneMod(hInstance, Hooks);
	}

	hookCmd();

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

	HookAPIOneMod(GetModuleHandle(NULL), Hooks);
	HookAPIDelayMod(GetModuleHandle(NULL), DelayedHooks);

	for (k = 0; k < kh_end(variables); ++k) {
		if (kh_exist(variables, k)) {
			free((void*) kh_key(variables, k));
			free((void*) kh_val(variables, k));
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
		PIMAGE_NT_HEADERS phdr = (PIMAGE_NT_HEADERS)(base + *(DWORD*)(base + 0x3C));
		if (phdr->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI) {
			GetModuleFileName(hDll, enh_dll, lenof(enh_dll));
		} else {
			DisableThreadLibraryCalls(hDll);
			hSpeaking = CreateEvent(NULL, TRUE, TRUE, NULL);
			hook(hDll);
		}
	} else if (dwReason == DLL_PROCESS_DETACH) {
		if (variables != NULL) {
			unhook();
			CloseHandle(hSpeaking);
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
	HANDLE hSnap, ph;
	PROCESSENTRY32 pe, ppe;
	BOOL parent_wow64, me_wow64;
	typedef BOOL (WINAPI * LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;

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

	fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
			GetModuleHandle(L"kernel32.dll"), "IsWow64Process");
	if (fnIsWow64Process != NULL) {
		ph = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe.th32ProcessID);
		if (ph == NULL) {
			return 0;
		}
		fnIsWow64Process(ph, &parent_wow64);
		fnIsWow64Process(GetCurrentProcess(), &me_wow64);
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
	enh_name = wcsrchr(enh_dll, '\\') + 1;

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
	snwprintf(subblock, lenof(subblock),
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
					snwprintf(stringBuffer, STRINGBUFFERMAX,
							  NotCmdStr, cmdname);
				} else {
					GetCmdVersion(varBuffer, STRINGBUFFERMAX);
					snwprintf(stringBuffer, STRINGBUFFERMAX,
							  NotSupportedStr, varBuffer);
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
