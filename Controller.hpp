#pragma once

#include "Drone.hpp"
#include <array>
#include <chrono>
#include <fstream>

class Controller
{
	Drone& drone;
	float pParam = 2.f;
	float iParam = 0.f;
	float dParam = 0.f;
	float vParam = -2.f;

	float cumul = 0.f;
	float lastE = 0.f;
	float pAlphaE = 0.f;
	float pBetaE = 0.f;
	float psp1 = 0.f;
	float psp0 = 0.f;
	float prevEuler = 0.f;

	using timer = std::chrono::high_resolution_clock;
	timer::time_point t;

	std::ofstream file;

public:
	Controller(Drone& d);
	void Go(std::array<float,3> position, float angle);
};
