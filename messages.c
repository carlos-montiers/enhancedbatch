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

const WCHAR NotOptionStr[] = L"Unknown option: %s.\n";
const WCHAR NotExtensionStr[] = L"Unknown extension: %s.\n";

const WCHAR ConstStr[] = L"Constants";
const WCHAR GetSetStr[] = L"Read/Write Variables";
const WCHAR GetterStr[] = L"Read-only Variables";
const WCHAR SetterStr[] = L"Write-only Variables";
const WCHAR CallStr[] = L"Functions";
const WCHAR ModStr[] = L"Modifiers";

const char AttrBriefStr[]				= "Console attributes";
const char BatchFileBriefStr[]			= "Batch file name in error messages";
const char BatchLineBriefStr[]			= "Current batch file line number";
const char BackgroundBriefStr[] 		= "Console background color";
const char ReadcharBriefStr[]			= "Read a character from the keyboard";
const char CmdVersionBriefStr[] 		= "Version number of CMD";
const char ColorBriefStr[]				= "Console colors";
const char ColumnBriefStr[] 			= "Console window cursor column";
const char CtrlBriefStr[]				= "Control characters";
const char CtrlCAbortsBriefStr[]		= "Ctrl+C behavior";
const char ConsoleCursorBriefStr[]		= "Console cursor visibility/size";
const char DateBriefStr[]				= "Date as YYYY-MM-DD";
const char DateTimeBriefStr[]			= "Date and time as YYYY-MM-DD HH:MM:SS";
const char DateTimemsBriefStr[] 		= "Date and time as YYYY-MM-DD HH:MM:SS.mmm";
const char DayBriefStr[]				= "Day of month";
const char DayNameBriefStr[]			= "Day of week (name)";
const char DayOfWeekBriefStr[]			= "Day of week (number)";
const char DayShortBriefStr[]			= "Day of week (short name)";
const char DecSepBriefStr[] 			= "Decimal separator";
const char EchoBriefStr[]				= "Echo status";
const char DelayedExpansionBriefStr[]	= "Delayed expansion behavior";
const char EnglishBriefStr[]			= "Use English names";
const char EnhancedBatchBriefStr[]		= "Enhanced Batch feature number";
const char ExtensionsBriefStr[] 		= "Extensions behavior";
const char ForegroundBriefStr[] 		= "Console foreground color";
const char ForLineBriefStr[]			= "Line number of FOR /F file";
const char ForLinesBriefStr[]			= "Total number of lines of FOR /F file";
const char GetcharBriefStr[]			= "Wait for a character from the keyboard";
const char GetkeyBriefStr[] 			= "Wait for a key";
const char HeightBriefStr[] 			= "Console window height";
const char HourBriefStr[]				= "Hour value of the current time";
const char InputCodePageBriefStr[]		= "Input code page";
const char ElevatedBriefStr[]			= "Running as administrator";
const char ReadkeyBriefStr[]			= "Read a key";
const char MillisecondsBriefStr[]		= "Milliseconds value of the current time";
const char MinuteBriefStr[] 			= "Minute value of the current time";
const char MonthBriefStr[]				= "Month of the current date";
const char MonthNameBriefStr[]			= "Name of the current month";
const char MonthShortBriefStr[] 		= "Short name of the current month";
const char NoBriefStr[] 				= "Character for no";
const char OpacityBriefStr[]			= "Opacity of the window";
const char OSBuildBriefStr[]			= "Windows build version number";
const char OSMajorBriefStr[]			= "Windows major version number";
const char OSMinorBriefStr[]			= "Windows minor version number";
const char OSRevisionBriefStr[] 		= "Windows revision version number";
const char OSVersionBriefStr[]			= "Windows version number";
const char OutputCodePageBriefStr[] 	= "Output code page";
const char PositionBriefStr[]			= "Console window cursor position";
const char RowBriefStr[]				= "Console window cursor row";
const char SecondBriefStr[] 			= "Seconds value of the current time";
const char SizeBriefStr[]				= "Console window size";
const char StderrBriefStr[] 			= "Standard error mode";
const char StdinBriefStr[]				= "Standard input mode";
const char StdoutBriefStr[] 			= "Standard output mode";
const char TempDirBriefStr[]			= "Create a temporary directory";
const char TempFileBriefStr[]			= "Create a temporary file";
const char TimeBriefStr[]				= "Time as HH:MM:SS";
const char TimemsBriefStr[] 			= "Time as HH:MM:SS.mmm";
const char TimerVarBriefStr[]			= "Time of the millisecond timer";
const char HiTimerBriefStr[]			= "Time of the microsecond timer";
const char TitleBriefStr[]				= "Title of the window";
const char TraceBriefStr[]				= "Trace commands";
const char TransientBriefStr[]			= "Transient state of CMD";
const char UnderlineBriefStr[]			= "Console underline attribute";
const char UnicodeBriefStr[]			= "Unicode output";
const char UniqueBriefStr[] 			= "Create a unique file";
const char UnixTimeBriefStr[]			= "Time as number of seconds";
const char EBVersionBriefStr[]			= "Enhanced Batch version number";
const char VoiceBriefStr[]				= "Text-to-speech voice";
const char WidthBriefStr[]				= "Console window width";
const char YearBriefStr[]				= "Year of the current date";
const char YesBriefStr[]				= "Character for yes";

