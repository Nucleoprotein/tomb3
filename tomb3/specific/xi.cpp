#include "../tomb3/pch.h"
#include "xi.h"

void XI_Read(long* out)
{
	long linput = 0;
	static bool walk = false;
	XINPUT_STATE xinput_state;

	DWORD result = XInputGetState(0, &xinput_state);

	if (result == ERROR_SUCCESS)
	{

		if (xinput_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
			linput |= IN_FORWARD;

		if (xinput_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			linput |= IN_BACK;

		if (xinput_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
			linput |= IN_LEFT;

		if (xinput_state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
			linput |= IN_RIGHT;

		if (xinput_state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			linput |= IN_DUCK;

		if (xinput_state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			linput |= IN_SPRINT;

		if (xinput_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			linput |= IN_WALK;
			walk = true;
		}
		else
			walk = false;

		if (xinput_state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
			linput |= IN_JUMP;

		if (xinput_state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
			linput |= IN_ACTION;

		if (xinput_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
			linput |= IN_DRAW;

		if (xinput_state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
			linput |= IN_LOOK;

		if (xinput_state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
			linput |= (IN_ROLL | IN_DESELECT);

		if (xinput_state.Gamepad.wButtons & XINPUT_GAMEPAD_START)
			linput |= IN_PAUSE;

		if (walk && xinput_state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
			linput |= IN_FLARE;

		if (!walk && xinput_state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
			linput |= IN_OPTION;

		*out = linput;
	}
}