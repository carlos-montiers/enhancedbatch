@Echo Off
:: Compiler used: TDM GCC MinGW-w64 edition 5.1.0-2

windres -F pe-i386 -U _WIN64 enhancedbatch.rc enhancedbatch_info_x86.o
gcc -Wl,-e,__dllstart,--enable-stdcall-fixup -nostartfiles -O3 -s -m32 -shared dll_enhancedbatch.c extensions.c offsets.c patch.c enhancedbatch_info_x86.o -lversion -o enhancedbatch_x86.dll -Wall

windres -F pe-x86-64 enhancedbatch.rc enhancedbatch_info_amd64.o
gcc -Wl,-e,_dllstart -nostartfiles -O3 -s -m64 -shared -lshell32 dll_enhancedbatch.c extensions.c offsets.c patch.c enhancedbatch_info_amd64.o -lversion -o enhancedbatch_amd64.dll -Wall

del *.o