const char CodePageBriefStr[]			= "Set both input and output code pages";
const char DumpParseBriefStr[]			= "Display parser processing";
const char DumpTokensBriefStr[] 		= "Display token processing";
const char NextBriefStr[]				= "Set next FOR range value";

const char CheckkeyBriefStr[]			= "Check if any key was pressed";
const char ClearBriefStr[]				= "Clear a window";
const char ElevateBriefStr[]			= "Run a command as administrator";
const char HelpBriefStr[]				= "This list";
const char ImageBriefStr[]				= "Draw an image";
const char ImgBriefStr[]				= "Display an image";
const char SayBriefStr[]				= "Speak a message";
const char SleepBriefStr[]				= "Suspend execution";
const char TextBriefStr[]				= "Draw a message";
const char TimerBriefStr[]				= "Millisecond timer";
const char TimerHiBriefStr[]			= "Microsecond timer";
const char UnloadBriefStr[] 			= "Remove Enhanced Batch";
const char WaitkeyBriefStr[]			= "Wait for any key";
const char WriteBriefStr[]				= "Display a message";

const char DefaultBriefStr[] 			= "Default value";
const char AltBriefStr[] 				= "Test if a key contains Alt";
const char CapitalBriefStr[] 			= "Upper case characters after space or tab";
const char CapitalListBriefStr[] 		= "Upper case characters after those in LIST";
const char CtrlModBriefStr[]			= "Test if a key contains Ctrl";
const char HexifyBriefStr[] 			= "Convert to hexadecimal";
const char KeyBriefStr[] 				= "The key itself, without modifiers";
const char LengthBriefStr[] 			= "The length of the variable";
const char LowerBriefStr[]				= "Convert all to lower case";
const char LtrimBriefStr[]				= "Remove leading spaces and tabs";
const char LtrimListBriefStr[]			= "Remove leading characters from LIST";
const char NumberBriefStr[] 			= "Padding";
const char RtrimBriefStr[]				= "Remove trailing spaces and tabs";
const char RtrimListBriefStr[]			= "Remove trailing characters from LIST";
const char ShiftBriefStr[]				= "Test if a key contains Shift";
const char TrimBriefStr[]				= "Remove leading and trailing spaces and tabs";
const char TrimListBriefStr[]			= "Remove leading and trailing characters from LIST";
const char UnhexifyBriefStr[]			= "Convert from hexadecimal";
const char UpperBriefStr[]				= "Convert all to upper case";
const char TildeBriefStr[]				= "Same as FOR";

const char AttrHelpStr[] =
	"Get or set the console attributes.\r\n"
	"\r\n"
	"The attributes are a four-digit hexadecimal number.  Set an empty value to\r\n"
	"restore the original attributes."
;

const char BatchFileHelpStr[] =
	"Get or set the batch file name in error messages.\r\n"
	"\r\n"
	"  0         do not show the file name\r\n"
	"  1         show only the file name (default)\r\n"
	"  2         show the complete path\r\n"
	"\r\n"
	"If the name is shown the line number will be shown after it."
;

const char BatchLineHelpStr[] =
	"Get the line number of the current batch file (0 if none)."
;

const char BackgroundHelpStr[] =
	"Get or set the console background color.\r\n"
	"\r\n"
	"The color is a single hexadecimal digit.  Set an empty value to restore the\r\n"
	"original background."
;

const char ReadcharHelpStr[] =
	"Read a character from the keyboard.\r\n"
	"\r\n"
	"Only reads characters, other keys are discarded.  If no character is available\r\n"
	"uses -1."
;

const char CmdVersionHelpStr[] =
	"Get the version number of CMD."
;

const char ColorHelpStr[] =
	"Get or set the console colors.\r\n"
	"\r\n"
	"The colors are a two-digit hexadecimal number (background and foreground).  Set\r\n"
	"an empty value to restore the original colors."
;

const char ColumnHelpStr[] =
	"Get or set the window cursor column.\r\n"
	"\r\n"
	"The left column is 0; will be -1 if there is no console."
;

