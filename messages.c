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

const WCHAR ClearBriefStr[]   = L"Clear a window.";
const WCHAR EchoBriefStr[]	  = L"Display a message.";
const WCHAR GetkbBriefStr[]   = L"Wait for a key.";
const WCHAR HelpBriefStr[]	  = L"This list.";
const WCHAR ImageBriefStr[]   = L"Draw an image.";
const WCHAR KbhitBriefStr[]   = L"Test for a key.";
const WCHAR SayBriefStr[]	  = L"Speak a message.";
const WCHAR SleepBriefStr[]   = L"Suspend execution.";
const WCHAR TextBriefStr[]	  = L"Draw a message.";
const WCHAR TimerBriefStr[]   = L"Millisecond timer.";
const WCHAR TimerHiBriefStr[] = L"Microsecond timer.";
const WCHAR UnloadBriefStr[]  = L"Remove Enhanced Batch.";

const WCHAR ClearHelpStr[] =
	L"Clear a window.\r\n"
	L"\r\n"
	L"CALL @CLEAR [/A color] [/C char] [/F] [/N] [row column rows columns]\r\n"
	L"\r\n"
	L"  /A        set attributes (default is current)\r\n"
	L"  /C        set character (default is space)\r\n"
	L"  /F        fill (set one, preserve the other)\r\n"
	L"  /N        do not move the cursor (default is to move to row column)\r\n"
	L"\r\n"
	L"If a region is not specified the first time @CLEAR is used the current row will\r\n"
	L"become the top line of a new window."
;

const WCHAR EchoHelpStr[] =
	L"Display a message.\r\n"
	L"\r\n"
	L"CALL @ECHO [/A[color]] [/B[color]] [/C] [/Ccolumn] [/E[c]] [/F[color]] [/L]\r\n"
	L"           [/N] [/Prow column] [/Rrow] [/U] [/V] [//] message ...\r\n"
	L"\r\n"
	L"  /A        set background and foreground colors\r\n"
	L"  /B        set background color\r\n"
	L"  /C        output message to the console (toggle)\r\n"
	L"            move to the column\r\n"
	L"  /E        process C-style escapes\r\n"
	L"  /F        set foreground color\r\n"
	L"  /L        toggle underline\r\n"
	L"  /N        do not output a final newline\r\n"
	L"  /P        move to the row and column\r\n"
	L"  /R        move to the row\r\n"
	L"  /U        finish with a Unix line ending\r\n"
	L"  /V        output message vertically (toggle)\r\n"
	L"  //        no more options\r\n"
	L"\r\n"
	L"A single space is added between message arguments, but nothing is added\r\n"
	L"between message and option.\r\n"
	L"\r\n"
	L"The escape character is \\ by default; supply c to use any non-alphanumeric\r\n"
	L"except ?, which will list the supported escapes.\r\n"
	L"\r\n"
	L"Color is a single hexadecimal digit for /F and /B, one or two digits for /A.\r\n"
	L"If absent the original color will be restored.  The original colors (and\r\n"
	L"underline) are restored on exit.\r\n"
	L"\r\n"
	L"If row or column is used /N is implied and the original position will be\r\n"
	L"restored on exit."
;

const WCHAR EscapeHelpStr[] =
	L"a   Alert (U+0007)\r\n"
	L"b   Backspace (U+0008)\r\n"
	L"e   Escape (U+001B)\r\n"
	L"f   Form feed (U+000C)\r\n"
	L"n   Line feed (U+000A)\r\n"
	L"r   Carriage return (U+000D)\r\n"
	L"t   Tab (U+0009)\r\n"
	L"v   Vertical tab (U+000B)\r\n"
	L"x   Unicode, one or two hex digits\r\n"
	L"u   Unicode, one to four hex digits\r\n"
	L"U   Unicode, one to six hex digits\r\n"
	L"\r\n"
	L"The escape character will generate itself; any other character will be\r\n"
	L"ignored, preserving the escape character."
;

const WCHAR GetkbHelpStr[] =
	L"Wait for a key.\r\n"
	L"\r\n"
	L"CALL @GETKB\r\n"
	L"\r\n"
	L"Errorlevel will contain the code of the key pressed."
;

const WCHAR HelpHelpStr[] =
	L"Display CALL commands added by Enhanced Batch.\r\n"
	L"\r\n"
	L"CALL @HELP"
