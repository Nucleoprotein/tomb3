#pragma once
#include "../global/vars.h"

void inject_mpgun(bool replace);

void MPGunControl(short item_number);

enum MPGUN_STATES
{
	MPGUN_EMPTY,
	MPGUN_WAIT,
	MPGUN_WALK,
	MPGUN_RUN,
	MPGUN_AIM1,
	MPGUN_SHOOT1,
	MPGUN_AIM2,
	MPGUN_SHOOT2,
	MPGUN_SHOOT3A,
	MPGUN_SHOOT3B,
	MPGUN_SHOOT4A,
	MPGUN_AIM3,
	MPGUN_AIM4,
	MPGUN_DEATH,
	MPGUN_SHOOT4B,
	MPGUN_DUCK,
	MPGUN_DUCKED,
	MPGUN_DUCKAIM,
	MPGUN_DUCKSHOT,
	MPGUN_DUCKWALK,
	MPGUN_STAND
};