const char CtrlHelpStr[] =
	"Get a control character.\r\n"
	"\r\n"
	"  A-Z       characters 1 to 26 (U+0001 to U+001A)\r\n"
	"  [         character 27 (U+001B)\r\n"
	"  \\         character 28 (U+001C)\r\n"
	"  ]         character 29 (U+001D)\r\n"
	"  ^         character 30 (U+001E)\r\n"
	"  _         character 31 (U+001F)"
;

const char CtrlCAbortsHelpStr[] =
	"Get or set the Ctrl+C behavior.\r\n"
	"\r\n"
	"  -1/empty  ask to terminate (default)\r\n"
	"  0/OFF     do not terminate\r\n"
	"  1/ON      terminate"
;

const char ConsoleCursorHelpStr[] =
	"Get or set the console cursor size or visibility.\r\n"
	"\r\n"
	"  0         hide\r\n"
	"  1         show\r\n"
	"  2-100     cursor size as a percentage of the font size"
;

const char DateHelpStr[] =
	"Get the current date.\r\n"
	"\r\n"
	"The format is always YYYY-MM-DD."
;

const char DateTimeHelpStr[] =
	"Get the current date and time.\r\n"
	"\r\n"
	"The format is always YYYY-MM-DD HH:MM:SS."
;

const char DateTimemsHelpStr[] =
	"Get the current date and time, with milliseconds.\r\n"
	"\r\n"
	"The format is always YYYY-MM-DD HH:MM:SS.mmm (but '.' is the value of @decsep)."
;

const char DayHelpStr[] =
	"Get the current day of the month."
;

const char DayNameHelpStr[] =
	"Get the current day of the week, as a name.\r\n"
	"\r\n"
	"The name will be English if @english is on, otherwise locale dependent."
;

const char DayOfWeekHelpStr[] =
	"Get the current day of the week, as a number.\r\n"
	"\r\n"
	"Sunday is 0."
;

const char DayShortHelpStr[] =
	"Get the current day of the week, as an abbreviated name.\r\n"
	"\r\n"
	"The name will be English if @english is on, otherwise locale dependent."
;

const char DecSepHelpStr[] =
	"Get or set the decimal separator.\r\n"
	"\r\n"
	"Any character can be used (but only the first if more than one).  Set an empty\r\n"
	"value to restore the original separator."
;

const char DelayedExpansionHelpStr[] =
	"Get or set delayed expansion."
;

const char EchoHelpStr[] =
	"Get or set the echo status."
;

const char EnglishHelpStr[] =
	"Get or set English names\r\n"
	"\r\n"
	"If this is on English will be used for month and day names, as well as the Yes\r\n"
	"and No characters."
;

const char EnhancedBatchHelpStr[] =
	"Get the feature number of Enhanced Batch.\r\n"
	"\r\n"
	"This is currently 0; it will change to a number that will increment as features\r\n"
	"are added to EB after the first release."
;

const char ExtensionsHelpStr[] =
	"Get or set CMD extensions."
;

const char ForegroundHelpStr[] =
	"Get or set the console foreground color.\r\n"
	"\r\n"
	"The color is a single hexadecimal digit.  Set an empty value to restore the\r\n"
	"original foreground."
;

const char ForLineHelpStr[] =
	"Get the current line number of FOR /F.\r\n"
	"\r\n"
	"Blank lines are still skipped, but they count as line numbers."
;

const char ForLinesHelpStr[] =
	"Get the total number of lines of the previous FOR /F.";
;

const char GetcharHelpStr[] =
	"Wait for a character from the keyboard.\r\n"
	"\r\n"
	"Only reads characters, other keys are discarded."
;

const char GetkeyHelpStr[] =
	"Wait for a key from the keyboard."
;

const char HeightHelpStr[] =
	"Get the height (number of rows) of the console window."
;

const char HourHelpStr[] =
	"Get the hour of the current time."
;

const char InputCodePageHelpStr[] =
	"Get or set the input code page.\r\n"
	"\r\n"
	"Set to empty or OEM to use the system OEM code page; UTF-8, UTF8 or UTF_8 to\r\n"
	"use UTF-8; or ANSI to use the system ANSI code page."
;

const char ElevatedHelpStr[] =
	"Get the elevated (running as administrator) status of CMD."
;

const char ReadkeyHelpStr[] =
	"Read a key from the keyboard.\r\n"
	"\r\n"
	"If no key is available uses -1."
;

const char MillisecondsHelpStr[] =
	"Get the milliseconds of the current time."
;

const char MinuteHelpStr[] =
	"Get the minutes of the current time."
;

const char MonthHelpStr[] =
	"Get the month of the current date."
;

const char MonthNameHelpStr[] =
	"Get the name of the month of the current date.\r\n"
	"\r\n"
	"The name will be English if @english is on, otherwise locale dependent."
;

const char MonthShortHelpStr[] =
	"Get the abbreviated name of the month of the current date.\r\n"
	"\r\n"
	"The name will be English if @english is on, otherwise locale dependent."
