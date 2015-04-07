#pragma once

#include "HID.hpp"

#include <cassert>
#include <functional>
#include <mutex>
#include <thread>
#include <array>

#import <IOKit/hid/IOHIDLib.h>

class Pad : public HID
{
	//const size_t MAX_CONTROLLERS = 4;
	struct PadState
	{
		std::array<double, 3> position;
		std::array<double, 3> gimbal;
		std::array<bool, 2> button;
		IOHIDDeviceRef device;
	};
	IOHIDManagerRef hidManager;
	PadState state;
	
//	std::array<double, 3> position;
//	std::array<double, 3> gimbal;
	
	//std::function<HDCallbackCode HDCALLBACK(void*)> callback;
	mutable std::mutex device_mutex;
public:
	Pad();
	~Pad();
	
	/*
		position and gimbal sono la differenza rispetto alla posizione e l'orientamento alla pressione dei relativi bottoni (botton_1 displacement, botton_2 gimbal)
		position mm
		gimbal rad
	 */
	StatusStruct Status() override;
	void Force(std::array<double, 3> f) override;
	
private:
	static void gamepadWasAdded(void* inContext, IOReturn inResult, void* inSender, IOHIDDeviceRef device);
	static void gamepadWasRemoved(void* inContext, IOReturn inResult, void* inSender, IOHIDDeviceRef device);
	void updateStatus();
};