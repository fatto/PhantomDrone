#pragma once

extern "C" {
#include "extApi.h"
	/*	#include "extApiCustom.h" if you wanna use custom remote API functions! */
}

#include <thread>
#include <mutex>
#include <atomic>
#include <array>

class Drone
{
	simxInt clientID;
	simxInt quadcopter;
	//simxFloat quadposition[3];
	//simxInt targetobj;
	/*simxInt propeller[4];*/
	
	std::array<simxFloat, 3> position;
	std::array<simxFloat, 3> angle;
	std::array<simxFloat, 3> velocity;
	std::array<simxFloat, 3> rotation;
	std::array<simxFloat, 4> propeller_velocity;

	std::thread updater;
	std::mutex mtx;
	std::atomic_bool end;

public:
	struct StatusStruct
	{
		std::array<simxFloat, 3> position;
		std::array<simxFloat, 3> angle;
		std::array<simxFloat, 3> velocity;
	};
	Drone();
	~Drone();
	StatusStruct Status();
	void Apply(std::array<simxFloat, 4> rotor);
private:
	void updateStatus();
};