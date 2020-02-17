@echo off
rundll32 "%~dp0enhancedbatch_%processor_architecture%" load
If Not Defined @enhancedbatch (
Echo Load Failed
Goto :Eof
)
set "@delayedexpansion=1"

call @clear
call @say "Welcome to the test demo of Enhanced Batch"
pause & call @clear

::Save the current state.
set $prevcolor=!@color!
set $prevopacity=!@opacity!

set @opacity=80
Echo Using %@opacity% %% of opacity.

pause & call @clear

echo Playing with @forline and @forlines
set "file=!$0;~dp!LICENSE.txt"
for /f "usebackq" %%a in ("!file!") do (
  echo !@forline!:%%a
)
echo TOTAL lines:!@forlines!

pause & call @clear

set $spinner0=\
set $spinner1=^|
set $spinner2=/
set $spinner3=-
set $i=0
echo Playing with for infinite
for %%j in (:*) do (
  call @write /n "j = %%j press a key for stop !$spinner$i!!@CR!"
  set /a $i=(!$i!+1^)^&3
  call @checkkey && set @next=
)
echo !@LF!key pressed.

pause & call @clear

echo Playing with for range

for %%j in (:range*) do (
  for %%k in (:range*) do (
    echo %%j.%%k
    if %%k == 2 set @next=5
    if %%k == 6 set @next=
  )
  if %%j == 2 set @next=7
  if %%j == 8 set @next=
)
echo -------
for %%j in (:range*3) do echo %%j
echo -------
for %%j in (:range*4:6) do echo %%j
echo -------
for %%j in (:range*1:5:2) do echo %%j
echo -------
for %%j in (:range*-3) do echo %%j
echo -------
for %%j in (:range*3:0) do echo %%j
echo -------

pause & call @clear

set $A=1
echo $A=%$A%

echo Left aligned:  !$A;-5!.
echo Right aligned: !$A;5!.
echo Zero padded:   !$A;05!.

pause & call @clear

set "$var= oKay "
echo   Default: !$no-var;'undefined'!.
echo $var;typo: !$var;typo!.
echo     Upper: !$var;upper!.
echo     Lower: !$var;lower!.
echo   Capital: !$var;capital!.
echo    Length: !$var;length!.
echo      Trim: !$var;trim!.
echo     LTrim: !$var;ltrim!.
echo     RTrim: !$var;rtrim!.
echo Trim[ oy]: !$var;trim[ oy]!.
echo LTrim[ o]: !$var;ltrim[ o]!.
echo RTrim[ y]: !$var;rtrim[ y]!.
echo  Trim,Cap: !$var;trim;capital!.

pause & call @clear

echo Character TAB:!@TAB!.
::echo Character ESC:!@ESC!.
echo Character BS:!@BS!.
echo Character CR+LF:!@CR!!@LF!.
echo Character CRLF:!@CRLF!.
echo Character EXCL:!@EXCL!.
echo Character QUOT:!@QUOT!.
echo Character AMP:!@AMP!.
echo Character BAR:!@BAR!.
echo Character GT:!@GT!.
echo Character LT:!@LT!.
echo Character OPAR:!@OPAR!.
echo Character CPAR:!@CPAR!.
echo Character OBRC:!@OBRC!.
echo Character CBRC:!@CBRC!.
echo Character STAR:!@STAR!.
echo Character QUES:!@QUES!.
echo Character DOLLAR:!@DOLLAR!.
echo Character SEMI:!@SEMI!.
echo Character COMMA:!@COMMA!.
echo Character EQ:!@EQ!.

call @write /n "No quotes or newline."
echo.
call @write /u LF line.!@CR!
echo "A long string "^+
               "spread across "^+
               "multiple lines."

pause & call @clear

echo @osversion=%@osversion%
echo @osmajor=%@osmajor%
echo @osminor=%@osminor%
echo @osbuild=%@osbuild%
echo @osrevision=%@osrevision%

echo @cmdversion=%@cmdversion%

echo @version=%@version%

pause & call @clear

echo Changing color to A1
set @color=A1

echo Setting $myvar
set "$myvar=okay"
echo $myvar = %$myvar%
echo Displaying via SET:
set $myvar
echo Testing via DEFINED:
if defined $myvar echo Yep, it is defined.

echo Calling to extension @getch. Enter a character ...
set "code=%@getch%"
echo char: !code!

echo Calling to extension @chhit. Enter a character ...
set $spinner=\^|/-
set $i=0
:chloop
call @write /n // !$spinner:~%$i%,1!!@BS!
set /a $i=(%$i%+1)^&3
set "lastcode=%@chhit%"
if "!lastcode!" == "-1" call @sleep 100&goto chloop
echo last char: !lastcode!

echo Calling to extension @getkb. Press a key ...
set "code=%@getkb%"
echo pressed: !code!
echo key:     %code;key%
echo shift:   %code;shift%
echo ctrl:    %code;ctrl%
echo alt:     %code;alt%
echo s+key:   %code;shift;key%
echo c+a+key: %code;key;ctrl;alt%

echo Calling to extension @kbhit. Press a key ...
set $i=0
:kbloop
call @write /n // !$spinner:~%$i%,1!!@BS!
set /a $i=(%$i%+1)^&3
set "lastcode=%@kbhit%"
if "!lastcode!" == "VK_NONE" call @sleep 100&goto kbloop
echo last code: !lastcode!

set /a $row=%@row%+3
if %$row% geq %@height% echo.&echo.&echo.&set /a $row=%@height%-1
echo Setting position to %$row% 40 and printing text
set "@position=%$row% 40"
echo Hello
call @write /n Setting column to 45 and printing text
set "@column=45"
echo world^^!

call @write /n Current position: % %
Echo !@position! (row !@row!, column !@column!)
Echo Current size: !@size! (!@height! rows, !@width! columns)

pause & call @clear

echo Restoring color.
color !$prevcolor!

pause & call @clear

echo Restoring opacity.
set @opacity=!$prevopacity!

pause & call @clear

call @unload
