#pragma once

extern "C" {
#include "extApi.h"
	/*	#include "extApiCustom.h" if you wanna use custom remote API functions! */
}

#include "Copter.hpp"
#include <thread>
#include <mutex>
#include <atomic>
#include <array>
#include <vector>

#include "Sat.hpp"

class Drone
{
	simxInt clientID;
	simxInt quadcopter;
	simxInt cuboid;
	std::vector<simxInt> cuboids;
	sat::Box bbox;
	std::vector<sat::Box> boxs;
	//simxFloat quadposition[3];
	//simxInt targetobj;
	/*simxInt propeller[4];*/

	std::array<simxFloat, 3> position;
	std::array<simxFloat, 3> angle;
	std::array<simxFloat, 3> velocity;
	std::array<simxFloat, 3> rotation;
	std::array<simxFloat, 4> propeller_velocity;
	
	std::array<double, 3> reaction;

	// std::thread updater;
	// std::mutex mtx;
	// std::atomic_bool end;

public:
	struct StatusStruct
	{
		vector4 position;
		vector4 angle;
		vector4 velocity;
		vector4 reaction;
	};
	Drone();
	~Drone();

	void AddCuboid(std::array<double, 3> _position, std::array<double, 3> _angle);

	StatusStruct Status() const;
	void Apply(vector4 rotor);
private:
	void updateStatus();
};