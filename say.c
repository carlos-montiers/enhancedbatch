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
#include <sapi.h>
#include <initguid.h>

#define vcall(this, fn, args...) this->lpVtbl->fn(this, ## args)

DEFINE_GUID(CLSID_SpVoice, 0x96749377, 0x3391, 0x11d2, 0x9e,0xe3, 0x00,0xc0,0x4f,0x79,0x73,0x96);
DEFINE_GUID(IID_ISpVoice, 0x6c44df74, 0x72b9, 0x4992, 0xa1,0xec, 0xef,0x99,0x6e,0x04,0x22,0xd4);
DEFINE_GUID(CLSID_SpObjectTokenCategory, 0xa910187f, 0x0c7a, 0x45ac, 0x92,0xcc, 0x59,0xed,0xaf,0xb7,0x7b,0x53);
DEFINE_GUID(IID_ISpObjectTokenCategory, 0x2d3d3845, 0x39af, 0x4850, 0xbb,0xf9, 0x40,0xb4,0x97,0x80,0x01,0x1d);

#define SPCAT_CORE_VOICES L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Speech_OneCore\\Voices"

ISpObjectToken* pDefaultVoice;
ISpObjectToken* pCurrentVoice;
SPEAKFLAGS spFlags;
LPWSTR text;
LPCWSTR fmt;

ISpObjectToken* selectVoice(LPCWSTR voice)
{
	ISpObjectTokenCategory* pCategory = NULL;
	IEnumSpObjectTokens* pEnum = NULL;
	ISpObjectToken* pToken = NULL;

	if FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)) {
		return NULL;
	}
	if SUCCEEDED(CoCreateInstance(&CLSID_SpObjectTokenCategory, NULL, CLSCTX_ALL,
								  &IID_ISpObjectTokenCategory, (void**)&pCategory)) {
		if ((SUCCEEDED(vcall(pCategory, SetId, SPCAT_CORE_VOICES, FALSE))
			 || SUCCEEDED(vcall(pCategory, SetId, SPCAT_VOICES, FALSE)))
			&& SUCCEEDED(vcall(pCategory, EnumTokens, NULL, NULL, &pEnum))) {
			LPWSTR name, v;
			if (voice) {
				v = _wcsdup(voice);
				_wcslwr(v);
			} else {
				v = NULL;
			}
			ULONG i, count;
			vcall(pEnum, GetCount, &count);
			for (i = 0; i < count; ++i) {
				vcall(pEnum, Item, i, &pToken);
				vcall(pToken, GetStringValue, NULL, &name);
				if (v == NULL) {
					cmd_printf(L"%s\r\n", name);
				} else {
					_wcslwr(name);
					if (wcsstr(name, v) != NULL) {
						CoTaskMemFree(name);
						break;
					}
				}
				CoTaskMemFree(name);
				name = NULL;
				vcall(pToken, Release);
				pToken = NULL;
			}
			if (v) {
				free(v);
			}
			vcall(pEnum, Release);
		}
		vcall(pCategory, Release);
	}

	CoUninitialize();

	return pToken;
}

BOOL SetVoice(int argc, LPCWSTR argv[])
{
	if (argc == 0) {
		if (pDefaultVoice) {
			vcall(pDefaultVoice, Release);
			pDefaultVoice = NULL;
		}
		return TRUE;
	}

	if (argc != 1) {
		return FALSE;
	}

	pDefaultVoice = selectVoice(argv[0]);
	return pDefaultVoice != NULL;
}

LPWSTR stripTags(LPCWSTR tagged)
{
	LPWSTR tagless = _wcsdup(tagged), p = tagless;
	while (*p != L'\0') {
		if (*p == L'<') {
			LPWSTR tag = p;
			// SAPI does not recognise '>' within values.
			while (*tag != L'>' && *tag != L'\0') {
				++tag;
			}
			if (*tag != L'\0') {
				if (p != tagless && p[-1] == L' ' && tag[1] == L' ') {
					++tag;
				}
				wcscpy(p, tag+1);
			}
		} else {
			++p;
		}
	}
	return tagless;
}

DWORD WINAPI SayThread(LPVOID unused)
{
	ISpVoice* pVoice = NULL;
	DWORD said = 0;

	if SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)) {
		if SUCCEEDED(CoCreateInstance(&CLSID_SpVoice, NULL, CLSCTX_ALL,
									  &IID_ISpVoice, (void**)&pVoice)) {
			if (pCurrentVoice) {
				vcall(pVoice, SetVoice, pCurrentVoice);
			} else if (pDefaultVoice) {
				vcall(pVoice, SetVoice, pDefaultVoice);
			}
			said = SUCCEEDED(vcall(pVoice, Speak, text, spFlags, NULL));
			vcall(pVoice, Release);
		}
		CoUninitialize();
	}

	if (pCurrentVoice) {
		vcall(pCurrentVoice, Release);
		pCurrentVoice = NULL;
	}
	free(text);
	text = NULL;

	SetEvent(hSpeaking);

	return said;
}

BOOL Say(int argc, LPCWSTR argv[])
{
	if (argc == 0) {
		return FALSE;
	}

	WaitForSingleObject(hSpeaking, INFINITE);

	spFlags = SPF_DEFAULT;
	fmt = L"%s\r\n";
	BOOL wait = FALSE;
	LPCWSTR voice = NULL;

	int i;
	for (i = 0; i < argc; ++i) {
		if (*argv[i] != L'/') {
			break;
		}
		if (WCSIEQ(argv[i], L"/v")) {
			if (i == argc-1) {
				selectVoice(NULL);
				return TRUE;
			}
			voice = argv[++i];
		} else if (WCSIEQ(argv[i], L"/n")) {
			fmt = L"%s";
		} else if (WCSIEQ(argv[i], L"/s")) {
			fmt = NULL;
		} else if (WCSIEQ(argv[i], L"/u")) {
			fmt = L"%s\n";
		} else if (WCSIEQ(argv[i], L"/w")) {
			wait = TRUE;
		} else if (WCSIEQ(argv[i], L"/x")) {
			spFlags |= SPF_IS_XML;
		}
	}
	if (i == argc) {
		return FALSE;
	}
	if (!(spFlags & SPF_IS_XML)) {
		spFlags |= SPF_IS_NOT_XML;
	}

	if (i == argc-1) {
		text = _wcsdup(argv[i]);
		if (text == NULL) {
			return FALSE;
		}
	} else {
		int j, len = 1;
		for (j = i; j < argc; ++j) {
			len += wcslen(argv[j]) + 1;
		}
		text = malloc(WSZ(len));
		if (text == NULL) {
			return FALSE;
		}
		LPWSTR p = text;
		for (j = i; j < argc; ++j) {
			p += snwprintf(p, len, L"%s ", argv[j]);
		}
		p[-1] = L'\0';
	}

	if (fmt != NULL) {
		if (spFlags & SPF_IS_XML) {
			LPWSTR tagless = stripTags(text);
			cmd_printf(fmt, tagless);
			free(tagless);
		} else {
			cmd_printf(fmt, text);
		}
	}
	if (voice) {
		pCurrentVoice = selectVoice(voice);
	}

	ResetEvent(hSpeaking);
	if (wait) {
		return SayThread(NULL);
	}
	SafeCloseHandle(CreateThread(NULL, 4096, SayThread, NULL, 0, NULL));
	return TRUE;
}
