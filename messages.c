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

const WCHAR CloseWindowStr[] = L"\r\nPress any key to close the window.";

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

const WCHAR ConstStr[] = L"Constants";
const WCHAR GetSetStr[] = L"Read/Write Variables";
const WCHAR GetterStr[] = L"Read-only Variables";
const WCHAR SetterStr[] = L"Write-only Variables";
const WCHAR CallStr[] = L"Functions";

const WCHAR AttrBriefStr[]				= L"Console attributes";
const WCHAR BatchFileBriefStr[] 		= L"Batch file name in error messages";
const WCHAR BatchLineBriefStr[] 		= L"Current batch file line number";
const WCHAR BackgroundBriefStr[]		= L"Console background color";
const WCHAR ReadcharBriefStr[]			= L"Read a character from the keyboard";
const WCHAR CmdVersionBriefStr[]		= L"Version number of CMD";
const WCHAR ColorBriefStr[] 			= L"Console colors";
const WCHAR ColumnBriefStr[]			= L"Console window cursor column";
const WCHAR CtrlBriefStr[]				= L"Control characters";
const WCHAR CtrlCAbortsBriefStr[]		= L"Ctrl+C behavior";
const WCHAR ConsoleCursorBriefStr[] 	= L"Console cursor visibility/size";
const WCHAR DateBriefStr[]				= L"Date as YYYY-MM-DD";
const WCHAR DateTimeBriefStr[]			= L"Date and time as YYYY-MM-DD HH:MM:SS";
const WCHAR DateTimemsBriefStr[]		= L"Date and time as YYYY-MM-DD HH:MM:SS.mmm";
const WCHAR DayBriefStr[]				= L"Day of month";
const WCHAR DayNameBriefStr[]			= L"Day of week (name)";
const WCHAR DayOfWeekBriefStr[] 		= L"Day of week (number)";
const WCHAR DayShortBriefStr[]			= L"Day of week (short name)";
const WCHAR DecSepBriefStr[]			= L"Decimal separator";
const WCHAR EchoBriefStr[]				= L"Echo status";
const WCHAR DelayedExpansionBriefStr[]	= L"Delayed expansion behavior";
const WCHAR EnglishBriefStr[]			= L"Use English names";
const WCHAR EnhancedBatchBriefStr[] 	= L"Enhanced Batch feature number";
const WCHAR ExtensionsBriefStr[]		= L"Extensions behavior";
const WCHAR ForegroundBriefStr[]		= L"Console foreground color";
const WCHAR ForLineBriefStr[]			= L"Line number of FOR /F file";
const WCHAR ForLinesBriefStr[]			= L"Total number of lines of FOR /F file";
const WCHAR GetcharBriefStr[]			= L"Wait for a character from the keyboard";
const WCHAR GetkeyBriefStr[]			= L"Wait for a key";
const WCHAR HeightBriefStr[]			= L"Console window height";
const WCHAR HourBriefStr[]				= L"Hour value of the current time";
const WCHAR InputCodePageBriefStr[] 	= L"Input code page";
const WCHAR ElevatedBriefStr[]			= L"Running as administrator";
const WCHAR ReadkeyBriefStr[]			= L"Read a key";
const WCHAR MillisecondsBriefStr[]		= L"Milliseconds value of the current time";
const WCHAR MinuteBriefStr[]			= L"Minute value of the current time";
const WCHAR MonthBriefStr[] 			= L"Month of the current date";
const WCHAR MonthNameBriefStr[] 		= L"Name of the current month";
const WCHAR MonthShortBriefStr[]		= L"Short name of the current month";
const WCHAR NoBriefStr[]				= L"Character for no";
const WCHAR OpacityBriefStr[]			= L"Opacity of the window";
const WCHAR OSBuildBriefStr[]			= L"Windows build version number";
const WCHAR OSMajorBriefStr[]			= L"Windows major version number";
const WCHAR OSMinorBriefStr[]			= L"Windows minor version number";
const WCHAR OSRevisionBriefStr[]		= L"Windows revision version number";
const WCHAR OSVersionBriefStr[] 		= L"Windows version number";
const WCHAR OutputCodePageBriefStr[]	= L"Output code page";
const WCHAR PositionBriefStr[]			= L"Console window cursor position";
const WCHAR RowBriefStr[]				= L"Console window cursor row";
const WCHAR SecondBriefStr[]			= L"Seconds value of the current time";
const WCHAR SizeBriefStr[]				= L"Console window size";
const WCHAR StderrBriefStr[]			= L"Standard error mode";
const WCHAR StdinBriefStr[] 			= L"Standard input mode";
const WCHAR StdoutBriefStr[]			= L"Standard output mode";
const WCHAR TempDirBriefStr[]			= L"Create a temporary directory";
const WCHAR TempFileBriefStr[]			= L"Create a temporary file";
const WCHAR TimeBriefStr[]				= L"Time as HH:MM:SS";
const WCHAR TimemsBriefStr[]			= L"Time as HH:MM:SS.mmm";
const WCHAR TimerVarBriefStr[]			= L"Time of the millisecond timer";
const WCHAR HiTimerBriefStr[]			= L"Time of the microsecond timer";
const WCHAR TitleBriefStr[] 			= L"Title of the window";
const WCHAR TraceBriefStr[] 			= L"Trace commands";
const WCHAR TransientBriefStr[] 		= L"Transient state of CMD";
const WCHAR UnderlineBriefStr[] 		= L"Console underline attribute";
const WCHAR UnicodeBriefStr[]			= L"Unicode output";
const WCHAR UniqueBriefStr[]			= L"Create a unique file";
const WCHAR UnixTimeBriefStr[]			= L"Time as number of seconds";
const WCHAR EBVersionBriefStr[] 		= L"Enhanced Batch version number";
const WCHAR VoiceBriefStr[] 			= L"Text-to-speech voice";
const WCHAR WidthBriefStr[] 			= L"Console window width";
const WCHAR YearBriefStr[]				= L"Year of the current date";
const WCHAR YesBriefStr[]				= L"Character for yes";