;

const WCHAR ImageHelpStr[] =
	L"Draw an image.\r\n"
	L"\r\n"
	L"CALL @IMAGE [/C[H] column] [/COPY] [/DISCARD] [/F frame] [/FB pixels] [/FC w h]\r\n"
	L"            [/FL pixels] [/FR pixels] [/FS w h] [/FT pixels] [/N] [/O x y]\r\n"
	L"            [/P[H] row column] [/Q] [/R[H] row] [/RESTORE] [/S w h] image\r\n"
	L"\r\n"
	L"  /C        column to place image (H adds half a character width)\r\n"
	L"  /COPY     make a copy of what the image will replace\r\n"
	L"  /DISCARD  dispose of the copy\r\n"
	L"  /F        frame number to display (0 or invalid is first)\r\n"
	L"  /FB       bottom pixels between frames\r\n"
	L"  /FC       count of frames\r\n"
	L"  /FL       left pixels between frames\r\n"
	L"  /FR       right pixels between frames\r\n"
	L"  /FS       size of a frame\r\n"
	L"  /FT       top pixels between frames\r\n"
	L"  /N        return the number of frames (implies /Q)\r\n"
	L"  /O        display the image from this position (origin)\r\n"
	L"  /P        row and column to place image (H adds half a character)\r\n"
	L"  /Q        do not display the image\r\n"
	L"  /R        row to place image (H adds half a character height)\r\n"
	L"  /RESTORE  display the copy, then dispose of it\r\n"
	L"  /S        display this much of the image (size)\r\n"
	L"  image     file name of the image\r\n"
	L"\r\n"
	L"Errorlevel will be 0 if the image could not be loaded; otherwise the high word\r\n"
	L"will contain the delay between this frame and the next (in milliseconds) and\r\n"
	L"the low word will contain the size (in characters): columns in the low byte and\r\n"
	L"rows in the high byte.\r\n"
	L"\r\n"
	L"Copy will preserve what the image will replace; subsequent images with the same\r\n"
	L"position and file name will use the copy as their background."
;

const WCHAR KbhitHelpStr[] =
	L"Test if a key has been pressed.\r\n"
	L"\r\n"
	L"CALL @KBHIT\r\n"
	L"\r\n"
	L"Errorlevel will contain the code of the key pressed, 0 if none."
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

const WCHAR TextHelpStr[] =
	L"Draw a message.\r\n"
	L"\r\n"
	L"CALL @TEXT [/B] [/BG color] [/C[H] column] [/D[H | W]] [/E angle] [/F font]\r\n"
	L"           [/FG color] [/I] [/N] [/O angle] [/P[H] row column] [/R[H] row]\r\n"
	L"           [/RL] [/RR] [/RT] [/S] [/T] [/U] message\r\n"
	L"\r\n"
	L"  /B        bold text\r\n"
	L"  /BG       background color\r\n"
	L"  /C        column to place text (H adds half a character width)\r\n"
	L"  /D        double height and/or width\r\n"
	L"  /E        text escapement angle (tenths of a degree)\r\n"
	L"  /F        face name of the font\r\n"
	L"  /FG       text color\r\n"
	L"  /I        italic text\r\n"
	L"  /N        non-antialiased text\r\n"
	L"  /O        character orientation angle (tenths of a degree)\r\n"
	L"  /P        row and column to place text (H adds half a character)\r\n"
	L"  /R        row to place text (H adds half a character height)\r\n"
	L"  /RL       rotate left (90\xb0)\r\n"
	L"  /RR       rotate right (-90\xb0)\r\n"
	L"  /RT       rotate twice (180\xb0)\r\n"
	L"  /S        strikeout text\r\n"
	L"  /T        transparent background\r\n"
	L"  /U        underlined text\r\n"
	L"\r\n"
	L"Tabs are not expanded.\r\n"
	L"\r\n"
	L"Color is either a single hexadecimal digit (normal index); two digits (decimal\r\n"
	L"index); three hexadecimal digits (RGB expanded to RRGGBB), otherwise RRGGBB.\r\n"
	L"\r\n"
	L"Rotation will also rotate the origin; escapement/orientation will use the\r\n"
	L"bottom-left as the origin."
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
