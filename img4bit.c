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
#include "spng.h"
#include <inttypes.h>

BOOL Img4Bit(int argc, LPCWSTR argv[]) {
	int r = 0;
    FILE *png;
    char *pngbuf = NULL;
    spng_ctx *ctx = NULL;
    unsigned char *out = NULL;

    if(argc < 1)
    {
        wprintf(L"no input file\n");
        goto err;
    }

    png = _wfopen(argv[0], L"rb");
    if(png == NULL)
    {
        wprintf(L"error opening input file %s\n", argv[0]);
        goto err;
    }

    fseek(png, 0, SEEK_END);

    long siz_pngbuf = ftell(png);
    rewind(png);

    if(siz_pngbuf < 1) goto err;

    pngbuf = malloc(siz_pngbuf);
    if(pngbuf == NULL)
    {
        wprintf(L"malloc() failed\n");
        goto err;
    }

    if(fread(pngbuf, siz_pngbuf, 1, png) != 1)
    {
        wprintf(L"fread() failed\n");
        goto err;
    }

    ctx = spng_ctx_new(0);
    if(ctx == NULL)
    {
        wprintf(L"spng_ctx_new() failed\n");
        goto err;
    }

    r = spng_set_crc_action(ctx, SPNG_CRC_USE, SPNG_CRC_USE);

    if(r)
    {
        wprintf(L"spng_set_crc_action() error: %s\n", spng_strerror(r));
        goto err;
    }

    r = spng_set_png_buffer(ctx, pngbuf, siz_pngbuf);

    if(r)
    {
        wprintf(L"spng_set_png_buffer() error: %s\n", spng_strerror(r));
        goto err;
    }

    struct spng_ihdr ihdr;
    r = spng_get_ihdr(ctx, &ihdr);

    if(r)
    {
        wprintf(L"spng_get_ihdr() error: %s\n", spng_strerror(r));
        goto err;
    }

    WCHAR *clr_type_str;
    if(ihdr.color_type == SPNG_COLOR_TYPE_GRAYSCALE)
        clr_type_str = L"grayscale";
    else if(ihdr.color_type == SPNG_COLOR_TYPE_TRUECOLOR)
        clr_type_str = L"truecolor";
    else if(ihdr.color_type == SPNG_COLOR_TYPE_INDEXED)
        clr_type_str = L"indexed color";
    else if(ihdr.color_type == SPNG_COLOR_TYPE_GRAYSCALE_ALPHA)
        clr_type_str = L"grayscale with alpha";
    else
        clr_type_str = L"truecolor with alpha";

    wprintf(L"width: %" PRIu32 L"\nheight: %" PRIu32 L"\n"
           L"bit depth: %" PRIu8 "\ncolor type: %" PRIu8 L" - %s\n",
           ihdr.width, ihdr.height,
           ihdr.bit_depth, ihdr.color_type, clr_type_str);
    wprintf(L"compression method: %" PRIu8 L"\nfilter method: %" PRIu8 L"\n"
           L"interlace method: %" PRIu8 L"\n",
           ihdr.compression_method, ihdr.filter_method,
           ihdr.interlace_method);

    size_t out_size;

    r = spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &out_size);

    if(r) goto err;

    out = malloc(out_size);
    if(out == NULL) goto err;

    r = spng_decode_image(ctx, out, out_size, SPNG_FMT_RGBA8, 0);

    if(r)
    {
        wprintf(L"spng_decode_image() error: %s\n", spng_strerror(r));
        goto err;
    }

err:
    spng_ctx_free(ctx);

    free(out);
    free(pngbuf);

	return r;
}
