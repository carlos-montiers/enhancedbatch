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

// Instead of importing a function, make a local call to it here.  The first
// call jumps to an initialisation routine, which loads the library and imports
// the functions.  Subsequent calls jump straight to the import.
asm(
	".macro delay func stack\n"
#ifdef _WIN64
	".globl \\func\n"
	"\\func:\n"
	"jmp *p\\func(%rip)\n"
#else
	".globl _\\func\\()@\\stack\n"      // stdcall functions are suffixed
	"_\\func\\()@\\stack:\n"            //  with @ and the number of bytes
	"jmp *_p\\func\n"                   //  pushed on the stack
#endif
	".endm\n"
);

#define IMPORT(name, stack) \
	asm("delay " #name " " #stack "\n")

#define INIT_IMPORT(name, stack, type, params) \
	type WINAPI imp ## name params;\
	type (WINAPI * p ## name)params = imp ## name;\
	IMPORT(name, stack)

#define DECL_IMPORT(name, stack) \
	LPVOID p ## name;\
	IMPORT(name, stack)

HMODULE ole32;

// This is always called first, so it can load them all.
INIT_IMPORT(CoInitializeEx, 8, HRESULT, (LPVOID, DWORD));
DECL_IMPORT(CoCreateInstance, 20);
DECL_IMPORT(CoTaskMemFree, 4);
DECL_IMPORT(CoUninitialize, 0);

STDAPI impCoInitializeEx(LPVOID pvReserved, DWORD dwCoInit)
{
	ole32 = LoadLibrary(L"ole32.dll");
	GETPROC(ole32, CoCreateInstance);
	GETPROC(ole32, CoTaskMemFree);
	GETPROC(ole32, CoUninitialize);
	GETPROC(ole32, CoInitializeEx);
	return pCoInitializeEx(pvReserved, dwCoInit);
}

void unload_delayed(void)
{
	if (ole32 != NULL) {
		FreeLibrary(ole32);
	}
}