const WCHAR CodePageBriefStr[]			= L"Set both input and output code pages";
const WCHAR DumpParseBriefStr[] 		= L"Display parser processing";
const WCHAR DumpTokensBriefStr[]		= L"Display token processing";
const WCHAR NextBriefStr[]				= L"Set next FOR range value";

const WCHAR CheckkeyBriefStr[]			= L"Check if any key was pressed";
const WCHAR ClearBriefStr[] 			= L"Clear a window";
const WCHAR HelpBriefStr[]				= L"This list";
const WCHAR ImageBriefStr[] 			= L"Draw an image";
const WCHAR ImgBriefStr[]				= L"Display an image";
const WCHAR SayBriefStr[]				= L"Speak a message";
const WCHAR SleepBriefStr[] 			= L"Suspend execution";
const WCHAR TextBriefStr[]				= L"Draw a message";
const WCHAR TimerBriefStr[] 			= L"Millisecond timer";
const WCHAR TimerHiBriefStr[]			= L"Microsecond timer";
const WCHAR UnloadBriefStr[]			= L"Remove Enhanced Batch";
const WCHAR WaitkeyBriefStr[]			= L"Wait for any key";
const WCHAR WriteBriefStr[] 			= L"Display a message";

const WCHAR AttrHelpStr[] =
	L"Get or set the console attributes.\r\n"
	L"\r\n"
	L"The attributes are a four-digit hexadecimal number.  Set an empty value to\r\n"
	L"restore the original attributes."
;

const WCHAR BatchFileHelpStr[] =
	L"Get or set the batch file name in error messages.\r\n"
	L"\r\n"
	L"  0         do not show the file name\r\n"
	L"  1         show only the file name (default)\r\n"
	L"  2         show the complete path\r\n"
	L"\r\n"
	L"If the name is shown the line number will be shown after it."
