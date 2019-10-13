@echo off
rundll32.exe %~dp0enhancedbatch_%processor_architecture%.dll,load
if not defined @enhancedbatch echo Enhanced Batch failed to load.&goto :eof
set $temp=@tempfile
set @echooptions=off
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
>%$temp% echo;~"%~nx0:%$lnum%: "
2>>%$temp% set /a
set @batchfile=1
set $temp2=@tempfile
2>>%$temp2% set /a
fc >nul %$temp% %$temp2% && goto :passed
:: It may have failed because language resources are missing.
findstr >nul 0x2371 %$temp2% && goto :passed
echo FAILED!
echo;~"Expected: "
type %$temp%
echo
echo;~"Actual:   "
type %$temp2%
echo
goto :out
:passed
echo Passed.
:out
del %$temp% %$temp2%
goto :eof

:test
echo /?
echo on
echo
echo off
set $prompt=%PROMPT%
prompt $G
set @echo=on
set @echo=off
set @echooptions=on
echo Echo options: %@echooptions%
echo on
echo off
set @echooptions=off
echo Echo options: %@echooptions%
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
echo This is line %@batchline%.
call :args 1 2 3 4 5 6 7 8 9 10 11
set @dumpparse=true
set @dumpparse=false
set @dumptokens=1
set @dumptokens=
set @unicode=1
echo;Unicode
set @unicode=0
echo
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
