@echo off
pushd zlib-1.2.11
::mingw32-make.exe  -f win32/Makefile.gcc
set arq=-m32
set flags=%arq% -O3 -s -Wall -c -o
gcc %flags% adler32.o adler32.c
gcc %flags% compress.o compress.c
gcc %flags% crc32.o crc32.c
gcc %flags% deflate.o deflate.c
gcc %flags% gzclose.o gzclose.c
gcc %flags% gzlib.o gzlib.c
gcc %flags% gzread.o gzread.c
gcc %flags% gzwrite.o gzwrite.c
gcc %flags% infback.o infback.c
gcc %flags% inffast.o inffast.c
gcc %flags% inflate.o inflate.c
gcc %flags% inftrees.o inftrees.c
gcc %flags% trees.o trees.c
gcc %flags% uncompr.o uncompr.c
gcc %flags% zutil.o zutil.c
ar rcs libz.a adler32.o compress.o crc32.o deflate.o gzclose.o gzlib.o gzread.o gzwrite.o infback.o inffast.o inflate.o inftrees.o trees.o uncompr.o zutil.o
popd
if "%arq%"=="-m32" gcc %arq% -O3 -s example.c spng.c zlib-1.2.11\libz.a -o example.exe
if "%arq%"=="-m64" gcc %arq% -O3 -s example.c spng.c zlib-1.2.11\libz.a -o example.exe
