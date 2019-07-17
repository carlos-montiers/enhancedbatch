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

#ifndef UNICODE
#define UNICODE
#endif

#define _WIN32_WINNT 0x0500
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <conio.h>
#include <string.h>

#define OPACITY_DELTA_INTERVAL 12

#define MAX_OPACITY_PERCENT 100
#define MIN_OPACITY_PERCENT 30
#define MAX_OPACITY_ALPHA 255
#define MIN_OPACITY_ALPHA 77

typedef DWORD (*fnGetExt)(LPWSTR, DWORD);

typedef BOOL (*fnSetExt)(int argc, LPCWSTR argv[]);

DWORD Getch(LPWSTR buffer, DWORD size);
DWORD Chhit(LPWSTR buffer, DWORD size);
DWORD Getkb(LPWSTR buffer, DWORD size);
DWORD Kbhit(LPWSTR buffer, DWORD size);

BOOL SetPosition(int argc, LPCWSTR argv[]);
BOOL SetRow(int argc, LPCWSTR argv[]);
BOOL SetColumn(int argc, LPCWSTR argv[]);

DWORD GetPosition(LPWSTR buffer, DWORD size);
DWORD GetRow(LPWSTR buffer, DWORD size);
DWORD GetColumn(LPWSTR buffer, DWORD size);

DWORD GetSize(LPWSTR buffer, DWORD size);
DWORD GetWidth(LPWSTR buffer, DWORD size);
DWORD GetHeight(LPWSTR buffer, DWORD size);

BOOL SetColor(int argc, LPCWSTR argv[]);
BOOL SetOpacity(int argc, LPCWSTR argv[]);
BOOL SetConsoleCursor(int argc, LPCWSTR argv[]);
BOOL SetCodePage(int argc, LPCWSTR argv[]);

DWORD GetColor(LPWSTR buffer, DWORD size);
DWORD GetOpacity(LPWSTR buffer, DWORD size);
DWORD GetConsoleCursor(LPWSTR buffer, DWORD size);
DWORD GetCodePage(LPWSTR buffer, DWORD size);

DWORD GetArgCount(LPWSTR buffer, DWORD size);
DWORD GetArgs(DWORD first, DWORD last, LPWSTR buffer, DWORD size);

BOOL SetUnicode(int argc, LPCWSTR argv[]);
BOOL SetDelayedExpansion(int argc, LPCWSTR argv[]);
BOOL SetExtensions(int argc, LPCWSTR argv[]);

DWORD GetUnicode(LPWSTR buffer, DWORD size);
DWORD GetDelayedExpansion(LPWSTR buffer, DWORD size);
DWORD GetExtensions(LPWSTR buffer, DWORD size);
DWORD GetTransient(LPWSTR buffer, DWORD size);

DWORD GetStdin(LPWSTR buffer, DWORD size);
DWORD GetStdout(LPWSTR buffer, DWORD size);
DWORD GetStderr(LPWSTR buffer, DWORD size);

DWORD GetUnique(LPWSTR buffer, DWORD size);
DWORD GetTempFile(LPWSTR buffer, DWORD size);
DWORD GetTempDir(LPWSTR buffer, DWORD size);

BOOL WaitMilliseconds(int argc, LPCWSTR argv[]);

BOOL SetMyTimer(int argc, LPCWSTR argv[]);

DWORD GetTimer(LPWSTR buffer, DWORD size);

DWORD GetDate(LPWSTR buffer, DWORD size);
DWORD GetDateTime(LPWSTR buffer, DWORD size);
DWORD GetDateTimems(LPWSTR buffer, DWORD size);
DWORD GetDateTimedotms(LPWSTR buffer, DWORD size);
DWORD GetTime(LPWSTR buffer, DWORD size);
DWORD GetTimems(LPWSTR buffer, DWORD size);
DWORD GetTimedotms(LPWSTR buffer, DWORD size);
DWORD GetYear(LPWSTR buffer, DWORD size);
DWORD GetMonth(LPWSTR buffer, DWORD size);
DWORD GetMonthName(LPWSTR buffer, DWORD size);
DWORD GetMonthShort(LPWSTR buffer, DWORD size);
DWORD GetDay(LPWSTR buffer, DWORD size);
DWORD GetDayName(LPWSTR buffer, DWORD size);
DWORD GetDayShort(LPWSTR buffer, DWORD size);
DWORD GetDayOfWeek(LPWSTR buffer, DWORD size);
DWORD GetHour(LPWSTR buffer, DWORD size);
DWORD GetMinute(LPWSTR buffer, DWORD size);
DWORD GetSecond(LPWSTR buffer, DWORD size);
DWORD GetMilliseconds(LPWSTR buffer, DWORD size);

DWORD GetEnglish(LPWSTR buffer, DWORD size);
DWORD GetDecSep(LPWSTR buffer, DWORD size);

BOOL SetEnglish(int argc, LPCWSTR argv[]);
BOOL SetDecSep(int argc, LPCWSTR argv[]);

DWORD GetBatchLine(LPWSTR buffer, DWORD size);
DWORD GetBatchFile(LPWSTR buffer, DWORD size);

BOOL SetBatchFile(int argc, LPCWSTR argv[]);

DWORD GetEBVersion(LPWSTR buffer, DWORD size);
DWORD GetEnhancedBatch(LPWSTR buffer, DWORD size);

DWORD GetOSVersion(LPWSTR buffer, DWORD size);
DWORD GetOSMajor(LPWSTR buffer, DWORD size);
DWORD GetOSMinor(LPWSTR buffer, DWORD size);
DWORD GetOSBuild(LPWSTR buffer, DWORD size);
DWORD GetOSRevision(LPWSTR buffer, DWORD size);

DWORD GetCmdVersion(LPWSTR buffer, DWORD size);

DWORD GetTitle(LPWSTR buffer, DWORD size);

DWORD GetElevated(LPWSTR buffer, DWORD size);

DWORD GetRun(LPCWSTR cmd, LPWSTR buffer, DWORD size);

BOOL SetDumpTokens(int argc, LPCWSTR argv[]);
BOOL SetDumpParse(int argc, LPCWSTR argv[]);

HANDLE getOutputHandle(void);