;

const WCHAR BatchLineHelpStr[] =
	L"Get the line number of the current batch file (0 if none)."
;

const WCHAR BackgroundHelpStr[] =
	L"Get or set the console background color.\r\n"
	L"\r\n"
	L"The color is a single hexadecimal digit.  Set an empty value to restore the\r\n"
	L"original background."
;

const WCHAR ReadcharHelpStr[] =
	L"Read a character from the keyboard.\r\n"
	L"\r\n"
	L"Only reads characters, other keys are discarded.  If no character is available\r\n"
	L"uses -1."
;

const WCHAR CmdVersionHelpStr[] =
	L"Get the version number of CMD."
;

const WCHAR ColorHelpStr[] =
	L"Get or set the console colors.\r\n"
	L"\r\n"
	L"The colors are a two-digit hexadecimal number (background and foreground).  Set\r\n"
	L"an empty value to restore the original colors."
;

const WCHAR ColumnHelpStr[] =
	L"Get or set the window cursor column.\r\n"
	L"\r\n"
	L"The left column is 0; will be -1 if there is no console.\r\n"
;

const WCHAR CtrlHelpStr[] =
	L"Get a control character.\r\n"
	L"\r\n"
	L"  A-Z       characters 1 to 26 (U+0001 to U+001A)\r\n"
	L"  [         character 27 (U+001B)\r\n"
	L"  \\         character 28 (U+001C)\r\n"
	L"  ]         character 29 (U+001D)\r\n"
	L"  ^         character 30 (U+001E)\r\n"
	L"  _         character 31 (U+001F)"
;

const WCHAR CtrlCAbortsHelpStr[] =
	L"Get or set the Ctrl+C behavior.\r\n"
	L"\r\n"
	L"  -1/empty  ask to terminate (default)\r\n"
	L"  0/OFF     do not terminate\r\n"
	L"  1/ON      terminate"
;

const WCHAR ConsoleCursorHelpStr[] =
	L"Get or set the console cursor size or visibility.\r\n"
	L"\r\n"
	L"  0         hide\r\n"
	L"  1         show\r\n"
	L"  2-100     cursor size as a percentage of the font size"
;

const WCHAR DateHelpStr[] =
	L"Get the current date.\r\n"
	L"\r\n"
	L"The format is always YYYY-MM-DD."
;

const WCHAR DateTimeHelpStr[] =
	L"Get the current date and time.\r\n"
	L"\r\n"
	L"The format is always YYYY-MM-DD HH:MM:SS."
;

const WCHAR DateTimemsHelpStr[] =
	L"Get the current date and time, with milliseconds.\r\n"
	L"\r\n"
	L"The format is always YYYY-MM-DD HH:MM:SS.mmm (but '.' is the value of @decsep)."
;

const WCHAR DayHelpStr[] =
	L"Get the current day of the month."
;

const WCHAR DayNameHelpStr[] =
	L"Get the current day of the week, as a name.\r\n"
	L"\r\n"
	L"The name will be English if @english is on, otherwise locale dependent."
;

const WCHAR DayOfWeekHelpStr[] =
	L"Get the current day of the week, as a number.\r\n"
	L"\r\n"
	L"Sunday is 0."
;

const WCHAR DayShortHelpStr[] =
	L"Get the current day of the week, as an abbreviated name.\r\n"
	L"\r\n"
	L"The name will be English if @english is on, otherwise locale dependent."
;

const WCHAR DecSepHelpStr[] =
	L"Get or set the decimal separator.\r\n"
	L"\r\n"
	L"Any character can be used (but only the first if more than one).  Set an empty\r\n"
	L"value to restore the original separator."
;

const WCHAR DelayedExpansionHelpStr[] =
	L"Get or set delayed expansion."
;

const WCHAR EchoHelpStr[] =
	L"Get or set the echo status."
