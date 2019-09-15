/*
 Copyright (c) 2019 Carlos Montiers Aguilera
 Copyright (c) 2019 Jason Hood

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.

 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.

 3. This notice may not be removed or altered from any source distribution.

 Carlos Montiers Aguilera   cmontiers@gmail.com
 Jason Hood                 jadoxa@yahoo.com.au

*/

#include "dll_enhancedbatch.h"

#include "khash.h"
#ifdef _WIN64
KHASH_MAP_INIT_INT64(ptrdw, DWORD)
#else
KHASH_MAP_INIT_INT(ptrdw, DWORD)
#endif

khash_t(ptrdw) *sfwork_map;


struct sCmdEntry {
	LPCWSTR   name;
	fnCmdFunc func;
	DWORD	  flags;
	DWORD	  helpid;
	DWORD	  exthelpid;
	DWORD	  helpextra;
};

#ifdef _WIN64
LPBYTE redirect;
BYTE oldCtrlCAborts[7];
BYTE SFWork_saved;
#else
BYTE oldCtrlCAborts[5];
DWORD SFWork_esp;
BYTE SFWork_stdcall;
int SFWork_saved, SFWork_passed, SFWork_first;
DWORD ForFbegin_org;
#endif

int iPutMsg;
BYTE oldLexText[5], oldEchoOnOff[5], oldSFWorkmkstr[6], oldSFWorkresize[6];
BYTE oldForFbegin[6], oldForFend[6];
DWORD_PTR ForFend_org;
int ForF_stack[FORF_STACKSIZE], ForF_stacktop;

DWORD_PTR SFWork_mkstr_org;
DWORD_PTR FreeStack;
BYTE SFWork_mkstr_reg;


int SFWork_hook(LPWSTR saved, LPWSTR *passed, int first)
{
	khint_t k;
	int absent;

	if (saved == NULL) {
		k = kh_put(ptrdw, sfwork_map, (DWORD_PTR) passed, &absent);
		kh_val(sfwork_map, k) = 0;
		return 0;
	}
	k = kh_get(ptrdw, sfwork_map, (DWORD_PTR) passed);
	if (kh_val(sfwork_map, k) == 0 || first) {
		kh_val(sfwork_map, k) = *pDCount;
		return -1;
	}
	return kh_val(sfwork_map, k);
}


void ForFbegin_hook(void) {
	if (++ForF_stacktop == lenof(ForF_stack)) {
		ForF_stacktop = 0;
	}
	ForF_stack[ForF_stacktop] = 1;
}

void __attribute((fastcall)) ForFend_hook(BYTE end) {
	if (end || *pGotoFlag) {
		if (--ForF_stacktop == -1) {
			ForF_stacktop = lenof(ForF_stack) - 1;
		}
	} else {
		++ForF_stack[ForF_stacktop];
	}
}


#ifndef _WIN64
void MyLexTextESI(void)
{
	asm("call _MyLexText");
	asm("movl %eax,%esi");
}

int __stdcall stdPutStdErrMsg(UINT a, int b, UINT c, va_list *d)
{
	return MyPutStdErrMsg(a, b, c, d);
}

int __fastcall fastPutStdErrMsg(UINT a, int b, UINT c, va_list *d)
{
	return MyPutStdErrMsg(a, b, c, d);
}

int __fastcall fastPutStdErrMsg62(int b, va_list *d, UINT a, UINT c)
{
	return MyPutStdErrMsg(a, b, c, d);
}

