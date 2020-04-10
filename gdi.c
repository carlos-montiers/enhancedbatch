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
#include <wtypes.h>
#include <gdiplus.h>
#define _SHLWAPI_		// declare non-imported functions, for my delay code
#include <shlwapi.h>

#include <initguid.h>
DEFINE_GUID(FrameDimensionTime, 0x6aedbd6d,0x3fb5,0x418a,0x83,0xa6,0x7f,0x45,0x22,0x9d,0xc8,0x72);
DEFINE_GUID(FrameDimensionPage, 0x7462dc86,0x6180,0x4c7e,0x8e,0x3f,0xee,0x73,0x33,0xa7,0xa4,0x83);


BOOL WINAPI impGetCurrentConsoleFontEx(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);

BOOL (WINAPI *pGetCurrentConsoleFontEx)(HANDLE, BOOL, PCONSOLE_FONT_INFOEX)
				= impGetCurrentConsoleFontEx;
BOOL (WINAPI *pGetConsoleScreenBufferInfoEx)(HANDLE, PCONSOLE_SCREEN_BUFFER_INFOEX);

BOOL WINAPI myGetCurrentConsoleFontEx(HANDLE, BOOL, PCONSOLE_FONT_INFOEX);
BOOL WINAPI myGetConsoleScreenBufferInfoEx(HANDLE, PCONSOLE_SCREEN_BUFFER_INFOEX);

BOOL WINAPI impGetCurrentConsoleFontEx(HANDLE a, BOOL b, PCONSOLE_FONT_INFOEX c)
{
	HMODULE kernel32 = GetModuleHandle(L"kernel32.dll");
	GETPROC(kernel32, GetConsoleScreenBufferInfoEx);
	GETPROC(kernel32, GetCurrentConsoleFontEx);
	if (pGetCurrentConsoleFontEx == NULL) {
		pGetConsoleScreenBufferInfoEx = myGetConsoleScreenBufferInfoEx;
		pGetCurrentConsoleFontEx = myGetCurrentConsoleFontEx;
	}

	return pGetCurrentConsoleFontEx(a, b, c);
}