;

const WCHAR EnglishHelpStr[] =
	L"Get or set English names\r\n"
	L"\r\n"
	L"If this is on English will be used for month and day names, as well as the Yes\r\n"
	L"and No characters."
;

const WCHAR EnhancedBatchHelpStr[] =
	L"Get the feature number of Enhanced Batch.\r\n"
	L"\r\n"
	L"This is currently 0; it will change to a number that will increment as features\r\n"
	L"are added to EB after the first release."
;

const WCHAR ExtensionsHelpStr[] =
	L"Get or set CMD extensions."
;

const WCHAR ForegroundHelpStr[] =
	L"Get or set the console foreground color.\r\n"
	L"\r\n"
	L"The color is a single hexadecimal digit.  Set an empty value to restore the\r\n"
	L"original foreground."
;

const WCHAR ForLineHelpStr[] =
	L"Get the current line number of FOR /F.\r\n"
	L"\r\n"
	L"Blank lines are still skipped, but they count as line numbers."
;

const WCHAR ForLinesHelpStr[] =
	L"Get the total number of lines of the previous FOR /F.";
;

const WCHAR GetcharHelpStr[] =
	L"Wait for a character from the keyboard.\r\n"
	L"\r\n"
	L"Only reads characters, other keys are discarded."
;

const WCHAR GetkeyHelpStr[] =
	L"Wait for a key from the keyboard."
;

const WCHAR HeightHelpStr[] =
	L"Get the height (number of rows) of the console window."
;

const WCHAR HourHelpStr[] =
	L"Get the hour of the current time."
;

const WCHAR InputCodePageHelpStr[] =
	L"Get or set the input code page.\r\n"
	L"\r\n"
	L"Set to empty or OEM to use the system OEM code page; UTF-8, UTF8 or UTF_8 to\r\n"
	L"use UTF-8; or ANSI to use the system ANSI code page."
;

const WCHAR ElevatedHelpStr[] =
	L"Get the elevated (running as administrator) status of CMD."
;

const WCHAR ReadkeyHelpStr[] =
	L"Read a key from the keyboard.\r\n"
	L"\r\n"
	L"If no key is available uses -1."
;

const WCHAR MillisecondsHelpStr[] =
	L"Get the milliseconds of the current time."
;

const WCHAR MinuteHelpStr[] =
	L"Get the minutes of the current time."
;

const WCHAR MonthHelpStr[] =
	L"Get the month of the current date."
;

const WCHAR MonthNameHelpStr[] =
	L"Get the name of the month of the current date.\r\n"
	L"\r\n"
	L"The name will be English if @english is on, otherwise locale dependent."
;

const WCHAR MonthShortHelpStr[] =
	L"Get the abbreviated name of the month of the current date.\r\n"
	L"\r\n"
	L"The name will be English if @english is on, otherwise locale dependent."
;

const WCHAR NoHelpStr[] =
	L"Get the character for no.\r\n"
	L"\r\n"
	L"This will be N if @english is on, otherwise locale dependent."
;

const WCHAR OpacityHelpStr[] =
	L"Get or set the opacity of the console window.\r\n"
	L"\r\n"
	L"This will work with Windows Terminal, but it applies to all tabs."
;

const WCHAR OSBuildHelpStr[] =
	L"Get the build version number of Windows."
;

const WCHAR OSMajorHelpStr[] =
	L"Get the major version number of Windows."
;

const WCHAR OSMinorHelpStr[] =
	L"Get the minor version number of Windows."
;

const WCHAR OSRevisionHelpStr[] =
	L"Get the revision version number of Windows."
;

const WCHAR OSVersionHelpStr[] =
	L"Get the version number of Windows."
;

const WCHAR OutputCodePageHelpStr[] =
	L"Get or set the output code page.\r\n"
	L"\r\n"
	L"Set to empty or OEM to use the system OEM code page; UTF-8, UTF8 or UTF_8 to\r\n"
	L"use UTF-8; or ANSI to use the system ANSI code page."
