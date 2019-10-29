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
#include "upng.h"
#include <inttypes.h>

BOOL Img4Bit(int argc, LPCWSTR argv[]) {
	upng_t* upng;

    if(argc < 1) {
        wprintf(L"no input file\n");
        return FALSE;
    }

	upng = upng_new_from_file(argv[0]);
	if (upng == NULL) {
		wprintf(L"error: no memory\n");
		return FALSE;
	}
	upng_decode(upng);
	if (upng_get_error(upng) != UPNG_EOK) {
		wprintf(L"error: %d %u\n", upng_get_error(upng), upng_get_error_line(upng));
		return FALSE;
	}

	wprintf(L"dimensions: %u x %u\n", upng_get_width(upng), upng_get_height(upng));
	wprintf(L"depth: %u\n", upng_get_bpp(upng));

	upng_free(upng);

	return TRUE;
}