void SFWork_mkstr(void)
{
	asm(
		"push %ecx\n"               // preserve possible register argument
		"mov _SFWork_first,%eax\n"
		"pushl (%eax,%ebp)\n"
		"mov _SFWork_passed,%eax\n"
		"pushl (%eax,%ebp)\n"
		"mov _SFWork_saved,%eax\n"
		"test %eax,%eax\n"
		"mov (%eax,%ebp),%eax\n"
		"js 1f\n"
		"mov -28(%ebp),%ecx\n"      // debug version doesn't store it
		"lea 4(%ecx,%eax,4),%eax\n"
		"1:\n"
		"pushl (%eax)\n"
		"call _SFWork_hook\n"
		"add $12,%esp\n"
		"pop %ecx\n"
		"test %eax,%eax\n"
		"js 1f\n"
		"jz org\n"
		"mov %eax,%ecx\n"           // 6.2.9200.16384 passes in eax, 6.3+ in ecx
		"mov %esp,_SFWork_esp\n"    // 6.2.8102.0 is stdcall, but mkstr is fast
		"push %eax\n"
		"call *_FreeStack\n"        // free everything allocated between loops
		"mov _SFWork_esp,%esp\n"
		"1:\n"
		"mov _SFWork_passed,%eax\n"
		"mov (%eax,%ebp),%eax\n"
		"mov (%eax),%eax\n"         // reuse the original mkstr
		"mov -8(%eax),%ecx\n"       // no terminator is added, reset to 0
		"sub $8,%ecx\n"             // length that was requested
		"push %eax\n"
		"push %edi\n"
		"mov %eax,%edi\n"
		"xor %eax,%eax\n"
		"rep stosb\n"
		"pop %edi\n"
		"pop %eax\n"
		"mov _SFWork_mkstr_reg,%cl\n"
		"test %cl,%cl\n"
		"jnz 1f\n"                  // inline
		"cmp %cl,_SFWork_stdcall\n"
		"jz exit\n"                 // fastcall, just return
		"ret $4\n"                  // stdcall, tidy up
		"1:\n"
		"addl $0x35,(%esp)\n"       // skip over all the inline code
		"cmp $0xF6,%cl\n"           // select the register it uses
		"cmove %eax,%esi\n"
		"cmovb %eax,%ebx\n"         // 0xDB
		"cmova %eax,%edi\n"         // 0xFF
		"ret $8\n"
		"org:\n"
		"jmp *_SFWork_mkstr_org\n"
		"exit:"
	);
}

void ForFend(void)
{
	asm(
		"pushf\n"
		"setnc %cl\n"
		"call _ForFend_hook\n"
		"popf\n"
		"jnc 1f\n"
		"mov _ForFend_org,%eax\n"
		"mov %eax,(%esp)\n"
		"1:"
	);
}

void ForFend_opp(void)
{
	asm(
		"pushf\n"
		"setnc %cl\n"
		"call _ForFend_hook\n"
		"popf\n"
		"jc 1f\n"
		"mov _ForFend_org,%eax\n"
		"mov %eax,(%esp)\n"
		"1:"
	);
}

void ForFbegin_jmp(void)
{
	asm(
		"call _ForFbegin_hook\n"
		"jmp *_ForFbegin_org\n"
	);
}

#else

void SFWork_mkstr(void)
{
	asm(
		"cmpb $0,cmdDebug(%rip)\n"      // debug version (only one, so far)
		"cmovnz 0xB0(%rsp),%r9d\n"      // retrieve the values
		"cmovnz %r12,%rdx\n"
		"mov %r9d,%r8d\n"
		"push %rcx\n"
		"push %rdx\n"
		"movzxb SFWork_saved(%rip),%eax\n"
		"test $0x80,%al\n"
		"jns 2f\n"
		"cmp $0xA0,%al\n"
		"je 1f\n"
		"mov 0x18(%rsp,%rax),%rax\n"    // 0x90, 0x80
		"mov 8(%rax,%rdi,8),%rcx\n"
		"jmp 3f\n"
		"org:\n"
		"jmp *SFWork_mkstr_org(%rip)\n"
		"1:\n"
		"mov 8(%rsp,%rax),%rax\n"       // 0xA0
		"mov 8(%rax,%r13,8),%rcx\n"
		"jmp 3f\n"
		"2:\n"
		"cmp $0x34,%al\n"
		"jne 1f\n"
		"mov (%r14),%rcx\n"             // 0x34
		"jmp 3f\n"
		"1:\n"
		"cmova %r15,%rax\n"             // 0x3C
		"cmovb %r13,%rax\n"             // 0x2C
		"mov 8(%rax),%rcx\n"
		"3:\n"
		"sub $32,%rsp\n"                // shadow space
		"call SFWork_hook\n"
		"add $32,%rsp\n"
		"pop %rdx\n"
		"pop %rcx\n"
		"test %eax,%eax\n"
		"js 1f\n"
		"jz org\n"
		"push %rdx\n"
		"sub $32,%rsp\n"                // shadow space
		"mov %eax,%ecx\n"
		"call *FreeStack(%rip)\n"       // free everything allocated between loops
		"add $32,%rsp\n"
		"pop %rdx\n"
		"1:\n"
		"mov (%rdx),%rax\n"             // reuse the original mkstr
		"mov -16(%rax),%rcx\n"          // no terminator is added, reset to 0
		"sub $16,%rcx\n"                // length that was requested
		"push %rax\n"
		"push %rdi\n"
		"mov %rax,%rdi\n"
		"xor %eax,%eax\n"
		"rep stosb\n"
		"pop %rdi\n"
		"pop %rax\n"
		"mov SFWork_mkstr_reg(%rip),%cl\n"
		"test %cl,%cl\n"
		"jz exit\n"
		"addq $0x49,(%rsp)\n"           // skip over all the inline code
		"cmp $0xFF,%cl\n"               // select the register it uses
		"cmove %rax,%rdi\n"
		"cmovne %rax,%rbx\n"
		"exit:");
}

