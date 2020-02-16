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

#define EBVERSION 0

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

// Test if A equals B.
#define STREQ(a, b) (strcmp(a, b) == 0)
#define WCSEQ(a, b) (wcscmp(a, b) == 0)

// Test if A equals B, ignoring case.
#define STRIEQ(a, b) (_stricmp(a, b) == 0)
#define WCSIEQ(a, b) (_wcsicmp(a, b) == 0)

// Test if A starts with B (a static string), ignoring case.
#define STRIBEG(a, b) (_strnicmp(a, b, sizeof(b)-1) == 0)
#define WCSIBEG(a, b) (_wcsnicmp(a, b, lenof(b)-1) == 0)

// Test if A starts with B (a static string).
#define WCSBEG(a, b) (wcsncmp(a, b, lenof(b)-1) == 0)

// Macro to convert a relative virtual address into a virtual address.
// Requires pDosHeader to point to the base.
#define MakeVA(cast, addValue) \
	(cast)((DWORD_PTR) pDosHeader + (DWORD_PTR)(addValue))

HRESULT DllLoad(void);
#define Export_DllLoad_Entrypoint(name) \
	HRESULT name(void) __attribute__((dllexport, alias("DllLoad")))

// Allow optimize specific function for size
#define AttrOptSize __attribute__((optimize("Os")))

// Import a function to its pointer (Function becomes pFunction)
#define GETPROC(mod, name) p ## name = (LPVOID) GetProcAddress(mod, #name)

struct sCMD {
	DWORD verMS, verLS;
	const DWORD *offsets;
};

extern const struct sCMD cmd_versions[];

extern LPVOID cmd_end;
extern DWORD cmdFileVersionMS, cmdFileVersionLS, cmdDebug;

#define CMD_MAJOR_MINOR(op, maj,min) (cmdFileVersionMS op ((maj << 16) | min))
#define CMD_BUILD_REVISION(op, bld,rev) (cmdFileVersionLS op ((bld << 16) | rev))
#define CMD_MAJOR(op, maj) (HIWORD(cmdFileVersionMS) op maj)
#define CMD_BUILD(op, bld) (HIWORD(cmdFileVersionLS) op bld)
#define CMD_REVISION(op, rev) (LOWORD(cmdFileVersionLS) op rev)
#define CMD_VERSION(maj,min,bld,rev) \
	(cmdFileVersionMS == ((maj << 16) | min) &&\
	 cmdFileVersionLS == ((bld << 16) | rev))

extern BOOL onWindowsTerminal;
extern HWND consoleHwnd;
extern HANDLE consoleOutput;
extern HANDLE hSpeaking;

void uninitCo(void);
void unload_delayed(void);

extern LPBYTE cmd_addrs[];

struct redirnode {
	int fh;
	LPWSTR name;
	int unknown;
	int append;
	WCHAR symbol;
	struct redirnode *next;
};

struct cmdnode {		// partial definition
	LPVOID stuff[13];
	struct redirnode *redir;
	LPWSTR cmd;
	LPWSTR arg;
};

typedef int (WINAPI *fnCmdFunc)(struct cmdnode *node);

extern fnCmdFunc *peCall, eCall;
extern int *pLastRetCode;

int WINAPI MyCall(struct cmdnode *node);
int MyPutStdErrMsg(UINT a, int b, UINT c, va_list *d);
UINT MyLexText(void);

typedef int (*fncmd_printf)(LPCWSTR fmt, ...);
extern fncmd_printf cmd_printf;
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
extern BYTE trace;

#ifdef _WIN64
#define ARCH L"amd64"
#else
#define ARCH L"x86"
#endif

#define STRINGBUFFERMAX 32768
extern WCHAR stringBuffer[STRINGBUFFERMAX];
extern WCHAR varBuffer[STRINGBUFFERMAX];
extern WCHAR sayBuffer[STRINGBUFFERMAX];

#define FINDFOR_STACKSIZE 128
#define FORF_STACKSIZE 32		// should be way more than enough


extern const WCHAR ProgramNameStr[];
extern const WCHAR CloseWindowStr[];
extern const WCHAR ParentErrStr[], ArchErrStr[], NotCmdStr[], NotSupportedStr[];
extern const WCHAR ArgErrorStr[], WrongArgsStr[], MoreArgsStr[];
extern const WCHAR EscapeHelpStr[], ModHelpStr[];