int CallText(int argc, LPCWSTR argv[])
{
	CONSOLE_SCREEN_BUFFER_INFOEX csbix;
	CONSOLE_FONT_INFOEX cfi;
	HDC dc;
	HFONT font, old_font;

	if (argc == 0 || onWindowsTerminal
		|| !haveOutputHandle() || !haveWindowHandle()) {
		return EXIT_FAILURE;
	}

	cfi.cbSize = sizeof(cfi);
	csbix.cbSize = sizeof(csbix);
	if (!pGetCurrentConsoleFontEx(consoleOutput, FALSE, &cfi)
		|| cfi.dwFontSize.X == 0
		|| !pGetConsoleScreenBufferInfoEx(consoleOutput, &csbix)) {
		return EXIT_FAILURE;
	}

	COLORREF fg = csbix.ColorTable[csbix.wAttributes & 15];
	COLORREF bg = csbix.ColorTable[(BYTE) csbix.wAttributes >> 4];
	int x = csbix.dwCursorPosition.X - csbix.srWindow.Left;
	int y = csbix.dwCursorPosition.Y - csbix.srWindow.Top;
	int half_x = 0;
	int half_y = 0;
	LPCWSTR face = cfi.FaceName;
	DWORD quality = DEFAULT_QUALITY;
	int escapement = 0;
	int orientation = 0;
	int scale_width = 1;
	int scale_height = 1;
	BOOL transparent = FALSE;
	DWORD weight = FW_NORMAL;
	DWORD underline = 0;
	DWORD italic = 0;
	DWORD strikeout = 0;
	BOOL vertical = FALSE;
	int rotation = 0;
	LPWSTR text;

	COLORREF parseColor(LPCWSTR color)
	{
		COLORREF cr = (COLORREF) wcstoul(color, NULL, 16);
		int len = wcslen(color);
		// One digit is the normal index.
		if (len == 1) {
			return csbix.ColorTable[cr];
		}
		// Two digits may be a decimal index.
		if (len == 2 && cr >= 0x10 && cr <= 0x15) {
			return csbix.ColorTable[cr-0x10+10];
		}
		// Expand three digits: RGB -> RRGGBB.
		if (len == 3) {
			BYTE r = cr >> 8;
			BYTE g = (BYTE) cr >> 4;
			BYTE b = cr & 0xF;
			return RGB(r | r << 4, g | g << 4, b | b << 4);
		}
		// Swap R & B.
		return RGB(GetBValue(cr), GetGValue(cr), GetRValue(cr));
	}

	int i;
	for (i = 0; i < argc; ++i) {
		if (*argv[i] != L'/') {
			break;
		}
		if (WCSIEQ(argv[i], L"/n")) {
			quality = NONANTIALIASED_QUALITY;
		} else if (WCSIEQ(argv[i], L"/rl")) {
			rotation = 900;
		} else if (WCSIEQ(argv[i], L"/rr")) {
			rotation = 2700;	// -900 doesn't work with some fonts
		} else if (WCSIEQ(argv[i], L"/rt")) {
			rotation = 1800;
		} else if (WCSIEQ(argv[i], L"/d")) {
			scale_width = scale_height = 2;
		} else if (WCSIEQ(argv[i], L"/dw")) {
			scale_width = 2;
		} else if (WCSIEQ(argv[i], L"/dh")) {
			scale_height = 2;
		} else if (WCSIEQ(argv[i], L"/t")) {
			transparent = TRUE;
		} else if (WCSIEQ(argv[i], L"/b")) {
			weight = FW_BOLD;
		} else if (WCSIEQ(argv[i], L"/i")) {
			italic = TRUE;
		} else if (WCSIEQ(argv[i], L"/u")) {
			underline = TRUE;
		} else if (WCSIEQ(argv[i], L"/s")) {
			strikeout = TRUE;
		} else if (WCSIEQ(argv[i], L"/v")) {
			vertical = TRUE;
		} else {
			if (i == argc-1) {
				return EXIT_FAILURE;
			}
			if (WCSIEQ(argv[i], L"/fg")) {
				fg = parseColor(argv[++i]);
			} else if (WCSIEQ(argv[i], L"/bg")) {
				bg = parseColor(argv[++i]);
			} else if (WCSIEQ(argv[i], L"/f")) {
				face = argv[++i];
			} else if (WCSIEQ(argv[i], L"/e")) {
				escapement = (int) wcstol(argv[++i], NULL, 10);
				if (escapement < 0) {
					escapement += 3600;
				}
			} else if (WCSIEQ(argv[i], L"/o")) {
				orientation = (int) wcstol(argv[++i], NULL, 10);
				if (orientation < 0) {
					orientation += 3600;
				}
			} else if (WCSIEQ(argv[i], L"/c") ||
					   WCSIEQ(argv[i], L"/ch")) {
				if (argv[i][2] != L'\0') {
					half_x = cfi.dwFontSize.X / 2;
				}
				x = (int) wcstol(argv[++i], NULL, 10);
			} else if (WCSIEQ(argv[i], L"/r") ||
					   WCSIEQ(argv[i], L"/rh")) {
				if (argv[i][2] != L'\0') {
					half_y = cfi.dwFontSize.Y / 2;
				}
				y = (int) wcstol(argv[++i], NULL, 10);
			} else if (WCSIEQ(argv[i], L"/p") ||
					   WCSIEQ(argv[i], L"/ph")) {
				if (argv[i][2] != L'\0') {
					half_x = cfi.dwFontSize.X / 2;
					half_y = cfi.dwFontSize.Y / 2;
				}
				y = (int) wcstol(argv[++i], NULL, 10);
				if (i == argc-1) {
					return EXIT_FAILURE;
				}
				x = (int) wcstol(argv[++i], NULL, 10);
			}
		}
	}
	if (i == argc) {
		return EXIT_FAILURE;
	}

	if (vertical || rotation != 0) {
		if (vertical) {
			orientation = rotation;
			escapement = rotation == 0 ? 2700 : rotation - 900;
		} else {
			escapement = orientation = rotation;
		}
	}
	if (scale_width == scale_height) {
		scale_width = 0;
	}
	font = CreateFont(cfi.dwFontSize.Y * scale_height,
					  cfi.dwFontSize.X * scale_width,
					  escapement, orientation,
					  weight, italic, underline, strikeout,
					  DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					  quality, DEFAULT_PITCH | FF_DONTCARE, face );
	if (font == NULL) {
		return EXIT_FAILURE;
	}

	if (i == argc-1) {
		text = (LPWSTR) argv[i];
	} else {
		text = stringBuffer;
		for (; i < argc; ++i) {
			text += sbprintf(text, L"%s ", argv[i]);
		}
		text[-1] = L'\0';
		text = stringBuffer;
	}

	if (escapement != orientation && wcscmp(face, L"Terminal") != 0) {
		// TabbedTextOut squishes characters together, so use TextOut.
		// However, that has insufficient spacing, so quadruple the spaces,
		// but not if it's a raster font (sheesh!).
		LPWSTR p = varBuffer;
		while (*text != L'\0') {
			if (*text == L' ' || *text == L'\t') {
				*p++ = L' ';
				*p++ = L' ';
				*p++ = L' ';
			}
			*p++ = *text++;
			if (p == varBuffer + STRINGBUFFERMAX - 1) {
				break;
			}
		}
		*p = L'\0';
		text = varBuffer;
	}

	dc = GetDC(consoleHwnd);
	SetGraphicsMode(dc, GM_ADVANCED);
	SetBkColor(dc, bg);
	SetTextColor(dc, fg);
	old_font = (HFONT) SelectObject(dc, font);
	x *= cfi.dwFontSize.X;
	y *= cfi.dwFontSize.Y;
	if (escapement != 0 || orientation != 0) {
		SetTextAlign(dc, TA_LEFT | TA_BOTTOM | TA_NOUPDATECP);
		if (rotation == 0 && !vertical) {
			y += cfi.dwFontSize.Y - 1 - half_y;
			x += half_x;
			half_x = half_y = 0;
		}
	}
	if (half_x != 0 || half_y != 0) {
		switch (rotation) {
		case 0:
			x += half_x;
			y += half_y;
			break;
		case 900:
			x += half_x;
			y -= half_y;
			break;
		case 2700:
			x -= half_x;
			y += half_y;
			break;
		case 1800:
			x -= half_x;
			y -= half_y;
			break;
		}
	}

	int len = wcslen(text);
	RECT r;

	void getBBox(LPRECT r)
	{
		BeginPath(dc);
		TextOut(dc, x, y, text, len);
		EndPath(dc);
		HRGN rgn = PathToRegion(dc);
		if (rgn != NULL) {
			GetRgnBox(rgn, r);
			DeleteObject(rgn);
			return;
		}
		// Probably a raster font, try normal extent.
		SIZE sz;
		GetTextExtentPoint32(dc, text, len, &sz);
		if (escapement != orientation) {
			// Draw to memory and scan for a color change.
			HDC mdc = CreateCompatibleDC(dc);
			SetGraphicsMode(mdc, GM_ADVANCED);
			//SetBkColor(mdc, RGB(255,255,255));	// default
			SetTextColor(mdc, RGB(255,255,255));	// for this purpose text is bg
			HGDIOBJ old_mfont = SelectObject(mdc, font);
			SetTextAlign(mdc, TA_LEFT | TA_BOTTOM | TA_NOUPDATECP);
			if (orientation == 900 || orientation == 2700) {
				LONG t = sz.cx;
				sz.cx = sz.cy;
				sz.cy = t;
			}
			int w = sz.cx * 2;
			int h = sz.cy * 4;
			int ox = w / 2;
			int oy = h / 2;
			int dx, dy = 0;
			HBITMAP mbm = CreateCompatibleBitmap(dc, w, h);
			HGDIOBJ old_mbm = SelectObject(mdc, mbm);
			TextOut(mdc, ox, oy, text, len);
			// This is still a rectangle, as other angles use a different font.
			for (dx = 0; dx < w; ++dx) {
				for (dy = 0; dy < oy; ++dy) {
					if (GetPixel(mdc, dx, dy) == RGB(255,255,255)) {
						goto done;
					}
				}
			}
			done:
			DeleteObject(SelectObject(mdc, old_mbm));
			SelectObject(mdc, old_mfont);
			DeleteDC(mdc);
			r->left = x + dx - ox;
			r->top = y + dy - oy;
			r->right = r->left + sz.cx;
			r->bottom = r->top + sz.cy;
		} else {
			if (escapement == 0) {
				r->left = x;
				r->right = x + sz.cx;
				r->top = y;
				r->bottom = y + sz.cy;
			} else if (escapement == 900) {
				r->left = x - sz.cy;
				r->right = x;
				r->top = y+1 - sz.cx;
				r->bottom = y+1;
			} else if (escapement == 2700) {
				r->left = x;
				r->right = x + sz.cy;
				r->top = y;
				r->bottom = y + sz.cx;
			} else if (escapement == 1800) {
				r->left = x+1 - sz.cx;
				r->right = x+1;
				r->top = y;
				r->bottom = y + sz.cy;
			}
		}
	}

	if (vertical) {
		// Different characters move by different amounts, so use a path to get
		// the bounding box and determine the offsets.
		getBBox(&r);
		x += x - r.left;
		y += y - r.top;
		if (rotation == 900 || rotation == 1800) {
			y -= r.bottom - r.top - cfi.dwFontSize.Y;
		}
		if (rotation == 2700 || rotation == 1800) {
			x -= r.right - r.left - cfi.dwFontSize.X;
		}
	} else if (rotation != 0) {
		SIZE sz;
		GetTextExtentPoint32(dc, text, len, &sz);
		if (rotation == 900) {
			x += sz.cy;
			y += cfi.dwFontSize.Y - 1;
		} else if (rotation == 2700) {
			x -= sz.cy - cfi.dwFontSize.X;
		} else if (rotation == 1800) {
			x += cfi.dwFontSize.X - 1;
			y -= sz.cy - cfi.dwFontSize.Y;
		}
	}
	// Clip it to what it says it is, otherwise ClearType draws extra.
	getBBox(&r);
	SelectClipRgn(dc, CreateRectRgnIndirect(&r));
	if (transparent) {
		SetBkMode(dc, TRANSPARENT);
	}
	TextOut(dc, x, y, text, len);
	DeleteObject(SelectObject(dc, old_font));
	ReleaseDC(consoleHwnd, dc);

	return EXIT_SUCCESS;
}


