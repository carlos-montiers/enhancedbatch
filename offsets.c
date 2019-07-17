/*
 Copyright (c) 2019 Carlos Montiers Aguilera
 Copyright (c) 2019 Jason Hood

 This software is provided 'as-is', without any express or implied
 warranty.  In no event will the authors be held liable for any damages
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

LPBYTE cmd_addrs[] = {
	0,			// eol
	0,			// MSCmdVar
	0,			// Lex:text
	0,			// TmpBuf
	0,			// PutStdErr:PutMsg
	0,			// CurrentBatchFile
	0,			// fOutputUnicode
	0,			// fDelayedExpansion
	0,			// fEnableExtensions
	0,			// fDumpTokens
	0,			// fDumpParse
};

#ifdef _WIN64

const DWORD cmd_10_0_10240_16384[] = {
	0x05151,	// eol
	0x06cac,	// MSCmdVar
	0x0a555,	// Lex:text
	0x3c900,	// TmpBuf
	0x10ec8,	// PutStdErr:PutMsg
	0x46c00,	// CurrentBatchFile
	0x46c10,	// fOutputUnicode
	0x46c11,	// fDelayedExpansion
	0x46c12,	// fEnableExtensions
	0x2f964,	// fDumpTokens
	0x2f848,	// fDumpParse
};

const DWORD cmd_10_0_10586_0[] = {
	0x05394,	// eol
	0x067f4,	// MSCmdVar
	0x095aa,	// Lex:text
	0x3d920,	// TmpBuf
	0x116f8,	// PutStdErr:PutMsg
	0x47c20,	// CurrentBatchFile
	0x47c30,	// fOutputUnicode
	0x47c31,	// fDelayedExpansion
	0x47c32,	// fEnableExtensions
	0x30984,	// fDumpTokens
	0x30868,	// fDumpParse
};

const DWORD cmd_10_0_14393_0[] = {
	0x0fc64,	// eol
	0x0dc90,	// MSCmdVar
	0x0a895,	// Lex:text
	0x32810,	// TmpBuf
	0x0580c,	// PutStdErr:PutMsg
	0x46d30,	// CurrentBatchFile
	0x4b0b4,	// fOutputUnicode
	0x46d38,	// fDelayedExpansion
	0x46d39,	// fEnableExtensions
	0x46bf4,	// fDumpTokens
	0x46d10,	// fDumpParse
};

const DWORD cmd_10_0_15063_0[] = {
	0x03794,	// eol
	0x10be8,	// MSCmdVar
	0x0dd3a,	// Lex:text
	0x4c080,	// TmpBuf
	0x502b0,	// PutStdErr:PutMsg
	0x43cd4,	// CurrentBatchFile
	0x14398,	// fOutputUnicode
	0x502b8,	// fDelayedExpansion
	0x502b9,	// fEnableExtensions
	0x4bf64,	// fDumpTokens
	0x4c070,	// fDumpParse
};

const DWORD cmd_10_0_16299_15[] = {
	0x038c1,	// eol
	0x10e78,	// MSCmdVar
	0x0df33,	// Lex:text
	0x4c080,	// TmpBuf
	0x14568,	// PutStdErr:PutMsg
	0x502b0,	// CurrentBatchFile
	0x54694,	// fOutputUnicode
	0x502b8,	// fDelayedExpansion
	0x502b9,	// fEnableExtensions
	0x4bf64,	// fDumpTokens
	0x4c070,	// fDumpParse
};

const DWORD cmd_10_0_17134_1[] = {
	0x02fc4,	// eol
	0x11698,	// MSCmdVar
	0x0e718,	// Lex:text
	0x4c080,	// TmpBuf
	0x14db8,	// PutStdErr:PutMsg
	0x502b0,	// CurrentBatchFile
	0x54694,	// fOutputUnicode
	0x502b8,	// fDelayedExpansion
	0x502b9,	// fEnableExtensions
	0x4bf64,	// fDumpTokens
	0x4c070,	// fDumpParse
};

const DWORD cmd_10_0_17763_1[] = {
	0x0aa64,	// eol
	0x11b9c,	// MSCmdVar
	0x0fabb,	// Lex:text
	0x520d0,	// TmpBuf
	0x14990,	// PutStdErr:PutMsg
	0x43a90,	// CurrentBatchFile
	0x5667c,	// fOutputUnicode
	0x43aa4,	// fDelayedExpansion
	0x43aa5,	// fEnableExtensions
	0x56685,	// fDumpTokens
	0x56686,	// fDumpParse
};

const DWORD cmd_10_0_18362_1[] = {
	0x09754,	// eol
	0x10bac,	// MSCmdVar
	0x0ea7b,	// Lex:text
	0x520f0,	// TmpBuf
	0x14fe8,	// PutStdErr:PutMsg
	0x43a90,	// CurrentBatchFile
	0x56aa4,	// fOutputUnicode
	0x43aa4,	// fDelayedExpansion
	0x43aa5,	// fEnableExtensions
	0x56ab1,	// fDumpTokens
	0x56ab2,	// fDumpParse
};

const DWORD cmd_5_2_3790_1830[] = {
	0x0a947,	// eol
	0x07670,	// MSCmdVar
	0x02e3d,	// Lex:text
	0x4f420,	// TmpBuf
	0x0f72a,	// PutStdErr:PutMsg
	0x42040,	// CurrentBatchFile
	0x5e734,	// fOutputUnicode
	0x42068,	// fDelayedExpansion
	0x42058,	// fEnableExtensions
	0x4b385,	// fDumpTokens
	0x4b384,	// fDumpParse
};

const DWORD cmd_5_2_3790_3959[] = {
	0x0a967,	// eol
	0x07690,	// MSCmdVar
	0x02e5d,	// Lex:text
	0x4f420,	// TmpBuf
	0x0f72a,	// PutStdErr:PutMsg
	0x42040,	// CurrentBatchFile
	0x5e734,	// fOutputUnicode
	0x42068,	// fDelayedExpansion
	0x42058,	// fEnableExtensions
	0x4b385,	// fDumpTokens
	0x4b384,	// fDumpParse
};

const DWORD cmd_6_0_6000_16386[] = {
	0x0b817,	// eol
	0x042f0,	// MSCmdVar
	0x02e1e,	// Lex:text
	0x37b00,	// TmpBuf
	0x11c78,	// PutStdErr:PutMsg
	0x2e190,	// CurrentBatchFile
	0x37306,	// fOutputUnicode
	0x2e08b,	// fDelayedExpansion
	0x2e08a,	// fEnableExtensions
	0x3bd60,	// fDumpTokens
	0x3beb0,	// fDumpParse
};

const DWORD cmd_6_0_6001_18000[] = {
	0x0bdc3,	// eol
	0x04ad8,	// MSCmdVar
	0x02a2e,	// Lex:text
	0x37d40,	// TmpBuf
	0x0b46c,	// PutStdErr:PutMsg
	0x2e188,	// CurrentBatchFile
	0x4a6c1,	// fOutputUnicode
	0x2e08b,	// fDelayedExpansion
	0x2e08a,	// fEnableExtensions
	0x37d20,	// fDumpTokens
	0x3bd80,	// fDumpParse
};

const DWORD cmd_6_1_7100_0[] = {
	0x0e295,	// eol
	0x04d7c,	// MSCmdVar
	0x01d13,	// Lex:text
	0x36c80,	// TmpBuf
	0x0abe8,	// PutStdErr:PutMsg
	0x2d1e8,	// CurrentBatchFile
	0x36c50,	// fOutputUnicode
	0x2d0cb,	// fDelayedExpansion
	0x2d0ca,	// fEnableExtensions
	0x3ac84,	// fDumpTokens
	0x3afa0,	// fDumpParse
};

const DWORD cmd_6_1_7600_16385[] = {
	0x0e2bd,	// eol
	0x05588,	// MSCmdVar
	0x020b3,	// Lex:text
	0x36be0,	// TmpBuf
	0x0ac18,	// PutStdErr:PutMsg
	0x2d1e8,	// CurrentBatchFile
	0x3b300,	// fOutputUnicode
	0x2d0cb,	// fDelayedExpansion
	0x2d0ca,	// fEnableExtensions
	0x3ae04,	// fDumpTokens
	0x3ae05,	// fDumpParse
};

const DWORD cmd_6_1_7601_17514[] = {
	0x0e8e1,	// eol
	0x055c0,	// MSCmdVar
	0x02043,	// Lex:text
	0x37b60,	// TmpBuf
	0x0b8d8,	// PutStdErr:PutMsg
	0x2e1e8,	// CurrentBatchFile
	0x4a340,	// fOutputUnicode
	0x2e0cb,	// fDelayedExpansion
	0x2e0ca,	// fEnableExtensions
	0x3bd84,	// fDumpTokens
	0x3bd85,	// fDumpParse
};

const DWORD cmd_6_1_7601_23403[] = {
	0x0e8e1,	// eol
	0x055c0,	// MSCmdVar
	0x02043,	// Lex:text
	0x37b60,	// TmpBuf
	0x0b8d8,	// PutStdErr:PutMsg
	0x2e1e8,	// CurrentBatchFile
	0x4a340,	// fOutputUnicode
	0x2e0cb,	// fDelayedExpansion
	0x2e0ca,	// fEnableExtensions
	0x3bd84,	// fDumpTokens
	0x3bd85,	// fDumpParse
};

const DWORD cmd_6_2_8102_0[] = {
	0x0e8e3,	// eol
	0x06d80,	// MSCmdVar
	0x01f2d,	// Lex:text
	0x41120,	// TmpBuf
	0x1294a,	// PutStdErr:PutMsg
	0x3c1a0,	// CurrentBatchFile
	0x4f6e0,	// fOutputUnicode
	0x3c093,	// fDelayedExpansion
	0x3c094,	// fEnableExtensions
	0x4d351,	// fDumpTokens
	0x4d350,	// fDumpParse
};

const DWORD cmd_6_2_9200_16384[] = {
	0x0d847,	// eol
	0x08750,	// MSCmdVar
	0x049df,	// Lex:text
	0x3d160,	// TmpBuf
	0x10c2a,	// PutStdErr:PutMsg
	0x381e0,	// CurrentBatchFile
	0x4baa0,	// fOutputUnicode
	0x380d3,	// fDelayedExpansion
	0x380d4,	// fEnableExtensions
	0x496e1,	// fDumpTokens
	0x496e0,	// fDumpParse
};

const DWORD cmd_6_3_9431_0[] = {
	0x0c423,	// eol
	0x081a8,	// MSCmdVar
	0x03a29,	// Lex:text
	0x33100,	// TmpBuf
	0x111c4,	// PutStdErr:PutMsg
	0x2e1a8,	// CurrentBatchFile
	0x41a20,	// fOutputUnicode
	0x2e093,	// fDelayedExpansion
	0x2e094,	// fEnableExtensions
	0x3f489,	// fDumpTokens
	0x3f488,	// fDumpParse
};

const DWORD cmd_6_3_9431_0u[] = {	// debug version
	0x08690,	// eol
	0x167bc,	// MSCmdVar
	0x14e83,	// Lex:text (EBX)
	0x40fc0,	// TmpBuf
	0x21e29,	// PutStdErr:PutMsg
	0x2f768,	// CurrentBatchFile
	0x2f788,	// fOutputUnicode
	0x2f789,	// fDelayedExpansion
	0x2f78a,	// fEnableExtensions
	0x34024,	// fDumpTokens
	0x33f08,	// fDumpParse
};

const DWORD cmd_6_3_9471_0[] = {
	0x08963,	// eol
	0x0f850,	// MSCmdVar
	0x0c0c9,	// Lex:text
	0x3daa0,	// TmpBuf
	0x05da8,	// PutStdErr:PutMsg
	0x2c768,	// CurrentBatchFile
	0x2c788,	// fOutputUnicode
	0x2c789,	// fDelayedExpansion
	0x2c78a,	// fEnableExtensions
	0x30b04,	// fDumpTokens
	0x309e8,	// fDumpParse
};

const DWORD cmd_6_3_9600_16384[] = {
	0x0bc83,	// eol
	0x080d4,	// MSCmdVar
	0x03e59,	// Lex:text
	0x32100,	// TmpBuf
	0x0fad8,	// PutStdErr:PutMsg
	0x2d1a8,	// CurrentBatchFile
	0x40a20,	// fOutputUnicode
	0x2d093,	// fDelayedExpansion
	0x2d094,	// fEnableExtensions
	0x3e379,	// fDumpTokens
	0x3e378,	// fDumpParse
};

const DWORD cmd_6_3_9600_17415[] = {
	0x0e601,	// eol
	0x08610,	// MSCmdVar
	0x03f75,	// Lex:text
	0x3b200,	// TmpBuf
	0x0cdc8,	// PutStdErr:PutMsg
	0x2e128,	// CurrentBatchFile
	0x419e0,	// fOutputUnicode
	0x2e013,	// fDelayedExpansion
	0x2e014,	// fEnableExtensions
	0x3f420,	// fDumpTokens
	0x3f200,	// fDumpParse

};

#else

const DWORD cmd_10_0_10240_16384[] = {
	0x0f949,	// eol
	0x0cbda,	// MSCmdVar
	0x0b3c3,	// Lex:text
	0x32700,	// TmpBuf
	0x10866,	// PutStdErr:PutMsg
	0x3c9c8,	// CurrentBatchFile
	0x3c9d4,	// fOutputUnicode
	0x3c9d5,	// fDelayedExpansion
	0x3c9d6,	// fEnableExtensions
	0x256c4,	// fDumpTokens
	0x2559c,	// fDumpParse
};

const DWORD cmd_10_0_10586_0[] = {
	0x0f679,	// eol
	0x08824,	// MSCmdVar
	0x0b0c3,	// Lex:text
	0x33700,	// TmpBuf
	0x064b4,	// PutStdErr:PutMsg
	0x3d9c8,	// CurrentBatchFile
	0x3d9d4,	// fOutputUnicode
	0x3d9d5,	// fDelayedExpansion
	0x3d9d6,	// fEnableExtensions
	0x266c4,	// fDumpTokens
	0x2659c,	// fDumpParse
};

const DWORD cmd_10_0_14393_0[] = {
	0x0f2e9,	// eol
	0x08574,	// MSCmdVar
	0x0ad31,	// Lex:text
	0x29530,	// TmpBuf
	0x06424,	// PutStdErr:PutMsg
	0x3d7ac,	// CurrentBatchFile
	0x41b14,	// fOutputUnicode
	0x3d7b4,	// fDelayedExpansion
	0x3d7b5,	// fEnableExtensions
	0x3d684,	// fDumpTokens
	0x3d798,	// fDumpParse
};

const DWORD cmd_10_0_15063_0[] = {
	0x08cf9,	// eol
	0x1088e,	// MSCmdVar
	0x0ea45,	// Lex:text
	0x3fab0,	// TmpBuf
	0x14822,	// PutStdErr:PutMsg
	0x43cd4,	// CurrentBatchFile
	0x48474,	// fOutputUnicode
	0x43cd8,	// fDelayedExpansion
	0x43cd9,	// fEnableExtensions
	0x3f994,	// fDumpTokens
	0x3fa98,	// fDumpParse
};

const DWORD cmd_10_0_16299_15[] = {
	0x1857b,	// eol
	0x0d075,	// MSCmdVar
	0x080c9,	// Lex:text
	0x3da10,	// TmpBuf
	0x0ba29,	// PutStdErr:PutMsg
	0x41c48,	// CurrentBatchFile
	0x46005,	// fOutputUnicode
	0x41c4c,	// fDelayedExpansion
	0x41c4d,	// fEnableExtensions
	0x3d8f4,	// fDumpTokens
	0x3da04,	// fDumpParse
};

const DWORD cmd_10_0_17134_1[] = {
	0x071b9,	// eol
	0x11969,	// MSCmdVar
	0x0f5ef,	// Lex:text
	0x3faa0,	// TmpBuf
	0x0c5c2,	// PutStdErr:PutMsg
	0x43cc4,	// CurrentBatchFile
	0x4805c,	// fOutputUnicode
	0x43cc8,	// fDelayedExpansion
	0x43cc9,	// fEnableExtensions
	0x3f984,	// fDumpTokens
	0x3fa88,	// fDumpParse
};

const DWORD cmd_10_0_17763_1[] = {
	0x08049,	// eol
	0x08ed1,	// MSCmdVar
	0x0ba4a,	// Lex:text
	0x43ae0,	// TmpBuf
	0x12464,	// PutStdErr:PutMsg
	0x35738,	// CurrentBatchFile
	0x4803c,	// fOutputUnicode
	0x35744,	// fDelayedExpansion
	0x35745,	// fEnableExtensions
	0x48045,	// fDumpTokens
	0x48046,	// fDumpParse
};

const DWORD cmd_10_0_18362_1[] = {
	0x10a09,	// eol
	0x0ebf3,	// MSCmdVar
	0x0d00a,	// Lex:text
	0x43ae0,	// TmpBuf
	0x07834,	// PutStdErr:PutMsg
	0x35738,	// CurrentBatchFile
	0x4803c,	// fOutputUnicode
	0x35744,	// fDelayedExpansion
	0x35745,	// fEnableExtensions
	0x48045,	// fDumpTokens
	0x48046,	// fDumpParse
};

const DWORD cmd_5_0_2144_1[] = {
	0x030ac,	// eol
	0x0be4e,	// MSCmdVar
	0x0aeca,	// Lex:text
	0x24320,	// TmpBuf
	0x13962,	// PutStdErr:PutMsg
	0x1cf40,	// CurrentBatchFile
	0x1cf78,	// fOutputUnicode
	0x1cf80,	// fDelayedExpansion
	0x1cf7c,	// fEnableExtensions
	0x1faec,	// fDumpTokens
	0x1f9d8,	// fDumpParse
};

const DWORD cmd_5_0_2195_1600[] = {
	0x030ac,	// eol
	0x0be42,	// MSCmdVar
	0x0aebe,	// Lex:text
	0x24320,	// TmpBuf
	0x13956,	// PutStdErr:PutMsg
	0x1cf40,	// CurrentBatchFile
	0x1cf78,	// fOutputUnicode
	0x1cf80,	// fDelayedExpansion
	0x1cf7c,	// fEnableExtensions
	0x1faec,	// fDumpTokens
	0x1f9d8,	// fDumpParse
};

const DWORD cmd_5_0_2195_2104[] = {
	0x030ac,	// eol
	0x0be3b,	// MSCmdVar
	0x0aeb7,	// Lex:text
	0x24320,	// TmpBuf
	0x13952,	// PutStdErr:PutMsg
	0x1cf40,	// CurrentBatchFile
	0x1cf78,	// fOutputUnicode
	0x1cf80,	// fDelayedExpansion
	0x1cf7c,	// fEnableExtensions
	0x1faec,	// fDumpTokens
	0x1f9d8,	// fDumpParse
};

const DWORD cmd_5_0_2195_4803[] = {
	0x030ac,	// eol
	0x0be92,	// MSCmdVar
	0x0af0e,	// Lex:text
	0x24320,	// TmpBuf
	0x139a9,	// PutStdErr:PutMsg
	0x1cf40,	// CurrentBatchFile
	0x1cf78,	// fOutputUnicode
	0x1cf80,	// fDelayedExpansion
	0x1cf7c,	// fEnableExtensions
	0x1faec,	// fDumpTokens
	0x1f9d8,	// fDumpParse
};

const DWORD cmd_5_0_2195_6656[] = {
	0x030bc,	// eol
	0x0bec6,	// MSCmdVar
	0x0af42,	// Lex:text
	0x24320,	// TmpBuf
	0x139d8,	// PutStdErr:PutMsg
	0x1cf40,	// CurrentBatchFile
	0x1cf78,	// fOutputUnicode
	0x1cf80,	// fDelayedExpansion
	0x1cf7c,	// fEnableExtensions
	0x1faec,	// fDumpTokens
	0x1f9d8,	// fDumpParse
};

const DWORD cmd_5_0_2195_6995[] = {
	0x03173,	// eol
	0x0bf84,	// MSCmdVar
	0x0b000,	// Lex:text
	0x24320,	// TmpBuf
	0x13a96,	// PutStdErr:PutMsg
	0x1cf40,	// CurrentBatchFile
	0x1cf78,	// fOutputUnicode
	0x1cf80,	// fDelayedExpansion
	0x1cf7c,	// fEnableExtensions
	0x1faec,	// fDumpTokens
	0x1f9d8,	// fDumpParse
};

const DWORD cmd_5_1_2600_0[] = {
	0x06400,	// eol
	0x042b3,	// MSCmdVar
	0x0308b,	// Lex:text
	0x220a0,	// TmpBuf
	0x0b564,	// PutStdErr:PutMsg
	0x2cac4,	// CurrentBatchFile
	0x30c3c,	// fOutputUnicode
	0x30b80,	// fDelayedExpansion
	0x2ca64,	// fEnableExtensions
	0x31688,	// fDumpTokens
	0x31704,	// fDumpParse
};

const DWORD cmd_5_1_2600_2180[] = {
	0x0a8cb,	// eol
	0x088f9,	// MSCmdVar
	0x025b3,	// Lex:text
	0x250a0,	// TmpBuf
	0x06088,	// PutStdErr:PutMsg
	0x2fac4,	// CurrentBatchFile
	0x3484c,	// fOutputUnicode
	0x33b7c,	// fDelayedExpansion
	0x2fa64,	// fEnableExtensions
	0x346c8,	// fDumpTokens
	0x346c9,	// fDumpParse
};

const DWORD cmd_5_1_2600_3311[] = {
	0x0aa06,	// eol
	0x06e7d,	// MSCmdVar
	0x0259b,	// Lex:text
	0x250a0,	// TmpBuf
	0x05fce,	// PutStdErr:PutMsg
	0x2fac4,	// CurrentBatchFile
	0x34b74,	// fOutputUnicode
	0x33b7c,	// fDelayedExpansion
	0x2fa64,	// fEnableExtensions
	0x346c8,	// fDumpTokens
	0x346c9,	// fDumpParse
};

const DWORD cmd_5_1_2600_5512[] = {
	0x0aa06,	// eol
	0x06e7d,	// MSCmdVar
	0x0259b,	// Lex:text
	0x250a0,	// TmpBuf
	0x05fce,	// PutStdErr:PutMsg
	0x2fac4,	// CurrentBatchFile
	0x34b74,	// fOutputUnicode
	0x33b7c,	// fDelayedExpansion
	0x2fa64,	// fEnableExtensions
	0x346c8,	// fDumpTokens
	0x346c9,	// fDumpParse
};

const DWORD cmd_5_2_3790_0[] = {
	0x06a3c,	// eol
	0x038ed,	// MSCmdVar
	0x028c6,	// Lex:text
	0x20ba0,	// TmpBuf
	0x0b0ec,	// PutStdErr:PutMsg
	0x20044,	// CurrentBatchFile
	0x32f6c,	// fOutputUnicode
	0x20048,	// fDelayedExpansion
	0x20041,	// fEnableExtensions
	0x28d80,	// fDumpTokens
	0x28db4,	// fDumpParse
};

const DWORD cmd_5_2_3790_1830[] = {
	0x0cda7,	// eol
	0x07efa,	// MSCmdVar
	0x02227,	// Lex:text
	0x25060,	// TmpBuf
	0x06e2e,	// PutStdErr:PutMsg
	0x2fa7c,	// CurrentBatchFile
	0x347cc,	// fOutputUnicode
	0x34334,	// fDelayedExpansion
	0x2fa24,	// fEnableExtensions
	0x346c1,	// fDumpTokens
	0x346c0,	// fDumpParse
};

const DWORD cmd_5_2_3790_3959[] = {
	0x0ae3b,	// eol
	0x05425,	// MSCmdVar
	0x0281f,	// Lex:text
	0x25060,	// TmpBuf
	0x050e2,	// PutStdErr:PutMsg
	0x2fa7c,	// CurrentBatchFile
	0x3cb20,	// fOutputUnicode
	0x33b3c,	// fDelayedExpansion
	0x2fa24,	// fEnableExtensions
	0x34455,	// fDumpTokens
	0x34454,	// fDumpParse
};

const DWORD cmd_6_0_6000_16386[] = {
	0x088fe,	// eol
	0x052b7,	// MSCmdVar
	0x02d22,	// Lex:text
	0x38640,	// TmpBuf
	0x05908,	// PutStdErr:PutMsg
	0x24074,	// CurrentBatchFile
	0x4064c,	// fOutputUnicode
	0x24042,	// fDelayedExpansion
	0x24041,	// fEnableExtensions
	0x40660,	// fDumpTokens
	0x40661,	// fDumpParse
};

const DWORD cmd_6_0_6001_18000[] = {
	0x0a3db,	// eol
	0x04a4f,	// MSCmdVar
	0x028fb,	// Lex:text
	0x385e0,	// TmpBuf
	0x080fb,	// PutStdErr:PutMsg
	0x24074,	// CurrentBatchFile
	0x405ec,	// fOutputUnicode
	0x24042,	// fDelayedExpansion
	0x24041,	// fEnableExtensions
	0x40601,	// fDumpTokens
	0x40600,	// fDumpParse
};

const DWORD cmd_6_1_7100_0[] = {
	0x0e4b2,	// eol
	0x057db,	// MSCmdVar
	0x02737,	// Lex:text
	0x38600,	// TmpBuf
	0x0641c,	// PutStdErr:PutMsg
	0x240b4,	// CurrentBatchFile
	0x40624,	// fOutputUnicode
	0x24082,	// fDelayedExpansion
	0x24081,	// fEnableExtensions
	0x40618,	// fDumpTokens
	0x40619,	// fDumpParse
};

const DWORD cmd_6_1_7600_16385[] = {
	0x0e7f9,	// eol
	0x06c7e,	// MSCmdVar
	0x0245d,	// Lex:text
	0x38600,	// TmpBuf
	0x0aba4,	// PutStdErr:PutMsg
	0x240b4,	// CurrentBatchFile
	0x40624,	// fOutputUnicode
	0x24082,	// fDelayedExpansion
	0x24081,	// fEnableExtensions
	0x40618,	// fDumpTokens
	0x40619,	// fDumpParse
};

const DWORD cmd_6_1_7601_17514[] = {
	0x0e485,	// eol
	0x05129,	// MSCmdVar
	0x0285d,	// Lex:text
	0x38640,	// TmpBuf
	0x06d5d,	// PutStdErr:PutMsg
	0x240b4,	// CurrentBatchFile
	0x40668,	// fOutputUnicode
	0x24082,	// fDelayedExpansion
	0x24081,	// fEnableExtensions
	0x40658,	// fDumpTokens
	0x40660,	// fDumpParse
};

const DWORD cmd_6_1_7601_23403[] = {
	0x0e485,	// eol
	0x05129,	// MSCmdVar
	0x0285d,	// Lex:text
	0x38640,	// TmpBuf
	0x06d5d,	// PutStdErr:PutMsg
	0x240b4,	// CurrentBatchFile
	0x40668,	// fOutputUnicode
	0x24082,	// fDelayedExpansion
	0x24081,	// fEnableExtensions
	0x40658,	// fDumpTokens
	0x40660,	// fDumpParse
};

const DWORD cmd_6_2_8102_0[] = {
	0x0d647,	// eol
	0x07f0c,	// MSCmdVar
	0x02986,	// Lex:text
	0x3fa60,	// TmpBuf
	0x187d9,	// PutStdErr:PutMsg
	0x3008c,	// CurrentBatchFile
	0x4ba72,	// fOutputUnicode
	0x30043,	// fDelayedExpansion
	0x30044,	// fEnableExtensions
	0x4ba70,	// fDumpTokens
	0x4ba71,	// fDumpParse
};

const DWORD cmd_6_2_9200_16384[] = {
	0x0c3e4,	// eol
	0x0777a,	// MSCmdVar
	0x042ff,	// Lex:text
	0x3dac0,	// TmpBuf
	0x0f821,	// PutStdErr:PutMsg
	0x2e0cc,	// CurrentBatchFile
	0x49ad2,	// fOutputUnicode
	0x2e083,	// fDelayedExpansion
	0x2e084,	// fEnableExtensions
	0x49ad0,	// fDumpTokens
	0x49ad1,	// fDumpParse
};

const DWORD cmd_6_3_9431_0[] = {
	0x0a00b,	// eol
	0x0686d,	// MSCmdVar
	0x03824,	// Lex:text
	0x34840,	// TmpBuf
	0x0d646,	// PutStdErr:PutMsg
	0x25094,	// CurrentBatchFile
	0x40852,	// fOutputUnicode
	0x25043,	// fDelayedExpansion
	0x25044,	// fEnableExtensions
	0x40851,	// fDumpTokens
	0x40850,	// fDumpParse
};

const DWORD cmd_6_3_9431_0u[] = {	// debug version
	0x07b68,	// eol
	0x13ccb,	// MSCmdVar
	0x126f6,	// Lex:text (ESI)
	0x39a40,	// TmpBuf
	0x1d8a2,	// PutStdErr:PutMsg
	0x2849c,	// CurrentBatchFile
	0x284b4,	// fOutputUnicode
	0x284b5,	// fDelayedExpansion
	0x284b6,	// fEnableExtensions
	0x2ca04,	// fDumpTokens
	0x2c8dc,	// fDumpParse
};

const DWORD cmd_6_3_9471_0[] = {
	0x07049,	// eol
	0x0d06a,	// MSCmdVar
	0x0a532,	// Lex:text
	0x36800,	// TmpBuf
	0x1269e,	// PutStdErr:PutMsg
	0x2547c,	// CurrentBatchFile
	0x25494,	// fOutputUnicode
	0x25495,	// fDelayedExpansion
	0x25496,	// fEnableExtensions
	0x297c4,	// fDumpTokens
	0x2969c,	// fDumpParse
};

const DWORD cmd_6_3_9600_16384[] = {
	0x093b6,	// eol
	0x06ce7,	// MSCmdVar
	0x037f4,	// Lex:text
	0x34840,	// TmpBuf
	0x0e472,	// PutStdErr:PutMsg
	0x25094,	// CurrentBatchFile
	0x40852,	// fOutputUnicode
	0x25043,	// fDelayedExpansion
	0x25044,	// fEnableExtensions
	0x40851,	// fDumpTokens
	0x40850,	// fDumpParse
};

const DWORD cmd_6_3_9600_17415[] = {
	0x0cfa7,	// eol
	0x0632d,	// MSCmdVar
	0x03a0e,	// Lex:text
	0x35800,	// TmpBuf
	0x0bd22,	// PutStdErr:PutMsg
	0x26054,	// CurrentBatchFile
	0x41812,	// fOutputUnicode
	0x26003,	// fDelayedExpansion
	0x26004,	// fEnableExtensions
	0x41811,	// fDumpTokens
	0x41810,	// fDumpParse
};

#endif

#define CMD(a,b,c,d) \
	{ (a << 16) | b, (c << 16) | d, cmd_ ## a ## _ ## b ## _ ## c ## _ ## d },

const struct sCMD cmd_versions[] = {
#ifdef _WIN64
	CMD(10,0,18362,1)
	CMD(10,0,17763,1)
	CMD(10,0,17134,1)
	CMD(10,0,16299,15)
	CMD(10,0,15063,0)
	CMD(10,0,14393,0)
	CMD(10,0,10586,0)
	CMD(10,0,10240,16384)
	CMD(6,3,9600,17415)
	CMD(6,3,9600,16384)
	CMD(6,3,9471,0)
	CMD(6,3,9431,0)
	CMD(6,3,9431,0u)
	CMD(6,2,9200,16384)
	CMD(6,2,8102,0)
	CMD(6,1,7601,23403)
	CMD(6,1,7601,17514)
	CMD(6,1,7600,16385)
	CMD(6,1,7100,0)
	CMD(6,0,6001,18000)
	CMD(6,0,6000,16386)
	CMD(5,2,3790,3959)
	CMD(5,2,3790,1830)
#else
	CMD(10,0,18362,1)
	CMD(10,0,17763,1)
	CMD(10,0,17134,1)
	CMD(10,0,16299,15)
	CMD(10,0,15063,0)
	CMD(10,0,14393,0)
	CMD(10,0,10586,0)
	CMD(10,0,10240,16384)
	CMD(6,3,9600,17415)
	CMD(6,3,9600,16384)
	CMD(6,3,9471,0)
	CMD(6,3,9431,0)
	CMD(6,3,9431,0u)
	CMD(6,2,9200,16384)
	CMD(6,2,8102,0)
	CMD(6,1,7601,23403)
	CMD(6,1,7601,17514)
	CMD(6,1,7600,16385)
	CMD(6,1,7100,0)
	CMD(6,0,6001,18000)
	CMD(6,0,6000,16386)
	CMD(5,2,3790,3959)
	CMD(5,2,3790,1830)
	CMD(5,2,3790,0)
	CMD(5,1,2600,5512)
	CMD(5,1,2600,3311)
	CMD(5,1,2600,2180)
	CMD(5,1,2600,0)
	CMD(5,0,2195,6995)
	CMD(5,0,2195,6656)
	CMD(5,0,2195,4803)
	CMD(5,0,2195,2104)
	CMD(5,0,2195,1600)
	CMD(5,0,2144,1)
#endif
	{ 0, 0, NULL }
};