extern const WCHAR ConstStr[], GetSetStr[], GetterStr[], SetterStr[], CallStr[];

#define DEFHELPSTR(ext) ext##BriefStr[], ext##HelpStr[]

extern const WCHAR
	DEFHELPSTR(Attr), DEFHELPSTR(BatchFile), DEFHELPSTR(BatchLine),
	DEFHELPSTR(Background), DEFHELPSTR(Readchar), DEFHELPSTR(CmdVersion),
	DEFHELPSTR(Color), DEFHELPSTR(Column), DEFHELPSTR(CtrlCAborts),
	DEFHELPSTR(ConsoleCursor), DEFHELPSTR(Date), DEFHELPSTR(DateTime),
	DEFHELPSTR(DateTimems), DEFHELPSTR(Day), DEFHELPSTR(DayName),
	DEFHELPSTR(DayOfWeek), DEFHELPSTR(DayShort), DEFHELPSTR(DecSep),
	DEFHELPSTR(DelayedExpansion), DEFHELPSTR(Echo),
	DEFHELPSTR(English), DEFHELPSTR(EnhancedBatch), DEFHELPSTR(Extensions),
	DEFHELPSTR(Foreground), DEFHELPSTR(ForLine), DEFHELPSTR(ForLines),
	DEFHELPSTR(Getchar), DEFHELPSTR(Getkey), DEFHELPSTR(Height),
	DEFHELPSTR(Hour), DEFHELPSTR(InputCodePage), DEFHELPSTR(Elevated),
	DEFHELPSTR(Readkey), DEFHELPSTR(Milliseconds), DEFHELPSTR(Minute),
	DEFHELPSTR(Month), DEFHELPSTR(MonthName), DEFHELPSTR(MonthShort),
	DEFHELPSTR(Opacity), DEFHELPSTR(OSBuild), DEFHELPSTR(OSMajor),
	DEFHELPSTR(OSMinor), DEFHELPSTR(OSRevision), DEFHELPSTR(OSVersion),
	DEFHELPSTR(OutputCodePage), DEFHELPSTR(Position), DEFHELPSTR(Row),
	DEFHELPSTR(Second), DEFHELPSTR(Size), DEFHELPSTR(Stderr), DEFHELPSTR(Stdin),
	DEFHELPSTR(Stdout), DEFHELPSTR(TempDir), DEFHELPSTR(TempFile),
	DEFHELPSTR(Time), DEFHELPSTR(Timems), DEFHELPSTR(TimerVar),
	DEFHELPSTR(HiTimer), DEFHELPSTR(Title), DEFHELPSTR(Transient),
	DEFHELPSTR(Underline), DEFHELPSTR(Unicode), DEFHELPSTR(Unique),
	DEFHELPSTR(UnixTime), DEFHELPSTR(EBVersion), DEFHELPSTR(Voice),
	DEFHELPSTR(Width), DEFHELPSTR(Year),
	DEFHELPSTR(Yes), DEFHELPSTR(No), DEFHELPSTR(Ctrl), DEFHELPSTR(Trace),

	DEFHELPSTR(CodePage), DEFHELPSTR(DumpParse), DEFHELPSTR(DumpTokens),
	DEFHELPSTR(Next),

	DEFHELPSTR(Checkkey), DEFHELPSTR(Clear), DEFHELPSTR(Help),
	DEFHELPSTR(Image), DEFHELPSTR(Img), DEFHELPSTR(Say),
	DEFHELPSTR(Sleep), DEFHELPSTR(Text), DEFHELPSTR(Timer), DEFHELPSTR(TimerHi),
	DEFHELPSTR(Unload), DEFHELPSTR(Waitkey), DEFHELPSTR(Write);


int wsnprintf(LPWSTR buf, size_t size, LPCWSTR fmt, ...);
int sbprintf(LPWSTR buf, LPCWSTR fmt, ...);
#define wsncpy(dst, size, src) wsnprintf(dst, size, L"%s", src)
#define sbcpy(dst, src) sbprintf(dst, L"%s", src)

LPSTR readBatchFile(DWORD size, LPSTR buf, DWORD buf_size);
BOOL SafeCloseHandle(HANDLE handle);
BOOL haveOutputHandle(void);
BOOL haveWindowHandle(void);

void WriteMemory(LPVOID dst, LPCVOID src, int size);
#define WriteByte(dst, b) WriteMemory(dst, (LPCVOID) b, 1)