#define MAX_BG 4
struct sBg {
	HBITMAP bitmap;
	int x, y;
	WCHAR name[MAX_PATH];
} BgCache[MAX_BG];


int CallImage(int argc, LPCWSTR argv[])
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	CONSOLE_FONT_INFOEX cfi;

	if (argc == 0 || onWindowsTerminal
		|| !haveOutputHandle() || !haveWindowHandle()) {
		return 0;
	}

	cfi.cbSize = sizeof(cfi);
	if (!pGetCurrentConsoleFontEx(consoleOutput, FALSE, &cfi)
		|| cfi.dwFontSize.X == 0
		|| !GetConsoleScreenBufferInfo(consoleOutput, &csbi)) {
		return 0;
	}

	int x = csbi.dwCursorPosition.X - csbi.srWindow.Left;
	int y = csbi.dwCursorPosition.Y - csbi.srWindow.Top;
	int half_x = 0;
	int half_y = 0;
	int sx = 0, sy = 0, sw = 0, sh = 0;
	int fcw = 0, fch = 0, fsw = 0, fsh = 0;
	int fl = 0, fr = 0, ft = 0, fb = 0;
	int fpc = -1, fpr = -1;
	LPCWSTR file = NULL;
	int frame = 0;
	BOOL quiet = FALSE, ret_frames = FALSE;
	BOOL copy = FALSE, discard = FALSE, restore = FALSE;

	int i;
	for (i = 0; i < argc; ++i) {
		if (*argv[i] != L'/') {
			if (file != NULL) {
				return 0;
			}
			file = argv[i];
		} else if (WCSIEQ(argv[i], L"/copy")) {
			copy = TRUE;
		} else if (WCSIEQ(argv[i], L"/discard")) {
			discard = TRUE;
		} else if (WCSIEQ(argv[i], L"/restore")) {
			discard = restore = TRUE;
		} else if (WCSIEQ(argv[i], L"/n")) {
			ret_frames = quiet = TRUE;
		} else if (WCSIEQ(argv[i], L"/q")) {
			quiet = TRUE;
		} else {
			if (i == argc-1) {
				return 0;
			}
			if (WCSIEQ(argv[i], L"/c")
				|| WCSIEQ(argv[i], L"/ch")) {
				if (argv[i][2] != L'\0') {
					half_x = cfi.dwFontSize.X / 2;
				}
				x = (int) wcstol(argv[++i], NULL, 10);
			} else if (WCSIEQ(argv[i], L"/r") ||
					   WCSIEQ(argv[i], L"/rh")) {
				if (argv[i][2] != L'\0') {
					half_y = cfi.dwFontSize.Y / 2;
				}
				y = (int) wcstol(argv[++i], NULL, 10);
			} else if (WCSIEQ(argv[i], L"/f")) {
				frame = (int) wcstol(argv[++i], NULL, 10);
			} else if (WCSIEQ(argv[i], L"/fl")) {
				fl = (int) wcstol(argv[++i], NULL, 10);
			} else if (WCSIEQ(argv[i], L"/fr")) {
				fr = (int) wcstol(argv[++i], NULL, 10);
			} else if (WCSIEQ(argv[i], L"/ft")) {
				ft = (int) wcstol(argv[++i], NULL, 10);
			} else if (WCSIEQ(argv[i], L"/fb")) {
				fb = (int) wcstol(argv[++i], NULL, 10);
			} else {
				if (i == argc-2) {
					return 0;
				}
				if (WCSIEQ(argv[i], L"/p") ||
						   WCSIEQ(argv[i], L"/ph")) {
					if (argv[i][2] != L'\0') {
						half_x = cfi.dwFontSize.X / 2;
						half_y = cfi.dwFontSize.Y / 2;
					}
					y = (int) wcstol(argv[++i], NULL, 10);
					x = (int) wcstol(argv[++i], NULL, 10);
				} else if (WCSIEQ(argv[i], L"/o")) {
					sx = (int) wcstol(argv[++i], NULL, 10);
					sy = (int) wcstol(argv[++i], NULL, 10);
				} else if (WCSIEQ(argv[i], L"/s")) {
					sw = (int) wcstol(argv[++i], NULL, 10);
					sh = (int) wcstol(argv[++i], NULL, 10);
				} else if (WCSIEQ(argv[i], L"/fc")) {
					fch = (int) wcstol(argv[++i], NULL, 10);
					fcw = (int) wcstol(argv[++i], NULL, 10);
				} else if (WCSIEQ(argv[i], L"/fs")) {
					fsw = (int) wcstol(argv[++i], NULL, 10);
					fsh = (int) wcstol(argv[++i], NULL, 10);
				} else if (WCSIEQ(argv[i], L"/fp")) {
					fpr = (int) wcstol(argv[++i], NULL, 10);
					fpc = (int) wcstol(argv[++i], NULL, 10);
				}
			}
		}
	}
	if (file == NULL) {
		return 0;
	}

	ULONG_PTR gdiplusToken;
	GdiplusStartupInput si = {
		.GdiplusVersion = 1,
		.DebugEventCallback = NULL,
		.SuppressBackgroundThread = FALSE,
		.SuppressExternalCodecs = FALSE
	};
	if (GdiplusStartup(&gdiplusToken, &si, NULL) != Ok) {
		return 0;
	}

	x = x * cfi.dwFontSize.X + half_x;
	y = y * cfi.dwFontSize.Y + half_y;

	UINT ret = 0;
	GpImage *image = NULL;
	if (GdipLoadImageFromFile(file, &image) == Ok) {
		if (frame != 0 && fcw == 0 && fsw == 0) {
			if (Ok != GdipImageSelectActiveFrame(image, &FrameDimensionTime, frame)) {
				if (Ok != GdipImageSelectActiveFrame(image, &FrameDimensionPage, frame)) {
					frame = 0;
				}
			}
		}
		int w, h;
		REAL rw, rh;
		GdipGetImageDimension(image, &rw, &rh);
		w = (int) rw - sx;
		h = (int) rh - sy;
		if (sw != 0) {
			if (sw < w) w = sw;
			if (sh < h) h = sh;
		}
		if (fcw != 0) {
			fsw = w / fcw - fl - fr;
			fsh = h / fch - ft - fb;
		}
		if (fsw != 0) {
			fcw = w / (fl + fsw + fr);
			fch = h / (ft + fsh + fb);
			if (fpc != -1) {
				frame = fcw * fpr + fpc;
			}
			if (frame < 0 || frame >= fcw * fch) {
				frame = 0;
			}
			w = fsw;
			h = fsh;
			sx += (frame % fcw) * (fl + fsw + fr) + fl;
			sy += (frame / fcw) * (ft + fsh + fb) + ft;
		}
		if (ret_frames) {
			if (fcw != 0) {
				ret = fcw * fch;
			} else if (Ok != GdipImageGetFrameCount(image, &FrameDimensionTime, &ret)) {
				GdipImageGetFrameCount(image, &FrameDimensionPage, &ret);
			}
		} else {
			int cw = (w + cfi.dwFontSize.X - 1) / cfi.dwFontSize.X;
			int ch = (h + cfi.dwFontSize.Y - 1) / cfi.dwFontSize.Y;
			ret = ch << 8 | cw;
			UINT size = 0;
			GdipGetPropertyItemSize(image, PropertyTagFrameDelay, &size);
			if (size != 0) {
				PropertyItem *pi = malloc(size);
				GdipGetPropertyItem(image, PropertyTagFrameDelay, size, pi);
				ret |= (((PLONG) pi->value)[frame] * 10) << 16;
				free(pi);
			}
		}
		if (copy) {
			for (int b = 0; b < MAX_BG; ++b) {
				if (BgCache[b].bitmap == NULL) {
					HDC dc = GetDC(consoleHwnd);
					HDC mdc = CreateCompatibleDC(dc);
					BgCache[b].bitmap = CreateCompatibleBitmap(dc, w, h);
					HGDIOBJ old_mbm = SelectObject(mdc, BgCache[b].bitmap);
					BitBlt(mdc, 0, 0, w, h, dc, x, y, SRCCOPY);
					SelectObject(dc, old_mbm);
					DeleteDC(mdc);
					ReleaseDC(consoleHwnd, dc);
					BgCache[b].x = x;
					BgCache[b].y = y;
					wcsncpy(BgCache[b].name, file, MAX_PATH-1);
					break;
				}
			}
		}
		if (!quiet) {
			GpGraphics *graphics = NULL;
			GdipCreateFromHWND(consoleHwnd, &graphics);
			for (int b = 0; b < MAX_BG; ++b) {
				if (BgCache[b].bitmap != NULL
					&& BgCache[b].x == x && BgCache[b].y == y
					&& WCSEQ(BgCache[b].name, file)) {
					GpImage *bg = NULL;
					if (Ok == GdipCreateBitmapFromHBITMAP(BgCache[b].bitmap,
														  NULL, &bg)) {
						if (!restore) {
							GpGraphics *g = NULL;
							GdipGetImageGraphicsContext(bg, &g);
							GdipDrawImageRectRectI(g, image, 0, 0, w, h,
													sx, sy, w, h, UnitPixel,
													NULL, NULL, NULL);
							GdipDeleteGraphics(g);
						}
						GdipDisposeImage(image);
						image = bg;
						sx = sy = 0;
					}
					break;
				}
			}
			GdipDrawImageRectRectI(graphics, image, x, y, w, h, sx, sy, w, h,
								   UnitPixel, NULL, NULL, NULL);
			GdipDeleteGraphics(graphics);
		}
		GdipDisposeImage(image);
	}

	GdiplusShutdown(gdiplusToken);

	if (discard) {
		for (int b = 0; b < MAX_BG; ++b) {
			if (BgCache[b].bitmap != NULL
				&& BgCache[b].x == x && BgCache[b].y == y
				&& WCSEQ(BgCache[b].name, file)) {
				DeleteObject(BgCache[b].bitmap);
				BgCache[b].bitmap = NULL;
				*BgCache[b].name = L'\0';
				break;
			}
		}
	}

	return ret;
}