;

const WCHAR PositionHelpStr[] =
	L"Get or set the window cursor position.\r\n"
	L"\r\n"
	L"  row column\r\n"
	L"\r\n"
	L"Top left is 0 0; will be -1 -1 if there is no console."
;

const WCHAR RowHelpStr[] =
	L"Get or set the window cursor row.\r\n"
	L"\r\n"
	L"The top row is 0; will be -1 if there is no console."
;

const WCHAR SecondHelpStr[] =
	L"Get the seconds of the current time."
;

const WCHAR SizeHelpStr[] =
	L"Get the size (rows columns) of the console window."
;

const WCHAR StderrHelpStr[] =
	L"Get the mode of standard error.\r\n"
	L"\r\n"
	L"  0     redirected\r\n"
	L"  1     the console"
;

const WCHAR StdinHelpStr[] =
	L"Get the mode of standard input.\r\n"
	L"\r\n"
	L"  0     redirected\r\n"
	L"  1     the keyboard"
;

const WCHAR StdoutHelpStr[] =
	L"Get the mode of standard output.\r\n"
	L"\r\n"
	L"  0     redirected\r\n"
	L"  1     the console"
;

const WCHAR TempDirHelpStr[] =
	L"Create a temporary directory.\r\n"
	L"\r\n"
	L"The directory is created in the user's temporary folder."
;

const WCHAR TempFileHelpStr[] =
	L"Create a temporary file.\r\n"
	L"\r\n"
	L"The file is created in the user's temporary folder."
;

const WCHAR TimeHelpStr[] =
	L"Get the current time.\r\n"
	L"\r\n"
	L"The format is always HH:MM:SS."
;

const WCHAR TimemsHelpStr[] =
	L"Get the current time, with milliseconds.\r\n"
	L"\r\n"
	L"The format is always HH:MM:SS.mmm (but '.' is the value of @decsep)."
;

const WCHAR TimerVarHelpStr[] =
	L"Get the time of the millisecond timer."
;

const WCHAR HiTimerHelpStr[] =
	L"Get the time of the microsecond timer."
;

const WCHAR TitleHelpStr[] =
	L"Get the title of the console window.\r\n"
	L"\r\n"
	L"The \"Administrator: \" prefix is removed."
;

const WCHAR TraceHelpStr[] =
	L"Get or set the trace status.\r\n"
	L"\r\n"
	L"Enabling trace will write commands to standard error, in accordance with the\r\n"
	L"@batchfile setting.  Commands will be placed in square brackets if writing to\r\n"
	L"the console (e.g. \"test.bat:5: [echo trace]\")."
;

const WCHAR TransientHelpStr[] =
	L"Get the transient state of CMD.\r\n"
	L"\r\n"
	L"  0     CMD is not transient\r\n"
	L"  1     CMD is transient (/C)"
;

const WCHAR UnderlineHelpStr[] =
	L"Get or set the console's underline attribute.\r\n"
	L"\r\n"
	L"This is only effective with DBCS or Windows 10."
;

const WCHAR UnicodeHelpStr[] =
	L"Get or set Unicode output."
;

const WCHAR UniqueHelpStr[] =
	L"Create a unique file.\r\n"
	L"\r\n"
	L"The file is created in the current directory."
;

const WCHAR UnixTimeHelpStr[] =
	L"Get the current time as a number of seconds.\r\n"
	L"\r\n"
	L"The time is the number of seconds since 1970-01-01 0:00:00 UTC."
;

const WCHAR EBVersionHelpStr[] =
	L"Get the version number of Enhanced Batch.\r\n"
	L"\r\n"
	L"This is currently the date of compilation; it will change to a version number\r\n"
	L"after the first release."
;