;

const char NoHelpStr[] =
	"Get the character for no.\r\n"
	"\r\n"
	"This will be N if @english is on, otherwise locale dependent."
;

const char OpacityHelpStr[] =
	"Get or set the opacity of the console window.\r\n"
	"\r\n"
	"This will work with Windows Terminal, but it applies to all tabs."
;

const char OSBuildHelpStr[] =
	"Get the build version number of Windows."
;

const char OSMajorHelpStr[] =
	"Get the major version number of Windows."
;

const char OSMinorHelpStr[] =
	"Get the minor version number of Windows."
;

const char OSRevisionHelpStr[] =
	"Get the revision version number of Windows."
;

const char OSVersionHelpStr[] =
	"Get the version number of Windows."
;

const char OutputCodePageHelpStr[] =
	"Get or set the output code page.\r\n"
	"\r\n"
	"Set to empty or OEM to use the system OEM code page; UTF-8, UTF8 or UTF_8 to\r\n"
	"use UTF-8; or ANSI to use the system ANSI code page."
;

const char PositionHelpStr[] =
	"Get or set the window cursor position.\r\n"
	"\r\n"
	"  row column\r\n"
	"\r\n"
	"Top left is 0 0; will be -1 -1 if there is no console."
;

const char RowHelpStr[] =
	"Get or set the window cursor row.\r\n"
	"\r\n"
	"The top row is 0; will be -1 if there is no console."
;

const char SecondHelpStr[] =
	"Get the seconds of the current time."
;

const char SizeHelpStr[] =
	"Get the size (rows columns) of the console window."
;

const char StderrHelpStr[] =
	"Get the mode of standard error.\r\n"
	"\r\n"
	"  0     redirected\r\n"
	"  1     the console"
;

const char StdinHelpStr[] =
	"Get the mode of standard input.\r\n"
	"\r\n"
	"  0     redirected\r\n"
	"  1     the keyboard"
;

const char StdoutHelpStr[] =
	"Get the mode of standard output.\r\n"
	"\r\n"
	"  0     redirected\r\n"
	"  1     the console"
;

const char TempDirHelpStr[] =
	"Create a temporary directory.\r\n"
	"\r\n"
	"The directory is created in the user's temporary folder."
;

const char TempFileHelpStr[] =
	"Create a temporary file.\r\n"
	"\r\n"
	"The file is created in the user's temporary folder."
;

const char TimeHelpStr[] =
	"Get the current time.\r\n"
	"\r\n"
	"The format is always HH:MM:SS."
;

const char TimemsHelpStr[] =
	"Get the current time, with milliseconds.\r\n"
	"\r\n"
	"The format is always HH:MM:SS.mmm (but '.' is the value of @decsep)."
;

const char TimerVarHelpStr[] =
	"Get the time of the millisecond timer."
;

const char HiTimerHelpStr[] =
	"Get the time of the microsecond timer."
;

const char TitleHelpStr[] =
	"Get the title of the console window.\r\n"
	"\r\n"
	"The \"Administrator: \" prefix is removed."
;

const char TraceHelpStr[] =
	"Get or set the trace status.\r\n"
	"\r\n"
	"Enabling trace will write commands to standard error, in accordance with the\r\n"
	"@batchfile setting.  Commands will be placed in square brackets if writing to\r\n"
	"the console (e.g. \"test.bat:5: [echo trace]\")."
;

const char TransientHelpStr[] =
	"Get the transient state of CMD.\r\n"
	"\r\n"
	"  0     CMD is not transient\r\n"
	"  1     CMD is transient (/C)"
;

const char UnderlineHelpStr[] =
	"Get or set the console's underline attribute.\r\n"
	"\r\n"
	"This is only effective with DBCS or Windows 10."
;

const char UnicodeHelpStr[] =
	"Get or set Unicode output."
;

const char UniqueHelpStr[] =
	"Create a unique file.\r\n"
	"\r\n"
	"The file is created in the current directory."
;

const char UnixTimeHelpStr[] =
	"Get the current time as a number of seconds.\r\n"
	"\r\n"
	"The time is the number of seconds since 1970-01-01 0:00:00 UTC."
;

const char EBVersionHelpStr[] =
	"Get the version number of Enhanced Batch.\r\n"
	"\r\n"
	"This is currently the date of compilation; it will change to a version number\r\n"
	"after the first release."
;

const char VoiceHelpStr[] =
	"Get or set the text-to-speech voice.\r\n"
	"\r\n"
	"  empty     restore the default voice\r\n"
	"\r\n"
	"Use \"CALL @SAY /V\" to list available voices.  Only a portion of the voice need\r\n"
	"be given, the first match will be used."
;

const char WidthHelpStr[] =
	"Get the width (number of columns) of the console window."
;

