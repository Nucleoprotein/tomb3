#pragma once
#include "../global/vars.h"

void HybridControl(short item_number);

void inject_hybrid(bool replace);

enum HYBRID_STATES
{
	HYBRID_EMPTY,
	HYBRID_STOP,
	HYBRID_WALK,
	HYBRID_RUN,
	HYBRID_JUMP_STR,
	HYBRID_JUMP_MID,
	HYBRID_JUMP_END,
	HYBRID_SLASH_LEFT,
	HYBRID_KICK,
	HYBRID_RUN_ATAK,
	HYBRID_WALK_ATAK,
	HYBRID_DEATH
};