const WCHAR VoiceHelpStr[] =
	L"Get or set the text-to-speech voice.\r\n"
	L"\r\n"
	L"  empty     restore the default voice\r\n"
	L"\r\n"
	L"Use \"CALL @SAY /V\" to list available voices.  Only a portion of the voice need\r\n"
	L"be given, the first match will be used."
;

const WCHAR WidthHelpStr[] =
	L"Get the width (number of columns) of the console window."
;

const WCHAR YearHelpStr[] =
	L"Get the year of the current date."
;

const WCHAR YesHelpStr[] =
	L"Get the character for yes.\r\n"
	L"\r\n"
	L"This will be Y if @english is on, otherwise locale dependent."
;


const WCHAR CodePageHelpStr[] =
	L"Set both the input and output code pages.\r\n"
	L"\r\n"
	L"Set to empty or OEM to use the system OEM code page; UTF-8, UTF8 or UTF_8 to\r\n"
	L"use UTF-8; or ANSI to use the system ANSI code page."
;

const WCHAR DumpParseHelpStr[] =
	L"Set CMD's internal parser processing."
;

const WCHAR DumpTokensHelpStr[] =
	L"Set CMD's internal token processing."
;

const WCHAR NextHelpStr[] =
	L"Set the next value of EB's range loops.\r\n"
	L"\r\n"
	L"  empty     terminate the loop\r\n"
	L"\r\n"
	L"The value takes effect on the next iteration."
;


const WCHAR CheckkeyHelpStr[] =
	L"Check if any key was pressed.\r\n"
	L"\r\n"
	L"CALL @CHECKKEY\r\n"
	L"\r\n"
	L"Errorlevel will be 0 if a key was pressed, else 1."
;

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

const WCHAR WriteHelpStr[] =
	L"Display a message.\r\n"
	L"\r\n"
	L"CALL @WRITE [/A[color]] [/B[color]] [/C column] [/CON] [/E[c]] [/F[color]] [/L]\r\n"
	L"            [/N] [/P row column] [/R row] [/U] [/V] [//] message ...\r\n"
	L"\r\n"
	L"  /A        set background and foreground colors\r\n"
	L"  /B        set background color\r\n"
	L"  /C        move to the column\r\n"
	L"  /CON      output message to the console (toggle)\r\n"
	L"  /E        process C-style escapes (toggle)\r\n"
	L"  /F        set foreground color\r\n"
	L"  /L        toggle underline\r\n"
	L"  /N        do not output a final newline\r\n"
	L"  /P        move to the row and column\r\n"
	L"  /R        move to the row\r\n"
	L"  /U        finish with a Unix line ending\r\n"
	L"  /V        output message vertically (toggle)\r\n"
	L"  //        no more options\r\n"
	L"\r\n"
	L"A single space is added between consecutive message arguments, but nothing is\r\n"
	L"added between message, option and message.\r\n"
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

const WCHAR HelpHelpStr[] =
	L"Display the extensions added by Enhanced Batch.\r\n"
	L"\r\n"
	L"CALL @HELP [extension | ALL | CALL | CONST | GET | SET]\r\n"
	L"\r\n"
	L"  extension  display the help for this extension\r\n"
	L"  ALL        display the help for all extensions\r\n"
	L"  CALL       display the functions\r\n"
	L"  CONST      display the constants\r\n"
	L"  GET        display the readable variables\r\n"
	L"  SET        display the writable variables"
;

const WCHAR ImageHelpStr[] =
	L"Draw an image.\r\n"
	L"\r\n"
	L"CALL @IMAGE [/C[H] column] [/COPY] [/DISCARD] [/F frame] [/FB pixels]\r\n"
	L"            [/FC rows columns] [/FL pixels] [/FP row column] [/FR pixels]\r\n"
	L"            [/FS width height] [/FT pixels] [/N] [/O x y] [/P[H] row column]\r\n"
	L"            [/Q] [/R[H] row] [/RESTORE] [/S width height] image\r\n"
	L"\r\n"
	L"  /C        column to place image (H adds half a character width)\r\n"
	L"  /COPY     make a copy of what the image will replace\r\n"
	L"  /DISCARD  dispose of the copy\r\n"
	L"  /F        frame number to display (0 or invalid is first)\r\n"
	L"  /FB       bottom pixels between frames\r\n"
	L"  /FC       count of frames\r\n"
	L"  /FL       left pixels between frames\r\n"
	L"  /FP       position of frame to display\r\n"
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

