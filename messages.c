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

// Not sure if this should be translated, but I'll put it here, anyway.
const WCHAR ProgramNameStr[] = L"Enhanced Batch";

const WCHAR ParentErrStr[] = L"The parent process cannot be accessed.";

const WCHAR ArchErrStr[] =
#ifdef _WIN64
	L"The parent process is 32-bit, but this is the 64-bit (amd64) DLL.";
#else
	L"The parent process is 64-bit, but this is the 32-bit (x86) DLL.";
#endif

const WCHAR NotCmdStr[] = L"This process does not appear to be CMD.\n\n%s";
const WCHAR NotSupportedStr[] = L"CMD version %s is not supported.";

const WCHAR ArgErrorStr[]  = L"Failed to retrieve arguments.\n";
const WCHAR WrongArgsStr[] = L"Incorrect arguments: %d needed, %d provided.\n";
const WCHAR MoreArgsStr[]  = L"Incorrect arguments: at least %d needed, %d provided.\n";

const WCHAR HelpBriefStr[]	  = L"This list.";
const WCHAR Img4BitBriefStr[] = L"Display an image.";
const WCHAR SayBriefStr[]	  = L"Speak a message.";
const WCHAR SleepBriefStr[]   = L"Suspend execution.";
const WCHAR TimerBriefStr[]   = L"Millisecond timer.";
const WCHAR TimerHiBriefStr[] = L"Microsecond timer.";
const WCHAR UnloadBriefStr[]  = L"Remove Enhanced Batch.";

const WCHAR HelpHelpStr[] =
	L"Display CALL commands added by Enhanced Batch.\r\n"
	L"\r\n"
	L"CALL @HELP"
;

const WCHAR Img4BitHelpStr[] =
	L"Display an image.\r\n"
;

const WCHAR SayHelpStr[] =
	L"Speak a message.\r\n"
	L"\r\n"
	L"CALL @SAY [/N] [/S] [/U] [/V voice] [/W] [/X] message\r\n"
	L"CALL @SAY /V\r\n"
	L"\r\n"
	L"  /N        output message without newline\r\n"
	L"  /S        do not output message\r\n"
	L"  /U        output message with Unix line ending\r\n"
	L"  /V        select a voice or list voices\r\n"
	L"  /W        wait for the speech to finish\r\n"
	L"  /X        process SAPI XML\r\n"
	L"\r\n"
	L"Only a substring of the voice need be given; the first match will be used.\r\n"
	L"An invalid voice will be ignored.  A default voice can be set using the\r\n"
	L"@VOICE variable."
;

const WCHAR SleepHelpStr[] =
	L"Suspend execution for the specified time.\r\n"
	L"\r\n"
	L"CALL @SLEEP milliseconds"
;

const WCHAR TimerHelpStr[] =
	L"Run a low-resolution timer.\r\n"
	L"\r\n"
	L"CALL @TIMER [START | STOP]\r\n"
	L"\r\n"
	L"  <none>    toggle start/stop\r\n"
	L"  START     start the timer (even if it's running)\r\n"
	L"  STOP      stop the timer\r\n"
	L"\r\n"
	L"The time is measured in milliseconds and has a resolution of about 10ms.\r\n"
	L"It can be retrieved with the @TIMER variable."
;

const WCHAR TimerHiHelpStr[] =
	L"Run a high-resolution timer.\r\n"
	L"\r\n"
	L"CALL @TIMERHI [START | STOP]\r\n"
	L"\r\n"
	L"  <none>    toggle start/stop\r\n"
	L"  START     start the timer (even if it's running)\r\n"
	L"  STOP      stop the timer\r\n"
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
