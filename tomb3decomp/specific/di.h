#pragma once
#include "../global/vars.h"

void inject_di(bool replace);

void DI_ReadKeyboard(uchar* KeyMap);
long DI_ReadJoystick(long& x, long& y);
void DI_StartKeyboard();
void DI_FinishKeyboard();
bool DI_StartJoystick();

#define DI_Finish	( (void(__cdecl*)()) 0x004756F0 )