void ForFend(void)
{
	asm(
		"pushf\n"
		"push %rdx\n"               // one of these registers may contain LF
		//"push %r8\n"              // these registers aren't used by the hook
		//"push %r9\n"
		//"push %r10\n"
		"setnc %cl\n"
		//"sub $32,%rsp\n"          // shadow space not needed
		"call ForFend_hook\n"
		//"add $32,%rsp\n"
		//"pop %r10\n"
		//"pop %r9\n"
		//"pop %r8\n"
		"pop %rdx\n"
		"popf\n"
		"jnc 1f\n"
		"mov ForFend_org(%rip),%rax\n"
		"mov %rax,(%rsp)\n"
		"1:"
	);
}

void ForFend_opp(void)
{
	asm(
		"pushf\n"
		"push %rdx\n"
		//"push %r8\n"
		//"push %r9\n"
		//"push %r10\n"
		"setnc %cl\n"
		//"sub $32,%rsp\n"
		"call ForFend_hook\n"
		//"add $32,%rsp\n"
		//"pop %r10\n"
		//"pop %r9\n"
		//"pop %r8\n"
		"pop %rdx\n"
		"popf\n"
		"jc 1f\n"
		"mov ForFend_org(%rip),%rax\n"
		"mov %rax,(%rsp)\n"
		"1:"
	);
}

void ForFbegin(void)
{
	asm(
		"push %rdx\n"
		//"push %r8\n"
		//"push %r9\n"
		//"push %r10\n"
		//"sub $32,%rsp\n"
		"call ForFbegin_hook\n"
		//"add $32,%rsp\n"
		//"pop %r10\n"
		//"pop %r9\n"
		//"pop %r8\n"
		"pop %rdx\n"
	);
}

#endif


