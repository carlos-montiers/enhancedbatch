@echo off
setlocal enabledelayedexpansion

set ZLIBDIR=zlib-1.2.11

if "%~1" == "" (
  call :compile 32
  call :compile 64
) else if "%~1" == "32" (
  call :compile 32
) else if "%~1" == "64" (
  call :compile 64
) else if "%~1" == "clean" (
  del *.o %ZLIBDIR%\*.o
) else (
  echo Usage: %0 [32 ^| 64 ^| clean]
)
goto :eof

:compile
set flags=-m%1 -Os -s -Wall
set optimize=-flto -fomit-frame-pointer
set ZLIB=%ZLIBDIR%\adler32.c^
 %ZLIBDIR%\crc32.c^
 %ZLIBDIR%\inffast.c^
 %ZLIBDIR%\inflate.c^
 %ZLIBDIR%\inftrees.c^
 %ZLIBDIR%\zutil.c
:: Assume all or nothing.
if not exist %ZLIBDIR%\inflate_%1.o (
  for %%C in (%ZLIB%) do gcc %flags% %optimize% -c -o %%~pnC_%1.o %%C
)
if not exist spng_%1.o (
  gcc %flags% %optimize% -c -o spng_%1.o spng.c
)
gcc %flags% %optimize% example.c spng_%1.o !ZLIB:.c=_%1.o! -o example_%1.exe
