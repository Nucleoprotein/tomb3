#pragma once
#include "../global/vars.h"

void inject_effect2(bool replace);

void TriggerDynamic(long x, long y, long z, long falloff, long r, long g, long b);
void ClearDynamics();
void KillEverything();
void TriggerBreath(long x, long y, long z, long xv, long yv, long zv);
void TriggerAlertLight(long x, long y, long z, long r, long g, long b, short angle, short rn);
void TriggerFireFlame(long x, long y, long z, long body_part, long type);
void TriggerFireSmoke(long x, long y, long z, long body_part, long type);
void TriggerStaticFlame(long x, long y, long z, long size);
void TriggerSideFlame(long x, long y, long z, long angle, long speed, long pilot);
void TriggerRocketSmoke(long x, long y, long z, long c);
void TriggerRicochetSpark(GAME_VECTOR* pos, long angle, long size);
void TriggerBlood(long x, long y, long z, long angle, long num);
void TriggerBloodD(long x, long y, long z, long angle, long num);
void TriggerUnderwaterBlood(long x, long y, long z, long size);
void TriggerUnderwaterBloodD(long x, long y, long z, long size);

#define TriggerWaterfallMist	( (void(__cdecl*)(long, long, long, long)) 0x0042D1F0 )
#define TriggerGunSmoke	( (void(__cdecl*)(long, long, long, long, long, long, long, long, long)) 0x0042B4F0 )
#define TriggerGunShell	( (void(__cdecl*)(short, long, long)) 0x0042BE50 )
#define TriggerExplosionSparks	( (void(__cdecl*)(long, long, long, long, long, long, short)) 0x0042AB80 )
#define TriggerExplosionSmoke	( (void(__cdecl*)(long, long, long, long)) 0x0042B130 )
#define GetFreeSpark	( (long(__cdecl*)()) 0x00429FE0 )
#define TriggerFlareSparks	( (void(__cdecl*)(long, long, long, long, long, long, long, long)) 0x0042A8B0 )
#define TriggerRocketFlame	( (void(__cdecl*)(long, long, long, long, long, long, long)) 0x0042C510 )
#define UpdateSparks	( (void(__cdecl*)()) 0x0042A0D0 )
#define UpdateSplashes	( (void(__cdecl*)()) 0x0042CED0 )
#define UpdateBats	( (void(__cdecl*)()) 0x0042D3D0 )
#define SetupRipple	( (RIPPLE_STRUCT*(__cdecl*)(long, long, long, long, long)) 0x0042D080 )
#define SetupSplash	( (void(__cdecl*)(SPLASH_SETUP*)) 0x0042CAC0 )
#define TriggerBubble	( (void(__cdecl*)(long, long, long, long, long, short)) 0x0042DAB0 )
#define DetatchSpark	( (void(__cdecl*)(long, long)) 0x00429F00 )
