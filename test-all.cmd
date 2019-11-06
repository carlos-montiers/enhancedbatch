@echo off

call :test %PROCESSOR_ARCHITECTURE% System32

if defined PROCESSOR_ARCHITEW6432 (
	call :test %PROCESSOR_ARCHITEW6432% SysNative
) else if not %PROCESSOR_ARCHITECTURE% == x86 (
	call :test x86 SysWOW64
)

if "%~1" == "" pause
goto :eof

:test
echo Testing %1:
call :runtest %2 test1.cmd 
call :runtest %2 test2.cmd 
goto :eof

:runtest
<nul set /p "=* %~n2: "
%WinDir%\%1\cmd.exe /c %2
