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
	0,			// ForMkstr
	0,			// ForResize
	0,			// EchoFlag
	0,			// CheckHelp first arg
	0,			// cmd_printf
	0,			// (CtrlCAbort:)PromptUser
	0,			// mkstr
	0,			// FreeStack
	0,			// DCount
	0,			// SFWork:mkstr
	0,			// SFWork:resize
	0,			// SFWork:saved
	0,			// DESubWork:FreeStr
	0,			// GotoFlag
	0,			// FOR /F begin
	0,			// FOR /F end
	0,			// ParseFor initial token
	0,			// LexBufPtr
	0,			// LexBuffer end
	0,			// ParseFor /F options
	0,			// TokLen
	0,			// Goto:EOF
	0,			// Goto pos
	0,			// Goto start
	0,			// CallWork:resize
	0,			// MyGetEnvVarPtr
	0,			// DisplayEnv
	0,			// DisplayEnvVariable
	0,			// Dispatch:after delayed expansion
};

#ifdef _WIN64

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
	0x1d2f8,	// ForMkstr
	0x1d3d1,	// ForResize
	0x53820,	// EchoFlag
	0x0584a,	// CheckHelp first arg
	0x33da0,	// cmd_printf
	0x32ac3,	// PromptUser
	0x05980,	// mkstr
	0x0b830,	// FreeStack
	0x42c50,	// DCount
	0x07d4d,	// SFWork:mkstr (inline)
	0x07e0f,	// SFWork:resize
	0x0980c,	// SFWork:saved
	0x27b53,	// DESubWork:FreeStr (inline)
	0x42c40,	// GotoFlag
	0x07388,	// FOR /F begin
	0x075b4,	// FOR /F end
	0x0acd9,	// ParseFor initial token
	0x4b3a0,	// LexBufPtr
	0x4f3c2,	// LexBuffer end
	0x0ae3d,	// ParseFor /F options
	0x4b38c,	// TokLen
	0x08ee1,	// Goto:EOF
	0x08e7d,	// Goto pos
	0x08e40,	// Goto start
	0x13b68,	// CallWork:resize
	0x06f90,	// MyGetEnvVarPtr
	0x13f2f,	// DisplayEnv
	0x14008,	// DisplayEnvVariable
	0x0175e,	// Dispatch:after delayed expansion
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
	0x1d2f8,	// ForMkstr
	0x1d3d1,	// ForResize
	0x53820,	// EchoFlag
	0x0586a,	// CheckHelp first arg
	0x33db0,	// cmd_printf
	0x32ad3,	// PromptUser
	0x059a0,	// mkstr
	0x0b850,	// FreeStack
	0x42c50,	// DCount
	0x07d6d,	// SFWork:mkstr (inline)
	0x07e2f,	// SFWork:resize
	0x0982c,	// SFWork:saved
	0x27b53,	// DESubWork:FreeStr (inline)
	0x42c40,	// GotoFlag
	0x073a8,	// FOR /F begin
	0x075d4,	// FOR /F end
	0x0acf9,	// ParseFor initial token
	0x4b3a0,	// LexBufPtr
	0x4f3c2,	// LexBuffer end
	0x0ae5d,	// ParseFor /F options
	0x4b38c,	// TokLen
	0x08f01,	// Goto:EOF
	0x08e9d,	// Goto pos
	0x08e60,	// Goto start
	0x13b68,	// CallWork:resize
	0x06fb0,	// MyGetEnvVarPtr
	0x13f2f,	// DisplayEnv
	0x14008,	// DisplayEnvVariable
	0x0175e,	// Dispatch:after delayed expansion
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
	0x12622,	// ForMkstr
	0x126bb,	// ForResize
	0x2e0a8,	// EchoFlag
	0x044b2,	// CheckHelp first arg
	0x04850,	// cmd_printf
	0x282d0,	// PromptUser
	0x02820,	// mkstr
	0x032e0,	// FreeStack
	0x2e0e4,	// DCount
	0x0b0c8,	// SFWork:mkstr
	0x0b140,	// SFWork:resize
	0x0b402,	// SFWork:saved
	0x08301,	// DESubWork:FreeStr
	0x2e089,	// GotoFlag
	0x0bbe4,	// FOR /F begin
	0x0bdb5,	// FOR /F end
	0x0b53f,	// ParseFor initial token
	0x2e220,	// LexBufPtr
	0x37302,	// LexBuffer end
	0x0c1d7,	// ParseFor /F options
	0x2e124,	// TokLen
	0x07e8e,	// Goto:EOF
	0x07e69,	// Goto pos
	0x07e3c,	// Goto start
	0x194b0,	// CallWork:resize
	0x05e40,	// MyGetEnvVarPtr
	0x1a086,	// DisplayEnv
	0x1a075,	// DisplayEnvVariable
	0x01b0a,	// Dispatch:after delayed expansion
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
	0x1235a,	// ForMkstr
	0x123f3,	// ForResize
	0x2e0a4,	// EchoFlag
	0x014ae,	// CheckHelp first arg
	0x04274,	// cmd_printf
	0x2353c,	// PromptUser
	0x01588,	// mkstr
	0x02e50,	// FreeStack
	0x2e0e0,	// DCount
	0x0b808,	// SFWork:mkstr
	0x0b87c,	// SFWork:resize
	0x0b8a8,	// SFWork:saved
	0x0df7a,	// DESubWork:FreeStr
	0x2e089,	// GotoFlag
	0x0c24e,	// FOR /F begin
	0x0c3fb,	// FOR /F end
	0x0ba8f,	// ParseFor initial token
	0x2e218,	// LexBufPtr
	0x372e2,	// LexBuffer end
	0x0c478,	// ParseFor /F options
	0x2e120,	// TokLen
	0x0872a,	// Goto:EOF
	0x08705,	// Goto pos
	0x086d8,	// Goto start
	0x14157,	// CallWork:resize
	0x093b0,	// MyGetEnvVarPtr
	0x15181,	// DisplayEnv
	0x15170,	// DisplayEnvVariable
	0x0105a,	// Dispatch:after delayed expansion
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
	0x103c7,	// ForMkstr
	0x10425,	// ForResize
	0x2d0e8,	// EchoFlag
	0x04f2f,	// CheckHelp first arg
	0x05dec,	// cmd_printf
	0x24b00,	// PromptUser
	0x013e8,	// mkstr
	0x02240,	// FreeStack
	0x2d128,	// DCount
	0x0d3a8,	// SFWork:mkstr
	0x0d431,	// SFWork:resize
	0x0d40d,	// SFWork:saved
	0x04bf5,	// DESubWork:FreeStr
	0x2d0c9,	// GotoFlag
	0x0d893,	// FOR /F begin
	0x0da8c,	// FOR /F end
	0x0de16,	// ParseFor initial token
	0x2d278,	// LexBufPtr
	0x36362,	// LexBuffer end
	0x0e1fa,	// ParseFor /F options
	0x2d17c,	// TokLen
	0x065ae,	// Goto:EOF
	0x06585,	// Goto pos
	0x06558,	// Goto start
	0x1706b,	// CallWork:resize
	0x0410c,	// MyGetEnvVarPtr
	0x1801f,	// DisplayEnv
	0x1800e,	// DisplayEnvVariable
	0x0111e,	// Dispatch:after delayed expansion
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
	0x10562,	// ForMkstr
	0x105c0,	// ForResize
	0x2d0e8,	// EchoFlag
	0x039bb,	// CheckHelp first arg
	0x05dd4,	// cmd_printf
	0x24b50,	// PromptUser
	0x01118,	// mkstr
	0x01360,	// FreeStack
	0x2d128,	// DCount
	0x0d3d4,	// SFWork:mkstr
	0x0d45d,	// SFWork:resize
	0x0d439,	// SFWork:saved
	0x04b70,	// DESubWork:FreeStr
	0x2d0c9,	// GotoFlag
	0x0df6d,	// FOR /F begin
	0x0e166,	// FOR /F end
	0x0d8f2,	// ParseFor initial token
	0x2d278,	// LexBufPtr
	0x36362,	// LexBuffer end
	0x0e223,	// ParseFor /F options
	0x2d17c,	// TokLen
	0x0675e,	// Goto:EOF
	0x06735,	// Goto pos
	0x06708,	// Goto start
	0x1735b,	// CallWork:resize
	0x05228,	// MyGetEnvVarPtr
	0x183cc,	// DisplayEnv
	0x183bb,	// DisplayEnvVariable
	0x012b6,	// Dispatch:after delayed expansion
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
	0x0febc,	// ForMkstr
	0x0ff1a,	// ForResize
	0x2e0e8,	// EchoFlag
	0x035b3,	// CheckHelp first arg
	0x05fc4,	// cmd_printf
	0x24de0,	// PromptUser
	0x016dc,	// mkstr
	0x017e0,	// FreeStack
	0x2e128,	// DCount
	0x0da58,	// SFWork:mkstr
	0x0dae1,	// SFWork:resize
	0x0dabd,	// SFWork:saved
	0x04425,	// DESubWork:FreeStr
	0x2e0c9,	// GotoFlag
	0x0e591,	// FOR /F begin
	0x0e78a,	// FOR /F end
	0x0df06,	// ParseFor initial token
	0x2e278,	// LexBufPtr
	0x37362,	// LexBuffer end
	0x0e847,	// ParseFor /F options
	0x2e17c,	// TokLen
	0x06712,	// Goto:EOF
	0x066e9,	// Goto pos
	0x066bc,	// Goto start
	0x17470,	// CallWork:resize
	0x04a34,	// MyGetEnvVarPtr
	0x18431,	// DisplayEnv
	0x18420,	// DisplayEnvVariable
	0x01136,	// Dispatch:after delayed expansion
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
	0x0febc,	// ForMkstr
	0x0ff1a,	// ForResize
	0x2e0e8,	// EchoFlag
	0x035b3,	// CheckHelp first arg
	0x05fc4,	// cmd_printf
	0x24de0,	// PromptUser
	0x016dc,	// mkstr
	0x017e0,	// FreeStack
	0x2e128,	// DCount
	0x0da58,	// SFWork:mkstr
	0x0dae1,	// SFWork:resize
	0x0dabd,	// SFWork:saved
	0x04425,	// DESubWork:FreeStr
	0x2ec09,	// GotoFlag
	0x0e591,	// FOR /F begin
	0x0e78a,	// FOR /F end
	0x0df06,	// ParseFor initial token
	0x2e278,	// LexBufPtr
	0x37362,	// LexBuffer end
	0x0e847,	// ParseFor /F options
	0x2e17c,	// TokLen
	0x06712,	// Goto:EOF
	0x066e9,	// Goto pos
	0x066bc,	// Goto start
	0x17470,	// CallWork:resize
	0x04a34,	// MyGetEnvVarPtr
	0x18431,	// DisplayEnv
	0x18420,	// DisplayEnvVariable
	0x01136,	// Dispatch:after delayed expansion
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
	0x0ef23,	// ForMkstr
	0x0ef95,	// ForResize
	0x3c09c,	// EchoFlag
	0x0ee1f,	// CheckHelp first arg
	0x074d0,	// cmd_printf
	0x25818,	// PromptUser
	0x07a70,	// mkstr
	0x0d020,	// FreeStack
	0x3c1b8,	// DCount
	0x0f228,	// SFWork:mkstr (inline)
	0x0f31b,	// SFWork:resize (inline)
	0x0f20e,	// SFWork:saved
	0x0a02b,	// DESubWork:FreeStr (inline)
	0x3c092,	// GotoFlag
	0x0d6a6,	// FOR /F begin
	0x0d905,	// FOR /F end (opposite jump)
	0x0e43a,	// ParseFor initial token
	0x3c218,	// LexBufPtr
	0x49122,	// LexBuffer end
	0x0e7b0,	// ParseFor /F options
	0x3c124,	// TokLen
	0x0a30f,	// Goto:EOF
	0x0a2b2,	// Goto pos
	0x0a251,	// Goto start
	0x27e0d,	// CallWork:resize
	0x03670,	// MyGetEnvVarPtr
	0x29710,	// DisplayEnv
	0x296fe,	// DisplayEnvVariable
	0x0113d,	// Dispatch:after delayed expansion
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
	0x1102a,	// ForMkstr
	0x1109b,	// ForResize
	0x380dc,	// EchoFlag
	0x0e2db,	// CheckHelp first arg
	0x07e20,	// cmd_printf
	0x26640,	// PromptUser
	0x05c20,	// mkstr
	0x07700,	// FreeStack
	0x381f8,	// DCount
	0x0d486,	// SFWork:mkstr (inline)
	0x0d56c,	// SFWork:resize (inline)
	0x0d469,	// SFWork:saved
	0x0a77e,	// DESubWork:FreeStr (inline)
	0x380d2,	// GotoFlag
	0x0dc2d,	// FOR /F begin
	0x0de7e,	// FOR /F end
	0x0e8fc,	// ParseFor initial token
	0x38258,	// LexBufPtr
	0x45162,	// LexBuffer end
	0x0ebba,	// ParseFor /F options
	0x38164,	// TokLen
	0x09c0a,	// Goto:EOF
	0x09bad,	// Goto pos
	0x09b54,	// Goto start
	0x286d4,	// CallWork:resize
	0x02a30,	// MyGetEnvVarPtr
	0x2a050,	// DisplayEnv
	0x2a03e,	// DisplayEnvVariable
	0x01359,	// Dispatch:after delayed expansion
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
	0x0dcef,	// ForMkstr
	0x0dd45,	// ForResize
	0x2e0a8,	// EchoFlag
	0x07646,	// CheckHelp first arg
	0x08458,	// cmd_printf
	0x280a0,	// PromptUser
	0x01e40,	// mkstr
	0x076e8,	// FreeStack
	0x2e0ec,	// DCount
	0x0bb26,	// SFWork:mkstr (inline)
	0x0bc0c,	// SFWork:resize (inline)
	0x0bb09,	// SFWork:saved
	0x0a0f9,	// DESubWork:FreeStr
	0x2e092,	// GotoFlag
	0x0c277,	// FOR /F begin
	0x0c046,	// FOR /F end
	0x0c918,	// ParseFor initial token
	0x2e218,	// LexBufPtr
	0x3b102,	// LexBuffer end
	0x0cb74,	// ParseFor /F options
	0x2e12c,	// TokLen
	0x090d9,	// Goto:EOF
	0x090b4,	// Goto pos
	0x0907a,	// Goto start
	0x203d9,	// CallWork:resize
	0x032e0,	// MyGetEnvVarPtr
	0x1f6cc,	// DisplayEnv
	0x18607,	// DisplayEnvVariable
	0x0133d,	// Dispatch:after delayed expansion
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
	0x092b9,	// ForMkstr
	0x0931d,	// ForResize
	0x2f018,	// EchoFlag
	0x2362d,	// CheckHelp first arg
	0x246ec,	// cmd_printf
	0x23252,	// PromptUser
	0x19444,	// mkstr
	0x1915c,	// FreeStack
	0x33ce8,	// DCount
	0x09642,	// SFWork:mkstr
	0x09973,	// SFWork:resize
	0x095f2,	// SFWork:saved
	0x18f74,	// DESubWork:FreeStr
	0x2f760,	// GotoFlag
	0x08e1b,	// FOR /F begin
	0x09006,	// FOR /F end
	0x1b4be,	// ParseFor initial token
	0x34058,	// LexBufPtr
	0x3cca2,	// LexBuffer end
	0x1b551,	// ParseFor /F options
	0x33f04,	// TokLen
	0x09dd7,	// Goto:EOF
	0x09db0,	// Goto pos
	0x09d51,	// Goto start
	0x0ae39,	// CallWork:resize
	0x0d74c,	// MyGetEnvVarPtr
	0x0ea6e,	// DisplayEnv
	0x0ea4c,	// DisplayEnvVariable
	0x17619,	// Dispatch:after delayed expansion
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
	0x14b4f,	// ForMkstr
	0x14ba5,	// ForResize
	0x2c018,	// EchoFlag
	0x0e502,	// CheckHelp first arg
	0x10a98,	// cmd_printf
	0x26f34,	// PromptUser
	0x0b760,	// mkstr
	0x11f44,	// FreeStack
	0x2c7c0,	// DCount
	0x105f6,	// SFWork:mkstr (inline)
	0x106dc,	// SFWork:resize (inline)
	0x105d9,	// SFWork:saved
	0x0d679,	// DESubWork:FreeStr
	0x2c760,	// GotoFlag
	0x08d0d,	// FOR /F begin
	0x08ebb,	// FOR /F end
	0x08054,	// ParseFor initial token
	0x30b38,	// LexBufPtr
	0x39782,	// LexBuffer end
	0x081eb,	// ParseFor /F options
	0x309e4,	// TokLen
	0x09c29,	// Goto:EOF
	0x09c04,	// Goto pos
	0x09bca,	// Goto start
	0x1f4cb,	// CallWork:resize
	0x123e0,	// MyGetEnvVarPtr
	0x1ebee,	// DisplayEnv
	0x0fe0d,	// DisplayEnvVariable
	0x0d3bd,	// Dispatch:after delayed expansion
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
	0x0dc33,	// ForMkstr
	0x0dc89,	// ForResize
	0x2d0a8,	// EchoFlag
	0x07626,	// CheckHelp first arg
	0x07c88,	// cmd_printf
	0x27fe0,	// PromptUser
	0x01ed0,	// mkstr
	0x076d4,	// FreeStack
	0x2d0ec,	// DCount
	0x0b9d6,	// SFWork:mkstr (inline)
	0x0babc,	// SFWork:resize (inline)
	0x0b9b9,	// SFWork:saved
	0x0a2a2,	// DESubWork:FreeStr
	0x2d092,	// GotoFlag
	0x0c02b,	// FOR /F begin
	0x0c1d0,	// FOR /F end
	0x0c7e4,	// ParseFor initial token
	0x2d218,	// LexBufPtr
	0x3a102,	// LexBuffer end
	0x0c9b0,	// ParseFor /F options
	0x2d12c,	// TokLen
	0x08b79,	// Goto:EOF
	0x08b54,	// Goto pos
	0x08b1a,	// Goto start
	0x20894,	// CallWork:resize
	0x03420,	// MyGetEnvVarPtr
	0x1ff78,	// DisplayEnv
	0x1874b,	// DisplayEnvVariable
	0x0133d,	// Dispatch:after delayed expansion
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
	0x0fde1,	// ForMkstr
	0x0fe37,	// ForResize
	0x2e028,	// EchoFlag
	0x074b1,	// CheckHelp first arg
	0x087a8,	// cmd_printf
	0x2815c,	// PromptUser
	0x018d0,	// mkstr
	0x02110,	// FreeStack
	0x2e06c,	// DCount
	0x0d9a3,	// SFWork:mkstr
	0x0da21,	// SFWork:resize
	0x0d97d,	// SFWork:saved
	0x08e64,	// DESubWork:FreeStr
	0x2e012,	// GotoFlag
	0x0e376,	// FOR /F begin
	0x0e514,	// FOR /F end
	0x0dd64,	// ParseFor initial token
	0x2e198,	// LexBufPtr
	0x36f82,	// LexBuffer end
	0x0df30,	// ParseFor /F options
	0x2e0ac,	// TokLen
	0x09c19,	// Goto:EOF
	0x09bf4,	// Goto pos
	0x09bba,	// Goto start
	0x1a9ff,	// CallWork:resize
	0x02e90,	// MyGetEnvVarPtr
	0x1dd4f,	// DisplayEnv
	0x18be2,	// DisplayEnvVariable
	0x0135d,	// Dispatch:after delayed expansion
};

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
	0x01e9a,	// ForMkstr
	0x01ef3,	// ForResize
	0x2f020,	// EchoFlag
	0x07f3d,	// CheckHelp first arg
	0x10054,	// cmd_printf
	0x23dd4,	// PromptUser
	0x0b6c0,	// mkstr
	0x0b960,	// FreeStack
	0x4b138,	// DCount
	0x08533,	// SFWork:mkstr
	0x085b5,	// SFWork:resize
	0x0850d,	// SFWork:saved
	0x0772e,	// DESubWork:FreeStr
	0x46b88,	// GotoFlag
	0x054e5,	// FOR /F begin
	0x0569d,	// FOR /F end
	0x03278,	// ParseFor initial token
	0x2f998,	// LexBufPtr
	0x385e2,	// LexBuffer end
	0x0340f,	// ParseFor /F options
	0x2f844,	// TokLen
	0x09a59,	// Goto:EOF
	0x09a34,	// Goto pos
	0x099fa,	// Goto start
	0x1939f,	// CallWork:resize
	0x0f900,	// MyGetEnvVarPtr
	0x16250,	// DisplayEnv
	0x06bf9,	// DisplayEnvVariable
	0x0734d,	// Dispatch:after delayed expansion
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
	0x0162a,	// ForMkstr
	0x01683,	// ForResize
	0x30020,	// EchoFlag
	0x0bafd,	// CheckHelp first arg
	0x0de08,	// cmd_printf
	0x24c2c,	// PromptUser
	0x0ac90,	// mkstr
	0x0ea60,	// FreeStack
	0x4c158,	// DCount
	0x071a3,	// SFWork:mkstr
	0x07225,	// SFWork:resize
	0x0717d,	// SFWork:saved
	0x0b2d8,	// DESubWork:FreeStr
	0x47ba8,	// GotoFlag
	0x05721,	// FOR /F begin
	0x058c3,	// FOR /F end
	0x117a8,	// ParseFor initial token
	0x309b8,	// LexBufPtr
	0x39602,	// LexBuffer end
	0x1193f,	// ParseFor /F options
	0x30864,	// TokLen
	0x08ab9,	// Goto:EOF
	0x08a94,	// Goto pos
	0x08a5a,	// Goto start
	0x1a63f,	// CallWork:resize
	0x0eff0,	// MyGetEnvVarPtr
	0x19f8c,	// DisplayEnv
	0x0c5c9,	// DisplayEnvVariable
	0x0af0d,	// Dispatch:after delayed expansion
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
	0x026ea,	// ForMkstr
	0x02743,	// ForResize
	0x2f020,	// EchoFlag
	0x0ce3d,	// CheckHelp first arg
	0x0ed0c,	// cmd_printf
	0x248fc,	// PromptUser
	0x0b530,	// mkstr
	0x0bc30,	// FreeStack
	0x46d58,	// DCount
	0x0d15e,	// SFWork:mkstr (inline)
	0x0d215,	// SFWork:resize
	0x0d13d,	// SFWork:saved
	0x0c62a,	// DESubWork:FreeStr
	0x30350,	// GotoFlag
	0x0fff2,	// FOR /F begin
	0x10193,	// FOR /F end
	0x149d8,	// ParseFor initial token
	0x3e8a8,	// LexBufPtr
	0x3e8a2,	// LexBuffer end
	0x14b6f,	// ParseFor /F options
	0x46c00,	// TokLen
	0x09db1,	// Goto:EOF
	0x09d8c,	// Goto pos
	0x09d5a,	// Goto start
	0x187b9,	// CallWork:resize
	0x10700,	// MyGetEnvVarPtr
	0x1a6a2,	// DisplayEnv
	0x0e269,	// DisplayEnvVariable
	0x0c25d,	// Dispatch:after delayed expansion
};