const char YearHelpStr[] =
	"Get the year of the current date."
;

const char YesHelpStr[] =
	"Get the character for yes.\r\n"
	"\r\n"
	"This will be Y if @english is on, otherwise locale dependent."
;


const char CodePageHelpStr[] =
	"Set both the input and output code pages.\r\n"
	"\r\n"
	"Set to empty or OEM to use the system OEM code page; UTF-8, UTF8 or UTF_8 to\r\n"
	"use UTF-8; or ANSI to use the system ANSI code page."
;

const char DumpParseHelpStr[] =
	"Set CMD's internal parser processing."
;

const char DumpTokensHelpStr[] =
	"Set CMD's internal token processing."
;

const char NextHelpStr[] =
	"Set the next value of EB's range loops.\r\n"
	"\r\n"
	"  empty     terminate the loop\r\n"
	"\r\n"
	"The value takes effect on the next iteration."
;


const char CheckkeyHelpStr[] =
	"Check if any key was pressed.\r\n"
	"\r\n"
	"CALL @CHECKKEY\r\n"
	"\r\n"
	"Errorlevel will be 0 if a key was pressed, else 1."
;

const char ClearHelpStr[] =
	"Clear a window.\r\n"
	"\r\n"
	"CALL @CLEAR [/A color] [/C char] [/F] [/N] [row column rows columns]\r\n"
	"\r\n"
	"  /A        set attributes (default is current)\r\n"
	"  /C        set character (default is space)\r\n"
	"  /F        fill (set one, preserve the other)\r\n"
	"  /N        do not move the cursor (default is to move to row column)\r\n"
	"\r\n"
	"If a region is not specified the first time @CLEAR is used the current row will\r\n"
	"become the top line of a new window."
;

const char WriteHelpStr[] =
	"Display a message.\r\n"
	"\r\n"
	"CALL @WRITE [/A[color]] [/B[color]] [/C column] [/CON] [/E[c]] [/F[color]] [/L]\r\n"
	"            [/N] [/P row column] [/R row] [/U] [/V] [//] message ...\r\n"
	"\r\n"
	"  /A        set background and foreground colors\r\n"
	"  /B        set background color\r\n"
	"  /C        move to the column\r\n"
	"  /CON      output message to the console (toggle)\r\n"
	"  /E        process C-style escapes (toggle)\r\n"
	"  /F        set foreground color\r\n"
	"  /L        toggle underline\r\n"
	"  /N        do not output a final newline\r\n"
	"  /P        move to the row and column\r\n"
	"  /R        move to the row\r\n"
	"  /U        finish with a Unix line ending\r\n"
	"  /V        output message vertically (toggle)\r\n"
	"  //        no more options\r\n"
	"\r\n"
	"A single space is added between consecutive message arguments, but nothing is\r\n"
	"added between message, option and message.\r\n"
	"\r\n"
	"The escape character is \\ by default; supply c to use any non-alphanumeric\r\n"
	"except ?, which will list the supported escapes.\r\n"
	"\r\n"
	"Color is a single hexadecimal digit for /F and /B, one or two digits for /A.\r\n"
	"If absent the original color will be restored.  The original colors (and\r\n"
	"underline) are restored on exit.\r\n"
	"\r\n"
	"If row or column is used /N is implied and the original position will be\r\n"
	"restored on exit."
;

const char EscapeHelpStr[] =
	"a   Alert (U+0007)\r\n"
	"b   Backspace (U+0008)\r\n"
	"e   Escape (U+001B)\r\n"
	"f   Form feed (U+000C)\r\n"
	"n   Line feed (U+000A)\r\n"
	"r   Carriage return (U+000D)\r\n"
	"t   Tab (U+0009)\r\n"
	"v   Vertical tab (U+000B)\r\n"
	"x   Unicode, one or two hex digits\r\n"
	"u   Unicode, one to four hex digits\r\n"
	"U   Unicode, one to six hex digits\r\n"
	"\r\n"
	"The escape character will generate itself; any other character will be\r\n"
	"ignored, preserving the escape character."
;

const char ElevateHelpStr[] =
	"Run a command as administrator.\r\n"
	"\r\n"
	"CALL @ELEVATE [/N[K]] [command [arguments]]\r\n"
	"\r\n"
	"  /N         run in a new window (and Keep it open)\r\n"
	"\r\n"
	"The command is run via CMD.EXE, with Enhanced Batch present.  Environment\r\n"
	"variables present in the current CMD, but not in the elevated CMD, will be \r\n"
	"added; other variables will remain unchanged.  Elevation is handled by \"Windows\r\n"
	"host process (Rundll32)\" (default User Account Control setting will cause a\r\n"
	"prompt for authorization); it will remain available for the remainder of the\r\n"
	"batch file (avoiding multiple prompts).  Errorlevel will be 60399 if the\r\n"
	"elevated command failed to run (0xEBEF - Enhanced Batch Elevation Failed)."
