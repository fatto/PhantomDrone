//#include <Windows.h>

#ifdef  _WIN64
#pragma warning (disable:4996)
#endif

#include <cstdio>
#include <iostream>

#include <thread>



#include "Phantom.hpp"
#include "XInput.hpp"
#include "Drone.hpp"
#include "Controller.hpp"
#include "ArrayMath.hpp"
#include "Sat.hpp"
//#include "Pad.hpp"
#include <cmath>


int main(int argc, char* argv[])
{
	Drone drone;
	drone.AddCuboid({ 0.0, 0.0, 0.25 }, { 0.0, 0.0, 0.0 });
	drone.AddCuboid({ 0.0, 0.5, 0.25 }, { 0.0, 0.0, 0.0 });
	drone.AddCuboid({ 0.0, 1.0, 0.25 }, { 0.0, 0.0, 0.0 });
	drone.AddCuboid({ 0.0, 1.5, 0.25 }, { 0.0, 0.0, 0.0 });
	drone.AddCuboid({ 0.0, 2.0, 0.25 }, { 0.0, 0.0, 0.0 });
	//drone.AddCuboid({ 0.0, 1.0, 0.25 }, { 0.0, 0.0, 0.0 });
	Controller controller(drone);
	// XInput pad;
	Phantom phantom;
	auto destination = vector4{ 0.0, 0.0, 0.8, 0.0};
	auto angle = vector4{0.0, 0.0, 0.0, 0.0};
	short keystate;

	do
	{
		keystate = GetAsyncKeyState(VK_ESCAPE);
		//auto haptic_status = pad.Status();
		auto haptic_status = phantom.Status();
		auto drone_status = drone.Status();
		// auto displ = std::array<double, 3>{ haptic_status.position[0] * 0.01, haptic_status.position[1] * 0.01, haptic_status.position[2] * 0.01};
		// auto displ = std::array<double, 3>{ haptic_status.position[0], haptic_status.position[1], haptic_status.position[2]};
		vector4 dest;
		vector4 ang;

		dest = destination + haptic_status.position;
		ang = angle + haptic_status.gimbal;
		
		controller.Go(dest, ang );
		drone_status = drone.Status();
		phantom.Force(drone_status.reaction);
	} while (!((1 << 16) & keystate));
	
	return 0;
}