const DWORD cmd_10_0_15063_0[] = {
	0x03794,	// eol
	0x10be8,	// MSCmdVar
	0x0dd3a,	// Lex:text
	0x4c080,	// TmpBuf
	0x14398,	// PutStdErr:PutMsg
	0x502b0,	// CurrentBatchFile
	0x54694,	// fOutputUnicode
	0x502b8,	// fDelayedExpansion
	0x502b9,	// fEnableExtensions
	0x4bf64,	// fDumpTokens
	0x4c070,	// fDumpParse
	0x055c7,	// ForMkstr
	0x05620,	// ForResize
	0x39118,	// EchoFlag
	0x101e5,	// CheckHelp first arg
	0x119dc,	// cmd_printf
	0x2af05,	// PromptUser
	0x0f340,	// mkstr
	0x12984,	// FreeStack
	0x502d0,	// DCount
	0x04449,	// SFWork:mkstr (inline)
	0x04504,	// SFWork:resize
	0x0441d,	// SFWork:saved
	0x06dc2,	// DESubWork:FreeStr
	0x39890,	// GotoFlag
	0x03b27,	// FOR /F begin
	0x03cc5,	// FOR /F end
	0x07d70,	// ParseFor initial token
	0x47c38,	// LexBufPtr
	0x43c22,	// LexBuffer end
	0x07f08,	// ParseFor /F options
	0x4c074,	// TokLen
	0x02faa,	// Goto:EOF
	0x02f85,	// Goto pos
	0x02f53,	// Goto start
	0x20e6f,	// CallWork:resize
	0x0ba50,	// MyGetEnvVarPtr
	0x1e57e,	// DisplayEnv
	0x05305,	// DisplayEnvVariable
	0x0fc7d,	// Dispatch:after delayed expansion
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
	0x05737,	// ForMkstr
	0x05790,	// ForResize
	0x39118,	// EchoFlag
	0x10461,	// CheckHelp first arg
	0x11bf8,	// cmd_printf
	0x2b0d5,	// PromptUser
	0x0f5e0,	// mkstr
	0x12c04,	// FreeStack
	0x502d0,	// DCount
	0x04599,	// SFWork:mkstr (inline)
	0x0465b,	// SFWork:resize
	0x0456d,	// SFWork:saved
	0x06fe2,	// DESubWork:FreeStr
	0x39890,	// GotoFlag
	0x03c62,	// FOR /F begin
	0x03e03,	// FOR /F end
	0x07fcc,	// ParseFor initial token
	0x47c38,	// LexBufPtr
	0x43c22,	// LexBuffer end
	0x08164,	// ParseFor /F options
	0x4c074,	// TokLen
	0x0308b,	// Goto:EOF
	0x03067,	// Goto pos
	0x03032,	// Goto start
	0x20fa4,	// CallWork:resize
	0x0bc90,	// MyGetEnvVarPtr
	0x1e6c6,	// DisplayEnv
	0x05475,	// DisplayEnvVariable
	0x0fefd,	// Dispatch:after delayed expansion
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
	0x04f06,	// ForMkstr
	0x04f5f,	// ForResize
	0x39118,	// EchoFlag
	0x10c98,	// CheckHelp first arg
	0x1242c,	// cmd_printf
	0x2b314,	// PromptUser
	0x0fe10,	// mkstr
	0x13438,	// FreeStack
	0x502d0,	// DCount
	0x03cce,	// SFWork:mkstr (inline)
	0x03d97,	// SFWork:resize
	0x03cad,	// SFWork:saved
	0x067b4,	// DESubWork:FreeStr
	0x39890,	// GotoFlag
	0x0337a,	// FOR /F begin
	0x03537,	// FOR /F end
	0x0777c,	// ParseFor initial token
	0x47c38,	// LexBufPtr
	0x43c22,	// LexBuffer end
	0x07914,	// ParseFor /F options
	0x4c074,	// TokLen
	0x0279b,	// Goto:EOF
	0x02776,	// Goto pos
	0x02744,	// Goto start
	0x2119b,	// CallWork:resize
	0x0c450,	// MyGetEnvVarPtr
	0x1e890,	// DisplayEnv
	0x04c45,	// DisplayEnvVariable
	0x1072d,	// Dispatch:after delayed expansion
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
	0x06fd2,	// ForMkstr
	0x0702d,	// ForResize
	0x3b110,	// EchoFlag
	0x0c800,	// CheckHelp first arg
	0x136fc,	// cmd_printf
	0x2c5f0,	// PromptUser
	0x0d540,	// mkstr
	0x12ef0,	// FreeStack
	0x43ad0,	// DCount
	0x11960,	// SFWork:mkstr (inline)
	0x11a4c,	// SFWork:resize (inline)
	0x11952,	// SFWork:saved
	0x0cc51,	// DESubWork:FreeStr
	0x43a88,	// GotoFlag
	0x0ae22,	// FOR /F begin
	0x0afdb,	// FOR /F end
	0x14a04,	// ParseFor initial token
	0x438f0,	// LexBufPtr
	0x438e2,	// LexBuffer end
	0x14b9c,	// ParseFor /F options
	0x43a34,	// TokLen
	0x0ef2a,	// Goto:EOF
	0x0eefc,	// Goto pos
	0x0eecb,	// Goto start
	0x1b13c,	// CallWork:resize
	0x13b70,	// MyGetEnvVarPtr
	0x1ae38,	// DisplayEnv
	0x09979,	// DisplayEnvVariable
	0x0c8dd,	// Dispatch:after delayed expansion
};

