#include "XInput.hpp"

#include <cstring>

XInput::XInput()
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
		std::array<double, 3>{
			(double(g_Controllers.state.Gamepad.sThumbLX)/32768.0) - start_position[0],
			(double(g_Controllers.state.Gamepad.sThumbLY)/32768.0) - start_position[1],
			(double(g_Controllers.state.Gamepad.sThumbRY)/32768.0) - start_position[2]
		},
		std::array<double, 3>{0.0, 0.0,
			double(g_Controllers.state.Gamepad.sThumbRX)/32768.0- start_gimbal[2]
		}
	};
}

void XInput::Force(std::array<double, 3> f)
{
	// XInputSetState(0, &g_Controllers.vibration);
}

void XInput::updateStatus()
{
	g_Controllers.lastState = g_Controllers.state;
	g_Controllers.dwResult = XInputGetState(0, &g_Controllers.state);
	assert( g_Controllers.dwResult == ERROR_SUCCESS);

	if(!(g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		start_position = std::array<double, 3>{
			double(g_Controllers.state.Gamepad.sThumbLX)/32768.0,
			double(g_Controllers.state.Gamepad.sThumbLY)/32768.0,
			double(g_Controllers.state.Gamepad.sThumbRY)/32768.0
		};
	}
	if(!(g_Controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		start_gimbal = std::array<double, 3>{ 0.0, 0.0,
			double(g_Controllers.state.Gamepad.sThumbRX)/32768.0
		};
	}
}
