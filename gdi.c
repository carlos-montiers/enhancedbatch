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
			} else if (WCSIEQ(argv[i], L"/p") ||
					   WCSIEQ(argv[i], L"/ph")) {
				if (argv[i][2] != L'\0') {
					half_x = cfi.dwFontSize.X / 2;
					half_y = cfi.dwFontSize.Y / 2;
				}
				y = (int) wcstol(argv[++i], NULL, 10);
				if (i == argc-1) {
					return 0;
				}
				x = (int) wcstol(argv[++i], NULL, 10);
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
				if (WCSIEQ(argv[i], L"/o")) {
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
							GpGraphics *graphics = NULL;
							GdipGetImageGraphicsContext(bg, &graphics);
							GdipDrawImageRectRectI(graphics, image, 0, 0, w, h,
													sx, sy, w, h, UnitPixel,
													NULL, NULL, NULL);
							GdipDeleteGraphics(graphics);
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
