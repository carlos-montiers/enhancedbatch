@echo off
rundll32.exe %~dp0enhancedbatch_%processor_architecture%.dll,load
if not defined @enhancedbatch echo Enhanced Batch failed to load.&goto :eof
set $temp=@tempfile
>>%$temp% echo /?
>>%$temp% echo on
>>%$temp% echo
>>%$temp% echo off
>>%$temp% call :echo
set @extensions=0
>>%$temp% echo Extensions off, colon part of variable: %OS:_= %
>>%$temp% echo Tilde expansion: %$0;~nx%.
set @extensions=1
>>%$temp% echo Extensions on, replace underscore with space: %OS:_= %
set @delayedexpansion=1
>>%$temp% echo Delayed expansion on, expand variable: !OS!
set @delayedexpansion=0
>>%$temp% echo Delayed expansion off, literal text: !OS!
>>%$temp% echo Use character expansion!U+D!!U+A!for a new line.
for /f "usebackq eol=!U+22!" %%A in ('"skip"!U+A!token') do >>%$temp% echo %%A
for /f %%A in (";token") do >>%$temp% echo %%A
>>%$temp% echo This is line %@batchline%.
>>%$temp% call :args 1 2 3 4 5 6 7 8 9 10 11
>>%$temp% call :dumpparse
>>%$temp% call :dumptokens
set @unicode=1
>>%$temp% echo;Unicode
set @unicode=0
>>%$temp% echo.
::copy %$temp% test2.out
fc >nul test2.out %$temp% || (
  echo CMD %@cmdversion% failed.
  fc /n test2.out %$temp%
  del %$temp%
  goto :eof
)

set @batchfile=0
set /a $lnum=%@batchline% + 5
>%$temp% echo;~"%~nx0:%$lnum%: "
2>>%$temp% set /a
set @batchfile=1
set $temp2=@tempfile
2>>%$temp2% set /a
fc >nul %$temp% %$temp2% && goto :out
:: It may have failed because language resources are missing.
findstr >nul 0x2371 %$temp2% && goto :out
echo CMD %@cmdversion% failed.
echo;~"Expected: "
type %$temp%
echo.
echo;~"Actual:   "
type %$temp2%
echo.
goto :out

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

:echo
set $prompt=%PROMPT%
prompt $G
set @echo=1
set @echo=0
prompt %$prompt%
goto :eof

:dumpparse
set @dumpparse=1
set @dumpparse=0
goto :eof

:dumptokens
set @dumptokens=1
set @dumptokens=0
goto :eof

:out
del %$temp% %$temp2%