void hookCmd(void)
{
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNTHeader;
	LPBYTE cmd;
	LPDWORD data, end;
	struct sCmdEntry *cmdentry;
	fnCmdFunc pMyEcho, pMyCall;
	const struct sCMD *ver;

	cmd = (LPBYTE) GetModuleHandle(NULL);
	data = (LPDWORD) cmd;
	pDosHeader = (PIMAGE_DOS_HEADER) data;
	pNTHeader = MakeVA(PIMAGE_NT_HEADERS, pDosHeader->e_lfanew);
	end = MakeVA(LPDWORD, pNTHeader->OptionalHeader.SizeOfImage);
	cmd_end = end;
	pMyEcho = MyEcho;
	pMyCall = MyCall;

	sfwork_map = kh_init(ptrdw);

	// Search the image for the ECHO & SET help identifiers (to locate eEcho),
	// L"%s\r\n" (for its output) and the binary file version.
	while (data < end) {
		if (!eEcho) {
			cmdentry = (struct sCmdEntry *) data;
			if (cmdentry->helpid == 0x2392 &&		// ECHO
				(cmdentry+1)->helpid == 0x2389) {	// SET
				peEcho = &cmdentry->func;
				eEcho = cmdentry->func;
				WriteMemory(peEcho, &pMyEcho, sizeof(pMyEcho));
				peCall = &cmdentry[14].func;
				eCall = cmdentry[14].func;
				WriteMemory(peCall, &pMyCall, sizeof(pMyCall));
				// Use eCall to get the address of LastRetCode.
				LPBYTE p = (LPBYTE) eCall;
#ifdef _WIN64
				p += cmdDebug ? 25 : 15;
				pLastRetCode = (int *)(p + 4 + *(int *)p);
#else
				while (*p++ != 0xA3) ;
				pLastRetCode = *(int **)p;
#endif
			}
		}
		if (!Fmt17) {
			if (data[0] == 0x00730025 &&	// L"%s"
				data[1] == 0x000a000d) {	// L"\r\n"
				Fmt17 = (LPWSTR) data;
			}
		}
		if (data[0] == VS_FFI_SIGNATURE) {
			cmdFileVersionMS = data[2];
			cmdFileVersionLS = data[3];
			cmdDebug = data[7] & VS_FF_DEBUG;
			break;
		}
		++data;
	}

	for (ver = cmd_versions; ver->offsets; ++ver) {
		if (ver->verMS == cmdFileVersionMS &&
			ver->verLS == cmdFileVersionLS &&
			*(LPCWSTR)(cmd + *ver->offsets) == L';') {

			int i;
			for (i = 0; i < OFFSETS; ++i) {
				cmd_addrs[i] = cmd + ver->offsets[i];
			}

			memcpy(oldEchoOnOff, pEchoOnOff, 5);
			memcpy(oldCtrlCAborts, pCtrlCAborts, sizeof(oldCtrlCAborts));

			// Patch FOR to fix a substitute bug - each one has its own memory.
			// Allocate once and reuse it.	It also frees memory allocated
			// during each loop.
			memcpy(oldSFWorkmkstr, pSFWorkmkstr, 6);
			memcpy(oldSFWorkresize, pSFWorkresize, 6);
			FreeStack = (DWORD_PTR) pFreeStack;
			SFWork_saved = *pSFWorksaved;
#ifdef _WIN64
			if (*pSFWorkmkstr == 0xFF) {
				SFWork_mkstr_reg = pSFWorkmkstr[0x47];
				SFWork_mkstr_org = *(DWORD_PTR*)((DWORD_PTR)pSFWorkmkstr+6 + *(int *)(pSFWorkmkstr+2));
				WriteMemory(pSFWorkmkstr, (LPVOID) 0xE8, 1);	// call
				WriteMemory(pSFWorkmkstr+5, (LPVOID) 0x90, 1);	// nop
			} else {
				SFWork_mkstr_org = (DWORD_PTR)pSFWorkmkstr+5 + *(int *)(pSFWorkmkstr+1);
			}
			if (*pSFWorkresize == 0xFF) {
				WriteMemory(pSFWorkresize, "\x4C\x89\xC0"   // mov rax,r8
										   "\x0F\x1F"       // nop
										   , 6);			// include NUL
			} else {
				WriteMemory(pSFWorkresize, "\x48\x89\xC8"   // mov rax,rcx
										   "\x66\x90"       // nop
										   , 5);
			}
#else
			SFWork_passed = *pSFWorkpassed;
			SFWork_first = (cmdFileVersionMS >= 0x60002) ? 12 : 20;
			if (*pSFWorkmkstr == 0xE8) {
				SFWork_mkstr_org = (DWORD)pSFWorkmkstr+5 + *(int *)(pSFWorkmkstr+1);
				if (pSFWorkmkstr[-5] == 0x68) {
					SFWork_stdcall = 1;
				}
			} else {
				SFWork_mkstr_org = **(LPDWORD*)(pSFWorkmkstr+2);
				SFWork_mkstr_reg = pSFWorkmkstr[0x33];
				WriteMemory(pSFWorkmkstr, (LPVOID) 0xE8, 1);	// call
				WriteMemory(pSFWorkmkstr+5, (LPVOID) 0x90, 1);	// nop
			}
			i = (DWORD)SFWork_mkstr - ((DWORD)pSFWorkmkstr+5);
			WriteMemory(pSFWorkmkstr+1, &i, 4);
			if (*pSFWorkresize == 0xE8) {
				if (SFWork_stdcall) {
					WriteMemory(pSFWorkresize, "\x58"           // pop eax
											   "\x59"           // pop ecx
											   "\x66\x66\x90"   // nop
											   , 5);
				} else {
					WriteMemory(pSFWorkresize, "\x89\xC8"       // mov eax,ecx
											   "\x66\x66\x90"   // nop
											   , 5);
				}
			} else {
				WriteMemory(pSFWorkresize, "\x59"       // pop ecx
										   "\x59"       // pop ecx
										   "\x58"       // pop eax
										   "\x59"       // pop ecx
										   "\x66\x90"   // nop
										   , 6);
			}
#endif

			// Patch FOR to fix a bug with wildcard expansion - each name
			// accumulates, resizing bigger and bigger (this patch is not
			// undone on unload).
			// I've made the initial size big enough, no need to resize.
			WriteMemory(pForResize, (LPVOID) 0xEB, 1);
#ifdef _WIN64
			if (cmdFileVersionMS == 0x50002) {
				// 5.2.*.*
				WriteMemory(pForResize, "\x90\xE9", 2);
				WriteMemory(pForMkstr, "\x90\x90\x90"               // nop
									   "\x44\x8D\xA1\x00\x01\x00"   // lea r12d,rcx+256
									   , 10);
			} else if (cmdFileVersionMS == 0x60000 ||
					   cmdFileVersionMS == 0x60001) {
				// 6.0.*.*
				// 6.1.*.*
				WriteMemory(pForMkstr, "\x90\x90\x90"               // nop
									   "\x44\x8D\xA9\x00\x01\x00"   // lea r13d,rcx+256
									   , 10);
			} else if (cmdFileVersionMS == 0x60002) {
				if (cmdFileVersionLS == 0x1FA60000) {
					// 6.2.8102.0
					WriteMemory(pForMkstr, "\x90"                       // nop
										   "\x49\xFF\xC7"               // inc r15
										   "\x66\x42\x83\x3C\x79\x00"	// jmp word[rcx+r15*2],0
										   "\x75\xF5"                   // jnz inc
										   "\x41\x81\xC7\x00\x01\x00"   // add r15d,100
										   , 19);
				} else {
					// 6.2.9200.16384
					WriteMemory(pForMkstr, "\x90"                       // nop
										   "\x48\xFF\xC5"               // inc rbp
										   "\x66\x83\x3C\x69\x00"       // cmp word[rcx+rbp*2],0
										   "\x75\xF6"                   // jnz inc
										   "\x81\xC5\x00\x01\x00"       // add ebp,100
										   , 17);
				}
			} else if (cmdFileVersionMS == 0x60003 &&
					   cmdFileVersionLS == 0x24D70000 &&
					   cmdDebug) {
				// 6.3.9431.0u
				WriteMemory(pForMkstr, "\x90"                   // nop
									   "\x48\xFF\xC2"           // inc rdx
									   "\x66\x44\x39\x24\x51"   // cmp [rcx+rdx*2],r12w
									   "\x75\xF6"               // jnz inc
									   "\xFE\xC6"               // inc dh
									   "\x89\xD7", 15);         // mov edi,edx
			} else if (cmdFileVersionMS == 0xA0000 &&
					   HIWORD(cmdFileVersionLS) >= 17763) {
				// 10.0.17763.1
				// 10.0.18362.1
				WriteMemory(pForMkstr, "\x48\xFF\xC2"           // inc rdx
									   "\x90"                   // nop
									   "\x66\x44\x39\x34\x51"   // cmp [rcx+rdx*2],r14w
									   "\x75\xF5"               // jnz inc
									   "\xFE\xC6"               // inc dh
									   "\x89\xD7", 15);         // mov edi,edx
			} else {
				// 6.3.*.*
				// 10.0.*.*
				WriteMemory(pForMkstr, "\x90"                   // nop
									   "\x48\xFF\xC2"           // inc rdx
									   "\x66\x39\x2C\x51"       // cmp [rcx+rdx*2],bp
									   "\x75\xF7"               // jnz inc
									   "\xFE\xC6"               // inc dh
									   "\x89\xD7", 14);         // mov edi,edx
			}
#else
			if (cmdFileVersionMS == 0x50000) {
				// 5.0.*.*
				WriteMemory(pForMkstr, "\xEB\x00\xFE\xC4", 4);  // jmp $+2; inc ah
			} else if (HIWORD(cmdFileVersionMS) == 5) {
				if (LOWORD(cmdFileVersionLS) == 0) {
					// 5.1.2600.0
					// 5.2.3790.0
					WriteMemory(pForMkstr, pForMkstr+2, 8);
					WriteMemory(pForMkstr+8, "\xFE\xC4", 2);    // inc ah
				} else {
					// 5.1.*.*
					// 5.2.*.*
					WriteMemory(pForMkstr, pForMkstr+6, 8);
					WriteMemory(pForMkstr+8, "\x81\xC0\x00\x01\x00", 6);    // add eax,256
				}
			} else if (cmdFileVersionMS == 0x60000) {
				// 6.0.*.*
				WriteMemory(pForMkstr, "\xFE\xC4"   // inc ah
									   "\x93", 3);	// xchg ebx,eax
			} else if (cmdFileVersionMS == 0x60001) {
				// 6.1.*.*
				WriteMemory(pForMkstr, "\xFE\xC4"   // inc ah
									   "\x97", 3);	// xchg edi,eax
			} else if (cmdFileVersionMS == 0x60002) {
				// 6.2.*.*
				WriteMemory(pForMkstr, "\x85\xC0"   // test eax,eax
									   "\x75\xF6"   // jnz $-8
									   "\x2B\xD1"   // sub edx,ecx
									   "\xD1\xFA"   // sar edx,1
									   "\xFE\xC6"   // inc dh
									   "\x89\xD3"   // mov ebx,edx
									   , 12);
			} else {
				// 6.3.*.*
				// 10.0.*.*
				WriteMemory(pForMkstr, "\xFE\xC6"   // inc dh
									   "\x92", 3);	// xchg edx,eax
			}
#endif

			// Hook FOR /F to maintain a line number.
			ForFend_org = (DWORD_PTR)pForFend+6 + *(int *)(pForFend+2);
			memcpy(oldForFbegin, pForFbegin, 6);
			memcpy(oldForFend, pForFend, 6);

#ifdef _WIN64
			// CMD and the DLL could be more than 2GiB apart, so allocate some
			// memory before CMD to near jump to, which then does an absolute
			// jump to the DLL.
			static const char redirect_code[] =
			/*	0 */								// MyPutStdErrMsg
			/*	8 */								// MyLexText

			/* 16 */	"\xFF\x25\xEA\xFF\xFF\xFF"  // jmp [redirect]

			/* 22 */	"\xBB\00\x40\x00\x00"       // mov ebx,0x4000
			/* 27 */	"\xFF\x25\xE7\xFF\xFF\xFF"  // jmp [redirect+8]

			/* 33 */	"\x81\xFA\x7B\x23\x00\x00"  // cmp edx,0x237b ;Terminate batch job?
						"\x75\x05"                  // jne not_terminate
			/* 41 */	"\xB8\x00\x00\x00\x00"      // mov eax,0 ;or 1
						"\xC3"                      // ret
						"\x58"                      // pop rax ;return address
						"\x48\x83\xC0\x02"          // add rax,2 ;skip RET/NOP
			/* 52 */	"???????"                   // original code
						"\x50"                      // push rax
						"\xC3"                      // ret
						"???"                       // alignment
			/* 64 */	"????????"                  // SFWork_mkstr
			/* 72 */	"\xFF\x25\xF2\xFF\xFF\xFF"  // jmp [redirect+64]
						"??"                        // alignment
			/* 80 */	"????????"                  // ForFbegin_hook
			/* 88 */	"????????"                  // ForFend
			;

			for (redirect = cmd - 0x1000;;) {
				LPVOID mem = VirtualAlloc(redirect, 16 + sizeof(redirect_code),
										  MEM_COMMIT | MEM_RESERVE,
										  PAGE_EXECUTE_READWRITE);
				if (mem) {
					redirect = mem;
					break;
				} else {
					MEMORY_BASIC_INFORMATION mbi;
					VirtualQuery(redirect, &mbi, sizeof(mbi));
					redirect = (LPBYTE)mbi.AllocationBase - 0x1000;
				}
			}
			((LPVOID *)redirect)[0] = MyPutStdErrMsg;
			memcpy(redirect + 16, redirect_code, sizeof(redirect_code));
			memcpy(redirect + 52, pCtrlCAborts, 7);
			((LPVOID *)redirect)[8] = SFWork_mkstr;
			i = (DWORD_PTR)redirect + 72 - (DWORD_PTR)pSFWorkmkstr - 5;
			WriteMemory(pSFWorkmkstr+1, &i, 4);

			// No need for original begin, it can never be true.
			((LPVOID *)redirect)[10] = ForFbegin;
			if (pForFend[1] == 0x82) {
				((LPVOID *)redirect)[11] = ForFend;
			} else {
				// 6.2.8102.0 uses jnc, not jc.
				((LPVOID *)redirect)[11] = ForFend_opp;
			}
			i = (DWORD_PTR)redirect + 80 - (DWORD_PTR)pForFbegin - 6;
			WriteMemory(pForFbegin+2, &i, 4);
			i = (DWORD_PTR)redirect + 88 - (DWORD_PTR)pForFend - 6;
			WriteMemory(pForFend+2, &i, 4);
			WriteMemory(pForFbegin, "\xFF\x15", 2);     // call [rip]
			WriteMemory(pForFend, "\xFF\x15", 2);
#else
			if (pForFbegin[1] == 0x83) {
				// No need for original begin, it can never be true.
				i = (DWORD_PTR)ForFbegin_hook - (DWORD_PTR)pForFbegin - 5;
				WriteMemory(pForFbegin, (LPVOID) 0xE8, 1);
				WriteMemory(pForFbegin+1, &i, 4);
				WriteMemory(pForFbegin+5, (LPVOID) 0x90, 1);
				i = (DWORD)ForFend;
			} else {
				// No need to return, it can never be false.
				ForFbegin_org = (DWORD_PTR)pForFbegin+6 + *(int *)(pForFbegin+2);
				i = (DWORD_PTR)ForFbegin_jmp - (DWORD_PTR)pForFbegin - 6;
				WriteMemory(pForFbegin+2, &i, 4);
				i = (DWORD)ForFend_opp;
			}
			i -= (DWORD)pForFend + 5;
			WriteMemory(pForFend, (LPVOID) 0xE8, 1);
			WriteMemory(pForFend+1, &i, 4);
			WriteMemory(pForFend+5, (LPVOID) 0x90, 1);
#endif

			// Hook PutStdErr to write the batch file name and line number.
			iPutMsg = *pPutStdErrMsg;
			pPutMsg = (LPVOID)((DWORD_PTR)pPutStdErrMsg + 4 + iPutMsg);
#ifdef _WIN64
			i = (DWORD_PTR)redirect + 16 - (DWORD_PTR)pPutStdErrMsg - 4;
#else
			if (cmdFileVersionMS > 0x60002) {
				i = (DWORD_PTR)fastPutStdErrMsg;
			} else if (cmdFileVersionMS == 0x60002) {
				i = (DWORD_PTR)fastPutStdErrMsg62;
			} else {
				i = (DWORD_PTR)stdPutStdErrMsg;
			}
			i -= (DWORD_PTR)pPutStdErrMsg + 4;
#endif
			WriteMemory(pPutStdErrMsg, &i, 4);

			// Hook Lex text type to process Unicode characters.
			memcpy(oldLexText, pLexText, 5);
			WriteMemory(pLexText, (LPVOID) 0xE8, 1); // call
#ifdef _WIN64
			((LPVOID *)redirect)[1] = MyLexText;
			if (cmdDebug) {
				// Currently only the one debug version.
				/*if (cmdFileVersionMS = 0x60003 &&
					cmdFileVersionLS == 0x24d70000) */
				i = (DWORD_PTR)redirect + 22;
			} else {
				i = (DWORD_PTR)redirect + 27;
			}
#else
			if (cmdDebug) {
				// Currently only the one debug version.
				/*if (cmdFileVersionMS = 0x60003 &&
					cmdFileVersionLS == 0x24d70000) */
				i = (DWORD_PTR)MyLexTextESI;
			} else {
				i = (DWORD_PTR)MyLexText;
			}
#endif
			i -= (DWORD_PTR)pLexText + 5;
			WriteMemory(pLexText + 1, &i, 4);

			break;
		}
	}
}

