#pragma once

#include <type_traits>
#include <array>
#include "ArrayMath.hpp"

struct HID
{
	struct StatusStruct
	{
		std::array<bool, 2> button;
		vector4 position;
		vector4 gimbal;
	};
	virtual StatusStruct Status() = 0;
	virtual void Force(vector4 f) = 0;
};