struct sCBg {
	PCHAR_INFO bg;
	COORD p;
	WCHAR name[MAX_PATH];
} CBgCache[MAX_BG];

#define IMG_SOLID 0 	// system colors only
#define IMG_SHADE 1 	// use light/medium/dark shades for dithering
#define IMG_RGB   2 	// RGB control sequences

// Transparency threshold -- all pixels with alpha below 25%.
#define TRANSP_ALPHA 64

struct color_t {
	BYTE r, g, b;
	BYTE a, c; 	// 0x2500 + c
};

// 16 system colors plus 16 * 16 colors, without the duplicates, three shades.
#define SHADES (16 + (16 * 16 - 16) * 3)
static struct color_t color_match[SHADES] = {
	{ 0x00,0x00,0x00, 0x00, 0x88 },
	{ 0x00,0x00,0x80, 0x01, 0x88 },
	{ 0x00,0x80,0x00, 0x02, 0x88 },
	{ 0x00,0x80,0x80, 0x03, 0x88 },
	{ 0x80,0x00,0x00, 0x04, 0x88 },
	{ 0x80,0x00,0x80, 0x05, 0x88 },
	{ 0x80,0x80,0x00, 0x06, 0x88 },
	{ 0xC0,0xC0,0xC0, 0x07, 0x88 },
	{ 0x80,0x80,0x80, 0x08, 0x88 },
	{ 0x00,0x00,0xFF, 0x09, 0x88 },
	{ 0x00,0xFF,0x00, 0x0A, 0x88 },
	{ 0x00,0xFF,0xFF, 0x0B, 0x88 },
	{ 0xFF,0x00,0x00, 0x0C, 0x88 },
	{ 0xFF,0x00,0xFF, 0x0D, 0x88 },
	{ 0xFF,0xFF,0x00, 0x0E, 0x88 },
	{ 0xFF,0xFF,0xFF, 0x0F, 0x88 },
};

// Map console attribute to negative ANSI foreground (add 10 for background).
static int attr2ansi[16] = {
	-30, -34, -32, -36, -31, -35, -33, -37,
	-90, -94, -92, -96, -91, -95, -93, -97,
};

// HSV <-> RGB routines from Allegro.

/* hsv_to_rgb:
 *	Converts from HSV colorspace to RGB values.
 */
static void hsv_to_rgb(float h, float s, float v,
					   BYTE *r, BYTE *g, BYTE *b)
{
	float f, x, y, z;
	int i;

	v *= 255.0f;

	if (s == 0.0f) {
		*r = *g = *b = v;
	} else {
		h = fmod(h, 360.0) / 60.0;
		if (h < 0) {
			h += 6.0f;
		}
		i = h;
		f = h - i;
		x = v * (1.0f - s) + 0.5f;
		y = v * (1.0f - (s * f)) + 0.5f;
		z = v * (1.0f - (s * (1.0f - f))) + 0.5f;

		switch (i) {
			case 6:
			case 0: *r = v; *g = z; *b = x; break;
			case 1: *r = y; *g = v; *b = x; break;
			case 2: *r = x; *g = v; *b = z; break;
			case 3: *r = x; *g = y; *b = v; break;
			case 4: *r = z; *g = x; *b = v; break;
			case 5: *r = v; *g = x; *b = y; break;
		}
	}
}

/* rgb_to_hsv:
 *  Converts an RGB value into the HSV colorspace.
 */
static void rgb_to_hsv(BYTE r, BYTE g, BYTE b,
					   float *h, float *s, float *v)
{
	float min, max, delta, rc, gc, bc;

	rc = r / 255.0f;
	gc = g / 255.0f;
	bc = b / 255.0f;
	max = max(rc, max(gc, bc));
	min = min(rc, min(gc, bc));
	delta = max - min;
	*v = max;

	if (max != 0.0f) {
		*s = delta / max;
	} else {
		*s = 0.0f;
	}

	if (*s == 0.0f) {
		*h = 0.0f;
	} else {
		if (rc == max) {
			*h = (gc - bc) / delta;
		} else if (gc == max) {
			*h = 2.0f + (bc - rc) / delta;
		} else if (bc == max) {
			*h = 4.0f + (rc - gc) / delta;
		}
		*h *= 60.0f;
		if (*h < 0.0f) {
			*h += 360.0f;
		}
	}
}

// Find the "distance" between two colors.
// https://www.compuphase.com/cmetric.htm
static int color_distance(int r1, int g1, int b1, int r2, int g2, int b2)
{
	int rmean = (r1 + r2) / 2;
	int r = r1 - r2;
	int g = g1 - g2;
	int b = b1 - b2;
	return sqrt((((512 + rmean) * r * r) >> 8)
				+ 4 * g * g
				+ (((767 - rmean) * b * b) >> 8));
}

