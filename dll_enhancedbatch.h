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

#define BEVERSION 1

#ifndef UNICODE
#define UNICODE
#endif

#define _WIN32_WINNT 0x0500
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define WSZ(len) ((len) * sizeof(WCHAR))	// byte size of a wide string
#define lenof(a) (sizeof(a) / sizeof(*a))	// elements in a static array

struct sCMD {
	DWORD verMS, verLS;
	const DWORD *offsets;
};

extern const struct sCMD cmd_versions[];

extern DWORD cmdFileVersionMS, cmdFileVersionLS, cmdDebug;
extern DWORD eb_value;

extern LPBYTE cmd_addrs[];

#ifdef _WIN64
typedef int (*fnPutMsg)(UINT, int, UINT, va_list *);
extern fnPutMsg pPutMsg;
typedef LPWSTR (*fnMSCmdVar)(LPVOID, LPCWSTR, int *, LPCWSTR, LPWSTR *);
#else
typedef int (__stdcall *stdPutMsg)(UINT, int, UINT, va_list *);
typedef int (__fastcall *fastPutMsg)(UINT, int, UINT, va_list *);
typedef int (__fastcall *fastPutMsg62)(int, va_list *, UINT, UINT);
extern LPVOID pPutMsg;
typedef LPWSTR (__stdcall *stdMSCmdVar)(LPVOID, LPCWSTR, int *, LPCWSTR, LPWSTR *);
typedef LPWSTR (__fastcall *fastMSCmdVar)(LPVOID, LPCWSTR, int *, LPCWSTR, LPWSTR *);
typedef LPWSTR (__fastcall *fastMSCmdVar62)(LPCWSTR, LPVOID, LPCWSTR, int *, LPWSTR *);
#endif
extern int batchfile;

DWORD getBatchLine();

#define peol				cmd_addrs[0]			// expected first!
#ifdef _WIN64
#define pMSCmdVar			((fnMSCmdVar) cmd_addrs[1])
#else
#define pMSCmdVar			cmd_addrs[1]
#endif
#define pLexText			cmd_addrs[2]
#define pTmpBuf 			((LPWSTR)cmd_addrs[3])
#define pPutStdErrMsg		((int*)cmd_addrs[4])
#define pCurrentBatchFile	((LPWSTR**)cmd_addrs[5])
#define pfOutputUnicode 	cmd_addrs[6]
#define pfDelayedExpansion	cmd_addrs[7]
#define pfEnableExtensions	cmd_addrs[8]
#define pfDumpTokens		cmd_addrs[9]
#define pfDumpParse 		cmd_addrs[10]
#define pForMkstr			cmd_addrs[11]	// storage for FOR wildcard matches
#define pForResize			cmd_addrs[12]	// prevent resizing it

#define OFFSETS 13
