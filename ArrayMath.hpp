#pragma once

#include <array>

using array3d = std::array<double, 3>;

inline array3d operator+(const array3d& l, const array3d& r)
{
	return { l[0] + r[0], l[1] + r[1], l[2] + r[2] };
}

inline array3d operator-(const array3d& l, array3d& r)
{
	return{ l[0] - r[0], l[1] - r[1], l[2] - r[2] };
}

inline array3d operator*(const array3d& l, const double& r)
{
	return{ l[0] * r, l[1] * r, l[2] * r };
}

inline array3d operator*(const double& r, const array3d& l)
{
	return{ l[0] * r, l[1] * r, l[2] * r };
}

template <typename T>
int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

template < typename T>
T clamp(T val, T low, T high)
{
	return val > high ? val : val<low ? low : val;
}