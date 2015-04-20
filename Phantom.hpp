#pragma once

#include "HID.hpp"

#include <cassert>
#include <functional>
#include <mutex>
#include <thread>
#include <array>

#include <HD/hd.h>
#include <HDU/hduVector.h>


class Phantom : HID
{
	HHD hHD; HDSchedulerHandle gCallbackHandle;
	std::array<bool, 2> button;
	hduVector3Dd start_position, position;
	hduVector3Dd update_force;
	hduVector3Dd start_gimbal, gimbal;

	HDdouble max_force;

	//std::function<HDCallbackCode HDCALLBACK(void*)> callback;
	mutable std::mutex device_mutex;
public:
	Phantom();
	~Phantom();
	
	/*
		position and gimbal sono la differenza rispetto alla posizione e l'orientamento alla pressione dei relativi bottoni (botton_1 displacement, botton_2 gimbal)
		position mm
		gimbal rad
	*/
	virtual StatusStruct Status() override;
	virtual void Force(vector4 f) override;

private:
	bool testDevice();
	static HDCallbackCode HDCALLBACK monitorDevice(void *pUserData);
};