DWORD getBatchLine();
void hookCmd(void);
void unhookCmd(void);
void doneCmdBat(void);
void hookCtrlCAborts(char aborts);
DWORD getVar(LPCWSTR lpName);
int __fastcall displayVars(LPCWSTR prefix);

enum {
	ieol, iMSCmdVar, iLexText, iTmpBuf, iPutStdErrMsg, iCurrentBatchFile,
	ifOutputUnicode, ifDelayedExpansion, ifEnableExtensions, ifDumpTokens,
	ifDumpParse, iForMkstr, iForResize, iEchoFlag, iCheckHelpfirstarg,
	icmd_printf, iCtrlCAborts, imkstr, iFreeStack, iDCount, iSFWorkmkstr,
	iSFWorkresize, iSFWorksaved, iDESubWorkFreeStr, iGotoFlag, iForFbegin,
	iForFend, iParseFortoken, iLexBufPtr, iLexBufferend, iForFoptions, iTokLen,
	iGotoEof, iGotopos, iGotostart, iCallWorkresize, iMyGetEnvVarPtr,
	iDisplayEnv, iDisplayEnvVariable, iDispatchAfterDE,
	OFFSETS
};

// The eol character is no longer modified, but still used to verify the version.
//#define peol				cmd_addrs[ieol]			// expected first!
#ifdef _WIN64
#define pMSCmdVar			((fnMSCmdVar) cmd_addrs[iMSCmdVar])
#else
#define pMSCmdVar			cmd_addrs[iMSCmdVar]
#endif
#define pLexText			cmd_addrs[iLexText]
#define pTmpBuf 			((LPWSTR) cmd_addrs[iTmpBuf])
#define pPutStdErrMsg		((int *) cmd_addrs[iPutStdErrMsg])
#define pCurrentBatchFile	((LPWSTR **) cmd_addrs[iCurrentBatchFile])
#define pfOutputUnicode 	cmd_addrs[ifOutputUnicode]
#define pfDelayedExpansion	cmd_addrs[ifDelayedExpansion]
#define pfEnableExtensions	cmd_addrs[ifEnableExtensions]
#define pfDumpTokens		cmd_addrs[ifDumpTokens]
#define pfDumpParse 		cmd_addrs[ifDumpParse]
#define pForMkstr			cmd_addrs[iForMkstr]	// storage for FOR wildcard matches
#define pForResize			cmd_addrs[iForResize]	// prevent resizing it
#define pEchoFlag			cmd_addrs[iEchoFlag]
#define pCheckHelpfirstarg	cmd_addrs[iCheckHelpfirstarg]
#define pcmd_printf 		cmd_addrs[icmd_printf]
#define pCtrlCAborts		cmd_addrs[iCtrlCAborts]
#define pmkstr				cmd_addrs[imkstr]
#define pFreeStack			cmd_addrs[iFreeStack]
#define pDCount 			((LPDWORD) cmd_addrs[iDCount])
#define pSFWorkmkstr		cmd_addrs[iSFWorkmkstr]
#define pSFWorkresize		cmd_addrs[iSFWorkresize]
#define pSFWorksaved		((char *) cmd_addrs[iSFWorksaved])
#define pDESubWorkFreeStr	cmd_addrs[iDESubWorkFreeStr]
#define pGotoFlag			cmd_addrs[iGotoFlag]
#define pForFbegin			cmd_addrs[iForFbegin]
#define pForFend			cmd_addrs[iForFend]
#define pParseFortoken		((int *) cmd_addrs[iParseFortoken])
#define pLexBufPtr			((LPWSTR *) cmd_addrs[iLexBufPtr])
#define pLexBufferend		((LPWSTR) cmd_addrs[iLexBufferend])
#define pForFoptions		((int *) cmd_addrs[iForFoptions])
#define pTokLen 			((int *) cmd_addrs[iTokLen])
#define pGotoEof			cmd_addrs[iGotoEof]
#define pGotopos			cmd_addrs[iGotopos]
#define pGotostart			cmd_addrs[iGotostart]
#define pCallWorkresize 	((int *) cmd_addrs[iCallWorkresize])
#define pMyGetEnvVarPtr 	cmd_addrs[iMyGetEnvVarPtr]
#define pDisplayEnv 		((int *) cmd_addrs[iDisplayEnv])
#define pDisplayEnvVariable ((int *) cmd_addrs[iDisplayEnvVariable])
#define pDispatchAfterDE	cmd_addrs[iDispatchAfterDE]
