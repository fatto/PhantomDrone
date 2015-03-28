#pragma once

#include <array>

using array3f = std::array<float, 3>;

inline array3f operator+(const array3f& l, const array3f& r)
{
	return { l[0] + r[0], l[1] + r[1], l[2] + r[2] };
}

inline array3f operator-(const array3f& l, array3f& r)
{
	return{ l[0] - r[0], l[1] - r[1], l[2] - r[2] };
}

inline array3f operator*(const array3f& l, const float& r)
{
	return{ l[0] * r, l[1] * r, l[2] * r };
}

inline array3f operator*(const float& r, const array3f& l)
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