@echo off
setlocal enableextensions enabledelayedexpansion

rundll32.exe %~dp0enhancedbatch_%processor_architecture%.dll,load
if not defined @enhancedbatch echo Failed: Enhanced Batch didn't load.&goto :eof

set $var=1
2>nul set $var && (echo Failed: $var is in the environment.&goto :eof)
if not defined $var echo Failed: $var is not defined.&goto :eof

call @timer start
call @timerhi start
call @sleep 45
call @timerhi
call @timer
set $hi=@timerhi
set $lo=@timer
:: These aren't as good as they could be due to testing in virtual machines.
if %$lo% lss 30 echo Failed: low timer too short (%$lo% ^< 45).&goto :eof
if %$hi% lss 30000 echo Failed: high timer too short (%$hi% ^< 45000).&goto :eof

set $temp=@tempfile
if not exist %$temp% echo Failed: temporary file doesn't exist.&goto :eof
>>%$temp% call :test
::copy %$temp% test1.out
fc >nul /b test1.out %$temp% && (
  echo Passed.
  :: Silent test removing colon comments (this would normally error).
) || (
  echo FAILED^^!
  fc /n test1.out %$temp%
)
del %$temp%
goto :eof

:test
set $A=1
set $a = 2
echo $A=%$A% $a=%$a%

set $embedded1=e1
set $embedded1x2y=e1x2y
set $escape$=escaped
echo Embedded: !$embedded$A! !$embedded$A/x$a/y! !$embedded1;rtrim[$A]! !$escape$$!

echo Left aligned:  !$A;-5!.
echo Right aligned: !$A;5!.
echo Zero padded:   !$A;05!.

set "$var= oKay "
echo Default:   !$no-var;'undefined'!.
echo Invalid:   !$var;invalid!.
echo Upper:     !$var;upper!.
echo Lower:     !$var;lower!.
echo Capital:   !$var;capital!.
echo Length:    !$var;length!.
echo Trim:      !$var;trim!.
echo LTrim:     !$var;ltrim!.
echo RTrim:     !$var;rtrim!.
echo Trim[ oy]: !$var;trim[ oy]!.
echo LTrim[ o]: !$var;ltrim[ o]!.
echo RTrim[ y]: !$var;rtrim[ y]!.
echo Trim,Cap:  !$var;trim;capital!.
set $hex=!$var;hexify!
echo Hexify:    !$hex!.
echo Unhexify:  !$hex;unhexify!.
set $var += !$var!
set $var+=var+
set $ var+= var+
set $var +=var +
echo Append:    !$var!.
echo "+=":      !$var+!.
echo "+= ":     !$ var+!.
echo " +=":     !$var +!.

echo Character TAB:!$TAB!.
echo Character ESC:!$ESC!.
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

call @write /n "No quotes or newline."
echo.
call @write /u LF line.!$CR!
echo "A long string "^+
     "spread across "^+
     "multiple lines."

for %%j in (:range*) do (
  for %%k in (:range*) do (
    echo %%j.%%k
    if %%k == 2 set @next=5
    if %%k == 6 set @next=
  )
  if %%j == 2 set @next=7
  if %%j == 8 set @next=
)
for %%j in (:range*3) do echo %%j
for %%j in (:range*4:6) do echo %%j
for %%j in (:range*1:5:2) do echo %%j
for %%j in (:range*-3) do echo %%j
for %%j in (:range*3:1) do echo %%j

set $j=1
for %%_ in (:*) do (
  call @write /n !$j!..
  if !$j! == 5 (
    echo Stop
    set @next=
  )
  set /a $j=!$j!+1
)
