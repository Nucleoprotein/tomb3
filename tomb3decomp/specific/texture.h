#pragma once
#include "../global/vars.h"

#define DXTextureAdd	( (long(__cdecl*)(long, long, ushort*, TEXTURESHIT*, long, long)) 0x004B23D0 )
#define DXFreeTPages	( (void(__cdecl*)()) 0x004B1F10 )
#define DXCreateMaxTPages	( (void(__cdecl*)(long)) 0x004B1D90 )