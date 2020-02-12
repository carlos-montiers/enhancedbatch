@echo off
rundll32.exe %~dp0enhancedbatch_%processor_architecture%.dll,load
if not defined @enhancedbatch echo Enhanced Batch failed to load.&goto :eof
set $temp := @tempfile
>>%$temp% call :test
::copy %$temp% test2.out
fc >nul test2.out %$temp% || (
  echo FAILED!
  fc /n test2.out %$temp%
  del %$temp%
  goto :eof
)

set @batchfile=0
set /a $lnum=%@batchline% + 5
>%$temp% call @write /n "%~nx0:%$lnum%: "
2>>%$temp% set /a
set @batchfile=1
set $temp2 := @tempfile
2>>%$temp2% set /a
fc >nul %$temp% %$temp2% && goto :passed
:: It may have failed because language resources are missing.
findstr >nul 0x2371 %$temp2% && echo Passed (1 skipped).&goto :out
echo FAILED!
call @write /n "Expected: "
type %$temp%
echo.
call @write /n "Actual:   "
type %$temp2%
echo.
goto :out
:passed
echo Passed.
:out
del %$temp% %$temp2%
goto :eof

:test
set $prompt=%PROMPT%
prompt $G
set @echo=on
echo Echo on status: %@echo%
set @echo=off
echo Echo off status: %@echo%
prompt %$prompt%
set @extensions=no
echo Extensions off, colon part of variable: %OS:_= %
echo Tilde expansion: %$0;~nx%.
set @extensions=yes
echo Extensions on, replace underscore with space: %OS:_= %
set @delayedexpansion=1
echo Delayed expansion on, expand variable: !OS!
set @delayedexpansion=0
echo Delayed expansion off, literal text: !OS!
echo Use character expansion!U+D!!U+A!for a new line.
for /f "eol=!U+0!" %%A in (";token") do echo %%A
for /f "line" %%A in (";line one!U+A!!U+22!line two") do echo %%A
echo This is line %@batchline%.
call echo ^^"^"
call :args 1 2 3 4 5 6 7 8 9 10 11
call :loop
call :labels
set @dumpparse=true
set @dumpparse=false
set @dumptokens=1
set @dumptokens=
set @unicode=1
call @write /n Unicode
set @unicode=0
echo.
goto :eof

:loop
set @delayedexpansion=1
set $j=1
for do (
  call @write /n !$j!..
  if !$j! == 5 (
    echo Stop
    set @next=
  )
  set /a $j+=1
)
for (!$j! neq 1) do (
  set /a $j-=1
  call @write /n !$j!..
)
echo Done
for (not !$j! == 6) do (
  call @write /n !$j!..
  set /a $j+=1
)
echo Done
set @delayedexpansion=0
for %%j do (
  call @write /n %%j..
  if %%j == 5 (
    echo Stop
    set @next=
  )
)
for %%j 10 do call @write /n %%j..
echo Done
for %%j -10 do call @write /n %%j..
echo Done
goto :eof

:args
echo Initial args: %$#%: %$-%
echo tenth: %$10%
echo first 3: %$-3%
echo four onwards: %$4-%
echo 5-7: %$5-7%
echo twelfth: [%$12%]
shift
echo After shift: %$#%: %$-%
echo tenth: %$10%
echo first 3: %$-3%
echo from 4: %$4-%
echo 5-7: %$5-7%
echo twelfth: [%$12%]
goto :eof

:labels
goto one
:two<
echo first two
goto ~one
=,; :one>
echo first one
goto ~two
:two
echo second two
goto :eof
:one
echo second one
goto ~two