void hookEchoOptions(BOOL options)
{
	if (!options) {
		// Swap START & ECHO's help tests, so ECHO has no help and START
		// only looks at its first argument.
		WriteMemory(pStartHelp, (LPVOID) 9, 1);
		WriteMemory(pEchoHelp, (LPVOID) 31, 1);
		// Patch ECHO to always echo, ignoring state.
#ifdef _WIN64
		if (cmdFileVersionMS == 0x50002) {
			// 5.2.*.*
			WriteMemory(pEchoOnOff, "\x6A\x03"  // push 3
									"\x59"      // pop rcx
									, 3);
		} else if (cmdFileVersionMS == 0x60002) {
			// 6.2.*.*
			WriteMemory(pEchoOnOff, "\x31\xC9"      // xor ecx,ecx
									"\x83\xC9\x01"  // or ecx,1
									, 5);
		} else {
			// 6.0.*.*
			// 6.1.*.*
			// 6.3.*.*
			// 10.*.*.*
			WriteMemory(pEchoOnOff, "\xB8\x03\x00\x00", 5);     // mov eax,3
		}
#else
		if (cmdFileVersionMS < 0x60002)  {
			// 5.*.*.*
			// 6.0.*.*
			// 6.1.*.*
			WriteMemory(pEchoOnOff, "\x58"      // pop eax
									"\x58"      // pop eax
									"\x6A\x03"  // push 3
									"\x58"      // pop eax
									, 5);
		} else if (cmdFileVersionMS == 0x60002) {
			if (cmdFileVersionLS == 0x1FA60000) {
				// 6.2.8102.0
				WriteMemory(pEchoOnOff, "\x90"      // nop
										"\x58"      // pop eax
										"\x6A\x03"  // push 3
										"\x58"      // pop eax
										, 5);
			} else {
				// 6.2.9200.16384
				WriteMemory(pEchoOnOff, "\x33\xC0"      // xor eax,eax
										"\x83\xC8\x01"  // or eax,1
										, 5);
			}
		} else {
			// 6.3.*.*
			// 10.*.*.*
			WriteMemory(pEchoOnOff, "\xB8\x03\x00\x00", 5);     // mov eax,3
		}
#endif
	} else {
		WriteMemory(pStartHelp, (LPVOID) 31, 1);
		WriteMemory(pEchoHelp, (LPVOID) 9, 1);
		WriteMemory(pEchoOnOff, oldEchoOnOff, 5);
	}
}

