@Echo Off
:: Compiler used: TDM GCC MinGW-w64 edition 5.1.0-2

SetLocal
set FLAGS=-nostartfiles -O3 -s -shared -Wall
set LIBS=-lversion -lole32
set SRC=dll_enhancedbatch.c extensions.c say.c help.c offsets.c patch.c

windres -F pe-i386 -U _WIN64 enhancedbatch.rc enhancedbatch_info_x86.o
gcc -Wl,-e,__dllstart,--enable-stdcall-fixup -m32 %FLAGS% %SRC% enhancedbatch_info_x86.o %LIBS% -o enhancedbatch_x86.dll

windres -F pe-x86-64 enhancedbatch.rc enhancedbatch_info_amd64.o
gcc -Wl,-e,_dllstart -m64 %FLAGS% %SRC% enhancedbatch_info_amd64.o %LIBS% -o enhancedbatch_amd64.dll

del *.o
