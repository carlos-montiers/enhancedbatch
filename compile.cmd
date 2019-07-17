@Echo Off

gcc -Wl,-e,__dllstart,--enable-stdcall-fixup -nostartfiles -O3 -s -m32 -shared dll_enhancedbatch.c extensions.c offsets.c -o enhancedbatch_x86.dll -Wall

gcc -Wl,-e,_dllstart -nostartfiles -O3 -s -m64 -shared -lshell32 dll_enhancedbatch.c extensions.c offsets.c -o enhancedbatch_amd64.dll -Wall