const DWORD cmd_10_0_17763_592[] = {
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
	0x06fd2,	// ForMkstr
	0x0702d,	// ForResize
	0x3b110,	// EchoFlag
	0x0c800,	// CheckHelp first arg
	0x136fc,	// cmd_printf
	0x2c540,	// PromptUser
	0x0d540,	// mkstr
	0x12ef0,	// FreeStack
	0x43ad0,	// DCount
	0x11960,	// SFWork:mkstr (inline)
	0x11a4c,	// SFWork:resize (inline)
	0x11952,	// SFWork:saved
	0x0cc51,	// DESubWork:FreeStr
	0x43a88,	// GotoFlag
	0x0ae22,	// FOR /F begin
	0x0afdb,	// FOR /F end
	0x14a04,	// ParseFor initial token
	0x438f0,	// LexBufPtr
	0x438e2,	// LexBuffer end
	0x14b9c,	// ParseFor /F options
	0x43a34,	// TokLen
	0x0ef2a,	// Goto:EOF
	0x0eefc,	// Goto pos
	0x0eecb,	// Goto start
	0x1c322,	// CallWork:resize
	0x13b70,	// MyGetEnvVarPtr
	0x1c01e,	// DisplayEnv
	0x09979,	// DisplayEnvVariable
	0x0c8dd,	// Dispatch:after delayed expansion
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
	0x07d52,	// ForMkstr
	0x07dad,	// ForResize
	0x3b110,	// EchoFlag
	0x0b4c4,	// CheckHelp first arg
	0x1277c,	// cmd_printf
	0x2c9ac,	// PromptUser
	0x0c480,	// mkstr
	0x11f60,	// FreeStack
	0x43ad0,	// DCount
	0x10970,	// SFWork:mkstr (inline)
	0x10a5c,	// SFWork:resize (inline)
	0x10962,	// SFWork:saved
	0x0bc03,	// DESubWork:FreeStr
	0x43a88,	// GotoFlag
	0x09b12,	// FOR /F begin
	0x09ccb,	// FOR /F end
	0x15c5c,	// ParseFor initial token
	0x438f0,	// LexBufPtr
	0x438e2,	// LexBuffer end
	0x15df4,	// ParseFor /F options
	0x43a34,	// TokLen
	0x0dedd,	// Goto:EOF
	0x0deaf,	// Goto pos
	0x0de7e,	// Goto start
	0x1b0bc,	// CallWork:resize
	0x12bf0,	// MyGetEnvVarPtr
	0x1e282,	// DisplayEnv
	0x139a9,	// DisplayEnvVariable
	0x0b5ad,	// Dispatch:after delayed expansion
};

