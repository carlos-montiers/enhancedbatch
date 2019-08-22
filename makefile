CC=gcc
CFLAGS=-O3 -Wall
LDFLAGS=-nostartfiles -s -shared
RC ?= windres

SRC := $(wildcard *.c)
X64OBJ := $(patsubst %.c,%_amd64.o,$(SRC)) enhancedbatch_info_amd64.o
X86OBJ := $(patsubst %.c,%_x86.o,$(SRC)) enhancedbatch_info_x86.o

all: enhancedbatch_amd64.dll enhancedbatch_x86.dll

%_amd64.o: %.c
	$(CC) -m64 -c $(CFLAGS) $< -o $@

%_x86.o: %.c
	$(CC) -m32 -c $(CFLAGS) $< -o $@

%_info_amd64.o: %.rc
	$(RC) -F pe-x86-64 -U _WIN64 $< $@

%_info_x86.o: %.rc
	$(RC) -F pe-i386 $< -o $@

enhancedbatch_amd64.dll: $(X64OBJ)
	$(CC) -m64 $(LDFLAGS) -Wl,-e,_dllstart $^ -o $@
	
enhancedbatch_x86.dll: $(X86OBJ)
	$(CC) -m32 $(LDFLAGS) -Wl,-e,__dllstart,--enable-stdcall-fixup $^ -o $@

clean:
	-del *.o
