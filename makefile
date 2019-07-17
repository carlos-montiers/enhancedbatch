CC=gcc
CFLAGS=-O3 -Wall
LDFLAGS=-nostartfiles -s -shared

all: enhancedbatch_amd64.dll enhancedbatch_x86.dll

%_amd64.o: %.c
	$(CC) -m64 -c $(CFLAGS) $< -o $@

%_x86.o: %.c
	$(CC) -m32 -c $(CFLAGS) $< -o $@

enhancedbatch_amd64.dll: $(patsubst %.c,%_amd64.o,$(wildcard *.c))
	$(CC) -m64 $(LDFLAGS) -Wl,-e,_dllstart $^ -o $@
	
enhancedbatch_x86.dll: $(patsubst %.c,%_x86.o,$(wildcard *.c))
	$(CC) -m32 $(LDFLAGS) -Wl,-e,__dllstart,--enable-stdcall-fixup $^ -o $@

clean:
	-del *.o
