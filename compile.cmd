@Echo Off
:: Compiler used: TDM GCC MinGW-w64 edition 5.1.0-2

SetLocal EnableDelayedExpansion
set FLAGS=-nostartfiles -O3 -s -shared -Wall
set FLAGS=-nostartfiles -O3 -s -shared -Wall
set LIBS=-lversion -lole32
set SRC=dll_enhancedbatch.c extensions.c say.c img4bit.c messages.c offsets.c patch.c
set ZLIBDIR=zlib-1.2.11
set TINYFLAGS=-Os -s -flto -fomit-frame-pointer -Wall

set IMG4BIT=^
 %ZLIBDIR%\adler32.c^
 %ZLIBDIR%\crc32.c^
 %ZLIBDIR%\inffast.c^
 %ZLIBDIR%\inflate.c^
 %ZLIBDIR%\inftrees.c^
 %ZLIBDIR%\zutil.c^
 spng.c

for %%Z in (%IMG4BIT%) do for %%M in (32 64
) do gcc -m%%M %TINYFLAGS% -c -o %%~pnZ_%%M.o %%Z

windres -F pe-i386 -U _WIN64 enhancedbatch.rc enhancedbatch_info_x86.o
gcc -m32 -Wl,-e,__dllstart,--enable-stdcall-fixup %FLAGS% %SRC% enhancedbatch_info_x86.o !IMG4BIT:.c=_32.o! %LIBS% -o enhancedbatch_x86.dll

windres -F pe-x86-64 enhancedbatch.rc enhancedbatch_info_amd64.o
gcc -m64 -Wl,-e,_dllstart %FLAGS% %SRC% enhancedbatch_info_amd64.o !IMG4BIT:.c=_64.o! %LIBS% -o enhancedbatch_amd64.dll

del *.o
del %ZLIBDIR%\*.o