;

const char HelpHelpStr[] =
	"Display the extensions added by Enhanced Batch.\r\n"
	"\r\n"
	"CALL @HELP [extension | ALL | CALL | CONST | GET | MOD | SET]\r\n"
	"\r\n"
	"  extension  display the help for this extension\r\n"
	"  ALL        display the help for all extensions\r\n"
	"  CALL       display the functions\r\n"
	"  CONST      display the constants\r\n"
	"  GET        display the readable variables\r\n"
	"  MOD        display the variable modifiers\r\n"
	"  SET        display the writable variables"
;

const char ImageHelpStr[] =
	"Draw an image.\r\n"
	"\r\n"
	"CALL @IMAGE [/C[H] column] [/COPY] [/DISCARD] [/F frame] [/FB pixels]\r\n"
	"            [/FC rows columns] [/FL pixels] [/FP row column] [/FR pixels]\r\n"
	"            [/FS width height] [/FT pixels] [/N] [/O x y] [/P[H] row column]\r\n"
	"            [/Q] [/R[H] row] [/RESTORE] [/S width height] image\r\n"
	"\r\n"
	"  /C        column to place image (H adds half a character width)\r\n"
	"  /COPY     make a copy of what the image will replace\r\n"
	"  /DISCARD  dispose of the copy\r\n"
	"  /F        frame number to display (0 or invalid is first)\r\n"
	"  /FB       bottom pixels between frames\r\n"
	"  /FC       count of frames\r\n"
	"  /FL       left pixels between frames\r\n"
	"  /FP       position of frame to display\r\n"
	"  /FR       right pixels between frames\r\n"
	"  /FS       size of a frame\r\n"
	"  /FT       top pixels between frames\r\n"
	"  /N        return the number of frames (implies /Q)\r\n"
	"  /O        display the image from this position (origin)\r\n"
	"  /P        row and column to place image (H adds half a character)\r\n"
	"  /Q        do not display the image\r\n"
	"  /R        row to place image (H adds half a character height)\r\n"
	"  /RESTORE  display the copy, then dispose of it\r\n"
	"  /S        display this much of the image (size)\r\n"
	"  image     file name of the image\r\n"
	"\r\n"
	"Errorlevel will be 0 if the image could not be loaded; otherwise the high word\r\n"
	"will contain the delay between this frame and the next (in milliseconds) and\r\n"
	"the low word will contain the size (in characters): columns in the low byte and\r\n"
	"rows in the high byte.\r\n"
	"\r\n"
	"Copy will preserve what the image will replace; subsequent images with the same\r\n"
	"position and file name will use the copy as their background."
;

const char ImgHelpStr[] =
	"Display an image.\r\n"
	"\r\n"
	"CALL @IMG [/C column] [/Cn] [/COPY] [/D rows columns] [/DISCARD] [/F frame]\r\n"
	"          [/FB pixels] [/FL pixels] [/FC rows columns] [/FP row column]\r\n"
	"          [/FR pixels] [/FS width height] [/FT pixels] [/H rows] [/N] [/O x y]\r\n"
	"          [/P row column] [/Q] [/R row] [/RESTORE] [/S width height]\r\n"
	"          [/T[color]] [/W columns] image\r\n"
	"\r\n"
	"  /C        column to place image\r\n"
	"            color quality, where n is:\r\n"
	"              0: system colors\r\n"
	"              1: use light/medium/dark shades for dithering (legacy default)\r\n"
	"              2: RGB control sequences (Windows 10 default)\r\n"
	"  /COPY     make a copy of what the image will replace\r\n"
	"  /D        dimensions of the image\r\n"
	"  /DISCARD  dispose of the copy\r\n"
	"  /F        frame number to display (0 or invalid is first)\r\n"
	"  /FB       bottom pixels between frames\r\n"
	"  /FC       count of frames\r\n"
	"  /FL       left pixels between frames\r\n"
	"  /FP       position of frame to display\r\n"
	"  /FR       right pixels between frames\r\n"
	"  /FS       size of a frame\r\n"
	"  /FT       top pixels between frames\r\n"
	"  /H        height of the image (determine width automatically)\r\n"
	"  /N        return the number of frames (implies /Q)\r\n"
	"  /O        display the image from this position (origin)\r\n"
	"  /P        row and column to place image\r\n"
	"  /Q        do not display the image\r\n"
	"  /R        row to place image\r\n"
	"  /RESTORE  display the copy, then dispose of it\r\n"
	"  /S        display this much of the image (size)\r\n"
	"  /T        use space for transparent pixels (default color is current bg)\r\n"
	"  /W        width of the image (determine height automatically)\r\n"
	"  image     file name of the image\r\n"
	"\r\n"
	"Errorlevel will be 0 if the image could not be loaded; otherwise the high word\r\n"
	"will contain the delay between this frame and the next (in milliseconds) and\r\n"
	"the low word will contain the size: columns in the low byte and rows in the\r\n"
	"high byte.\r\n"
	"\r\n"
	"Specify 0 for a dimension to use the original (up to window size); rows will\r\n"
	"use half the size (there are two pixel lines per character row).\r\n"
	"\r\n"
	"Color is either a single hexadecimal digit (normal index); two digits (decimal\r\n"
	"index); three hexadecimal digits (RGB expanded to RRGGBB), otherwise RRGGBB.\r\n"
	"\r\n"
	"Copy will preserve what the image will replace; subsequent images with the same\r\n"
	"position and file name will use the copy as their background.  Due to console\r\n"
	"limitations RGB colors are mapped back to system colors."
