#pragma once

#include <cassert>
#include <functional>
#include <mutex>
#include <thread>
#include <array>

#include <HD/hd.h>
#include <HDU/hduVector.h>


class Haptic
{
	HHD hHD; HDSchedulerHandle gCallbackHandle;
	std::array<bool, 2> button;
	hduVector3Dd start_position, position;
	hduVector3Dd force;
	hduVector3Dd start_gimbal, gimbal;

	HDdouble max_force;

	//std::function<HDCallbackCode HDCALLBACK(void*)> callback;
	std::mutex device_mutex;
public:
	struct StatusStruct
	{
		std::array<bool, 2> button;
		//std::array<double, 3> position;
		std::array<double, 3> position;
		std::array<double, 3> gimbal;
	};
	Haptic();
	~Haptic();
	/*
		position and gimbal sono la differenza rispetto alla posizione e l'orientamento alla pressione dei relativi bottoni (botton_1 displacement, botton_2 gimbal)
		position mm
		gimbal rad
	*/
	StatusStruct Status();
	/*std::array<double, 3> Position();
	std::array<double, 3> Displacement();*/
	void Force(std::array<double, 3> f);

private:
	bool testDevice();
	static HDCallbackCode HDCALLBACK monitorDevice(void *pUserData);
};