// Find the index in the color_match table of the "nearest" color.
static int nearest_color(BOOL sys, int r, int g, int b)
{
	int md = 0x7fffffff;
	int mi, i;
	int n = sys ? 16 : SHADES;
	for (i = mi = 0; i < n; i++) {
		int d = color_distance(r, g, b, color_match[i].r,
							   color_match[i].g, color_match[i].b);
		if (d == 0) {
			return i;
		}
		if (d < md) {
			md = d;
			mi = i;
		}
	}
	return mi;
}

// Windows Terminal will probably be using the wrong font and palette, so this
// won't work too well (not too bothered, use RGB).
static void generate_colors(PCONSOLE_FONT_INFOEX pcfi)
{
	float HSV[16][3];
	int c;
	int i, j;
	static COLORREF ColorTable[16];

	CONSOLE_SCREEN_BUFFER_INFOEX csbi;
	csbi.cbSize = sizeof(csbi);
	pGetConsoleScreenBufferInfoEx(consoleOutput, &csbi);
	if (memcmp(ColorTable, csbi.ColorTable, sizeof(ColorTable)) == 0) {
		return;
	}
	memcpy(ColorTable, csbi.ColorTable, sizeof(ColorTable));

	for (c = 0; c < 16; ++c) {
		color_match[c].r = GetRValue(csbi.ColorTable[c]);
		color_match[c].g = GetGValue(csbi.ColorTable[c]);
		color_match[c].b = GetBValue(csbi.ColorTable[c]);
		rgb_to_hsv(color_match[c].r, color_match[c].g, color_match[c].b,
				   &HSV[c][0], &HSV[c][1], &HSV[c][2]);
	}

	HFONT font = CreateFont(pcfi->dwFontSize.Y, 0,
							0, 0, pcfi->FontWeight, 0, 0, 0,
							DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
							pcfi->FaceName );

	HDC dc = GetDC(GetConsoleWindow());
	HDC mdc = CreateCompatibleDC(dc);
	HBITMAP bm = CreateCompatibleBitmap(dc, pcfi->dwFontSize.X, pcfi->dwFontSize.Y);
	ReleaseDC(GetConsoleWindow(), dc);
	HGDIOBJ old_font = SelectObject(mdc, font);
	HGDIOBJ old_bm = SelectObject(mdc, bm);

	int count = pcfi->dwFontSize.X * pcfi->dwFontSize.Y;
	int r, g, b;
	float h, s, v;

	void average_rgb(WCHAR ch, int c, int i, int j)
	{
		TextOut(mdc, 0, 0, &ch, 1);
		r = g = b = 0;
		for (int y = 0; y < pcfi->dwFontSize.Y; ++y) {
			for (int x = 0; x < pcfi->dwFontSize.X; ++x) {
				COLORREF cr = GetPixel(mdc, x, y);
				r += GetRValue(cr);
				g += GetGValue(cr);
				b += GetBValue(cr);
			}
		}
		color_match[c].r = r / count;
		color_match[c].g = g / count;
		color_match[c].b = b / count;
		color_match[c].a = (i << 4) + j;
		color_match[c].c = ch;
		++c;
	}

	void average_hsv(WCHAR ch, int c, int i, int j)
	{
		TextOut(mdc, 0, 0, &ch, 1);
		h = s = v = 0;
		for (int y = 0; y < pcfi->dwFontSize.Y; ++y) {
			for (int x = 0; x < pcfi->dwFontSize.X; ++x) {
				COLORREF cr = GetPixel(mdc, x, y);
				float hh, ss, vv;
				rgb_to_hsv(GetRValue(cr), GetGValue(cr), GetBValue(cr),
						   &hh, &ss, &vv);
				h += hh;
				s += ss;
				v += vv;
			}
		}
		hsv_to_rgb(h / count, s / count, v / count,
				   &color_match[c].r, &color_match[c].g, &color_match[c].b);
		color_match[c].a = (i << 4) + j;
		color_match[c].c = ch;
		++c;
	}

	for (i = 0; i < 16; ++i) {
		SetBkColor(mdc, RGB(color_match[i].r, color_match[i].g, color_match[i].b));
		for (j = 0; j < 16; ++j) {
			if (i == j)
				continue;
			SetTextColor(mdc, RGB(color_match[j].r, color_match[j].g, color_match[j].b));
			// The medium shade averages the components.
			average_rgb(L'\x2592', c, i, j);
			// The light and dark shades convert to HSV and average that.
			average_hsv(L'\x2591', c, i, j);
			average_hsv(L'\x2593', c, i, j);
		}
	}

	DeleteObject(SelectObject(mdc, old_bm));
	DeleteObject(SelectObject(mdc, old_font));
	DeleteDC(mdc);
}

