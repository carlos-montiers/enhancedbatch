@echo off
pushd zlib-1.2.11
::mingw32-make.exe  -f win32/Makefile.gcc
set arq=-m32
set flags=%arq% -Os -s -Wall
set optimize=-flto -fomit-frame-pointer
gcc %flags% -c -o adler32.o adler32.c
gcc %flags% -c -o compress.o compress.c
gcc %flags% -c -o crc32.o crc32.c
gcc %flags% -c -o deflate.o deflate.c
gcc %flags% -c -o gzclose.o gzclose.c
gcc %flags% -c -o gzlib.o gzlib.c
gcc %flags% -c -o gzread.o gzread.c
gcc %flags% -c -o gzwrite.o gzwrite.c
gcc %flags% -c -o infback.o infback.c
gcc %flags% -c -o inffast.o inffast.c
gcc %flags% -c -o inflate.o inflate.c
gcc %flags% -c -o inftrees.o inftrees.c
gcc %flags% -c -o trees.o trees.c
gcc %flags% -c -o uncompr.o uncompr.c
gcc %flags% -c -o zutil.o zutil.c
ar rcs libz.a adler32.o compress.o crc32.o deflate.o gzclose.o gzlib.o gzread.o gzwrite.o infback.o inffast.o inflate.o inftrees.o trees.o uncompr.o zutil.o
popd
gcc %flags% %optimize% example.c spng.c zlib-1.2.11\libz.a -o example.exe