void hookCtrlCAborts(char aborts)
{
	if (aborts == -1) {
		WriteMemory(pCtrlCAborts, oldCtrlCAborts, sizeof(oldCtrlCAborts));
	} else {
#ifdef _WIN64
		char code[7];
		code[0] = 0xE8; 		// call redirect+33
		*(int *)(code+1) = (DWORD_PTR)redirect+33 - ((DWORD_PTR)pCtrlCAborts+5);
		code[5] = 0xC3; 		// ret
		code[6] = 0x90; 		// nop
		redirect[42] = aborts;
#else
		char code[5];
		if (cmdFileVersionMS < 0x60003) {
			code[0] = 0x58; 	// pop eax ;0
			code[1] = 0x59; 	// pop ecx
		} else {
			code[0] = 0x33; 	// xor eax,eax
			code[1] = 0xC0;
		}
		code[2] = 0x59; 		// pop ecx
		code[3] = 0xB0; 		// mov al, aborts
		code[4] = aborts;
#endif
		WriteMemory(pCtrlCAborts, code, sizeof(code));
	}
}

void unhookCmd(void)
{
	kh_destroy(ptrdw, sfwork_map);

	WriteMemory(peCall, &eCall, sizeof(eCall));
	WriteMemory(peEcho, &eEcho, sizeof(eEcho));
	WriteMemory(pPutStdErrMsg, &iPutMsg, 4);
	WriteMemory(pLexText, oldLexText, 5);
	WriteMemory(pEchoOnOff, oldEchoOnOff, 5);
	WriteMemory(pStartHelp, (LPVOID) 31, 1);
	WriteMemory(pEchoHelp, (LPVOID) 9, 1);
	WriteMemory(pCtrlCAborts, oldCtrlCAborts, sizeof(oldCtrlCAborts));
	WriteMemory(pSFWorkmkstr, oldSFWorkmkstr, 6);
	WriteMemory(pSFWorkresize, oldSFWorkresize, 6);
	WriteMemory(pForFbegin, oldForFbegin, 6);
	WriteMemory(pForFend, oldForFend, 6);

	*pfDumpTokens = 0;
	*pfDumpParse = 0;

#ifdef _WIN64
	VirtualFree(redirect, 0, MEM_RELEASE);
#endif
}