const DWORD cmd_10_0_18362_449[] = {
	0x09754,	// eol
	0x10bac,	// MSCmdVar
	0x0ea7b,	// Lex:text
	0x520f0,	// TmpBuf
	0x15140,	// PutStdErr:PutMsg
	0x43a90,	// CurrentBatchFile
	0x56aa4,	// fOutputUnicode
	0x43aa4,	// fDelayedExpansion
	0x43aa5,	// fEnableExtensions
	0x56ab1,	// fDumpTokens
	0x56ab2,	// fDumpParse
	0x07d52,	// ForMkstr
	0x07dad,	// ForResize
	0x3b110,	// EchoFlag
	0x0b4c4,	// CheckHelp first arg
	0x1277c,	// cmd_printf
	0x2c9ac,	// PromptUser
	0x0c480,	// mkstr
	0x11f60,	// FreeStack
	0x43ad0,	// DCount
	0x10970,	// SFWork:mkstr (inline)
	0x10a5c,	// SFWork:resize (inline)
	0x10962,	// SFWork:saved
	0x0bc03,	// DESubWork:FreeStr
	0x43a88,	// GotoFlag
	0x09b12,	// FOR /F begin
	0x09ccb,	// FOR /F end
	0x15db4,	// ParseFor initial token
	0x438f0,	// LexBufPtr
	0x438e2,	// LexBuffer end
	0x15f4c,	// ParseFor /F options
	0x43a34,	// TokLen
	0x0dedd,	// Goto:EOF
	0x0deaf,	// Goto pos
	0x0de7e,	// Goto start
	0x1b20c,	// CallWork:resize
	0x12bf0,	// MyGetEnvVarPtr
	0x1e3d2,	// DisplayEnv
	0x139a9,	// DisplayEnvVariable
	0x0b5ad,	// Dispatch:after delayed expansion
};

