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
#include <io.h>

#include "khash.h"
#ifdef _WIN64
KHASH_MAP_INIT_INT64(ptrdw, DWORD)
#else
KHASH_MAP_INIT_INT(ptrdw, DWORD)
#endif

khash_t(ptrdw) *sfwork_map;


fncmd_printf cmd_printf;

struct sCmdEntry {
	LPCWSTR   name;
	fnCmdFunc func;
	DWORD	  flags;
	DWORD	  helpid;
	DWORD	  exthelpid;
	DWORD	  helpextra;
};

BYTE trace;

#ifdef _WIN64
LPBYTE redirect;
BYTE oldCtrlCAborts[7];
BYTE SFWork_saved, Goto_pos, Goto_start;
BYTE oldDESubWorkFreeStr1[4], oldDESubWorkFreeStr2[4];
#else
BYTE oldCtrlCAborts[5];
DWORD SFWork_esp, ParseForF_ret;
BYTE SFWork_stdcall;
int SFWork_saved, SFWork_first, Goto_pos, Goto_start;
DWORD ForFbegin_org, ParseFor_org, ParseForF_org, CallWorkresize_org;
DWORD MyGetEnvVarPtr_org;
#endif
DWORD MyGetEnvVarPtr_size;

int oldPutMsg, oldParseFor, oldParseForF, oldCallWorkresize;
int oldDisplayEnv, oldDisplayEnvVariable;
BYTE oldLexText[5], oldSFWorkmkstr[6], oldSFWorkresize[6];
BYTE oldDESubWorkFreeStr[5], oldDispatchAfterDE[6];
BYTE oldForFbegin[6], oldForFend[6], oldGotoEof[6];
DWORD_PTR mkstr, FreeStack, DESubWork_FreeStr_org, ForFend_org;
DWORD_PTR DisplayEnv_org, DisplayEnvVariable_org, DispatchAfterDE_org;
BYTE SFWork_mkstr_reg, Goto_reg;
DWORD mkstr_size, mkstr_overhead, mkstr_alloc;

int ForF_stack[FORF_STACKSIZE], ForF_stacktop;


#define LABEL_WS 1
#define LABEL_DELIM 2

static char label_type[256];

#define islabel_ws(ch) (label_type[(BYTE) (ch)] & LABEL_WS)
#define islabel_delim(ch) (label_type[(BYTE) (ch)] != 0) // & (LABEL_WS | LABEL_DELIM))


int __fastcall SFWork_hook(LPWSTR *saved, int first)
{
	khint_t k;
	int absent;

	if CMD_VERSION(6,2,8102,0) {
		// This version inlines SFWork within SubFor, so there's additional
		// allocations that should not be freed.
		return 0;
	}

	if (*saved == NULL) {
		k = kh_put(ptrdw, sfwork_map, (DWORD_PTR) saved, &absent);
		kh_val(sfwork_map, k) = 0;
		return 0;
	}
	k = kh_get(ptrdw, sfwork_map, (DWORD_PTR) saved);
	if (kh_val(sfwork_map, k) == 0 || first) {
		kh_val(sfwork_map, k) = *pDCount;
		return 0;
	}
	return kh_val(sfwork_map, k);
}


void ForFbegin_hook(void)
{
	if (++ForF_stacktop == lenof(ForF_stack)) {
		ForF_stacktop = 0;
	}
	ForF_stack[ForF_stacktop] = 1;
}

void __fastcall ForFend_hook(BYTE end)
{
	if (end || *pGotoFlag) {
		if (--ForF_stacktop == -1) {
			ForF_stacktop = lenof(ForF_stack) - 1;
		}
	} else {
		++ForF_stack[ForF_stacktop];
	}
}


void ParseFor(void)
{
	DWORD len;

	LPWSTR p = *pLexBufPtr;
	while (iswspace(*p)) {
		++p;
	}
	if ((p[0] | 0x20) == L'd' && (p[1] | 0x20) == L'o' && iswspace(p[2])) {
		// Translate "for do ..." to "for %_ in (:*) do ...".
		len = wcslen(*pLexBufPtr);
		if (*pLexBufPtr + 10 + len >= pLexBufferend) {
			return;
		}
		memmove(*pLexBufPtr + 10, *pLexBufPtr, WSZ(len + 1));
		memcpy(*pLexBufPtr, L"%_ in (:*)", WSZ(10));
	} else if (p[0] == L'%' && p[1] != L'\0' && iswspace(p[2]) &&
			   ((p[3] | 0x20) == L'd' && (p[4] | 0x20) == L'o' && iswspace(p[5]))) {
		// Translate "for %? do ..." to "for %? in (:range*) do ...".
		p += 3;
		len = wcslen(p);
		if (p + 13 + len >= pLexBufferend) {
			return;
		}
		memmove(p + 13, p, WSZ(len + 1));
		memcpy(p, L"in (:range*) ", WSZ(13));
	} else if (p[0] == L'%' && p[1] != L'\0' && iswspace(p[2])
			   && (iswdigit(p[3]) || (p[3] == L'-' && iswdigit(p[4]))
				   || (*pfDelayedExpansion && p[3] == L'!'))) {
		// Translate "for %? N ..." to "for %? in (:range*N) ...".
		int digits = 1;
		p += 4;
		if (p[-1] == L'!') {
			if (*p == L'!') {
				return;
			}
			for (;;) {
				if (*p == L'\0') {
					return;
				}
				++digits;
				++p;
				if (p[-1] == L'!') {
					break;
				}
			}
		} else {
			while (iswdigit(*p) || *p == L':' || *p == L'-') {
				++digits;
				++p;
			}
		}
		len = wcslen(p);
		if (p + 12 + len >= pLexBufferend) {
			return;
		}
		memmove(p + 12, p, WSZ(len + 1));
		p[11] = L')';
		memmove(p + 11 - digits, p - digits, WSZ(digits));
		memcpy(p - digits, L"in (:range*", WSZ(11));
	} else if (*p == L'(') {
		// Translate "for (COND) do ..." to
		// "for %_ in (:*) do if not COND (set @next=) else ...".
		LPWSTR cond = ++p;
		BOOL quote = FALSE;
		for (; *p != L'\0'; ++p) {
			if (quote) {
				if (*p == L'"') {
					quote = FALSE;
				}
			} else if (*p == L'"') {
				quote = TRUE;
			} else if (*p == L')') {
				break;
			}
		}
		if (*p == L'\0') {
			return;
		}
		LPWSTR cond_end = p;
		while (iswspace(*++p)) {
			// do nothing
		}
		if ((p[0] | 0x20) != L'd' || (p[1] | 0x20) != L'o' || !iswspace(p[2])) {
			return;
		}
		p += 2;
		while (iswspace(*cond)) {
			++cond;
		}
		BOOL not = TRUE;
		if ((cond[0] | 0x20) == L'n' && (cond[1] | 0x20) == L'o'
			&& (cond[2] | 0x20) == L't' && iswspace(cond[3])) {
			not = FALSE;
			cond += 4;
		}
		DWORD new_len = cond_end - cond + 35;
		if (not) {
			new_len += 4;
		}
		len = wcslen(p);
		if (*pLexBufPtr + new_len + len >= pLexBufferend) {
			return;
		}
		memmove(*pLexBufPtr + new_len, p, WSZ(len + 1));
		memmove(*pLexBufPtr + 17 + (not ? 4 : 0), cond, WSZ(cond_end - cond));
		memcpy(*pLexBufPtr + new_len - 18, L" (set @next=) else", WSZ(18));
		if (not) {
			memcpy(*pLexBufPtr + 17, L"not ", WSZ(4));
		}
		memcpy(*pLexBufPtr, L"%_ in (:*) do if ", WSZ(17));
	}
}

void ParseForF(void)
{
	LPWSTR p = pTmpBuf;
	if (*p == L'%' || *p == L'/') {
		return;
	}
	if (*p == L'"' || *p == L'\'') {
		++p;
	}
	for (;;) {
		while (*p <= L' ') {
			if (*p == L'\0') {
				return;
			}
			++p;
		}
		if (WCSIBEG(p, L"line")) {
			// Translate "... line..." to "...     ... delims= eol=".
			p[0] = p[1] = p[2] = p[3] = L' ';
			p += wcslen(p);
			if (p[-1] == L'"' || p[-1] == L'\'') {
				--p;
			}
			wcscpy(p, L" delims= eol=");
			*pTokLen = wcslen(pTmpBuf) + 1; 	// includes NUL
			return;
		}
		while (*p > L' ') {
			++p;
		}
	}
}


