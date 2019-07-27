@echo off
setlocal enableextensions enabledelayedexpansion

rundll32.exe %~dp0enhancedbatch_%processor_architecture%.dll,load
if not defined @enhancedbatch echo Failed: Enhanced Batch didn't load.&goto :eof

set $var=1
2>nul set $var && (echo Failed: $var is in the environment.&goto :eof)
if not defined $var echo Failed: $var is not defined.&goto :eof

set $temp=@tempfile
if not exist %$temp% echo Failed: temporary file doesn't exist.&goto :eof

set @timer=start
set @timerhi=start
set @sleep=45
set @timerhi=
set @timer=
set $hi=@timerhi
set $lo=@timer
if %$lo% lss 30 echo Failed: low timer too short (%$lo% ^< 45).&goto :eof
if %$hi% lss 30000 echo Failed: high timer too short (%$hi% ^< 45000).&goto :eof

set $A=1
set $a=2
>>%$temp% echo $A=%$A% $a=%$a%

>>%$temp% echo Left aligned:  !$A;-5!.
>>%$temp% echo Right aligned: !$A;5!.
>>%$temp% echo Zero padded:   !$A;05!.

set "$var= oKay "
>>%$temp% echo Default:   !$no-var;'undefined'!.
>>%$temp% echo Invalid:   !$var;invalid!.
>>%$temp% echo Upper:     !$var;upper!.
>>%$temp% echo Lower:     !$var;lower!.
>>%$temp% echo Capital:   !$var;capital!.
>>%$temp% echo Length:    !$var;length!.
>>%$temp% echo Trim:      !$var;trim!.
>>%$temp% echo LTrim:     !$var;ltrim!.
>>%$temp% echo RTrim:     !$var;rtrim!.
>>%$temp% echo Trim[ oy]: !$var;trim[ oy]!.
>>%$temp% echo LTrim[ o]: !$var;ltrim[ o]!.
>>%$temp% echo RTrim[ y]: !$var;rtrim[ y]!.
>>%$temp% echo Trim,Cap:  !$var;trim,capital!.

>>%$temp% echo Character TAB:!$TAB!.
>>%$temp% echo Character ESC:!$ESC!.
>>%$temp% echo Character BS:!$BS!.
>>%$temp% echo Character CR+LF:!$CR!!$LF!.
>>%$temp% echo Character CRLF:!$CRLF!.
>>%$temp% echo Character EXCL:!$EXCL!.
>>%$temp% echo Character QUOT:!$QUOT!.
>>%$temp% echo Character AMP:!$AMP!.
>>%$temp% echo Character BAR:!$BAR!.
>>%$temp% echo Character GT:!$GT!.
>>%$temp% echo Character LT:!$LT!.
>>%$temp% echo Character OPAR:!$OPAR!.
>>%$temp% echo Character CPAR:!$CPAR!.
>>%$temp% echo Character OBRC:!$OBRC!.
>>%$temp% echo Character CBRC:!$CBRC!.
>>%$temp% echo Character STAR:!$STAR!.
>>%$temp% echo Character QUES:!$QUES!.
>>%$temp% echo Character DOLLAR:!$DOLLAR!.
>>%$temp% echo Character SEMI:!$SEMI!.
>>%$temp% echo Character COMMA:!$COMMA!.
>>%$temp% echo Character EQ:!$EQ!.

>>%$temp% echo;~"No quotes or newline."
>>%$temp% echo.
>>%$temp% echo,LF line.!$CR!
>>%$temp% echo "A long string "^
               "spread across "^
               "multiple lines."

for %%j in (:range*) do (
  for %%k in (:range*) do (
    >>%$temp% echo %%j.%%k
    if %%k == 2 set @next=5
    if %%k == 6 set @next=
  )
  if %%j == 2 set @next=7
  if %%j == 8 set @next=
)
for %%j in (:range*3) do >>%$temp% echo %%j
for %%j in (:range*4:6) do >>%$temp% echo %%j
for %%j in (:range*1:5:2) do >>%$temp% echo %%j
for %%j in (:range*-3) do >>%$temp% echo %%j
for %%j in (:range*3:1) do >>%$temp% echo %%j

set $j=1
for %%_ in (:*) do (
  >>%$temp% echo;!$j!..
  if !$j! == 5 (
    >>%$temp% echo Stop
    set @next=
  )
  set /a $j=!$j!+1
)

::copy %$temp% test1.out
fc >nul /b test1.out %$temp% || (
  echo Failed.
  fc /n test1.out %$temp%
)

del %$temp%
