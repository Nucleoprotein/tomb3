#pragma once
#include "../global/vars.h"

void inject_collide(bool replace);

void ShiftItem(ITEM_INFO* item, COLL_INFO* coll);
void ObjectCollision(short item_num, ITEM_INFO* l, COLL_INFO* coll);
void ObjectCollisionSub(short item_num, ITEM_INFO* l, COLL_INFO* coll);
void GetNewRoom(long x, long y, long z, short room_number);
void GetNearByRooms(long x, long y, long z, long r, long h, short room_number);
short GetTiltType(FLOOR_INFO* floor, long x, long y, long z);
long CollideStaticObjects(COLL_INFO* coll, long x, long y, long z, short room_number, long hite);
long FindGridShift(long src, long dst);
void GetCollisionInfo(COLL_INFO* coll, long x, long y, long z, short room_number, long hite);
void UpdateLaraRoom(ITEM_INFO* item, long height);
void DoorCollision(short item_number, ITEM_INFO* l, COLL_INFO* coll);
void TrapCollision(short item_number, ITEM_INFO* l, COLL_INFO* coll);
long TestBoundsCollide(ITEM_INFO* item, ITEM_INFO* l, long rad);

#define LaraBaddieCollision	( (void(__cdecl*)(ITEM_INFO*, COLL_INFO*)) 0x0041E8D0 )
#define ItemPushLara	( (void(__cdecl*)(ITEM_INFO*, ITEM_INFO*, COLL_INFO*, long, long)) 0x0041EDA0 )
#define TestLaraPosition	( (long(__cdecl*)(short*, ITEM_INFO*, ITEM_INFO*)) 0x0041F1B0 )
#define MoveLaraPosition	( (long(__cdecl*)(PHD_VECTOR*, ITEM_INFO*, ITEM_INFO*)) 0x0041F430 )
#define AlignLaraPosition	( (void(__cdecl*)(PHD_VECTOR*, ITEM_INFO*, ITEM_INFO*)) 0x0041F2F0 )
