#pragma once

#include "Drone.hpp"
#include <array>
#include <chrono>
#include <fstream>


class Controller
{
	Drone& drone;
	double pParam = 2.0;
	double iParam = 0.0;
	double dParam = 0.0;
	double vParam = -2.0;

	double cumul = 0.0;
	double lastE = 0.0;
	double pAlphaE = 0.0;
	double pBetaE = 0.0;
	double psp1 = 0.0;
	double psp0 = 0.0;
	double prevEuler = 0.0;

	using timer = std::chrono::high_resolution_clock;
	timer::time_point t;

	std::ofstream file;

public:
	Controller(Drone& d);

	// we only check the z-axis angle
	void Go(vector4 position, vector4 angle);
};
