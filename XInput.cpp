#include "XInput.hpp"
#include "ArrayMath.hpp"
#include <cstring>

XInput::XInput() : position(), gimbal()
{
	memset(&g_Controllers, 0, sizeof(CONTROLLER_STATE));
}

XInput::~XInput()
{
}

XInput::StatusStruct XInput::Status()
{
	updateStatus();

	return StatusStruct{
		std::array<bool, 2>{ bool(g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER),  bool(g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)},
		position,
		gimbal
	};
}

void XInput::Force(vector4 f)
{
	// XInputSetState(0, &g_Controllers.vibration);
}

void XInput::updateStatus()
{
	g_Controllers.lastState = g_Controllers.state;
	g_Controllers.dwResult = XInputGetState(0, &g_Controllers.state);
	if (g_Controllers.dwResult == ERROR_SUCCESS)
	{

		if (g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			position = position + vector4{
				(double(g_Controllers.state.Gamepad.sThumbLX) / 32768.0)*0.1,
				(double(g_Controllers.state.Gamepad.sThumbLY) / 32768.0)*0.1,
				(double(g_Controllers.state.Gamepad.sThumbRY) / 32768.0)*0.1,
				0.0
			};
		}
		if (g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		{
			gimbal = gimbal + vector4{ 0.0, 0.0, (double(g_Controllers.state.Gamepad.sThumbRX) / 32768.0)*0.1, 0.0 };
		}
	}
}
