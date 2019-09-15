@echo off
rundll32 "%~dp0enhancedbatch_%processor_architecture%" load
If Not defined @enhancedbatch (
Echo Something failed
Goto :Eof
)
set "@delayedexpansion=1"

::Save the current state.
set $prevcolor=!@color!
set $prevopacity=!@opacity!

set @opacity=80
Echo Using %@opacity% %% of opacity.

pause & cls

echo You can configure echo for not show or use options
set "@echooptions=no"
echo
echo /?

pause & cls

echo Playing with forline and forlines
set "file=%~dp0LICENSE.txt"
for /f "usebackq" %%a in ("!file!") do (
  echo !@forline!:%%a
)
echo TOTAL lines:!@forlines!

pause & cls

echo Playing with infinite
for %%j in (:*) do (
  echo j = %%j press a key for stop
  set "lastcode=!@kbhit!"
  if !lastcode! gtr 0 set "@next="
)
echo key pressed.

pause & cls

echo Playing with range

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

pause & cls

set $A=1
set $a=2
echo $A=%$A% $a=%$a%

echo Left aligned:  !$A;-5!.
echo Right aligned: !$A;5!.
echo Zero padded:   !$A;05!.

pause & cls

set "$var= oKay "
echo   Default: !$no-var;'undefined'!.
echo   Invalid: !$var;invalid!.
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
echo  Trim,Cap: !$var;trim,capital!.

pause & cls

echo Character TAB:!$TAB!.
::echo Character ESC:!$ESC!.
echo Character BS:!$BS!.
echo Character CR+LF:!$CR!!$LF!.
echo Character CRLF:!$CRLF!.
echo Character EXCL:!$EXCL!.
echo Character QUOT:!$QUOT!.
echo Character AMP:!$AMP!.
echo Character BAR:!$BAR!.
echo Character GT:!$GT!.
echo Character LT:!$LT!.
echo Character OPAR:!$OPAR!.
echo Character CPAR:!$CPAR!.
echo Character OBRC:!$OBRC!.
echo Character CBRC:!$CBRC!.
echo Character STAR:!$STAR!.
echo Character QUES:!$QUES!.
echo Character DOLLAR:!$DOLLAR!.
echo Character SEMI:!$SEMI!.
echo Character COMMA:!$COMMA!.
echo Character EQ:!$EQ!.

echo;~"No quotes or newline."
echo.
echo,LF line.!$CR!
echo "A long string "^
               "spread across "^
               "multiple lines."

pause & cls

echo @osversion=%@osversion%
echo @osmajor=%@osmajor%
echo @osminor=%@osminor%
echo @osbuild=%@osbuild%
echo @osrevision=%@osrevision%

echo @cmdversion=%@cmdversion%

echo @version=%@version%

pause & cls

echo Changing color to A1
set @color=A1

echo Setting $myvar
set "$myvar=okay"
echo Looking in environment block:
set $myvar
echo Looking in heap:
if defined $myvar echo yep it is defined
echo $myvar = %$myvar%

echo Setting case sensitive variables:
set "$A=1"
set "$a=2"
echo $A = %$A% $a = %$a%

echo Calling to extension getch. Enter a character ...
set "code=%@getch%"
echo char: !code!

echo Calling to extension chhit. Enter a character ...
set $spinner=\^|/-
set $i=0
:chloop
echo;!$spinner:~%$i%,1!!$BS!
set /a $i=(%$i%+1)^&3
set "lastcode=%@chhit%"
if "!lastcode!" == "-1" call @sleep 100&goto chloop
echo last char: !lastcode!

echo Calling to extension getkb. Press a key ...
set "code=%@getkb%"
echo code: !code! (zero padded: !code;05!)

echo Calling to extension kbhit. Press a key ...
set $i=0
:kbloop
echo;!$spinner:~%$i%,1!!$BS!
set /a $i=(%$i%+1)^&3
set "lastcode=%@kbhit%"
if "!lastcode!" == "-1" call @sleep 100&goto kbloop
echo last code: !lastcode!

set /a $row=%@row%+3
if %$row% geq %@height% echo.&echo.&echo.&set /a $row=%@height%-1
echo Setting position to %$row% 40 and printing text
set "@position=%$row% 40"
echo Hello
echo;Setting column to 45 and printing text
set "@column=45"
echo world^^!

Echo;Current position: % %
Echo !@position! (row !@row!, column !@column!)
Echo Current size: !@size! (!@height! rows, !@width! columns)

pause & cls

echo Restoring color.
color !$prevcolor!

pause & cls

echo Restoring opacity.
set @opacity=!$prevopacity!

pause & cls

set "@unload="