int CallImg(int argc, LPCWSTR argv[])
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	CONSOLE_FONT_INFOEX cfi;

	if (argc == 0 || !haveOutputHandle()) {
		return 0;
	}

	cfi.cbSize = sizeof(cfi);
	if (!pGetCurrentConsoleFontEx(consoleOutput, FALSE, &cfi)
		|| !GetConsoleScreenBufferInfo(consoleOutput, &csbi)) {
		return 0;
	}
	if (cfi.dwFontSize.X == 0 || onWindowsTerminal) {
		if (haveWindowHandle()) {
			RECT r;
			GetClientRect(consoleHwnd, &r);
			cfi.dwFontSize.X = r.right / csbi.dwSize.X;
			cfi.dwFontSize.Y = r.bottom / csbi.dwSize.Y;
		} else {
			return 0;
		}
	}

	BOOL terminal = onWindowsTerminal;
	if (!terminal && winMajor >= 10) {	// is Windows 10 or greater
		DWORD mode = 0;
		GetConsoleMode(consoleOutput, &mode);
		if (mode & 4/*ENABLE_VIRTUAL_TERMINAL_PROCESSING*/) {
			terminal = TRUE;
		}
	}

	COORD p = csbi.dwCursorPosition;
	int cols = -1, rows = -1;
	int quality = terminal ? IMG_RGB : IMG_SHADE;
	int sx = 0, sy = 0, sw = 0, sh = 0;
	int fcw = 0, fch = 0, fsw = 0, fsh = 0;
	int fl = 0, fr = 0, ft = 0, fb = 0;
	int fpc = -1, fpr = -1;
	LPCWSTR file = NULL;
	int frame = 0;
	BOOL quiet = FALSE, ret_frames = FALSE;
	BOOL copy = FALSE, discard = FALSE, restore = FALSE;
	BOOL trans = TRUE;
	int space_attr = csbi.wAttributes >> 4 & 0xf;
	int space_rgb = -1;

	for (int i = 0; i < argc; ++i) {
		if (*argv[i] != L'/') {
			if (file != NULL) {
				return 0;
			}
			file = argv[i];
		} else if (WCSIEQ(argv[i], L"/c0")) {
			quality = IMG_SOLID;
		} else if (WCSIEQ(argv[i], L"/c1")) {
			quality = IMG_SHADE;
		} else if (WCSIEQ(argv[i], L"/c2")) {
			quality = IMG_RGB;
		} else if (WCSIEQ(argv[i], L"/copy")) {
			copy = TRUE;
		} else if (WCSIEQ(argv[i], L"/discard")) {
			discard = TRUE;
		} else if (WCSIEQ(argv[i], L"/restore")) {
			discard = restore = TRUE;
		} else if (WCSIEQ(argv[i], L"/n")) {
			ret_frames = quiet = TRUE;
		} else if (WCSIEQ(argv[i], L"/q")) {
			quiet = TRUE;
		} else if (WCSIBEG(argv[i], L"/t")) {
			trans = FALSE;
			if (argv[i][2] != L'\0') {
				int c = (int) wcstol(argv[i]+2, NULL, 16);
				int len = wcslen(argv[i]+2);
				if (len == 1) {
					space_attr = c;
				} else if (len == 2 && c >= 0x10 && c <= 0x15) {
					space_attr = c - 0x10 + 10;
				} else if (len == 3) {
					BYTE r = c >> 8;
					BYTE g = (BYTE) c >> 4;
					BYTE b = c & 0xF;
					r |= r << 4;
					g |= g << 4;
					b |= b << 4;
					space_rgb = (r << 16) | (g << 8) | b;
				} else {
					space_rgb = c;
				}
			}
		} else {
			if (i == argc-1) {
				return 0;
			}
			if (WCSIEQ(argv[i], L"/c")) {
				p.X = (int) wcstol(argv[++i], NULL, 10) + csbi.srWindow.Left;
			} else if (WCSIEQ(argv[i], L"/r")) {
				p.Y = (int) wcstol(argv[++i], NULL, 10) + csbi.srWindow.Top;
			} else if (WCSIEQ(argv[i], L"/w")) {
				cols = (int) wcstol(argv[++i], NULL, 10);
			} else if (WCSIEQ(argv[i], L"/h")) {
				rows = (int) wcstol(argv[++i], NULL, 10);
			} else if (WCSIEQ(argv[i], L"/f")) {
				frame = (int) wcstol(argv[++i], NULL, 10);
			} else if (WCSIEQ(argv[i], L"/fl")) {
				fl = (int) wcstol(argv[++i], NULL, 10);
			} else if (WCSIEQ(argv[i], L"/fr")) {
				fr = (int) wcstol(argv[++i], NULL, 10);
			} else if (WCSIEQ(argv[i], L"/ft")) {
				ft = (int) wcstol(argv[++i], NULL, 10);
			} else if (WCSIEQ(argv[i], L"/fb")) {
				fb = (int) wcstol(argv[++i], NULL, 10);
			} else {
				if (i == argc-2) {
					return 0;
				}
				if (WCSIEQ(argv[i], L"/p")) {
					p.Y = (int) wcstol(argv[++i], NULL, 10) + csbi.srWindow.Top;
					p.X = (int) wcstol(argv[++i], NULL, 10) + csbi.srWindow.Left;
				} else if (WCSIEQ(argv[i], L"/d")) {
					rows = (int) wcstol(argv[++i], NULL, 10);
					cols = (int) wcstol(argv[++i], NULL, 10);
				} else if (WCSIEQ(argv[i], L"/o")) {
					sx = (int) wcstol(argv[++i], NULL, 10);
					sy = (int) wcstol(argv[++i], NULL, 10);
				} else if (WCSIEQ(argv[i], L"/s")) {
					sw = (int) wcstol(argv[++i], NULL, 10);
					sh = (int) wcstol(argv[++i], NULL, 10);
				} else if (WCSIEQ(argv[i], L"/fc")) {
					fch = (int) wcstol(argv[++i], NULL, 10);
					fcw = (int) wcstol(argv[++i], NULL, 10);
				} else if (WCSIEQ(argv[i], L"/fs")) {
					fsw = (int) wcstol(argv[++i], NULL, 10);
					fsh = (int) wcstol(argv[++i], NULL, 10);
				} else if (WCSIEQ(argv[i], L"/fp")) {
					fpr = (int) wcstol(argv[++i], NULL, 10);
					fpc = (int) wcstol(argv[++i], NULL, 10);
				}
			}
		}
	}
	if (file == NULL) {
		return 0;
	}

	ULONG_PTR gdiplusToken;
	GdiplusStartupInput si = {
		.GdiplusVersion = 1,
		.DebugEventCallback = NULL,
		.SuppressBackgroundThread = FALSE,
		.SuppressExternalCodecs = FALSE
	};
	if (GdiplusStartup(&gdiplusToken, &si, NULL) != Ok) {
		return 0;
	}

	UINT ret = 0;
	GpImage *image = NULL;
	if (GdipLoadImageFromFile(file, &image) == Ok) {
		if (frame != 0 && fcw == 0 && fsw == 0) {
			if (Ok != GdipImageSelectActiveFrame(image, &FrameDimensionTime, frame)) {
				if (Ok != GdipImageSelectActiveFrame(image, &FrameDimensionPage, frame)) {
					frame = 0;
				}
			}
		}
		int w, h, dw, dh;
		REAL rw, rh;
		GdipGetImageDimension(image, &rw, &rh);
		w = (int) rw - sx;
		h = (int) rh - sy;
		if (sw != 0) {
			if (sw < w) w = sw;
			if (sh < h) h = sh;
		}
		if (fcw != 0) {
			fsw = w / fcw - fl - fr;
			fsh = h / fch - ft - fb;
		}
		if (fsw != 0) {
			fcw = w / (fl + fsw + fr);
			fch = h / (ft + fsh + fb);
			if (fpc != -1) {
				frame = fcw * fpr + fpc;
			}
			if (frame < 0 || frame >= fcw * fch) {
				frame = 0;
			}
			w = fsw;
			h = fsh;
			sx += (frame % fcw) * (fl + fsw + fr) + fl;
			sy += (frame / fcw) * (ft + fsh + fb) + ft;
		}
		int wcols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		int wrows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		if (cols == 0) {
			cols = w;
		}
		if (rows == 0) {
			rows = (h + 1) / 2;
			dh = h;
		} else {
			dh = 0;
		}
		if (cols == -1) {
			if (rows == -1) {
				rows = min(h, wrows);
			}
			cols = rows * w * cfi.dwFontSize.Y / cfi.dwFontSize.X / h;
		} else {
			if (rows == -1) {
				rows = cols * h * cfi.dwFontSize.X / cfi.dwFontSize.Y / w;
			}
			if (rows > wrows) {
				rows = wrows;
				cols = rows * w * cfi.dwFontSize.Y / cfi.dwFontSize.X / h;
			}
		}
		if (cols > wcols) {
			cols = wcols;
			rows = cols * h * cfi.dwFontSize.X / cfi.dwFontSize.Y / w;
		}
		if (dh == 0 || rows != (h + 1) / 2) {
			dh = rows * 2;
		}
		dw = cols;
		if (ret_frames) {
			if (fcw != 0) {
				ret = fcw * fch;
			} else if (Ok != GdipImageGetFrameCount(image, &FrameDimensionTime, &ret)) {
				GdipImageGetFrameCount(image, &FrameDimensionPage, &ret);
			}
		} else {
			ret = rows << 8 | cols;
			UINT size = 0;
			GdipGetPropertyItemSize(image, PropertyTagFrameDelay, &size);
			if (size != 0) {
				PropertyItem *pi = malloc(size);
				GdipGetPropertyItem(image, PropertyTagFrameDelay, size, pi);
				ret |= (((PLONG) pi->value)[frame] * 10) << 16;
				free(pi);
			}
		}
		int ox = 0, oy = 0, ch = dh;	// clipped height
		if (p.X < csbi.srWindow.Left) {
			ox = csbi.srWindow.Left - p.X;
			p.X = csbi.srWindow.Left;
			cols -= ox;
		}
		if (p.X + cols - 1 > csbi.srWindow.Right) {
			cols = csbi.srWindow.Right - p.X + 1;
		}
		if (p.Y < csbi.srWindow.Top) {
			oy = csbi.srWindow.Top - p.Y;
			p.Y = csbi.srWindow.Top;
			rows -= oy;
			oy *= 2;
			ch -= oy;
		}
		if (p.Y + rows - 1 > csbi.srWindow.Bottom) {
			rows = csbi.srWindow.Bottom - p.Y + 1;
			ch = rows * 2;
		}
		if (copy) {
			for (int b = 0; b < MAX_BG; ++b) {
				if (CBgCache[b].bg == NULL) {
					CBgCache[b].bg = malloc(cols * rows * sizeof(CHAR_INFO));
					COORD s = { cols, rows };
					COORD o = { 0, 0 };
					SMALL_RECT r = { p.X, p.Y, p.X + cols - 1, p.Y + rows - 1 };
					ReadConsoleOutput(consoleOutput, CBgCache[b].bg, s, o, &r);
					CBgCache[b].p = p;
					wcsncpy(CBgCache[b].name, file, MAX_PATH-1);
					break;
				}
			}
		}
		if (!quiet) {
			if (quality != IMG_RGB) {
				generate_colors(&cfi);
				if (space_rgb != -1) {
					space_attr = nearest_color(TRUE,
											   space_rgb >> 16,
											   space_rgb >> 8 & 0xff,
											   space_rgb & 0xff);
				}
				space_attr = (space_attr << 4) | (csbi.wAttributes & 0xf);
			}
			PCHAR_INFO pbg = NULL;
			for (int b = 0; b < MAX_BG; ++b) {
				if (CBgCache[b].bg != NULL
					&& CBgCache[b].p.X == p.X && CBgCache[b].p.Y == p.Y
					&& WCSEQ(CBgCache[b].name, file)) {
					pbg = CBgCache[b].bg;
					if (restore) {
						COORD s = { cols, rows };
						COORD o = { 0, 0 };
						SMALL_RECT r = { p.X, p.Y, p.X + cols - 1, p.Y + rows - 1 };
						WriteConsoleOutput(consoleOutput, pbg, s, o, &r);
					}
					break;
				}
			}
			if (!restore) {
				GpImage *scaled = NULL;
				GdipCreateBitmapFromScan0(dw, dh, 0, PixelFormat32bppARGB,
										  NULL, &scaled);
				GpGraphics *graphics = NULL;
				GdipGetImageGraphicsContext(scaled, &graphics);
				GdipSetInterpolationMode(graphics, InterpolationModeNearestNeighbor);
				GdipDrawImageRectRectI(graphics, image, 0, 0, dw, dh, sx, sy,
									   w, h, UnitPixel, NULL, NULL, NULL);
				GdipDeleteGraphics(graphics);

				DWORD dummy;
				COORD size = { (SHORT) cols, 1 }, buf = { 0, 0 };
				PCHAR_INFO ci = NULL, pci;
				SMALL_RECT sr;
				int transp = 0, prev_fg = -2, prev_bg = -2;
				LPWSTR sb = stringBuffer;

				if (quality == IMG_RGB) {
					// Save cursor position, turn off bold.
					WriteConsole(consoleOutput, L"\0337\33[22m", 7, &dummy, NULL);
					SetConsoleCursorPosition(consoleOutput, p);
				} else {
					ci = malloc(sizeof(CHAR_INFO) * cols);
				}

				void output(WCHAR ch, int fg, int bg)
				{
					if (ch == L'\n') {
						sb += sbprintf(sb, L"\33[B\33[%dD", cols - transp);
						if (sb - stringBuffer > 30000) {
							WriteConsole(consoleOutput, stringBuffer, sb - stringBuffer, &dummy, NULL);
							sb = stringBuffer;
						}
						transp = 0;
						return;
					}
					if (ch == 0) {
						++transp;
						return;
					}
					if (transp != 0) {
						sb += sbprintf(sb, L"\33[%dC", transp);
						transp = 0;
					}
					if (bg != prev_bg && bg != -1) {
						if (bg < 0) {
							sb += sbprintf(sb, L"\33[%dm", -bg + 10);
						} else {
							sb += sbprintf(sb, L"\33[48;2;%d;%d;%dm",
										   bg >> 16 & 0xff, bg >> 8 & 0xff, bg & 0xff);
						}
						prev_bg = bg;
					}
					if (fg != prev_fg && fg != -1) {
						if (fg < 0) {
							sb += sbprintf(sb, L"\33[%dm", -fg);
						} else {
							sb += sbprintf(sb, L"\33[38;2;%d;%d;%dm",
										   fg >> 16 & 0xff, fg >> 8 & 0xff, fg & 0xff);
						}
						prev_fg = fg;
					}
					sb += sbprintf(sb, L"%c", ch);
				}

				// Calculate the 8-bit console foreground, background color, and
				// the character from RGB color.
				sr.Top = sr.Bottom = p.Y;
				for (int y = 0; y < ch; y += 2) {
					pci = ci;
					ARGB c1, c2 = 0;
					for (int x = 0; x < cols; x++) {
						GdipBitmapGetPixel(scaled, ox + x, oy + y, &c1);
						if (y+1 < ch) {
							GdipBitmapGetPixel(scaled, ox + x, oy + y+1, &c2);
						}
						BOOL t1 = c1 >> 24 < TRANSP_ALPHA;
						BOOL t2 = c2 >> 24 < TRANSP_ALPHA;
						c1 &= 0xFFFFFF;
						c2 &= 0xFFFFFF;
						if (t1 && t2) {
							if (trans) {
								if (quality == IMG_RGB) {
									if (pbg == NULL) {
										output(0, -1, -1);
									} else {
										pci = &pbg[y/2 * cols + x];
										output(pci->Char.UnicodeChar,
											   attr2ansi[pci->Attributes & 0xf],
											   attr2ansi[pci->Attributes >> 4 & 0xf]);
									}
								} else {
									if (pbg == NULL) {
										if (pci != ci) {
											size.X = pci - ci;
											sr.Left = p.X + x - size.X;
											sr.Right = p.X + cols - 1;
											WriteConsoleOutput(consoleOutput, ci, size, buf, &sr);
											pci = ci;
										}
									} else {
										*pci++ = pbg[y/2 * cols + x];
									}
								}
							} else {
								if (quality == IMG_RGB) {
									output(L' ', -1,
										   space_rgb == -1 ? attr2ansi[space_attr]
														   : space_rgb);
								} else {
									pci->Attributes = space_attr;
									pci->Char.UnicodeChar = L' ';
									++pci;
								}
							}
							continue;
						}
						if (t1 || t2) {
							WORD attr;
							if (trans) {
								if (pbg == NULL) {
									csbi.dwCursorPosition.X = p.X + x;
									csbi.dwCursorPosition.Y = sr.Top;
									ReadConsoleOutputAttribute(consoleOutput, &attr, 1,
															   csbi.dwCursorPosition, &dummy);
								} else {
									attr = pbg[y/2 * cols + x].Attributes;
								}
								attr = attr >> 4 & 0xf;
							} else {
								attr = space_attr;
								if (quality != IMG_RGB) {
									attr = attr >> 4 & 0xf;
								}
							}
							if (quality == IMG_RGB) {
								int c = space_rgb == -1 ? attr2ansi[attr]
														: space_rgb;
								if (t1) {
									output(L'\x2584', c2, c);
								} else {
									output(L'\x2580', c1, c);
								}
								continue;
							}
							struct color_t *bg = &color_match[attr];
							ARGB c = (bg->r << 16) | (bg->g << 8) | bg->b;
							if (t1) {
								c1 = c;
							} else {
								c2 = c;
							}
						}
						if (quality == IMG_RGB) {
							if (c1 == c2) {
								output(L'\x2588', c2, -1);
							} else {
								output(L'\x2584', c2, c1);
							}
						} else {
							int r1 = c1 >> 16;
							int g1 = c1 >> 8 & 0xff;
							int b1 = c1 & 0xff;
							int r2 = c2 >> 16;
							int g2 = c2 >> 8 & 0xff;
							int b2 = c2 & 0xff;
							int i1 = nearest_color(TRUE, r1, g1, b1);
							int i2 = nearest_color(TRUE, r2, g2, b2);
							int ds, d12, i12;
							if (t1 || t2 || quality == IMG_SOLID) {
								ds = d12 = 0;
							} else {
								ds = color_distance(r1, g1, b1, color_match[i1].r,
													color_match[i1].g, color_match[i1].b)
									 + color_distance(r2, g2, b2, color_match[i2].r,
													  color_match[i2].g, color_match[i2].b);
								int r12 = (r1 + r2) / 2;
								int g12 = (g1 + g2) / 2;
								int b12 = (b1 + b2) / 2;
								i12 = nearest_color(FALSE, r12, g12, b12);
								d12 = color_distance(r1, g1, b1, color_match[i12].r,
													 color_match[i12].g, color_match[i12].b)
									  + color_distance(r2, g2, b2, color_match[i12].r,
													   color_match[i12].g, color_match[i12].b);
							}
							if (ds <= d12) {
								pci->Attributes = (color_match[i1].a << 4) | color_match[i2].a;
								pci->Char.UnicodeChar = 0x2584;
							} else {
								pci->Attributes = color_match[i12].a;
								pci->Char.UnicodeChar = 0x2500 + color_match[i12].c;
							}
							++pci;
						}
					}
					if (quality == IMG_RGB) {
						output(L'\n', -1, -1);
					} else {
						if (pci != ci) {
							size.X = pci - ci;
							sr.Left = p.X + cols - size.X;
							sr.Right = p.X + cols - 1;
							WriteConsoleOutput(consoleOutput, ci, size, buf, &sr);
						}
					}
					++sr.Top;
					++sr.Bottom;
				}
				GdipDisposeImage(scaled);

				if (quality == IMG_RGB) {
					WriteConsole(consoleOutput, stringBuffer, sb - stringBuffer, &dummy, NULL);
					SetConsoleTextAttribute(consoleOutput, csbi.wAttributes);
					WriteConsole(consoleOutput, L"\0338", 2, &dummy, NULL);
				} else {
					free(ci);
				}
			}
		}
		GdipDisposeImage(image);
	}

	GdiplusShutdown(gdiplusToken);

	if (discard) {
		for (int b = 0; b < MAX_BG; ++b) {
			if (CBgCache[b].bg != NULL
				&& CBgCache[b].p.X == p.X && CBgCache[b].p.Y == p.Y
				&& WCSEQ(CBgCache[b].name, file)) {
				free(CBgCache[b].bg);
				CBgCache[b].bg = NULL;
				*CBgCache[b].name = L'\0';
				break;
			}
		}
	}

	return ret;
}