;

const char SayHelpStr[] =
	"Speak a message.\r\n"
	"\r\n"
	"CALL @SAY [/N] [/S] [/U] [/V voice] [/W] [/X] message\r\n"
	"CALL @SAY /V\r\n"
	"\r\n"
	"  /N        output message without newline\r\n"
	"  /S        do not output message\r\n"
	"  /U        output message with Unix line ending\r\n"
	"  /V        select a voice or list voices\r\n"
	"  /W        wait for the speech to finish\r\n"
	"  /X        process SAPI XML\r\n"
	"\r\n"
	"Only a substring of the voice need be given; the first match will be used.\r\n"
	"An invalid voice will be ignored.  A default voice can be set using the\r\n"
	"@VOICE variable."
;

const char SleepHelpStr[] =
	"Suspend execution for the specified time.\r\n"
	"\r\n"
	"CALL @SLEEP milliseconds"
;

const char TextHelpStr[] =
	"Draw a message.\r\n"
	"\r\n"
	"CALL @TEXT [/B] [/BG color] [/C[H] column] [/D[H | W]] [/E angle] [/F font]\r\n"
	"           [/FG color] [/I] [/N] [/O angle] [/P[H] row column] [/R[H] row]\r\n"
	"           [/RL] [/RR] [/RT] [/S] [/T] [/U] message\r\n"
	"\r\n"
	"  /B        bold text\r\n"
	"  /BG       background color\r\n"
	"  /C        column to place text (H adds half a character width)\r\n"
	"  /D        double height and/or width\r\n"
	"  /E        text escapement angle (tenths of a degree)\r\n"
	"  /F        face name of the font\r\n"
	"  /FG       text color\r\n"
	"  /I        italic text\r\n"
	"  /N        non-antialiased text\r\n"
	"  /O        character orientation angle (tenths of a degree)\r\n"
	"  /P        row and column to place text (H adds half a character)\r\n"
	"  /R        row to place text (H adds half a character height)\r\n"
	"  /RL       rotate left (90\xb0)\r\n"
	"  /RR       rotate right (-90\xb0)\r\n"
	"  /RT       rotate twice (180\xb0)\r\n"
	"  /S        strikeout text\r\n"
	"  /T        transparent background\r\n"
	"  /U        underlined text\r\n"
	"\r\n"
	"Tabs are not expanded.\r\n"
	"\r\n"
	"Color is either a single hexadecimal digit (normal index); two digits (decimal\r\n"
	"index); three hexadecimal digits (RGB expanded to RRGGBB), otherwise RRGGBB.\r\n"
	"\r\n"
	"Rotation will also rotate the origin; escapement/orientation will use the\r\n"
	"bottom-left as the origin."
;

const char TimerHelpStr[] =
	"Run a low-resolution timer.\r\n"
	"\r\n"
	"CALL @TIMER [START | STOP]\r\n"
	"\r\n"
	"  <none>    toggle start/stop\r\n"
	"  START     start the timer (even if it's running)\r\n"
	"  STOP      stop the timer\r\n"
	"\r\n"
	"The time is measured in milliseconds and has a resolution of about 10ms.\r\n"
	"It can be retrieved with the @TIMER variable."
;

const char TimerHiHelpStr[] =
	"Run a high-resolution timer.\r\n"
	"\r\n"
	"CALL @TIMERHI [START | STOP]\r\n"
	"\r\n"
	"  <none>    toggle start/stop\r\n"
	"  START     start the timer (even if it's running)\r\n"
	"  STOP      stop the timer\r\n"
	"\r\n"
	"The time is measured in microseconds and has a resolution dependent on the\r\n"
	"CPU.  It can be retrieved with the @TIMERHI variable."
;

const char UnloadHelpStr[] =
	"Remove Enhanced Batch from CMD.\r\n"
	"\r\n"
	"CALL @UNLOAD\r\n"
	"\r\n"
	"This happens automatically when the batch exits, so is not normally needed."
