@Echo Off
:: Compiler used: TDM GCC MinGW-w64 edition 5.1.0-2

SetLocal EnableDelayedExpansion
set FLAGS=-nostartfiles -O3 -s -shared -Wall
set UPNGFLAGS=-Os -s -Wall
set LIBS=-lversion -lole32
set SRC=dll_enhancedbatch.c extensions.c say.c img4bit.c messages.c offsets.c patch.c

windres -F pe-i386 -U _WIN64 enhancedbatch.rc enhancedbatch_info_x86.o
gcc -m32 %UPNGFLAGS% -c -o upng_x86.o upng.c
gcc -m32 -Wl,-e,__dllstart,--enable-stdcall-fixup %FLAGS% upng_x86.o %SRC% enhancedbatch_info_x86.o %LIBS% -o enhancedbatch_x86.dll

windres -F pe-x86-64 enhancedbatch.rc enhancedbatch_info_amd64.o
gcc -m64 %UPNGFLAGS% -c -o upng_amd64.o upng.c
gcc -m64 -Wl,-e,_dllstart %FLAGS% upng_amd64.o %SRC% enhancedbatch_info_amd64.o %LIBS% -o enhancedbatch_amd64.dll

del *.o