HKEY get_console_key(HKEY hConsoleKey)
{
	WCHAR title[1024], name[1024];
	if (GetConsoleTitle(title, lenof(title)) == 0) {
		return NULL;
	}
	if (!PathUnExpandEnvStrings(title, name, lenof(name))) {
		wcscpy(name, title);
	}
	for (LPWSTR p = name; *p != L'\0'; ++p) {
		if (*p == L'\\') {
			*p = L'_';
		} else if (p[0] == L' ' && p[1] == L'-' && p[2] == L' ') {
			*p = L'\0';
			break;
		}
	}
	HKEY hKey = NULL;
	RegOpenKeyExW(hConsoleKey, name, 0, KEY_QUERY_VALUE, &hKey);
	return hKey;
}


BOOL WINAPI myGetCurrentConsoleFontEx(
	HANDLE hConsoleOutput,
	BOOL bMaximumWindow,
	PCONSOLE_FONT_INFOEX pcfix)
{
	CONSOLE_FONT_INFO fi;
	if (!GetCurrentConsoleFont(hConsoleOutput, bMaximumWindow, &fi )) {
		return FALSE;
	}
	pcfix->dwFontSize = GetConsoleFontSize(hConsoleOutput, fi.nFont);

	HKEY hKey;
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER, L"Console",
									  0, KEY_QUERY_VALUE, &hKey)) {
		return FALSE;
	}

	BOOL getFaceName(HKEY hKey)
	{
		DWORD len = LF_FACESIZE;
		return ERROR_SUCCESS == RegQueryValueEx(hKey, L"FaceName", NULL, NULL,
												(LPBYTE) pcfix->FaceName, &len);
	}

	*pcfix->FaceName = L'\0';
	BOOL gotFace = FALSE;
	HKEY hSubkey = get_console_key(hKey);
	if (hSubkey != NULL) {
		gotFace = getFaceName(hSubkey);
		RegCloseKey(hSubkey);
	}
	if (!gotFace) {
		getFaceName(hKey);
	}
	RegCloseKey(hKey);
	if (*pcfix->FaceName == L'\0') {
		wcscpy(pcfix->FaceName, L"Terminal");
	}

	return TRUE;
}


BOOL WINAPI myGetConsoleScreenBufferInfoEx(
	HANDLE hConsoleOutput,
	PCONSOLE_SCREEN_BUFFER_INFOEX pcsbix)
{
	if (!GetConsoleScreenBufferInfo(hConsoleOutput,
			(PCONSOLE_SCREEN_BUFFER_INFO) &pcsbix->dwSize)) {
		return FALSE;
	}

	void getColorTable(HKEY hKey)
	{
		WCHAR color[] = L"ColorTable00";
		for (int i = 0; i < 16; ++i) {
			DWORD len = 4;
			RegQueryValueEx(hKey, color, NULL, NULL,
							(LPBYTE)(pcsbix->ColorTable+i), &len);
			if (++color[11] > L'9') {
				color[11] = L'0';
				++color[10];
			}
		}
	}

	HKEY hKey;
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER, L"Console",
									  0, KEY_QUERY_VALUE, &hKey)) {
		return FALSE;
	}

	getColorTable(hKey);
	HKEY hSubkey = get_console_key(hKey);
	if (hSubkey != NULL) {
		getColorTable(hSubkey);
		RegCloseKey(hSubkey);
	}
	RegCloseKey(hKey);

	return TRUE;
}
