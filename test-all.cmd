@echo off

echo Testing amd64:
%WinDir%\System32\cmd.exe /c test1.cmd
%WinDir%\System32\cmd.exe /c test2.cmd

echo Testing x86:
%WinDir%\SysWOW64\cmd.exe /c test1.cmd
%WinDir%\SysWOW64\cmd.exe /c test2.cmd

pause
