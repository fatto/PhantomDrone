#pragma once

#include "HID.hpp"

#include <cassert>
#include <functional>
#include <mutex>
#include <thread>
#include <array>

#include <windows.h>
#include <XInput.h>
#pragma comment(lib,"xinput.lib")


class XInput : public HID
{
	//const size_t MAX_CONTROLLERS = 4;
	struct CONTROLLER_STATE
	{
		XINPUT_STATE lastState;
		XINPUT_STATE state;
		DWORD dwResult;
		bool bLockVibration;
		XINPUT_VIBRATION vibration;
	};
	CONTROLLER_STATE g_Controllers;

	std::array<double, 3> start_position;
	std::array<double, 3> start_gimbal;

	//std::function<HDCallbackCode HDCALLBACK(void*)> callback;
	mutable std::mutex device_mutex;
public:
	XInput();
	~XInput();
	
	/*
		position and gimbal sono la differenza rispetto alla posizione e l'orientamento alla pressione dei relativi bottoni (botton_1 displacement, botton_2 gimbal)
		position mm
		gimbal rad
	*/
	StatusStruct Status() override;
	void Force(std::array<double, 3> f) override;

private:
	void updateStatus();
};