#else

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
	0x03a2a,	// ForMkstr
	0x03a83,	// ForResize
	0x1c010,	// EchoFlag
	0x14a08,	// CheckHelp first arg
	0x157bf,	// cmd_printf
	0x12d0d,	// CtrlCAbort:PromptUser
	0x0d9da,	// mkstr
	0x0d883,	// FreeStack
	0x1cfbc,	// DCount
	0x03d6d,	// SFWork:mkstr
	0x03ec8,	// SFWork:resize
	0x03d4b,	// SFWork:saved
	0x0d6d9,	// DESubWork:FreeStr
	0x1cf44,	// GotoFlag
	0x036b0,	// FOR /F begin
	0x0382b,	// FOR /F end
	0x0ef7b,	// ParseFor initial token
	0x1fbc8,	// LexBufPtr
	0x21be2,	// LexBuffer end
	0x0f004,	// ParseFor /F options
	0x1faf0,	// TokLen
	0x03fad,	// Goto:EOF
	0x03f95,	// Goto pos
	0x03f66,	// Goto start
	0x04856,	// CallWork:resize
	0x061f1,	// MyGetEnvVarPtr
	0x06039,	// DisplayEnv
	0x06002,	// DisplayEnvVariable
	0x0c738,	// Dispatch:after delayed expansion
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
	0x03a2f,	// ForMkstr
	0x03a88,	// ForResize
	0x1c010,	// EchoFlag
	0x149fc,	// CheckHelp first arg
	0x157b3,	// cmd_printf
	0x12d01,	// CtrlCAbort:PromptUser
	0x0d9ce,	// mkstr
	0x0d877,	// FreeStack
	0x1cfbc,	// DCount
	0x03d72,	// SFWork:mkstr
	0x03ecd,	// SFWork:resize
	0x03d50,	// SFWork:saved
	0x0d6cd,	// DESubWork:FreeStr
	0x1cf44,	// GotoFlag
	0x036b0,	// FOR /F begin
	0x03830,	// FOR /F end
	0x0ef6f,	// ParseFor initial token
	0x1fbc8,	// LexBufPtr
	0x21be2,	// LexBuffer end
	0x0eff8,	// ParseFor /F options
	0x1faf0,	// TokLen
	0x03fb2,	// Goto:EOF
	0x03f9a,	// Goto pos
	0x03f6b,	// Goto start
	0x0485b,	// CallWork:resize
	0x061f6,	// MyGetEnvVarPtr
	0x0603e,	// DisplayEnv
	0x06007,	// DisplayEnvVariable
	0x0c72c,	// Dispatch:after delayed expansion
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
	0x03a2f,	// ForMkstr
	0x03a88,	// ForResize
	0x1c010,	// EchoFlag
	0x149f8,	// CheckHelp first arg
	0x157af,	// cmd_printf
	0x12cfd,	// CtrlCAbort:PromptUser
	0x0d9ca,	// mkstr
	0x0d873,	// FreeStack
	0x1cfbc,	// DCount
	0x03d72,	// SFWork:mkstr
	0x03ecd,	// SFWork:resize
	0x03d50,	// SFWork:saved
	0x0d6c9,	// DESubWork:FreeStr
	0x1cf44,	// GotoFlag
	0x036b0,	// FOR /F begin
	0x03830,	// FOR /F end
	0x0ef6b,	// ParseFor initial token
	0x1fbc8,	// LexBufPtr
	0x21be2,	// LexBuffer end
	0x0eff4,	// ParseFor /F options
	0x1faf0,	// TokLen
	0x03fb2,	// Goto:EOF
	0x03f9a,	// Goto pos
	0x03f6b,	// Goto start
	0x0485b,	// CallWork:resize
	0x061f6,	// MyGetEnvVarPtr
	0x0603e,	// DisplayEnv
	0x06007,	// DisplayEnvVariable
	0x0c725,	// Dispatch:after delayed expansion
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
	0x03a2f,	// ForMkstr
	0x03a88,	// ForResize
	0x1c010,	// EchoFlag
	0x14a4f,	// CheckHelp first arg
	0x15806,	// cmd_printf
	0x12d54,	// CtrlCAbort:PromptUser
	0x0da21,	// mkstr
	0x0d8ca,	// FreeStack
	0x1cfbc,	// DCount
	0x03d72,	// SFWork:mkstr
	0x03ecd,	// SFWork:resize
	0x03d50,	// SFWork:saved
	0x0d720,	// DESubWork:FreeStr
	0x1cf44,	// GotoFlag
	0x036b0,	// FOR /F begin
	0x03830,	// FOR /F end
	0x0efc2,	// ParseFor initial token
	0x1fbc8,	// LexBufPtr
	0x21be2,	// LexBuffer end
	0x0f04b,	// ParseFor /F options
	0x1faf0,	// TokLen
	0x03fb2,	// Goto:EOF
	0x03f9a,	// Goto pos
	0x03f6b,	// Goto start
	0x0485b,	// CallWork:resize
	0x0623c,	// MyGetEnvVarPtr
	0x06084,	// DisplayEnv
	0x0604d,	// DisplayEnvVariable
	0x0c77c,	// Dispatch:after delayed expansion
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
	0x03a3f,	// ForMkstr
	0x03a98,	// ForResize
	0x1c010,	// EchoFlag
	0x14a7e,	// CheckHelp first arg
	0x15830,	// cmd_printf
	0x12d88,	// CtrlCAbort:PromptUser
	0x0da55,	// mkstr
	0x0d8fe,	// FreeStack
	0x1cfbc,	// DCount
	0x03d82,	// SFWork:mkstr
	0x03edd,	// SFWork:resize
	0x03d60,	// SFWork:saved
	0x0d754,	// DESubWork:FreeStr
	0x1cf44,	// GotoFlag
	0x036c0,	// FOR /F begin
	0x03840,	// FOR /F end
	0x0eff6,	// ParseFor initial token
	0x1fbc8,	// LexBufPtr
	0x21be2,	// LexBuffer end
	0x0f07f,	// ParseFor /F options
	0x1faf0,	// TokLen
	0x03fc2,	// Goto:EOF
	0x03faa,	// Goto pos
	0x03f7b,	// Goto start
	0x04877,	// CallWork:resize
	0x0625d,	// MyGetEnvVarPtr
	0x0609e,	// DisplayEnv
	0x06061,	// DisplayEnvVariable
	0x0c7b0,	// Dispatch:after delayed expansion
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
	0x03af6,	// ForMkstr
	0x03b4f,	// ForResize
	0x1c010,	// EchoFlag
	0x14b3c,	// CheckHelp first arg
	0x158ee,	// cmd_printf
	0x12e46,	// CtrlCAbort:PromptUser
	0x0db13,	// mkstr
	0x0d9bc,	// FreeStack
	0x1cfbc,	// DCount
	0x03e39,	// SFWork:mkstr
	0x03f94,	// SFWork:resize
	0x03e17,	// SFWork:saved
	0x0d812,	// DESubWork:FreeStr
	0x1cf44,	// GotoFlag
	0x03777,	// FOR /F begin
	0x038f7,	// FOR /F end
	0x0f0b4,	// ParseFor initial token
	0x1fbc8,	// LexBufPtr
	0x21be2,	// LexBuffer end
	0x0f13d,	// ParseFor /F options
	0x1faf0,	// TokLen
	0x04079,	// Goto:EOF
	0x04061,	// Goto pos
	0x04032,	// Goto start
	0x0492e,	// CallWork:resize
	0x06314,	// MyGetEnvVarPtr
	0x06155,	// DisplayEnv
	0x06118,	// DisplayEnvVariable
	0x0c86e,	// Dispatch:after delayed expansion
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
	0x0e8c2,	// ForMkstr
	0x0e786,	// ForResize
	0x2cac8,	// EchoFlag
	0x01f91,	// CheckHelp first arg
	0x03e14,	// cmd_printf
	0x1b30e,	// CtrlCAbort:PromptUser
	0x01302,	// mkstr
	0x01817,	// FreeStack
	0x289f0,	// DCount
	0x02880,	// SFWork:mkstr
	0x028e7,	// SFWork:resize
	0x02869,	// SFWork:saved
	0x06d1d,	// DESubWork:FreeStr
	0x2caa0,	// GotoFlag
	0x0673b,	// FOR /F begin
	0x063b0,	// FOR /F end
	0x04b2a,	// ParseFor initial token
	0x2ca24,	// LexBufPtr
	0x22082,	// LexBuffer end
	0x067f1,	// ParseFor /F options
	0x2ca40,	// TokLen
	0x05b60,	// Goto:EOF
	0x05b48,	// Goto pos
	0x05b1c,	// Goto start
	0x10545,	// CallWork:resize
	0x0210b,	// MyGetEnvVarPtr
	0x0b5cb,	// DisplayEnv
	0x0b5c1,	// DisplayEnvVariable
	0x013d8,	// Dispatch:after delayed expansion
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
	0x0d84b,	// ForMkstr
	0x0d8b5,	// ForResize
	0x2fac8,	// EchoFlag
	0x05c69,	// CheckHelp first arg
	0x05e0c,	// cmd_printf
	0x0c60c,	// CtrlCAbort:PromptUser
	0x01725,	// mkstr
	0x014fd,	// FreeStack
	0x2b9f0,	// DCount
	0x101cd,	// SFWork:mkstr
	0x10234,	// SFWork:resize
	0x101b6,	// SFWork:saved
	0x1c2ea,	// DESubWork:FreeStr
	0x2faa0,	// GotoFlag
	0x0af6a,	// FOR /F begin
	0x0ae34,	// FOR /F end
	0x1081e,	// ParseFor initial token
	0x2fa24,	// LexBufPtr
	0x25082,	// LexBuffer end
	0x0acd2,	// ParseFor /F options
	0x2fa40,	// TokLen
	0x0a15e,	// Goto:EOF
	0x0a13f,	// Goto pos
	0x0a111,	// Goto start
	0x126da,	// CallWork:resize
	0x02be1,	// MyGetEnvVarPtr
	0x09daa,	// DisplayEnv
	0x12edc,	// DisplayEnvVariable
	0x013b8,	// Dispatch:after delayed expansion
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
	0x0e130,	// ForMkstr
	0x0e19a,	// ForResize
	0x2fac8,	// EchoFlag
	0x05c59,	// CheckHelp first arg
	0x05e00,	// cmd_printf
	0x10a9f,	// CtrlCAbort:PromptUser
	0x01711,	// mkstr
	0x014f8,	// FreeStack
	0x2b9f0,	// DCount
	0x080b4,	// SFWork:mkstr
	0x0811b,	// SFWork:resize
	0x0809d,	// SFWork:saved
	0x1c312,	// DESubWork:FreeStr
	0x2faa0,	// GotoFlag
	0x0b0a6,	// FOR /F begin
	0x0af70,	// FOR /F end
	0x08524,	// ParseFor initial token
	0x2fa24,	// LexBufPtr
	0x25082,	// LexBuffer end
	0x0ae0e,	// ParseFor /F options
	0x2fa40,	// TokLen
	0x0a2bd,	// Goto:EOF
	0x0a29e,	// Goto pos
	0x0a270,	// Goto start
	0x126eb,	// CallWork:resize
	0x02bc9,	// MyGetEnvVarPtr
	0x07e0a,	// DisplayEnv
	0x12ef0,	// DisplayEnvVariable
	0x013b8,	// Dispatch:after delayed expansion
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
	0x0e130,	// ForMkstr
	0x0e19a,	// ForResize
	0x2fac8,	// EchoFlag
	0x05c59,	// CheckHelp first arg
	0x05e00,	// cmd_printf
	0x10a9f,	// CtrlCAbort:PromptUser
	0x01711,	// mkstr
	0x014f8,	// FreeStack
	0x2b9f0,	// DCount
	0x080b4,	// SFWork:mkstr
	0x0811b,	// SFWork:resize
	0x0809d,	// SFWork:saved
	0x1c312,	// DESubWork:FreeStr
	0x2faa0,	// GotoFlag
	0x0b0a6,	// FOR /F begin
	0x0af70,	// FOR /F end
	0x08524,	// ParseFor initial token
	0x2fa24,	// LexBufPtr
	0x25082,	// LexBuffer end
	0x0ae0e,	// ParseFor /F options
	0x2fa40,	// TokLen
	0x0a2bd,	// Goto:EOF
	0x0a29e,	// Goto pos
	0x0a270,	// Goto start
	0x126eb,	// CallWork:resize
	0x02bc9,	// MyGetEnvVarPtr
	0x07e0a,	// DisplayEnv
	0x12ef0,	// DisplayEnvVariable
	0x013b8,	// Dispatch:after delayed expansion
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
	0x10e69,	// ForMkstr
	0x10ee2,	// ForResize
	0x20064,	// EchoFlag
	0x01c21,	// CheckHelp first arg
	0x02cd9,	// cmd_printf
	0x1cf1f,	// CtrlCAbort:PromptUser
	0x013b2,	// mkstr
	0x01b4f,	// FreeStack
	0x20014,	// DCount
	0x06588,	// SFWork:mkstr
	0x065eb,	// SFWork:resize
	0x06571,	// SFWork:saved
	0x049fe,	// DESubWork:FreeStr
	0x2004c,	// GotoFlag
	0x06e21,	// FOR /F begin
	0x06f6d,	// FOR /F end
	0x067fa,	// ParseFor initial token
	0x24d24,	// LexBufPtr
	0x28d42,	// LexBuffer end
	0x06fcc,	// ParseFor /F options
	0x28d84,	// TokLen
	0x04580,	// Goto:EOF
	0x04568,	// Goto pos
	0x0453c,	// Goto start
	0x1264a,	// CallWork:resize
	0x03709,	// MyGetEnvVarPtr
	0x12f11,	// DisplayEnv
	0x0be36,	// DisplayEnvVariable
	0x01983,	// Dispatch:after delayed expansion
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
	0x0ed53,	// ForMkstr
	0x0ebe8,	// ForResize
	0x2fa80,	// EchoFlag
	0x0552f,	// CheckHelp first arg
	0x058bf,	// cmd_printf
	0x1ab43,	// CtrlCAbort:PromptUser
	0x01732,	// mkstr
	0x01c6c,	// FreeStack
	0x2b9b0,	// DCount
	0x0a1bb,	// SFWork:mkstr
	0x0a222,	// SFWork:resize
	0x0a1a4,	// SFWork:saved
	0x0b81a,	// DESubWork:FreeStr
	0x2fa58,	// GotoFlag
	0x0cd31,	// FOR /F begin
	0x0cc3f,	// FOR /F end
	0x0a87f,	// ParseFor initial token
	0x2f9e4,	// LexBufPtr
	0x25042,	// LexBuffer end
	0x0d1de,	// ParseFor /F options
	0x2fa00,	// TokLen
	0x07c51,	// Goto:EOF
	0x07c32,	// Goto pos
	0x07c04,	// Goto start
	0x109ce,	// CallWork:resize
	0x037df,	// MyGetEnvVarPtr
	0x12529,	// DisplayEnv
	0x12519,	// DisplayEnvVariable
	0x013ea,	// Dispatch:after delayed expansion
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
	0x1183d,	// ForMkstr
	0x118a7,	// ForResize
	0x2fa80,	// EchoFlag
	0x045a7,	// CheckHelp first arg
	0x04115,	// cmd_printf
	0x11c16,	// CtrlCAbort:PromptUser
	0x0206b,	// mkstr
	0x01fe6,	// FreeStack
	0x2b9b0,	// DCount
	0x0abd6,	// SFWork:mkstr
	0x0ac2d,	// SFWork:resize
	0x0abbf,	// SFWork:saved
	0x04b7b,	// DESubWork:FreeStr
	0x2fa58,	// GotoFlag
	0x0b363,	// FOR /F begin
	0x0b487,	// FOR /F end
	0x0a961,	// ParseFor initial token
	0x2f9e4,	// LexBufPtr
	0x25042,	// LexBuffer end
	0x0b23e,	// ParseFor /F options
	0x2fa00,	// TokLen
	0x05f86,	// Goto:EOF
	0x05f6b,	// Goto pos
	0x05f3d,	// Goto start
	0x12c9d,	// CallWork:resize
	0x032ed,	// MyGetEnvVarPtr
	0x1346b,	// DisplayEnv
	0x1345b,	// DisplayEnvVariable
	0x01414,	// Dispatch:after delayed expansion
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
	0x13272,	// ForMkstr
	0x132c5,	// ForResize
	0x2404c,	// EchoFlag
	0x0313f,	// CheckHelp first arg
	0x0469f,	// cmd_printf
	0x14c7f,	// CtrlCAbort:PromptUser
	0x01c0d,	// mkstr
	0x022b7,	// FreeStack
	0x240c8,	// DCount
	0x08817,	// SFWork:mkstr
	0x08879,	// SFWork:resize
	0x08800,	// SFWork:saved
	0x07e22,	// DESubWork:FreeStr
	0x24078,	// GotoFlag
	0x08c15,	// FOR /F begin
	0x08d7a,	// FOR /F end
	0x09150,	// ParseFor initial token
	0x2415c,	// LexBufPtr
	0x40642,	// LexBuffer end
	0x092d4,	// ParseFor /F options
	0x24140,	// TokLen
	0x06698,	// Goto:EOF
	0x0667d,	// Goto pos
	0x0664f,	// Goto start
	0x1653d,	// CallWork:resize
	0x0363a,	// MyGetEnvVarPtr
	0x16eab,	// DisplayEnv
	0x16e9b,	// DisplayEnvVariable
	0x01a1e,	// Dispatch:after delayed expansion
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
	0x1144c,	// ForMkstr
	0x1132a,	// ForResize
	0x2404c,	// EchoFlag
	0x03259,	// CheckHelp first arg
	0x040bf,	// cmd_printf
	0x1f312,	// CtrlCAbort:PromptUser
	0x01e3d,	// mkstr
	0x03031,	// FreeStack
	0x240bc,	// DCount
	0x09d69,	// SFWork:mkstr
	0x09dcb,	// SFWork:resize
	0x09d52,	// SFWork:saved
	0x06a75,	// DESubWork:FreeStr
	0x24078,	// GotoFlag
	0x0a713,	// FOR /F begin
	0x0a878,	// FOR /F end
	0x0a23f,	// ParseFor initial token
	0x2413c,	// LexBufPtr
	0x405e2,	// LexBuffer end
	0x0a942,	// ParseFor /F options
	0x24120,	// TokLen
	0x065d8,	// Goto:EOF
	0x065bd,	// Goto pos
	0x0658f,	// Goto start
	0x144ed,	// CallWork:resize
	0x01d61,	// MyGetEnvVarPtr
	0x14e53,	// DisplayEnv
	0x14e43,	// DisplayEnvVariable
	0x01836,	// Dispatch:after delayed expansion
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
	0x10035,	// ForMkstr
	0x1007f,	// ForResize
	0x2408c,	// EchoFlag
	0x04cf3,	// CheckHelp first arg
	0x066b0,	// cmd_printf
	0x1fa56,	// CtrlCAbort:PromptUser
	0x018c8,	// mkstr
	0x01963,	// FreeStack
	0x240fc,	// DCount
	0x0de31,	// SFWork:mkstr
	0x0de93,	// SFWork:resize
	0x0de1a,	// SFWork:saved
	0x0736d,	// DESubWork:FreeStr
	0x240b8,	// GotoFlag
	0x0e87b,	// FOR /F begin
	0x0e9dc,	// FOR /F end
	0x0e1a1,	// ParseFor initial token
	0x2418c,	// LexBufPtr
	0x40602,	// LexBuffer end
	0x0e31d,	// ParseFor /F options
	0x24170,	// TokLen
	0x06d23,	// Goto:EOF
	0x06d04,	// Goto pos
	0x06cd6,	// Goto start
	0x147ca,	// CallWork:resize
	0x02f25,	// MyGetEnvVarPtr
	0x15176,	// DisplayEnv
	0x15166,	// DisplayEnvVariable
	0x01472,	// Dispatch:after delayed expansion
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
	0x10245,	// ForMkstr
	0x10122,	// ForResize
	0x2408c,	// EchoFlag
	0x047c6,	// CheckHelp first arg
	0x0735f,	// cmd_printf
	0x1fab6,	// CtrlCAbort:PromptUser
	0x018b8,	// mkstr
	0x019aa,	// FreeStack
	0x240fc,	// DCount
	0x0e135,	// SFWork:mkstr
	0x0e197,	// SFWork:resize
	0x0e11e,	// SFWork:saved
	0x076ca,	// DESubWork:FreeStr
	0x240b8,	// GotoFlag
	0x0ebc2,	// FOR /F begin
	0x0ed23,	// FOR /F end
	0x0e493,	// ParseFor initial token
	0x2418c,	// LexBufPtr
	0x40602,	// LexBuffer end
	0x0e60f,	// ParseFor /F options
	0x24170,	// TokLen
	0x07f82,	// Goto:EOF
	0x07f63,	// Goto pos
	0x07f35,	// Goto start
	0x14a6e,	// CallWork:resize
	0x0308d,	// MyGetEnvVarPtr
	0x154db,	// DisplayEnv
	0x154cb,	// DisplayEnvVariable
	0x01492,	// Dispatch:after delayed expansion
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
	0x0ffed,	// ForMkstr
	0x0feca,	// ForResize
	0x2408c,	// EchoFlag
	0x041c5,	// CheckHelp first arg
	0x058f3,	// cmd_printf
	0x1fcce,	// CtrlCAbort:PromptUser
	0x01896,	// mkstr
	0x01911,	// FreeStack
	0x24104,	// DCount
	0x0ddbe,	// SFWork:mkstr
	0x0de20,	// SFWork:resize
	0x0dda7,	// SFWork:saved
	0x0d638,	// DESubWork:FreeStr
	0x240b8,	// GotoFlag
	0x0e84e,	// FOR /F begin
	0x0e9af,	// FOR /F end
	0x0e10e,	// ParseFor initial token
	0x24194,	// LexBufPtr
	0x40642,	// LexBuffer end
	0x0e29b,	// ParseFor /F options
	0x24178,	// TokLen
	0x064f8,	// Goto:EOF
	0x064d9,	// Goto pos
	0x064ab,	// Goto start
	0x14b02,	// CallWork:resize
	0x0321b,	// MyGetEnvVarPtr
	0x154b7,	// DisplayEnv
	0x154a7,	// DisplayEnvVariable
	0x014f1,	// Dispatch:after delayed expansion
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
	0x0ffed,	// ForMkstr
	0x0feca,	// ForResize
	0x2408c,	// EchoFlag
	0x041c5,	// CheckHelp first arg
	0x058f3,	// cmd_printf
	0x1fcce,	// CtrlCAbort:PromptUser
	0x01896,	// mkstr
	0x01911,	// FreeStack
	0x24104,	// DCount
	0x0ddbe,	// SFWork:mkstr
	0x0de20,	// SFWork:resize
	0x0dda7,	// SFWork:saved
	0x0d638,	// DESubWork:FreeStr
	0x240b8,	// GotoFlag
	0x0e84e,	// FOR /F begin
	0x0e9af,	// FOR /F end
	0x0e10e,	// ParseFor initial token
	0x24194,	// LexBufPtr
	0x40642,	// LexBuffer end
	0x0e29b,	// ParseFor /F options
	0x24178,	// TokLen
	0x064f8,	// Goto:EOF
	0x064d9,	// Goto pos
	0x064ab,	// Goto start
	0x14b02,	// CallWork:resize
	0x0321b,	// MyGetEnvVarPtr
	0x154b7,	// DisplayEnv
	0x154a7,	// DisplayEnvVariable
	0x014f1,	// Dispatch:after delayed expansion
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
	0x0ee69,	// ForMkstr
	0x0ec8d,	// ForResize
	0x30050,	// EchoFlag
	0x016a6,	// CheckHelp first arg
	0x07bd0,	// cmd_printf
	0x22ed6,	// CtrlCAbort:PromptUser
	0x01a8f,	// mkstr
	0x0c4d8,	// FreeStack
	0x300b8,	// DCount
	0x0df4c,	// SFWork:mkstr (inline)
	0x0dff9,	// SFWork:resize (inline)
	0x0df35,	// SFWork:saved
	0x07b60,	// DESubWork:FreeStr (inline)
	0x30042,	// GotoFlag
	0x0d57a,	// FOR /F begin
	0x0e7d9,	// FOR /F end
	0x0dc04,	// ParseFor initial token
	0x30144,	// LexBufPtr
	0x4ba62,	// LexBuffer end
	0x0dea5,	// ParseFor /F options
	0x30118,	// TokLen
	0x0a09f,	// Goto:EOF
	0x0a0c5,	// Goto pos
	0x09ff2,	// Goto start
	0x25025,	// CallWork:resize
	0x03291,	// MyGetEnvVarPtr
	0x26970,	// DisplayEnv
	0x26960,	// DisplayEnvVariable
	0x01513,	// Dispatch:after delayed expansion
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
	0x0f23b,	// ForMkstr
	0x0f0cd,	// ForResize
	0x2e090,	// EchoFlag
	0x011e3,	// CheckHelp first arg
	0x07184,	// cmd_printf
	0x18660,	// CtrlCAbort:PromptUser
	0x04d45,	// mkstr
	0x06b67,	// FreeStack
	0x2e0f8,	// DCount
	0x0bff8,	// SFWork:mkstr (inline)
	0x0c0a9,	// SFWork:resize (inline)
	0x0bfe1,	// SFWork:saved
	0x07061,	// DESubWork:FreeStr (inline)
	0x2e082,	// GotoFlag
	0x0c732,	// FOR /F begin
	0x0c35f,	// FOR /F end
	0x0cbd1,	// ParseFor initial token
	0x2e194,	// LexBufPtr
	0x49ac2,	// LexBuffer end
	0x0ce4f,	// ParseFor /F options
	0x2e168,	// TokLen
	0x09c6d,	// Goto:EOF
	0x09c93,	// Goto pos
	0x09bc1,	// Goto start
	0x256bb,	// CallWork:resize
	0x02fcb,	// MyGetEnvVarPtr
	0x1a852,	// DisplayEnv
	0x1a842,	// DisplayEnvVariable
	0x0106a,	// Dispatch:after delayed expansion
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
	0x0d735,	// ForMkstr
	0x0d785,	// ForResize
	0x2505c,	// EchoFlag
	0x05770,	// CheckHelp first arg
	0x0618d,	// cmd_printf
	0x23078,	// CtrlCAbort:PromptUser
	0x02343,	// mkstr
	0x057df,	// FreeStack
	0x250bc,	// DCount
	0x09910,	// SFWork:mkstr (inline)
	0x099bf,	// SFWork:resize
	0x098f9,	// SFWork:saved
	0x05b24,	// DESubWork:FreeStr
	0x25042,	// GotoFlag
	0x0a375,	// FOR /F begin
	0x0a553,	// FOR /F end
	0x09e14,	// ParseFor initial token
	0x25148,	// LexBufPtr
	0x40842,	// LexBuffer end
	0x09f8a,	// ParseFor /F options
	0x2511c,	// TokLen
	0x078d4,	// Goto:EOF
	0x078b8,	// Goto pos
	0x07884,	// Goto start
	0x1a280,	// CallWork:resize
	0x03507,	// MyGetEnvVarPtr
	0x1b5f7,	// DisplayEnv
	0x1420f,	// DisplayEnvVariable
	0x017f3,	// Dispatch:after delayed expansion
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
	0x085eb,	// ForMkstr
	0x0863b,	// ForResize
	0x2800c,	// EchoFlag
	0x1eb94,	// CheckHelp first arg
	0x1faa5,	// cmd_printf
	0x1c976,	// CtrlCAbort:PromptUser
	0x1625d,	// mkstr
	0x16019,	// FreeStack
	0x284f4,	// DCount
	0x088fe,	// SFWork:mkstr
	0x08bd7,	// SFWork:resize
	0x088bb,	// SFWork:saved (index)
	0x15e86,	// DESubWork:FreeStr
	0x28498,	// GotoFlag
	0x081f3,	// FOR /F begin
	0x08392,	// FOR /F end
	0x17cfa,	// ParseFor initial token
	0x2cb10,	// LexBufPtr
	0x35742,	// LexBuffer end
	0x17d73,	// ParseFor /F options
	0x2c8d8,	// TokLen
	0x08fdd,	// Goto:EOF
	0x08fbe,	// Goto pos
	0x08f63,	// Goto start
	0x09da7,	// CallWork:resize
	0x0c074,	// MyGetEnvVarPtr
	0x0d053,	// DisplayEnv
	0x0d02f,	// DisplayEnvVariable
	0x14892,	// Dispatch:after delayed expansion
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
	0x10f19,	// ForMkstr
	0x10f69,	// ForResize
	0x2500c,	// EchoFlag
	0x0c6e0,	// CheckHelp first arg
	0x0e18a,	// cmd_printf
	0x224db,	// CtrlCAbort:PromptUser
	0x09ef0,	// mkstr
	0x0e902,	// FreeStack
	0x254c8,	// DCount
	0x0dda8,	// SFWork:mkstr (inline)
	0x0de53,	// SFWork:resize (inline)
	0x0dd91,	// SFWork:saved
	0x0ba57,	// DESubWork:FreeStr
	0x25478,	// GotoFlag
	0x073bb,	// FOR /F begin
	0x0753a,	// FOR /F end
	0x06b60,	// ParseFor initial token
	0x298d0,	// LexBufPtr
	0x32502,	// LexBuffer end
	0x06c92,	// ParseFor /F options
	0x29698,	// TokLen
	0x08379,	// Goto:EOF
	0x0835d,	// Goto pos
	0x08329,	// Goto start
	0x1a9d2,	// CallWork:resize
	0x06f30,	// MyGetEnvVarPtr
	0x1a584,	// DisplayEnv
	0x0d679,	// DisplayEnvVariable
	0x0b82f,	// Dispatch:after delayed expansion
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
	0x0f39f,	// ForMkstr
	0x0f3ef,	// ForResize
	0x2505c,	// EchoFlag
	0x052e4,	// CheckHelp first arg
	0x06570,	// cmd_printf
	0x23008,	// CtrlCAbort:PromptUser
	0x013cb,	// mkstr
	0x05353,	// FreeStack
	0x250bc,	// DCount
	0x0a2b9,	// SFWork:mkstr (inline)
	0x0a368,	// SFWork:resize (inline)
	0x0a2a2,	// SFWork:saved
	0x05e11,	// DESubWork:FreeStr
	0x25042,	// GotoFlag
	0x09720,	// FOR /F begin
	0x0990a,	// FOR /F end
	0x09acc,	// ParseFor initial token
	0x25148,	// LexBufPtr
	0x40842,	// LexBuffer end
	0x09c36,	// ParseFor /F options
	0x2511c,	// TokLen
	0x07cf9,	// Goto:EOF
	0x07cdd,	// Goto pos
	0x07ca9,	// Goto start
	0x175bc,	// CallWork:resize
	0x029c9,	// MyGetEnvVarPtr
	0x1bd77,	// DisplayEnv
	0x1332c,	// DisplayEnvVariable
	0x0113f,	// Dispatch:after delayed expansion
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
	0x0f334,	// ForMkstr
	0x0f384,	// ForResize
	0x2601c,	// EchoFlag
	0x058f0,	// CheckHelp first arg
	0x06992,	// cmd_printf
	0x234fb,	// CtrlCAbort:PromptUser
	0x01e20,	// mkstr
	0x059a0,	// FreeStack
	0x2607c,	// DCount
	0x0d976,	// SFWork:mkstr
	0x0d9dd,	// SFWork:resize
	0x0d961,	// SFWork:saved
	0x06072,	// DESubWork:FreeStr
	0x26002,	// GotoFlag
	0x0d2f2,	// FOR /F begin
	0x0d467,	// FOR /F end
	0x0cd67,	// ParseFor initial token
	0x26108,	// LexBufPtr
	0x41802,	// LexBuffer end
	0x0cee2,	// ParseFor /F options
	0x260dc,	// TokLen
	0x07637,	// Goto:EOF
	0x0761b,	// Goto pos
	0x075e9,	// Goto start
	0x1769f,	// CallWork:resize
	0x036cb,	// MyGetEnvVarPtr
	0x19a8a,	// DisplayEnv
	0x0e53e,	// DisplayEnvVariable
	0x01353,	// Dispatch:after delayed expansion
};

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
	0x03cc7,	// ForMkstr
	0x03d17,	// ForResize
	0x25010,	// EchoFlag
	0x08261,	// CheckHelp first arg
	0x0f0fc,	// cmd_printf
	0x219d4,	// CtrlCAbort:PromptUser
	0x0bdb0,	// mkstr
	0x0c410,	// FreeStack
	0x3c9e4,	// DCount
	0x09a06,	// SFWork:mkstr
	0x09a7b,	// SFWork:resize
	0x099f1,	// SFWork:saved
	0x08b37,	// DESubWork:FreeStr
	0x3c974,	// GotoFlag
	0x0fca0,	// FOR /F begin
	0x0fe1f,	// FOR /F end
	0x117dd,	// ParseFor initial token
	0x257d0,	// LexBufPtr
	0x2e402,	// LexBuffer end
	0x11921,	// ParseFor /F options
	0x25598,	// TokLen
	0x0a9cb,	// Goto:EOF
	0x0a9ab,	// Goto pos
	0x0a979,	// Goto start
	0x183ef,	// CallWork:resize
	0x0eca0,	// MyGetEnvVarPtr
	0x15b14,	// DisplayEnv
	0x081ca,	// DisplayEnvVariable
	0x0880e,	// Dispatch:after delayed expansion
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
	0x042b7,	// ForMkstr
	0x04307,	// ForResize
	0x26010,	// EchoFlag
	0x08790,	// CheckHelp first arg
	0x0e6c4,	// cmd_printf
	0x22849,	// CtrlCAbort:PromptUser
	0x0bcb0,	// mkstr
	0x0f31b,	// FreeStack
	0x3d9e4,	// DCount
	0x08fd6,	// SFWork:mkstr
	0x0904b,	// SFWork:resize
	0x08fc1,	// SFWork:saved
	0x0c765,	// DESubWork:FreeStr
	0x3d974,	// GotoFlag
	0x0f9d0,	// FOR /F begin
	0x0fb4f,	// FOR /F end
	0x117c2,	// ParseFor initial token
	0x267d0,	// LexBufPtr
	0x2f402,	// LexBuffer end
	0x11906,	// ParseFor /F options
	0x26598,	// TokLen
	0x0a6cb,	// Goto:EOF
	0x0a6ab,	// Goto pos
	0x0a679,	// Goto start
	0x07b91,	// CallWork:resize
	0x10000,	// MyGetEnvVarPtr
	0x190c6,	// DisplayEnv
	0x0da8a,	// DisplayEnvVariable
	0x0c42e,	// Dispatch:after delayed expansion
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
	0x04307,	// ForMkstr
	0x04357,	// ForResize
	0x26010,	// EchoFlag
	0x084cf,	// CheckHelp first arg
	0x0e363,	// cmd_printf
	0x22a8f,	// CtrlCAbort:PromptUser
	0x0b920,	// mkstr
	0x0bf40,	// FreeStack
	0x3d7cc,	// DCount
	0x08d36,	// SFWork:mkstr
	0x08dab,	// SFWork:resize
	0x08d21,	// SFWork:saved
	0x0c477,	// DESubWork:FreeStr
	0x2709c,	// GotoFlag
	0x0f640,	// FOR /F begin
	0x0f7bf,	// FOR /F end
	0x115bf,	// ParseFor initial token
	0x35598,	// LexBufPtr
	0x35592,	// LexBuffer end
	0x11700,	// ParseFor /F options
	0x3d690,	// TokLen
	0x0a36b,	// Goto:EOF
	0x0a34b,	// Goto pos
	0x0a319,	// Goto start
	0x170f6,	// CallWork:resize
	0x0fc80,	// MyGetEnvVarPtr
	0x1912d,	// DisplayEnv
	0x0d79a,	// DisplayEnvVariable
	0x0c13e,	// Dispatch:after delayed expansion
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
	0x09b6e,	// ForMkstr
	0x09bbe,	// ForResize
	0x2d0c8,	// EchoFlag
	0x0cc80,	// CheckHelp first arg
	0x0c37c,	// cmd_printf
	0x28ff7,	// CtrlCAbort:PromptUser
	0x0f4e0,	// mkstr
	0x11bd6,	// FreeStack
	0x43ce8,	// DCount
	0x09857,	// SFWork:mkstr
	0x098b8,	// SFWork:resize
	0x09844,	// SFWork:saved
	0x04415,	// DESubWork:FreeStr
	0x2d598,	// GotoFlag
	0x09050,	// FOR /F begin
	0x091d9,	// FOR /F end
	0x065a8,	// ParseFor initial token
	0x3b8b4,	// LexBufPtr
	0x378a2,	// LexBuffer end
	0x06700,	// ParseFor /F options
	0x3fa9c,	// TokLen
	0x0859d,	// Goto:EOF
	0x0857e,	// Goto pos
	0x08549,	// Goto start
	0x18280,	// CallWork:resize
	0x12570,	// MyGetEnvVarPtr
	0x1be33,	// DisplayEnv
	0x07aa5,	// DisplayEnvVariable
	0x0fd9e,	// Dispatch:after delayed expansion
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
	0x1835d,	// ForMkstr
	0x183ad,	// ForResize
	0x2b0c0,	// EchoFlag
	0x05163,	// CheckHelp first arg
	0x0c6ff,	// cmd_printf
	0x24d99,	// CtrlCAbort:PromptUser
	0x07b10,	// mkstr
	0x096e0,	// FreeStack
	0x41c68,	// DCount
	0x1975a,	// SFWork:mkstr
	0x198e5,	// SFWork:resize
	0x1973e,	// SFWork:saved
	0x1fd6a,	// DESubWork:FreeStr
	0x41c44,	// GotoFlag
	0x18c76,	// FOR /F begin
	0x18e2d,	// FOR /F end
	0x217fd,	// ParseFor initial token
	0x39814,	// LexBufPtr
	0x35802,	// LexBuffer end
	0x2188e,	// ParseFor /F options
	0x3da08,	// TokLen
	0x19fc7,	// Goto:EOF
	0x19fab,	// Goto pos
	0x19f82,	// Goto start
	0x17943,	// CallWork:resize
	0x0b180,	// MyGetEnvVarPtr
	0x1c66c,	// DisplayEnv
	0x1c648,	// DisplayEnvVariable
	0x07055,	// Dispatch:after delayed expansion
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
	0x08094,	// ForMkstr
	0x080e4,	// ForResize
	0x2d0c8,	// EchoFlag
	0x0dd6d,	// CheckHelp first arg
	0x125d9,	// cmd_printf
	0x29230,	// CtrlCAbort:PromptUser
	0x100b0,	// mkstr
	0x12a06,	// FreeStack
	0x43cd8,	// DCount
	0x07d66,	// SFWork:mkstr
	0x07dd2,	// SFWork:resize
	0x07d51,	// SFWork:saved
	0x08df6,	// DESubWork:FreeStr
	0x2d590,	// GotoFlag
	0x07520,	// FOR /F begin
	0x076cb,	// FOR /F end
	0x09a68,	// ParseFor initial token
	0x3b8a4,	// LexBufPtr
	0x37892,	// LexBuffer end
	0x09bc0,	// ParseFor /F options
	0x3fa8c,	// TokLen
	0x06a3d,	// Goto:EOF
	0x06a1e,	// Goto pos
	0x069e9,	// Goto start
	0x209f1,	// CallWork:resize
	0x13320,	// MyGetEnvVarPtr
	0x1a9d2,	// DisplayEnv
	0x0604c,	// DisplayEnvVariable
	0x10e6e,	// Dispatch:after delayed expansion
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
	0x06942,	// ForMkstr
	0x06992,	// ForResize
	0x2d0c0,	// EchoFlag
	0x08dc9,	// CheckHelp first arg
	0x0edec,	// cmd_printf
	0x29b3b,	// CtrlCAbort:PromptUser
	0x0ac20,	// mkstr
	0x0fc2d,	// FreeStack
	0x35768,	// DCount
	0x09a68,	// SFWork:mkstr (inline)
	0x09b06,	// SFWork:resize
	0x09a51,	// SFWork:saved
	0x0cfd6,	// DESubWork:FreeStr
	0x35734,	// GotoFlag
	0x08350,	// FOR /F begin
	0x084f4,	// FOR /F end
	0x115d3,	// ParseFor initial token
	0x355bc,	// LexBufPtr
	0x355b2,	// LexBuffer end
	0x1172b,	// ParseFor /F options
	0x356ec,	// TokLen
	0x0afcd,	// Goto:EOF
	0x0afae,	// Goto pos
	0x0af79,	// Goto start
	0x1c642,	// CallWork:resize
	0x07f10,	// MyGetEnvVarPtr
	0x1c044,	// DisplayEnv
	0x0d89c,	// DisplayEnvVariable
	0x0cd03,	// Dispatch:after delayed expansion
};

