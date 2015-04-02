#pragma once

#include "AlignedAllocator.hpp"
#include <cstddef>
#include <immintrin.h> // https://software.intel.com/sites/products/documentation/doclib/iss/2013/compiler/cpp-lin/index.htm#GUID-712779D8-D085-4464-9662-B630681F16F1.htm github nova-simd

// template <typename T>
// struct simd
// {
// 	typedef T type[4];
// };

// template <>
// struct simd<float>
// {
// 	typedef __m128 type;
// };

// template <>
// struct simd<double>
// {
// 	typedef __m256d type;
// };

typedef aligned_allocator<double, 32> simd_allocator;

struct vec4d
{
	typedef __m256d simd_type;
	typedef vec4d type;
	typedef double value_type;

	union
	{
		struct { double x, y, z, w; };
		struct { double r, g, b, a; };
		struct { double s, t, p, q; };
		simd_type data;
	};

	// typedef size_t size_type;
	// size_type size() const { return 4; }
	// double& operator[](size_type i);
	// double const& operator[](size_type i) const;

	vec4d();
	vec4d(simd_type v);
	explicit vec4d(double _s);
	explicit vec4d(double _a, double _b, double _c,double _d);
	~vec4d() {}

	vec4d& operator=(vec4d const& v);

	vec4d& operator+=(double _s);
	vec4d& operator+=(vec4d const& v);
	
	vec4d& operator-=(double _s);
	vec4d& operator-=(vec4d const& v);

	vec4d& operator*=(double _s);
	vec4d& operator*=(vec4d const& v);

	vec4d& operator/=(double _s);
	vec4d& operator/=(vec4d const& v);

	vec4d& normalize();
};

vec4d operator+(vec4d const& v, double _s);
vec4d operator+(double _s, vec4d const& v);
vec4d operator+(vec4d const& v1, vec4d const& v2);

vec4d operator-(vec4d const& v, double _s);
vec4d operator-(double _s, vec4d const& v);
vec4d operator-(vec4d const& v1, vec4d const& v2);

vec4d operator*(vec4d const& v, double _s);
vec4d operator*(double _s, vec4d const& v);
vec4d operator*(vec4d const& v1, vec4d const& v2);

vec4d operator/(vec4d const& v, double _s);
vec4d operator/(double _s, vec4d const& v);
vec4d operator/(vec4d const& v1, vec4d const& v2);

vec4d operator-(vec4d const& v);

vec4d operator<(vec4d const& v1, vec4d const& v2);
vec4d operator<=(vec4d const& v1, vec4d const& v2);
vec4d operator>(vec4d const& v1, vec4d const& v2);
vec4d operator>=(vec4d const& v1, vec4d const& v2);
vec4d operator==(vec4d const& v1, vec4d const& v2);
vec4d operator!=(vec4d const& v1, vec4d const& v2);

vec4d operator&(vec4d const& v1, vec4d const& v2);
vec4d operator|(vec4d const& v1, vec4d const& v2);
vec4d operator^(vec4d const& v1, vec4d const& v2);

vec4d lt(vec4d const& v1, vec4d const& v2);
vec4d le(vec4d const& v1, vec4d const& v2);
vec4d gt(vec4d const& v1, vec4d const& v2);
vec4d ge(vec4d const& v1, vec4d const& v2);
vec4d eq(vec4d const& v1, vec4d const& v2);
vec4d neq(vec4d const& v1, vec4d const& v2);

vec4d abs(vec4d const& v);
vec4d square(vec4d const& v);
vec4d sqrt(vec4d const& v);
vec4d normalize(vec4d const& v);
vec4d length(vec4d const& v);
vec4d lengthsquare(vec4d const& v);
vec4d dot(vec4d const& v1, vec4d const& v2);
vec4d max_(vec4d const& v1, vec4d const& v2);
vec4d min_(vec4d const& v1, vec4d const& v2);
bool nonzero(vec4d const& v);













#include <array>

using array3d = std::array<double, 3>;

inline array3d operator+(const array3d& l, const array3d& r)
{
	return{ l[0] + r[0], l[1] + r[1], l[2] + r[2] };
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

//template <typename T>
//int sgn(T val)
//{
//	return (T(0) < val) - (val < T(0));
//}

template < typename T>
T clamp(T val, T low, T high)
{
	return val > high ? val : val < low ? low : val;
}