LPCSTR findLabelForward(LPCSTR label, DWORD len, LPCSTR batch, DWORD size)
{
	LPCSTR p, q;

	for (p = batch;; ++p) {
		p = memchr(p, ':', size - (DWORD)(p - batch));
		if (p == NULL) {
			break;
		}
		if (p+1+len < batch+size && islabel_delim(p[1+len])
			&& _memicmp(p+1, label, len) == 0) {
			for (q = p;;) {
				if (q == batch || q[-1] == '\n') {
					return p;
				}
				if (!islabel_ws(*--q)) {
					break;
				}
			}
		}
	}

	return p;
}

LPCSTR findLabelBackward(LPCSTR label, DWORD len, LPCSTR batch, DWORD size)
{
	LPCSTR p, q;
	char first;

	first = tolower(*label++);
	--len;
	for (p = batch + size - len - 1; p >= batch + 1; p -= 2) {
		if (*p == ':' && tolower(p[1]) == first) {
			p += 2;
		} else if (p[-1] == ':' && tolower(*p) == first) {
			++p;
		} else {
			continue;
		}
		if (islabel_delim(p[len]) && _memicmp(p, label, len) == 0) {
			for (q = p - 2;;) {
				if (q == batch || q[-1] == '\n') {
					return p - 1;
				}
				if (!islabel_ws(*--q)) {
					break;
				}
			}
		}
		p -= 2; 		// back to the colon before this non-matching label
	}

	return NULL;
}

DWORD findLabel(LPCSTR label, int len, DWORD start, DWORD fsize)
{
	LPSTR batch;
	LPCSTR p;
	DWORD pos;
	char buf[65536];

	batch = readBatchFile(fsize, buf, sizeof(buf));
	if (batch == NULL) {
		return -1;
	}

	if (*label == '~' && label[1] != '\0') {
		++label;
		--len;
		p = findLabelBackward(label, len, batch, start);
		if (p == NULL) {
			p = findLabelBackward(label, len, batch + start, fsize - start);
		}
	} else {
		p = findLabelForward(label, len, batch + start, fsize - start);
		if (p == NULL) {
			p = findLabelForward(label, len, batch, start);
		}
	}
	if (p != NULL) {
		p += len;
		p = memchr(p, '\n', fsize - (DWORD)(p - batch));
		if (p != NULL) {
			pos = (DWORD)(p+1 - batch);
		} else {
			pos = fsize;
		}
	} else {
		pos = -1;
	}

	if (batch != buf) {
		free(batch);
	}

	return pos;
}

DWORD __fastcall Goto(LPCWSTR line, DWORD start, DWORD fsize)
{
	char label[512];		// maximum label size is 128, quadruple for UTF-8
	LPCWSTR w;

	if (*line == L':') {
		++line;
	}
	w = line;
	while (!(*w < 256 && islabel_delim(*w))) {
		++w;
	}
	int len = WideCharToMultiByte(GetConsoleOutputCP(), 0, line, w - line,
								  label, sizeof(label) - 1, NULL, NULL);
	if (len == 0) {
		return -1;
	}
	return findLabel(label, len, start, fsize);
}


void __fastcall CallWorkresize_hook(LPWSTR line)
{
	BOOL in_quote = FALSE;

	for (; *line != L'\0'; ++line) {
		if (in_quote) {
			if (*line == L'^') {
				wcscpy(line+1, line+2);
			} else if (*line == L'"') {
				in_quote = FALSE;
			}
		} else if (*line == L'"') {
			in_quote = TRUE;
		}
	}
}


void __fastcall TraceCommand(struct cmdnode *node)
{
	LPWSTR buf = varBuffer;
	LPCWSTR end = L"";
	if (_isatty(2)) {
		*buf++ = L'[';
		end = L"]";
	}
	if (node->redir != NULL) {
		struct redirnode *redir = node->redir;
		do {
			if ((redir->symbol == L'<' && redir->fh != 0)
				|| (redir->symbol == L'>' && redir->fh != 1)) {
				buf += sbprintf(buf, L"%d", redir->fh);
			}
			buf += sbprintf(buf, L"%c", redir->symbol);
			if (redir->append) {
				buf += sbprintf(buf, L">", redir->symbol);
			}
			buf += sbprintf(buf, L"%s ", redir->name);
			redir = redir->next;
		} while (redir != NULL);
	}
	sbprintf(buf, L"%s%s%s\r\n",
			 node->cmd, node->arg ?: L"", end);
	LPVOID args = &varBuffer;
	LPVOID pargs = &args;
	MyPutStdErrMsg(0x2371, 2, 1, (va_list *) &pargs);
}


LPCWSTR __fastcall MyGetEnvVarPtr_hook(LPWSTR var)
{
	if (var != NULL && (*var == L'$' || *var == L'@')) {
		if (getVar(var) != 0) {
			return stringBuffer;
		}
	}
	return NULL;
}


#ifndef _WIN64
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