const DWORD cmd_10_0_17763_592[] = {
	0x086f9,	// eol
	0x09581,	// MSCmdVar
	0x0c0fa,	// Lex:text
	0x43ae0,	// TmpBuf
	0x12b14,	// PutStdErr:PutMsg
	0x35738,	// CurrentBatchFile
	0x4803c,	// fOutputUnicode
	0x35744,	// fDelayedExpansion
	0x35745,	// fEnableExtensions
	0x48045,	// fDumpTokens
	0x48046,	// fDumpParse
	0x06fe2,	// ForMkstr
	0x07032,	// ForResize
	0x2d0c0,	// EchoFlag
	0x09479,	// CheckHelp first arg
	0x0f49c,	// cmd_printf
	0x299fb,	// CtrlCAbort:PromptUser
	0x0b2d0,	// mkstr
	0x102dd,	// FreeStack
	0x35768,	// DCount
	0x0a118,	// SFWork:mkstr (inline)
	0x0a1b6,	// SFWork:resize
	0x0a101,	// SFWork:saved
	0x0d686,	// DESubWork:FreeStr
	0x35734,	// GotoFlag
	0x08a00,	// FOR /F begin
	0x08ba4,	// FOR /F end
	0x11c83,	// ParseFor initial token
	0x355bc,	// LexBufPtr
	0x355b2,	// LexBuffer end
	0x11ddb,	// ParseFor /F options
	0x356ec,	// TokLen
	0x0b67d,	// Goto:EOF
	0x0b65e,	// Goto pos
	0x0b629,	// Goto start
	0x1c4ff,	// CallWork:resize
	0x085b0,	// MyGetEnvVarPtr
	0x1bf01,	// DisplayEnv
	0x0df4c,	// DisplayEnvVariable
	0x0d3b3,	// Dispatch:after delayed expansion
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
	0x06f3b,	// ForMkstr
	0x06f8b,	// ForResize
	0x2d0c0,	// EchoFlag
	0x0a639,	// CheckHelp first arg
	0x09740,	// cmd_printf
	0x2970b,	// CtrlCAbort:PromptUser
	0x0dac0,	// mkstr
	0x1179f,	// FreeStack
	0x35768,	// DCount
	0x0b3a6,	// SFWork:mkstr
	0x0b418,	// SFWork:resize
	0x0b391,	// SFWork:saved
	0x0e5c6,	// DESubWork:FreeStr
	0x35734,	// GotoFlag
	0x10ce0,	// FOR /F begin
	0x10e84,	// FOR /F end
	0x12867,	// ParseFor initial token
	0x355bc,	// LexBufPtr
	0x355b2,	// LexBuffer end
	0x129a8,	// ParseFor /F options
	0x356ec,	// TokLen
	0x0c56b,	// Goto:EOF
	0x0c54b,	// Goto pos
	0x0c519,	// Goto start
	0x1c0a4,	// CallWork:resize
	0x11ba0,	// MyGetEnvVarPtr
	0x1c916,	// DisplayEnv
	0x0a91a,	// DisplayEnvVariable
	0x0e2c3,	// Dispatch:after delayed expansion
};

