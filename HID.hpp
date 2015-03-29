#pragma once

#include <type_traits>
#include <array>

struct HID
{
	struct StatusStruct
	{
		std::array<bool, 2> button;
		std::array<double, 3> position;
		std::array<double, 3> gimbal;
	};
	virtual StatusStruct Status() = 0;
	virtual void Force(std::array<double, 3> f) = 0;
};