;

const char WaitkeyHelpStr[] =
	"Wait for any key to be pressed.\r\n"
	"\r\n"
	"CALL @WAITKEY [/C] [/F]\r\n"
	"\r\n"
	"  /C        only wait if the window would close (implies /F)\r\n"
	"  /F        flush all keys before waiting"
;

const char DefaultHelpStr[] =
	"Default value if the variable is not defined.\r\n"
	"\r\n"
	"Currently the quote character cannot be included in the string; this may change\r\n"
	"in the future."
;

const char AltHelpStr[] =
	"Test if a key contains Alt.\r\n"
	"\r\n"
	"  0         Alt was not pressed\r\n"
	"  1         Alt was pressed\r\n"
	"\r\n"
	"See \"key\" for more details."
;

const char CapitalHelpStr[] =
	"Upper case characters after a space or tab, lower case everything else."
;

const char CapitalListHelpStr[] =
	"Upper case characters after a space, tab or anything in LIST, lower case\r\n"
	"everything else.\r\n"
	"\r\n"
	"To include ']' in the list place it first."
;

const char CtrlModHelpStr[] =
	"Test if a key contains Ctrl.\r\n"
	"\r\n"
	"  0         Ctrl was not pressed\r\n"
	"  1         Ctrl was pressed\r\n"
	"\r\n"
	"See \"key\" for more details."
;

const char HexifyHelpStr[] =
	"Convert characters to hexadecimal.\r\n"
	"\r\n"
	"Only characters below U+0100 will be converted, others will be ignored.  Upper\r\n"
	"case letters are used, with a space between each pair of digits."
;

const char KeyHelpStr[] =
	"The key itself, with the key modifiers specified.\r\n"
	"\r\n"
	"Key by itself will remove the modifiers, leaving just the key (or \"0\" if the\r\n"
	"variable is not a key).  Including modifiers (before or after \"key\" doesn't\r\n"
	"matter) will return \"0\" if all the modifiers are not pressed, otherwise the key\r\n"
	"with the modifiers.  E.g.: \"%;'C+A+VK_A';ctrl;key%\" will return \"C+VK_A\";\r\n"
	"\"%;'C+VK_A';key;shift%\" will return \"0\".  The order is always \"S+C+A+VK_name\".\r\n"
	"\r\n"
	"These modifiers are processed after all others, but before padding.  This means\r\n"
	"some modifiers may not work as you'd expect.  E.g.: \"key;length\" will return\r\n"
	"\"0\" - the length of the complete key, which is now no longer a key."
;

const char LengthHelpStr[] =
	"The length of the variable (in characters) at this point."
;

const char LowerHelpStr[] =
	"Convert all characters to lower case."
;

const char LtrimHelpStr[] =
	"Remove spaces and tabs from the start of the variable."
;

const char LtrimListHelpStr[] =
	"Remove the characters in LIST from the start of the variable.\r\n"
	"\r\n"
	"To include ']' in the list place it first."
;

const char NumberHelpStr[] =
	"Increase the variable to a specified size.\r\n"
	"\r\n"
	"If NUMBER starts with '-' left pad instead of right; if it starts with '0' pad\r\n"
	"with zeros instead of spaces.  Padding is applied after all other modifiers.\r\n"
	"Padding is only a minimum; longer variables will not be truncated."
;

const char RtrimHelpStr[] =
	"Remove spaces and tabs from the end of the variable."
;

const char RtrimListHelpStr[] =
	"Remove the characters in LIST from the end of the variable.\r\n"
	"\r\n"
	"To include ']' in the list place it first."
;

const char ShiftHelpStr[] =
	"Test if a key contains Shift.\r\n"
	"\r\n"
	"  0         Shift was not pressed\r\n"
	"  1         Shift was pressed\r\n"
	"\r\n"
	"See \"key\" for more details."
;

const char TrimHelpStr[] =
	"Remove spaces and tabs from the start and end of the variable."
;

const char TrimListHelpStr[] =
	"Remove the characters in LIST from the start and end of the variable.\r\n"
	"\r\n"
	"To include ']' in the list place it first."
;

const char UnhexifyHelpStr[] =
	"Convert characters from hexadecimal.\r\n"
	"\r\n"
	"Pairs of hexadecimal digits (upper or lower case, optionally prefixed with\r\n"
	"\"0x\") will be converted to characters.  Whitespace and any of the characters\r\n"
	"\",-.:;\" will be ignored; any other characters, or an odd number of digits, will\r\n"
	"result in an empty value."
;

const char UpperHelpStr[] =
	"Convert all characters to upper case."
;

const char TildeHelpStr[] =
	"Perform the same conversions as FOR (please see its help for details).\r\n"
	"\r\n"
	"Note: \"%$0;~...%\" will always use the batch file name."
;
