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

// Don't really want to include anything just for WCHAR or wchar_t.
typedef unsigned short WCHAR;

const WCHAR HelpBriefStr[]	  = L"This help.";
const WCHAR SleepBriefStr[]   = L"Suspend execution for some milliseconds.";
const WCHAR TimerBriefStr[]   = L"Millisecond timer.";
const WCHAR TimerHiBriefStr[] = L"Microsecond timer.";
const WCHAR UnloadBriefStr[]  = L"Remove Enhanced Batch.";

const WCHAR HelpHelpStr[] =
	L"Display CALL commands added by Enhanced Batch.\r\n"
	L"\r\n"
	L"CALL @HELP"
;

const WCHAR SleepHelpStr[] =
	L"Suspend execution for the specified time.\r\n"
	L"\r\n"
	L"CALL @SLEEP milliseconds"
;

const WCHAR TimerHelpStr[] =
	L"Run a low-resolution timer.\r\n"
	L"\r\n"
	L"CALL @TIMER [start|stop]\r\n"
	L"\r\n"
	L"<none>    toggle start/stop\r\n"
	L"start     start the timer (even if it's running)\r\n"
	L"stop      stop the timer\r\n"
	L"\r\n"
	L"The time is measured in milliseconds and has a resolution of about 10ms.\r\n"
	L"It can be retrieved with the @TIMER variable."
;

const WCHAR TimerHiHelpStr[] =
	L"Run a high-resolution timer.\r\n"
	L"\r\n"
	L"CALL @TIMERHI [start|stop]\r\n"
	L"\r\n"
	L"The time is measured in microseconds and has a resolution dependent on the\r\n"
	L"CPU.  It can be retrieved with the @TIMERHI variable."
;

const WCHAR UnloadHelpStr[] =
	L"Remove Enhanced Batch from CMD.\r\n"
	L"\r\n"
	L"CALL @UNLOAD\r\n"
	L"\r\n"
	L"This happens automatically when the batch exits, so is not normally needed."
;