const WCHAR ImgHelpStr[] =
	L"Display an image.\r\n"
	L"\r\n"
	L"CALL @IMG [/C column] [/Cn] [/COPY] [/D rows columns] [/DISCARD] [/F frame]\r\n"
	L"          [/FB pixels] [/FL pixels] [/FC rows columns] [/FP row column]\r\n"
	L"          [/FR pixels] [/FS width height] [/FT pixels] [/H rows] [/N] [/O x y]\r\n"
	L"          [/P row column] [/Q] [/R row] [/RESTORE] [/S width height]\r\n"
	L"          [/T[color]] [/W columns] image\r\n"
	L"\r\n"
	L"  /C        column to place image\r\n"
	L"            color quality, where n is:\r\n"
	L"              0: system colors\r\n"
	L"              1: use light/medium/dark shades for dithering (legacy default)\r\n"
	L"              2: RGB control sequences (Windows 10 default)\r\n"
	L"  /COPY     make a copy of what the image will replace\r\n"
	L"  /D        dimensions of the image\r\n"
	L"  /DISCARD  dispose of the copy\r\n"
	L"  /F        frame number to display (0 or invalid is first)\r\n"
	L"  /FB       bottom pixels between frames\r\n"
	L"  /FC       count of frames\r\n"
	L"  /FL       left pixels between frames\r\n"
	L"  /FP       position of frame to display\r\n"
	L"  /FR       right pixels between frames\r\n"
	L"  /FS       size of a frame\r\n"
	L"  /FT       top pixels between frames\r\n"
	L"  /H        height of the image (determine width automatically)\r\n"
	L"  /N        return the number of frames (implies /Q)\r\n"
	L"  /O        display the image from this position (origin)\r\n"
	L"  /P        row and column to place image\r\n"
	L"  /Q        do not display the image\r\n"
	L"  /R        row to place image\r\n"
	L"  /RESTORE  display the copy, then dispose of it\r\n"
	L"  /S        display this much of the image (size)\r\n"
	L"  /T        use space for transparent pixels (default color is current bg)\r\n"
	L"  /W        width of the image (determine height automatically)\r\n"
	L"  image     file name of the image\r\n"
	L"\r\n"
	L"Errorlevel will be 0 if the image could not be loaded; otherwise the high word\r\n"
	L"will contain the delay between this frame and the next (in milliseconds) and\r\n"
	L"the low word will contain the size: columns in the low byte and rows in the\r\n"
	L"high byte.\r\n"
	L"\r\n"
	L"Specify 0 for a dimension to use the original (up to window size); rows will\r\n"
	L"use half the size (there are two pixel lines per character row).\r\n"
	L"\r\n"
	L"Color is either a single hexadecimal digit (normal index); two digits (decimal\r\n"
	L"index); three hexadecimal digits (RGB expanded to RRGGBB), otherwise RRGGBB.\r\n"
	L"\r\n"
	L"Copy will preserve what the image will replace; subsequent images with the same\r\n"
	L"position and file name will use the copy as their background.  Due to console\r\n"
	L"limitations RGB colors are mapped back to system colors."
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

const WCHAR WaitkeyHelpStr[] =
	L"Wait for any key to be pressed.\r\n"
	L"\r\n"
	L"CALL @WAITKEY [/C] [/F]\r\n"
	L"\r\n"
	L"  /C        only wait if the window would close (implies /F)\r\n"
	L"  /F        flush all keys before waiting"
;