const DWORD cmd_10_0_18362_449[] = {
	0x10b79,	// eol
	0x0ed43,	// MSCmdVar
	0x0d15a,	// Lex:text
	0x43ae0,	// TmpBuf
	0x07854,	// PutStdErr:PutMsg
	0x35738,	// CurrentBatchFile
	0x4803c,	// fOutputUnicode
	0x35744,	// fDelayedExpansion
	0x35745,	// fEnableExtensions
	0x48045,	// fDumpTokens
	0x48046,	// fDumpParse
	0x06f5d,	// ForMkstr
	0x06fad,	// ForResize
	0x2d0c0,	// EchoFlag
	0x0a789,	// CheckHelp first arg
	0x09890,	// cmd_printf
	0x2970b,	// CtrlCAbort:PromptUser
	0x0dc10,	// mkstr
	0x1190f,	// FreeStack
	0x35768,	// DCount
	0x0b4f6,	// SFWork:mkstr
	0x0b568,	// SFWork:resize
	0x0b4e1,	// SFWork:saved
	0x0e716,	// DESubWork:FreeStr
	0x35734,	// GotoFlag
	0x10e50,	// FOR /F begin
	0x10ff4,	// FOR /F end
	0x129d7,	// ParseFor initial token
	0x355bc,	// LexBufPtr
	0x355b2,	// LexBuffer end
	0x12b18,	// ParseFor /F options
	0x356ec,	// TokLen
	0x0c6bb,	// Goto:EOF
	0x0c69b,	// Goto pos
	0x0c669,	// Goto start
	0x1c096,	// CallWork:resize
	0x11d10,	// MyGetEnvVarPtr
	0x1c908,	// DisplayEnv
	0x0aa6a,	// DisplayEnvVariable
	0x0e413,	// Dispatch:after delayed expansion
};

#endif

#define CMD(a,b,c,d) \
	{ (a << 16) | b, (c << 16) | d, cmd_ ## a ## _ ## b ## _ ## c ## _ ## d },

const struct sCMD cmd_versions[] = {
#ifdef _WIN64
	CMD(10,0,18362,449)
	CMD(10,0,18362,1)
	CMD(10,0,17763,592)
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
	CMD(10,0,18362,449)
	CMD(10,0,18362,1)
	CMD(10,0,17763,592)
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