#define PROC(func) \
	void func();\
	asm("_" #func ":\n"
#define ENDPROC );

PROC(MyLexTextESI)
	"call _MyLexText\n"
	"movl %eax,%esi\n"
	"ret"
ENDPROC

PROC(SFWork_mkstr)
	"push %edi\n"
	"mov _SFWork_first,%eax\n"
	"mov (%eax,%ebp),%edx\n"
	"mov _SFWork_saved,%eax\n"
	"test %eax,%eax\n"
	"mov (%eax,%ebp),%edi\n"
	"js 1f\n"
	"mov -28(%ebp),%ecx\n"      // debug version doesn't store it
	"lea 4(%ecx,%edi,4),%edi\n"
	"1:\n"
	"mov %edi,%ecx\n"
	"call _SFWork_hook\n"
	"test %eax,%eax\n"
	"jz 1f\n"
	"mov %eax,%ecx\n"           // 6.2.9200.16384 passes in eax, 6.3+ in ecx
	"mov %esp,_SFWork_esp\n"    // 6.2.8102.0 is stdcall, but mkstr is fast
	"push %eax\n"
	"call *_FreeStack\n"        // free everything allocated between loops
	"mov _SFWork_esp,%esp\n"
	"1:\n"
	"add $20,%edi\n"            // saved + 5, unused space in the node
	"mov (%edi),%eax\n"
	"test %eax,%eax\n"
	"jnz 1f\n"                  // reuse the original mkstr
	"mov _mkstr_size,%eax\n"
	"mov %eax,%ecx\n"           // 6.2 passes in eax, 6.3+ in ecx
	"mov %esp,_SFWork_esp\n"
	"push %eax\n"               // before 6.2 is stdcall
	"call *_mkstr\n"
	"mov _SFWork_esp,%esp\n"
	"mov %eax,(%edi)\n"
	"1:\n"
	"mov -8(%eax),%ecx\n"       // no terminator is added, reset to 0
	"sub $8,%ecx\n"             // length that was requested
	"shr $2,%ecx\n"             //  is a multiple of four
	"push %eax\n"
	"mov %eax,%edi\n"
	"xor %eax,%eax\n"
	"rep stosl\n"
	"pop %eax\n"
	"pop %edi\n"
	"mov _SFWork_mkstr_reg,%cl\n"
	"test %cl,%cl\n"
	"jnz 1f\n"                  // inline
	"cmp %cl,_SFWork_stdcall\n"
	"jnz 2f\n"
	"ret\n"
	"2:\n"
	"ret $4\n"
	"1:\n"
	"addl $0x34,(%esp)\n"       // skip over all the inline code
	"cmp $0xF6,%cl\n"           // select the register it uses
	"cmove %eax,%esi\n"
	"cmovb %eax,%ebx\n"         // 0xDB
	"cmova %eax,%edi\n"         // 0xFF
	"ret $8"
ENDPROC

// Prevent DESubWork from freeing our reusable substitution string.
PROC(DESubWork_FreeStr_stdcall)
	"mov -8(%eax),%eax\n"
	"cmp _mkstr_alloc,%eax\n"
	"je 1f\n"
	"jmp *_DESubWork_FreeStr_org\n"
	"1:\n"
	"ret $4"
ENDPROC

PROC(DESubWork_FreeStr_fastcall)
	"mov -8(%ecx),%eax\n"
	"cmp _mkstr_alloc,%eax\n"
	"je 1f\n"
	"jmp *_DESubWork_FreeStr_org\n"
	"1:\n"
	"ret"
ENDPROC

PROC(DESubWork_FreeStr_inline)
	"mov 12(%ebp),%ecx\n"
	"mov (%ecx),%esi\n"
	"test %esi,%esi\n"
	"jz 1f\n"
	"mov _mkstr_alloc,%eax\n"
	"cmp -8(%esi),%eax\n"
	"jne 1f\n"
	"xor %esi,%esi\n"
	"1:\n"
	"ret"
ENDPROC

PROC(ForFend)
	"pushf\n"
	"setnc %cl\n"
	"call _ForFend_hook\n"
	"popf\n"
	"jnc 1f\n"
	"mov _ForFend_org,%eax\n"
	"mov %eax,(%esp)\n"
	"1:\n"
	"ret"
ENDPROC

PROC(ForFend_opp)
	"pushf\n"
	"setnc %cl\n"
	"call _ForFend_hook\n"
	"popf\n"
	"jc 1f\n"
	"mov _ForFend_org,%eax\n"
	"mov %eax,(%esp)\n"
	"1:\n"
	"ret"
ENDPROC

PROC(ForFbegin_jmp)
	"call _ForFbegin_hook\n"
	"jmp *_ForFbegin_org"
ENDPROC

PROC(ParseFor_hook)
	"push %eax\n"
	"push %ecx\n"
	"call _ParseFor\n"
	"pop %ecx\n"
	"pop %eax\n"
	"jmp *_ParseFor_org"
ENDPROC

PROC(ParseForF_hook)
	"pop _ParseForF_ret\n"
	"call *_ParseForF_org\n"
	"push _ParseForF_ret\n"
	"jmp _ParseForF"
ENDPROC

PROC(GotoEof)
	"push 12(%esp)\n"
	"push 12(%esp)\n"
	"push 12(%esp)\n"
	"call __wcsnicmp\n"             // should really call original, but meh
	"add $12,%esp\n"
	"test %eax,%eax\n"
	"jz 1f\n"
	"mov _Goto_pos,%ecx\n"
	"mov %ebp,%eax\n"
	"test %ecx,%ecx\n"
	"cmovns %esp,%eax\n"
	"add %eax,%ecx\n"
	"add _Goto_start,%eax\n"
	"mov (%eax),%edx\n"
	"push %ecx\n"
	"push (%ecx)\n"
	"mov 8+4(%esp),%ecx\n"
	"call _Goto\n"
	"test %eax,%eax\n"
	"pop %ecx\n"
	"js 1f\n"
	"mov %eax,(%ecx)\n"
	"xor %eax,%eax\n"
	"cmp _Goto_reg,%al\n"
	"jl 2f\n"
	"jg 3f\n"
	"mov %ax,(%esi)\n"
	"sub $8,%esi\n"
	"ret\n"
	"2:\n"
	"mov %ax,(%edi)\n"
	"sub $8,%edi\n"
	"ret\n"
	"3:\n"
	"mov %ax,(%ebx)\n"
	"sub $8,%ebx\n"
	"1:\n"
	"ret"
ENDPROC

PROC(CallWorkresize_stdcall_hook)
	"mov 4(%esp),%ecx\n"
	"call _CallWorkresize_hook\n"
	"jmp *_CallWorkresize_org\n"
ENDPROC

PROC(CallWorkresize_fastcall_hook)
	"push %ecx\n"
	"push %edx\n"
	"call _CallWorkresize_hook\n"
	"pop %edx\n"
	"pop %ecx\n"
	"jmp *_CallWorkresize_org\n"
ENDPROC

PROC(CallWorkresize_fastcall62_hook)
	"push %eax\n"
	"push %ecx\n"
	"mov %eax,%ecx\n"
	"call _CallWorkresize_hook\n"
	"pop %ecx\n"
	"pop %eax\n"
	"jmp *_CallWorkresize_org\n"
ENDPROC

PROC(DisplayEnv_hook)
	"push %ecx\n"
	"xor %ecx,%ecx\n"
	"call _displayVars\n"
	"pop %ecx\n"
	"jmp *_DisplayEnv_org"
ENDPROC

PROC(DisplayEnvVariable_hook)
	"movzxw (%esi),%eax\n"
	"cmp $'$',%eax\n"
	"je 1f\n"
	"cmp $'@',%eax\n"
	"je 1f\n"
	"jmp *_DisplayEnvVariable_org\n"
	"1:\n"
	"mov %esi,%ecx\n"
	"call _displayVars\n"
	"cmp 4(%esp),%esi\n"        // < 6.3 is stdcall
	"je 1f\n"
	"ret\n"
	"1:\n"
	"ret $4"
ENDPROC

void DispatchAfterDE_cc();

PROC(DispatchAfterDE_hook)
	"push %ecx\n"
	"pushf\n"
	"mov %esi,%ecx\n"
	"cmpl $0,(%ecx)\n"
	"jne 1f\n"
	"cmpb $0,_trace\n"
	"je 1f\n"
	"call _TraceCommand\n"
	"1:\n"
	"popf\n"
	"pop %ecx\n"
	"_DispatchAfterDE_cc:\n"
	"jne 1f\n"
	"mov _DispatchAfterDE_org,%eax\n"
	"mov %eax,(%esp)\n"
	"1:\n"
	"ret"
ENDPROC

void MyGetEnvVarPtr_fastcall_hook(), MyGetEnvVarPtr_fastcall62_hook();
void MyGetEnvVarPtrOrg();

PROC(MyGetEnvVarPtr_stdcall_hook)
	"mov 4(%esp),%ecx\n"
	"call _MyGetEnvVarPtr_hook\n"
	"test %eax,%eax\n"
	"jz 1f\n"
	"ret $4\n"

	"_MyGetEnvVarPtr_fastcall_hook:\n"
	"push %ecx\n"
	"call _MyGetEnvVarPtr_hook\n"
	"pop %ecx\n"
	"test %eax,%eax\n"
	"jz 1f\n"
	"2:\n"
	"ret\n"

	"_MyGetEnvVarPtr_fastcall62_hook:\n"
	"push %eax\n"
	"mov %eax,%ecx\n"
	"call _MyGetEnvVarPtr_hook\n"
	"pop %ecx\n"
	"test %eax,%eax\n"
	"jnz 2b\n"
	"mov %ecx,%eax\n"

	"1:\n"
	"_MyGetEnvVarPtrOrg:\n"
	".fill 7,1,0x90\n"              // original code patched in
	"jmp *_MyGetEnvVarPtr_org\n"
ENDPROC

#else

#define PROC(func) \
	void func();\
	asm(#func ":\n"
#define ENDPROC );

PROC(SFWork_mkstr)
	"cmpb $0,cmdDebug(%rip)\n"      // debug version (only one, so far)
	"cmovnz 0xB0(%rsp),%r9d\n"      // retrieve the value
	"mov %r9d,%edx\n"
	"movzxb SFWork_saved(%rip),%eax\n"
	"test $0x80,%al\n"
	"jns 2f\n"
	"cmp $0xA0,%al\n"
	"je 1f\n"
	"mov 8(%rsp,%rax),%rax\n"       // 0x90, 0x80
	"lea 8(%rax,%rdi,8),%rcx\n"
	"jmp 3f\n"
	"1:\n"
	"mov -8(%rsp,%rax),%rax\n"      // 0xA0
	"lea 8(%rax,%r13,8),%rcx\n"
	"jmp 3f\n"
	"2:\n"
	"cmp $0x34,%al\n"
	"jne 1f\n"
	"mov %r14,%rcx\n"               // 0x34
	"jmp 3f\n"
	"1:\n"
	"cmova %r15,%rax\n"             // 0x3C
	"cmovb %r13,%rax\n"             // 0x2C
	"lea 8(%rax),%rcx\n"
	"3:\n"
	"push %rdi\n"
	"sub $32,%rsp\n"                // shadow space
	"mov %rcx,%rdi\n"
	"call SFWork_hook\n"
	"test %eax,%eax\n"
	"jz 1f\n"
	"mov %eax,%ecx\n"
	"call *FreeStack(%rip)\n"       // free everything allocated between loops
	"1:\n"
	"add $40,%rdi\n"                // saved + 5, unused space in the node
	"mov (%rdi),%rax\n"
	"test %rax,%rax\n"
	"jnz 1f\n"                      // reuse the original mkstr
	"mov mkstr_size(%rip),%ecx\n"
	"call *mkstr(%rip)\n"
	"mov %rax,(%rdi)\n"
	"1:\n"
	"add $32,%rsp\n"
	"mov -16(%rax),%ecx\n"          // no terminator is added, reset to 0
	"sub $16,%ecx\n"                // length that was requested
	"shr $2,%ecx\n"                 //  which is a multiple of four
	"push %rax\n"
	"mov %rax,%rdi\n"
	"xor %eax,%eax\n"
	"rep stosl\n"
	"pop %rax\n"
	"pop %rdi\n"
	"mov SFWork_mkstr_reg(%rip),%cl\n"
	"test %cl,%cl\n"
	"jz 1f\n"
	"addq $0x48,(%rsp)\n"           // skip over all the inline code
	"cmp $0xFF,%cl\n"               // select the register it uses
	"cmove %rax,%rdi\n"
	"cmovne %rax,%rbx\n"
	"1:\n"
	"ret"
ENDPROC

PROC(GotoEof)
	"push %rcx\n"
	"sub $32,%rsp\n"
	"call _wcsnicmp\n"
	"add $32,%rsp\n"
	"pop %rcx\n"
	"test %eax,%eax\n"
	"jz 1f\n"
	"mov Goto_pos(%rip),%al\n"
	"test %al,%al\n"
	"mov 0x58(%rsp),%r8d\n"         // only one in the stack
	"jns 2f\n"
	"cmp $0xe0,%al\n"
	"mov %esi,%r8d\n"               // F0
	"cmove %r12,%r8\n"              // E0
	"cmovb %r14,%r8\n"              // 8B
	"2:\n"
	"movzxb Goto_start(%rip),%edx\n"
	"mov (%rsp,%rdx),%edx\n"
	"push %rcx\n"
	"sub $32,%rsp\n"
	"call Goto\n"
	"add $32,%rsp\n"
	"pop %rcx\n"
	"test %eax,%eax\n"
	"js 1f\n"
	"mov Goto_pos(%rip),%dl\n"
	"test %dl,%dl\n"
	"js 3f\n"
	"mov %eax,0x58(%rsp)\n"         // only one in the stack
	"jmp 4f\n"
	"3:\n"
	"cmp $0xe0,%dl\n"
	"cmova %rax,%rsi\n"             // F0
	"cmove %rax,%r12\n"             // E0
	"cmovb %rax,%r14\n"             // 8B
	"4:\n"
	"xor %eax,%eax\n"
	"cmp %rcx,%rbx\n"
	"jne 2f\n"
	"mov %ax,(%rbx)\n"
	"sub $8,%rbx\n"
	"ret\n"
	"2:\n"
	"mov %ax,(%rdi)\n"
	"sub $8,%rdi\n"
	"1:\n"
	"ret"
ENDPROC

// Using the labels in C isn't as efficient as storing the offsets directly.
#define redirect_data		((LPDWORD) redirect_code + 1)
#define redirect_code_start ((LPBYTE) redirect_code + redirect_data[0])
#define redirect_code_size	redirect_data[1]
#define rMyPutStdErrMsg 	(redirect + redirect_data[2])
#define rMyLexText			(redirect + redirect_data[3])
#define rPromptUser 		(redirect + redirect_data[4])
#define rAbortFlag			(redirect + redirect_data[5])
#define rPromptUserOrg		(redirect + redirect_data[6])
#define rSFWork_mkstr		(redirect + redirect_data[7])
#define rForFbegin			(redirect + redirect_data[8])
#define rForFend			(redirect + redirect_data[9])
#define rForFendj			(redirect + redirect_data[10])
#define rParseFor			(redirect + redirect_data[11])
#define rParseForF			(redirect + redirect_data[12])
#define rParseFor_org		(DWORD_PTR *)(redirect + redirect_data[13])
#define rParseForF_org		(DWORD_PTR *)(redirect + redirect_data[13]+8)
#define rGotoEof			(redirect + redirect_data[14])
#define rCallWorkresize 	(redirect + redirect_data[15])
#define rCallWorkresize_org (DWORD_PTR *)(redirect + redirect_data[16])
#define rMyGetEnvVarPtr 	(redirect + redirect_data[17])
#define rMyGetEnvVarPtrOrg	(redirect + redirect_data[18])
#define rMyGetEnvVarPtr_org (DWORD_PTR *)(redirect + redirect_data[19])
#define rDESubWork_FreeStr	(redirect + redirect_data[20])
#define rDESubWork_FreeStr_inline (redirect + redirect_data[21])
#define DisplayEnv_hook 	(redirect + redirect_data[22])
#define DisplayEnvVariable_hook (redirect + redirect_data[23])
#define DispatchAfterDE_hook (redirect + redirect_data[24])
#define DispatchAfterDE_cc	(redirect + redirect_data[25])

// This code gets relocated to a region of memory closer to CMD, to stay within
// the 32-bit relative address range.
asm(".align 4");
PROC(redirect_code)
	".long 0\n"                     // avoid type-punning on redirect_code
	".long redirect_code_start - redirect_code\n"
	".macro rel label\n"
	".long \\label - redirect_code_start\n"
	".endm\n"
	"rel redirect_code_end\n"
	"rel rMyPutStdErrMsg\n"
	"rel rMyLexText\n"
	"rel rPromptUser\n"
	"rel rAbortFlag+1\n"
	"rel rPromptUserOrg\n"
	"rel rSFWork_mkstr\n"
	"rel rForFbegin\n"
	"rel rForFend\n"
	"rel rForFendj\n"
	"rel rParseFor\n"
	"rel rParseForF\n"
	"rel aParseFor_org\n"
	"rel aGotoEof\n"
	"rel rCallWorkresize\n"
	"rel aCallWorkresize_org\n"
	"rel rMyGetEnvVarPtr\n"
	"rel rMyGetEnvVarPtrOrg\n"
	"rel aMyGetEnvVarPtr_org\n"
	"rel rDESubWork_FreeStr\n"
	"rel rDESubWork_FreeStr_inline\n"
	"rel DisplayEnv_hook\n"
	"rel DisplayEnvVariable_hook\n"
	"rel DispatchAfterDE_hook\n"
	"rel DispatchAfterDE_cc\n"

"redirect_code_start:\n"

"rMyPutStdErrMsg:\n"
	"jmp *aMyPutStdErrMsg(%rip)\n"

	"mov $0x4000,%ebx\n"            // used by the debug version
"rMyLexText:\n"
	"jmp *aMyLexText(%rip)\n"

"rPromptUser:\n"
	"cmp $0x237b,%edx\n"            // Terminate batch job?
	"jne 1f\n"
"rAbortFlag:\n"
	"mov $0,%eax\n"                 // value patched in
	"ret\n"
	"1:\n"
	"pop %rax\n"                    // return address
	"add $2,%rax\n"                 // skip RET/NOP
"rPromptUserOrg:\n"
	".fill 7,1,0x90\n"              // original code patched in
	"push %rax\n"
	"ret\n"

"rSFWork_mkstr:\n"
	"jmp *aSFWork_mkstr(%rip)\n"

"rDESubWork_FreeStr:\n"
	"movabs mkstr_alloc,%eax\n"
	"cmp -16(%rcx),%eax\n"
	"je 1f\n"
	"movabs DESubWork_FreeStr_org,%rax\n"
	"jmp *%rax\n"
	"1:\n"
	"ret\n"

"rDESubWork_FreeStr_inline:\n"
	"test %rdx,%rdx\n"
	"jz 1f\n"
	"movabs mkstr_alloc,%eax\n"
	"cmp -16(%rdx),%eax\n"
	"jne 1f\n"
	"movabs DESubWork_FreeStr_org,%rax\n"
	"add %rax,(%rsp)\n"
	"1:\n"
	"ret\n"

"rForFbegin:\n"
	"push %rdx\n"                   // one of these registers may contain LF
	//"push %r8\n"                  // these registers aren't used by the hook
	//"push %r9\n"
	//"push %r10\n"
	//"sub $40,%rsp\n"              // shadow space not needed
	"call *aForFbegin_hook(%rip)\n"
	//"add $40,%rsp\n"
	//"pop %r10\n"
	//"pop %r9\n"
	//"pop %r8\n"
	"pop %rdx\n"
	"ret\n"

"rForFend:\n"
	"pushf\n"
	"push %rdx\n"
	//"push %r8\n"
	//"push %r9\n"
	//"push %r10\n"
	"setnc %cl\n"
	//"sub $32,%rsp\n"
	"call *aForFend_hook(%rip)\n"
	//"add $32,%rsp\n"
	//"pop %r10\n"
	//"pop %r9\n"
	//"pop %r8\n"
	"pop %rdx\n"
	"popf\n"
"rForFendj:\n"
	"jnc 1f\n"                      // possibly patched to jc
	"movabs ForFend_org,%rax\n"
	"mov %rax,(%rsp)\n"
	"1:\n"
	"ret\n"

"rParseFor:\n"
	"push %rcx\n"
	"push %rdx\n"
	"call *aParseFor(%rip)\n"
	"pop %rdx\n"
	"pop %rcx\n"
	"jmp *aParseFor_org(%rip)\n"

"rParseForF:\n"
	"sub $40,%rsp\n"
	"call *aParseForF_org(%rip)\n"
	"add $40,%rsp\n"
	"push %rax\n"
	"call *aParseForF(%rip)\n"
	"pop %rax\n"
	"ret\n"

"rCallWorkresize:\n"
	"push %rcx\n"
	"push %rdx\n"
	"sub $40,%rsp\n"
	"call *aCallWorkresize_hook(%rip)\n"
	"add $40,%rsp\n"
	"pop %rdx\n"
	"pop %rcx\n"
	"jmp *aCallWorkresize_org(%rip)\n"

"DisplayEnv_hook:\n"
	"push %rcx\n"
	"xor %ecx,%ecx\n"
	"sub $32,%rsp\n"
	"call *adisplayVars(%rip)\n"
	"add $32,%rsp\n"
	"pop %rcx\n"
	"movabs DisplayEnv_org,%rax\n"
	"jmp *%rax\n"

"DisplayEnvVariable_hook:\n"
	"movzxw (%rcx),%eax\n"
	"cmp $'$',%eax\n"
	"je 1f\n"
	"cmp $'@',%eax\n"
	"je 1f\n"
	"movabs DisplayEnvVariable_org,%rax\n"
	"jmp *%rax\n"
	"1:\n"
	"sub $40,%rsp\n"
	"call *adisplayVars(%rip)\n"
	"add $40,%rsp\n"
	"ret\n"

"DispatchAfterDE_hook:\n"
	"pushf\n"
	"cmpl $0,(%rbx)\n"
	"jne 1f\n"
	"movabs trace,%al\n"
	"cmp $0,%al\n"
	"je 1f\n"
	"mov %rbx,%rcx\n"
	"sub $32,%rsp\n"
	"call *aTraceCommand(%rip)\n"
	"add $32,%rsp\n"
	"1:\n"
	"popf\n"
"DispatchAfterDE_cc:\n"
	"jne 1f\n"
	"movabs DispatchAfterDE_org,%rax\n"
	"mov %rax,(%rsp)\n"
"1:\n"
	"ret\n"

"rMyGetEnvVarPtr:\n"
	"push %rcx\n"
	"sub $32,%rsp\n"
	"call *aMyGetEnvVarPtr_hook(%rip)\n"
	"add $32,%rsp\n"
	"pop %rcx\n"
	"test %rax,%rax\n"
	"jnz 1b\n"
"rMyGetEnvVarPtrOrg:\n"
	".fill 8,1,0x90\n"              // original code patched in
	"jmp *aMyGetEnvVarPtr_org(%rip)\n"

	".align 8\n"
	".macro abs label\n"
	"a\\label: .quad \\label\n"
	".endm\n"
	"abs MyPutStdErrMsg\n"
	"abs MyLexText\n"
	"abs SFWork_mkstr\n"
	"abs ForFbegin_hook\n"
	"abs ForFend_hook\n"
	"abs ParseFor\n"
	"abs ParseForF\n"
	"aParseFor_org: .quad 0\n"
	"aParseForF_org: .quad 0\n"
	"abs GotoEof\n"
	"abs CallWorkresize_hook\n"
	"aCallWorkresize_org: .quad 0\n"
	"abs displayVars\n"
	"abs TraceCommand\n"
	"abs MyGetEnvVarPtr_hook\n"
	"aMyGetEnvVarPtr_org: .quad 0\n"

"redirect_code_end:"
ENDPROC

#endif

#define WriteCode(addr, code) \
	do { \
		static const char insns[sizeof(code)-1] = { code }; \
		WriteMemory(addr, insns, sizeof(insns)); \
	} while (0)

// Create a displacement to TO from FROM (the end of the instruction).
#define MKDISP(to, from) ((DWORD_PTR)(to) - (DWORD_PTR)(from))

// Given the address of a displacement get its absolute destination.
#define MKABS(disp) ((DWORD_PTR)(disp)+4 + *(int *)(disp))

void hookCmd(void)
{
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNTHeader;
	LPBYTE cmd;
	LPDWORD data, end;
	struct sCmdEntry *cmdentry;
	fnCmdFunc pMyCall;
	const struct sCMD *ver;
	int i;
	struct __attribute__((gcc_struct,packed)) {
		char nop;
		char op;
		int  disp;
	} call;

	cmd = (LPBYTE) GetModuleHandle(NULL);
	data = (LPDWORD) cmd;
	pDosHeader = (PIMAGE_DOS_HEADER) data;
	pNTHeader = MakeVA(PIMAGE_NT_HEADERS, pDosHeader->e_lfanew);
	end = MakeVA(LPDWORD, pNTHeader->OptionalHeader.SizeOfImage);
	cmd_end = end;
	pMyCall = MyCall;

	sfwork_map = kh_init(ptrdw);

	// Search the image for the ECHO & CALL help identifiers (to locate eCall),
	// and the binary file version.
	while (data < end) {
		if (eCall == NULL) {
			cmdentry = (struct sCmdEntry *) data;
			if (cmdentry->helpid == 0x2392 &&		// ECHO
				(cmdentry+14)->helpid == 0x238F) {	// CALL
				peCall = &cmdentry[14].func;
				eCall = cmdentry[14].func;
				WriteMemory(peCall, &pMyCall, sizeof(pMyCall));
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

	// Use eCall to get the address of LastRetCode.
	LPBYTE p = (LPBYTE) eCall;
#ifdef _WIN64
	p += cmdDebug ? 25 : 15;
	pLastRetCode = (int *) MKABS(p);
#else
	while (*p++ != 0xA3) ;
	pLastRetCode = *(int **) p;
#endif

	// This will be found because the loader finds it first.
	ver = cmd_versions;
	while (ver->verMS != cmdFileVersionMS ||
		   ver->verLS != cmdFileVersionLS ||
		   *(LPCWSTR)(cmd + *ver->offsets) != L';') {
		++ver;
	}
	for (i = 0; i < OFFSETS; ++i) {
		cmd_addrs[i] = cmd + ver->offsets[i];
	}

	cmd_printf = (fncmd_printf) pcmd_printf;

#ifdef _WIN64
	// CMD and the DLL could be more than 2GiB apart, so allocate some memory
	// before CMD to keep within the 32-bit relative address range.
	for (redirect = cmd - 0x1000;;) {
		LPVOID mem = VirtualAlloc(redirect, redirect_code_size,
								  MEM_COMMIT | MEM_RESERVE,
								  PAGE_EXECUTE_READWRITE);
		if (mem != NULL) {
			redirect = mem;
			break;
		} else {
			MEMORY_BASIC_INFORMATION mbi;
			VirtualQuery(redirect, &mbi, sizeof(mbi));
			redirect = (LPBYTE) mbi.AllocationBase - 0x1000;
		}
	}
	memcpy(redirect, redirect_code_start, redirect_code_size);
	memcpy(rPromptUserOrg, pCtrlCAborts, 7);
	if (pForFend[1] == 0x83) {	// 6.2.8102.0 uses jnc, not jc.
		--*rForFendj;			// jnc -> jc
	}
#endif

	oldPutMsg = *pPutStdErrMsg;
	oldParseFor = *pParseFortoken;
	oldParseForF = *pForFoptions;
	oldCallWorkresize = *pCallWorkresize;
	oldDisplayEnv = *pDisplayEnv;
	oldDisplayEnvVariable = *pDisplayEnvVariable;
	memcpy(oldCtrlCAborts, pCtrlCAborts, sizeof(oldCtrlCAborts));
	memcpy(oldLexText, pLexText, 5);
	memcpy(oldSFWorkmkstr, pSFWorkmkstr, 6);
	memcpy(oldSFWorkresize, pSFWorkresize, 6);
	memcpy(oldDESubWorkFreeStr, pDESubWorkFreeStr, 5);
	memcpy(oldDispatchAfterDE, pDispatchAfterDE, 6);
	memcpy(oldForFbegin, pForFbegin, 6);
	memcpy(oldForFend, pForFend, 6);
	memcpy(oldGotoEof, pGotoEof, 6);

	// Only check the first argument for help, so "call @ext /?" works.
	if (*pCheckHelpfirstarg == 0x0F) {
		WriteMemory(pCheckHelpfirstarg, "\x90\xE9", 2);
	} else {
		WriteByte(pCheckHelpfirstarg, 0xEB);
	}

#ifdef _WIN64
	call.nop = 0x40;	// dummy REX prefix
#else
	call.nop = 0x90;	// nop
#endif
	call.op = 0xE8; 	// call

	// Hook GOTO to more efficiently find a label.
#ifdef _WIN64
	Goto_pos = *pGotopos;				// register or stack
	Goto_start = *pGotostart + 8;		// stack, offset by call
	i = MKDISP(rGotoEof, pGotoEof+6);
	WriteMemory(pGotoEof+2, &i, 4);
#else
	if ((char) *pGotopos < 0) {
		Goto_pos = *(int *) pGotopos;	// ebp - int
		Goto_start = *(int *) pGotostart;
	} else {
		Goto_pos = *pGotopos + 8;		// esp + byte
		Goto_start = *pGotostart + 12;
	}
	if CMD_VERSION(6,2,8102,0) {
		++Goto_reg; 	// EDI
	} else if (/*CMD_VERSION(6,3,9431,0) &&*/ cmdDebug ||
			   CMD_VERSION(10,0,16299,15)) {
		--Goto_reg; 	// EBX
	}
	call.disp = MKDISP(GotoEof, pGotoEof+6);
	WriteMemory(pGotoEof, &call, 6);
#endif
	label_type['\f'] =
	label_type['\v'] = LABEL_WS;
	label_type['\t'] =
	label_type['\n'] =
	label_type['\r'] =
	label_type[' '] =
	label_type['='] =
	label_type[','] =
	label_type[';'] = LABEL_WS | LABEL_DELIM;
	label_type['\0'] =
	label_type['+'] =
	label_type[':'] =
	label_type['&'] =
	label_type['<'] =
	label_type['|'] =
	label_type['>'] = LABEL_DELIM;

	// Patch FOR to fix a substitute inefficiency: each loop has its own
	// memory.	Allocate once and reuse it.  Also free other memory allocated
	// during each loop.
	FreeStack = (DWORD_PTR) pFreeStack;
	SFWork_saved = *pSFWorksaved;
#ifdef _WIN64
	if (*pSFWorkmkstr == 0xFF) {
		SFWork_mkstr_reg = pSFWorkmkstr[0x47];
		call.disp = MKDISP(rSFWork_mkstr, pSFWorkmkstr+6);
		WriteMemory(pSFWorkmkstr, &call, 6);
	} else {
		i = MKDISP(rSFWork_mkstr, pSFWorkmkstr+5);
		WriteMemory(pSFWorkmkstr+1, &i, 4);
	}
	if (*pSFWorkresize == 0xFF) {
		WriteCode(pSFWorkresize, "\x4C\x89\xC0"		// mov rax,r8
								 "\x0F\x1F\x00");	// nop
		WriteCode(pSFWorkresize+18, "\x66\x90");    // nop
	} else {
		WriteCode(pSFWorkresize, "\x48\x89\xC8"		// mov rax,rcx
								 "\x66\x90");		// nop
	}
	if (pDESubWorkFreeStr[3] == 0x74) {
		DESubWork_FreeStr_org = pDESubWorkFreeStr[4];
		call.disp = MKDISP(rDESubWork_FreeStr_inline, pDESubWorkFreeStr+5);
		WriteMemory(pDESubWorkFreeStr, &call.op, 5);
	} else {
		DESubWork_FreeStr_org = MKABS(pDESubWorkFreeStr);
		i = MKDISP(rDESubWork_FreeStr, pDESubWorkFreeStr+4);
		WriteMemory(pDESubWorkFreeStr, &i, 4);
		if (CMD_VERSION(10,0,18362,1) || CMD_VERSION(10,0,18362,449)) {
			// DESubWork is inlined twice.
			LPBYTE p = pDESubWorkFreeStr - 0x3AE;
			memcpy(oldDESubWorkFreeStr1, p, 4);
			i = MKDISP(rDESubWork_FreeStr, p+4);
			WriteMemory(p, &i, 4);
			p -= 0xB0;
			memcpy(oldDESubWorkFreeStr2, p, 4);
			i = MKDISP(rDESubWork_FreeStr, p+4);
			WriteMemory(p, &i, 4);
		}
	}
	mkstr_size = 0x4004;
	mkstr_overhead = CMD_MAJOR_MINOR(>=, 6,0) ? 16 : 20;
#else
	SFWork_first = CMD_MAJOR_MINOR(>=, 6,2) ? 12 : 20;
	if (*pSFWorkmkstr == 0xE8) {
		if (pSFWorkmkstr[-5] == 0x68) {
			SFWork_stdcall = TRUE;
		}
		i = MKDISP(SFWork_mkstr, pSFWorkmkstr+5);
		WriteMemory(pSFWorkmkstr+1, &i, 4);
	} else {
		SFWork_mkstr_reg = pSFWorkmkstr[0x33];
		call.disp = MKDISP(SFWork_mkstr, pSFWorkmkstr+6);
		WriteMemory(pSFWorkmkstr, &call, 6);
	}
	if (*pSFWorkresize == 0xE8) {
		if (SFWork_stdcall) {
			WriteCode(pSFWorkresize, "\x58"				// pop eax
									 "\x59"				// pop ecx
									 "\x66\x66\x90");	// nop
		} else {
			WriteCode(pSFWorkresize, "\x89\xC8" 		// mov eax,ecx
									 "\x66\x66\x90");	// nop
		}
	} else {
		WriteCode(pSFWorkresize, "\x59"                 // pop ecx
								 "\x59"                 // pop ecx
								 "\x58"                 // pop eax
								 "\x59"                 // pop ecx
								 "\x66\x90");           // nop
		WriteCode(pSFWorkresize+19, "\x66\x90");        // nop
	}
	if CMD_MAJOR_MINOR(==, 6,2) {
		call.disp = MKDISP(DESubWork_FreeStr_inline, pDESubWorkFreeStr+5);
		WriteMemory(pDESubWorkFreeStr, &call.op, 5);
	} else {
		DESubWork_FreeStr_org = MKABS(pDESubWorkFreeStr);
		i = MKDISP(CMD_MAJOR_MINOR(<, 6,2) ? DESubWork_FreeStr_stdcall
										   : DESubWork_FreeStr_fastcall,
				   pDESubWorkFreeStr+4);
		WriteMemory(pDESubWorkFreeStr, &i, 4);
	}
	mkstr_size = CMD_MAJOR_MINOR(>, 5,0) ? 0x4004 : 0x2004;
	mkstr_overhead = CMD_MAJOR_MINOR(>=, 6,0) ? 8 : 12;
#endif
	mkstr_alloc = mkstr_size + mkstr_overhead;
	mkstr = (DWORD_PTR) pmkstr;

	// Patch FOR to fix a bug with wildcard expansion: each name accumulates,
	// resizing bigger and bigger (this patch is not undone on unload).
	// I've made the initial size big enough, no need to resize.
	WriteByte(pForResize, 0xEB);				// jmp
#ifdef _WIN64
	if CMD_MAJOR_MINOR(==, 5,2) {
		// 5.2.*.*
		WriteCode(pForResize, "\x40\xE9");		// jmp with dummy REX prefix
		WriteCode(pForMkstr,
				  "\x0F\x1F\x00"						// nop
				  "\x44\x8D\xA1\x00\x01\x00\x00");		// lea r12d,rcx+256
	} else if (CMD_MAJOR_MINOR(==, 6,0) || CMD_MAJOR_MINOR(==, 6,1)) {
		// 6.0.*.*
		// 6.1.*.*
		WriteCode(pForMkstr,
				  "\x0F\x1F\x00"						// nop
				  "\x44\x8D\xA9\x00\x01\x00\x00");		// lea r13d,rcx+256
	} else if CMD_MAJOR_MINOR(==, 6,2) {
		if CMD_BUILD_REVISION(==, 8102,0) {
			// 6.2.8102.0
			WriteCode(pForMkstr,
					  "\x90" 							// nop
					  "\x49\xFF\xC7"					// inc r15
					  "\x66\x42\x83\x3C\x79\x00"		// cmp word[rcx+r15*2],0
					  "\x75\xF5"						// jnz inc
					  "\x41\x81\xC7\x00\x01\x00\x00");	// add r15d,256
		} else {
			// 6.2.9200.16384
			WriteCode(pForMkstr,
					  "\x90" 							// nop
					  "\x48\xFF\xC5"					// inc rbp
					  "\x66\x83\x3C\x69\x00" 			// cmp word[rcx+rbp*2],0
					  "\x75\xF6"						// jnz inc
					  "\x81\xC5\x00\x01\x00\x00");		// add ebp,256
		}
	} else if (/*CMD_VERSION(6,3,9431,0) &&*/ cmdDebug) {
		// 6.3.9431.0u
		WriteCode(pForMkstr, "\x90"						// nop
							 "\x48\xFF\xC2"				// inc rdx
							 "\x66\x44\x39\x24\x51"		// cmp [rcx+rdx*2],r12w
							 "\x75\xF6"					// jnz inc
							 "\xFE\xC6"					// inc dh
							 "\x89\xD7");				// mov edi,edx
	} else if (CMD_MAJOR_MINOR(==, 10,0) && CMD_BUILD(>=, 17763)) {
		// 10.0.17763.1
		// 10.0.18362.*
		WriteCode(pForMkstr, "\x90" 					// nop
							 "\x48\xFF\xC2" 			// inc rdx
							 "\x66\x44\x39\x34\x51"		// cmp [rcx+rdx*2],r14w
							 "\x75\xF6" 				// jnz inc
							 "\xFE\xC6" 				// inc dh
							 "\x89\xD7");				// mov edi,edx
	} else {
		// 6.3.*.*
		// 10.0.*.*
		WriteCode(pForMkstr, "\x90"						// nop
							 "\x48\xFF\xC2"				// inc rdx
							 "\x66\x39\x2C\x51"			// cmp [rcx+rdx*2],bp
							 "\x75\xF7" 				// jnz inc
							 "\xFE\xC6" 				// inc dh
							 "\x89\xD7");				// mov edi,edx
	}
#else
	if CMD_MAJOR_MINOR(==, 5,0) {
		// 5.0.*.*
		WriteCode(pForMkstr, "\xEB\x00" 				// jmp inc
							 "\xFE\xC4");				// inc ah
	} else if CMD_MAJOR(==, 5) {
		if CMD_REVISION(==, 0) {
			// 5.1.2600.0
			// 5.2.3790.0
			WriteMemory(pForMkstr, pForMkstr+2, 8);
			WriteCode(pForMkstr+8, "\xFE\xC4");			// inc ah
		} else {
			// 5.1.*.*
			// 5.2.*.*
			WriteMemory(pForMkstr, pForMkstr+6, 8);
			WriteCode(pForMkstr+8, "\x81\xC0\x00\x01\x00\x00");  // add eax,256
		}
	} else if CMD_MAJOR_MINOR(==, 6,0) {
		// 6.0.*.*
		WriteCode(pForMkstr, "\xFE\xC4" 				// inc ah
							 "\x93"); 					// xchg ebx,eax
	} else if CMD_MAJOR_MINOR(==, 6,1) {
		// 6.1.*.*
		WriteCode(pForMkstr, "\xFE\xC4" 				// inc ah
							 "\x97"); 					// xchg edi,eax
	} else if CMD_MAJOR_MINOR(==, 6,2) {
		// 6.2.*.*
		WriteCode(pForMkstr, "\x85\xC0" 				// test eax,eax
							 "\x75\xF6" 				// jnz $-8
							 "\x2B\xD1" 				// sub edx,ecx
							 "\xD1\xFA" 				// sar edx,1
							 "\xFE\xC6" 				// inc dh
							 "\x89\xD3");				// mov ebx,edx
	} else {
		// 6.3.*.*
		// 10.0.*.*
		WriteCode(pForMkstr, "\xFE\xC6" 				// inc dh
							 "\x92"); 					// xchg edx,eax
	}
#endif

	// Hook PutStdErr to write the batch file name and line number.
	pPutMsg = (LPVOID) MKABS(pPutStdErrMsg);
#ifdef _WIN64
	i = MKDISP(rMyPutStdErrMsg, pPutStdErrMsg+1);
#else
	if CMD_MAJOR_MINOR(>, 6,2) {
		i = (DWORD) fastPutStdErrMsg;
	} else if CMD_MAJOR_MINOR(==, 6,2) {
		i = (DWORD) fastPutStdErrMsg62;
	} else {
		i = (DWORD) stdPutStdErrMsg;
	}
	i -= (DWORD) pPutStdErrMsg + 4;
#endif
	WriteMemory(pPutStdErrMsg, &i, 4);

	// Hook Lex text type to process Unicode characters.
#ifdef _WIN64
	call.disp = MKDISP(rMyLexText, pLexText+5);
	if (cmdDebug) {
		// Currently only the one debug version.
		/*if CMD_VERSION(6,3,9431,0)*/
		call.disp -= 5;
	}
#else
	if (cmdDebug) {
		// Currently only the one debug version.
		/*if CMD_VERSION(6,3,9431,0)*/
		call.disp = (DWORD) MyLexTextESI;
	} else {
		call.disp = (DWORD) MyLexText;
	}
	call.disp -= (DWORD) pLexText + 5;
#endif
	WriteMemory(pLexText, &call.op, 5);

	// Hook FOR /F to maintain a line number.
	ForFend_org = MKABS(pForFend+2);
#ifdef _WIN64
	// No need for original begin, it can never be true.
	call.disp = MKDISP(rForFbegin, pForFbegin+6);
	WriteMemory(pForFbegin, &call, 6);
	call.disp = MKDISP(rForFend, pForFend+6);
	WriteMemory(pForFend, &call, 6);
#else
	if (pForFbegin[1] == 0x83) {
		// No need for original begin, it can never be true.
		call.disp = MKDISP(ForFbegin_hook, pForFbegin+6);
		WriteMemory(pForFbegin, &call, 6);
		call.disp = (DWORD) ForFend;
	} else {
		// No need to return, it can never be false.
		ForFbegin_org = MKABS(pForFbegin+2);
		i = MKDISP(ForFbegin_jmp, pForFbegin+6);
		WriteMemory(pForFbegin+2, &i, 4);
		call.disp = (DWORD) ForFend_opp;
	}
	call.disp -= (DWORD) pForFend + 6;
	WriteMemory(pForFend, &call, 6);
#endif

	// Hook FOR to allow shorthand for infinite & range loops.
#ifdef _WIN64
	*rParseFor_org = MKABS(pParseFortoken);
	i = MKDISP(rParseFor, pParseFortoken+1);
#else
	ParseFor_org = MKABS(pParseFortoken);
	i = MKDISP(ParseFor_hook, pParseFortoken+1);
#endif
	WriteMemory(pParseFortoken, &i, 4);

	// Hook FOR /F to use "line" as shorthand for "delims= eol=".
#ifdef _WIN64
	*rParseForF_org = MKABS(pForFoptions);
	i = MKDISP(rParseForF, pForFoptions+1);
#else
	ParseForF_org = MKABS(pForFoptions);
	i = MKDISP(ParseForF_hook, pForFoptions+1);
#endif
	WriteMemory(pForFoptions, &i, 4);

	// Hook when CALL resizes the line containing doubled-up carets, in order
	// to remove incorrect doubles in strings.
#ifdef _WIN64
	*rCallWorkresize_org = MKABS(pCallWorkresize);
	i = MKDISP(rCallWorkresize, pCallWorkresize+1);
#else
	CallWorkresize_org = MKABS(pCallWorkresize);
	i = MKDISP(CMD_MAJOR_MINOR(<, 6,2) ? CallWorkresize_stdcall_hook :
			   CMD_MAJOR_MINOR(>, 6,2) ? CallWorkresize_fastcall_hook
									   : CallWorkresize_fastcall62_hook,
			   pCallWorkresize+1);
#endif
	WriteMemory(pCallWorkresize, &i, 4);

	// Hook DisplayEnv & DisplayEnvVariable to allow SET to display our heap
	// variables.
	DisplayEnv_org = MKABS(pDisplayEnv);
	i = MKDISP(DisplayEnv_hook, pDisplayEnv+1);
	WriteMemory(pDisplayEnv, &i, 4);
	DisplayEnvVariable_org = MKABS(pDisplayEnvVariable);
	i = MKDISP(DisplayEnvVariable_hook, pDisplayEnvVariable+1);
	WriteMemory(pDisplayEnvVariable, &i, 4);

	// Hook Dispatch after delayed expansion to possibly trace the command.
	DispatchAfterDE_org = MKABS(pDispatchAfterDE+2);
	call.disp = MKDISP(DispatchAfterDE_hook, pDispatchAfterDE+6);
	WriteMemory(pDispatchAfterDE, &call, 6);
#ifdef _WIN64
	if (cmdDebug) {
		// This version use jne, not je, patch our code to match.
		--*(LPBYTE) DispatchAfterDE_cc; 	// jne -> je
	}
#else
	if (CMD_MAJOR_MINOR(==, 5,0) || cmdDebug) {
		// These versions use jne, not je, patch our code to match.
		DWORD flOldProtect, flDummy;
		VirtualProtect(DispatchAfterDE_cc, 1, PAGE_READWRITE, &flOldProtect);
		--*(LPBYTE) DispatchAfterDE_cc; 	// jne -> je
		VirtualProtect(DispatchAfterDE_cc, 1, flOldProtect, &flDummy);
	}
	if (CMD_MAJOR_MINOR(==, 6,2) || CMD_VERSION(10,0,16299,15)) {
		// These versions use EDI, not ESI, patch our code to match.
		DWORD flOldProtect, flDummy;
		LPBYTE r = (LPBYTE) DispatchAfterDE_hook + 3;
		VirtualProtect(r, 1, PAGE_READWRITE, &flOldProtect);
		*r = 0xF9;
		VirtualProtect(r, 1, flOldProtect, &flDummy);
	}
#endif

	// Hook MyGetEnvVarPtr to recognise heap variables and extensions.	This
	// allows SET /A to use them directly.
#ifdef _WIN64
	if (CMD_MAJOR_MINOR(<, 6,2) || CMD_VERSION(6,2,8102,0) || cmdDebug) {
		MyGetEnvVarPtr_size = 7;
	} else if (CMD_MAJOR_MINOR(==, 10,0) && CMD_BUILD_REVISION(>=, 17763,1)) {
		MyGetEnvVarPtr_size = 8;
	} else {
		MyGetEnvVarPtr_size = 6;
	}
	memcpy(rMyGetEnvVarPtrOrg, pMyGetEnvVarPtr, MyGetEnvVarPtr_size);
	*rMyGetEnvVarPtr_org = (DWORD_PTR) pMyGetEnvVarPtr + MyGetEnvVarPtr_size;
	call.disp = MKDISP(rMyGetEnvVarPtr, pMyGetEnvVarPtr+5);
#else
	if (CMD_VERSION(5,0,2195,6656) || CMD_VERSION(5,0,2195,6995)) {
		MyGetEnvVarPtr_size = 6;
	} else if (CMD_VERSION(5,1,2600,0) || CMD_VERSION(5,2,3790,0)) {
		MyGetEnvVarPtr_size = 7;
	} else {
		MyGetEnvVarPtr_size = 5;
	}
	// Patch our own code to store the original bytes.
	DWORD flOldProtect, flDummy;
	VirtualProtect(MyGetEnvVarPtrOrg, MyGetEnvVarPtr_size, PAGE_READWRITE, &flOldProtect);
	memcpy(MyGetEnvVarPtrOrg, pMyGetEnvVarPtr, MyGetEnvVarPtr_size);
	VirtualProtect(MyGetEnvVarPtrOrg, MyGetEnvVarPtr_size, flOldProtect, &flDummy);
	MyGetEnvVarPtr_org = (DWORD) pMyGetEnvVarPtr + MyGetEnvVarPtr_size;
	call.disp = MKDISP(CMD_MAJOR_MINOR(<, 6,2) ? MyGetEnvVarPtr_stdcall_hook :
					   CMD_MAJOR_MINOR(>, 6,2) ? MyGetEnvVarPtr_fastcall_hook
											   : MyGetEnvVarPtr_fastcall62_hook,
					   pMyGetEnvVarPtr+5);
#endif
	call.op = 0xE9; 	// jmp
	WriteMemory(pMyGetEnvVarPtr, &call.op, 5);
}

void hookCtrlCAborts(char aborts)
{
	if (aborts == -1) {
		WriteMemory(pCtrlCAborts, oldCtrlCAborts, sizeof(oldCtrlCAborts));
	} else {
#ifdef _WIN64
		char code[7];
		code[0] = 0xE8; 		// call
		*(int *)(code+1) = MKDISP(rPromptUser, pCtrlCAborts+5);
		code[5] = 0xC3; 		// ret
		code[6] = 0x90; 		// nop
		*rAbortFlag = aborts;
#else
		char code[5];
		if CMD_MAJOR_MINOR(<, 6,3) {
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
	WriteMemory(pPutStdErrMsg, &oldPutMsg, 4);
	WriteMemory(pLexText, oldLexText, 5);
	if (*pCheckHelpfirstarg == 0x90) {
		WriteMemory(pCheckHelpfirstarg, "\x0F\x84", 2);
	} else {
		WriteByte(pCheckHelpfirstarg, 0x74);
	}
	WriteMemory(pCtrlCAborts, oldCtrlCAborts, sizeof(oldCtrlCAborts));
	WriteMemory(pSFWorkmkstr, oldSFWorkmkstr, 6);
	WriteMemory(pSFWorkresize, oldSFWorkresize, 6);
	WriteMemory(pDESubWorkFreeStr, oldDESubWorkFreeStr, 5);
	WriteMemory(pDispatchAfterDE, oldDispatchAfterDE, 6);
	WriteMemory(pForFbegin, oldForFbegin, 6);
	WriteMemory(pForFend, oldForFend, 6);
	WriteMemory(pGotoEof, oldGotoEof, 6);
	WriteMemory(pParseFortoken, &oldParseFor, 4);
	WriteMemory(pForFoptions, &oldParseForF, 4);
	WriteMemory(pCallWorkresize, &oldCallWorkresize, 4);
#ifdef _WIN64
	if (CMD_VERSION(10,0,18362,1) || CMD_VERSION(10,0,18362,449)) {
		LPBYTE p = pDESubWorkFreeStr - 0x3AE;
		WriteMemory(p, oldDESubWorkFreeStr1, 4);
		p -= 0xB0;
		WriteMemory(p, oldDESubWorkFreeStr2, 4);
	}
	WriteMemory(pMyGetEnvVarPtr, rMyGetEnvVarPtrOrg, MyGetEnvVarPtr_size);
#else
	WriteMemory(pMyGetEnvVarPtr, MyGetEnvVarPtrOrg, MyGetEnvVarPtr_size);
#endif
	WriteMemory(pDisplayEnv, &oldDisplayEnv, 4);
	WriteMemory(pDisplayEnvVariable, &oldDisplayEnvVariable, 4);

	*pfDumpTokens = 0;
	*pfDumpParse = 0;

#ifdef _WIN64
	VirtualFree(redirect, 0, MEM_RELEASE);
#endif
}

void doneCmdBat(void)
{
	kh_clear(ptrdw, sfwork_map);
	trace = 0;
}
