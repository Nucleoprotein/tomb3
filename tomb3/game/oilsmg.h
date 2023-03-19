#pragma once
#include "../global/vars.h"

void inject_oilsmg(bool replace);

void InitialiseOilSMG(short item_number);

enum OILSMG_STATES
{
	OILSMG_EMPTY,
	OILSMG_STOP,
	OILSMG_WALK,
	OILSMG_RUN,
	OILSMG_WAIT,
	OILSMG_SHOOT1,
	OILSMG_SHOOT2,
	OILSMG_DEATH,
	OILSMG_AIM1,
	OILSMG_AIM2,
	OILSMG_AIM3,
	OILSMG_SHOOT3
};
