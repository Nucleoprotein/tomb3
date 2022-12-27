#pragma once
#include "../global/vars.h"

void inject_51baton(bool replace);

void InitialiseBaton(short item_number);

enum BATON_STATES
{
	BATON_EMPTY,
	BATON_STOP,
	BATON_WALK,
	BATON_PUNCH2,
	BATON_AIM2,
	BATON_WAIT,
	BATON_AIM1,
	BATON_AIM0,
	BATON_PUNCH1,
	BATON_PUNCH0,
	BATON_RUN,
	BATON_DEATH,
	BATON_KICK,
	BATON_CLIMB3,
	BATON_CLIMB1,
	BATON_CLIMB2,
	BATON_FALL3
};
