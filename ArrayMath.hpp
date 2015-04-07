#pragma once

#include "AlignedAllocator.hpp"
#include <cstddef>

//#include <xmmintrin.h>
#include <immintrin.h>

//#ifdef __SSE2__
//#include <emmintrin.h>
//#endif
//
//#ifdef __SSE3__
//#include <pmmintrin.h>
//#endif
//
//#ifdef __SSE4_1__
//#include <smmintrin.h>
//#endif

//#include <immintrin.h> // https://software.intel.com/sites/products/documentation/doclib/iss/2013/compiler/cpp-lin/index.htm#GUID-712779D8-D085-4464-9662-B630681F16F1.htm github nova-simd

#if defined (__SSE4_2__)
typedef __m128 vec4;
const size_t vec4_alignment = 16;
#elif defined (__AVX__)
typedef __m256d vec4;
const size_t vec4_alignment = 16;
#endif

template <typename T>
struct simd;

template <>
struct simd<float>
{
	typedef __m128 type;
	static constexpr size_t size = sizeof(float)*4;
};

//template <>
//struct simd<double>
//{
//	typedef __m256d type;
//	static constexpr size_t size = sizeof(float)*4;
//};


template <typename T>
struct vec4_simd
{
	typedef typename simd<T>::type simd_type;
	typedef vec4_simd<T> type;
	typedef T value_type;
	typedef aligned_allocator<T, simd<T>::size> allocator;

	 union
	 {
	 	struct { T x, y, z, w; };
	 	struct { T r, g, b, a; };
	 	struct { T s, t, p, q; };
		simd_type data;
	 };

	// typedef size_t size_type;
	// size_type size() const { return 4; }
	// double& operator[](size_type i);
	// double const& operator[](size_type i) const;

	vec4_simd<T>();
	vec4_simd<T>(typename simd<T>::type v);
	explicit vec4_simd<T>(T _s);
	explicit vec4_simd<T>(T _a, T _b, T _c,T _d);
	~vec4_simd<T>() {}

	vec4_simd<T>& operator=(vec4_simd<T> const& v);

	vec4_simd<T>& operator+=(T _s);
	vec4_simd<T>& operator+=(vec4_simd<T> const& v);
	
	vec4_simd<T>& operator-=(T _s);
	vec4_simd<T>& operator-=(vec4_simd<T> const& v);

	vec4_simd<T>& operator*=(T _s);
	vec4_simd<T>& operator*=(vec4_simd<T> const& v);

	vec4_simd<T>& operator/=(T _s);
	vec4_simd<T>& operator/=(vec4_simd<T> const& v);

	vec4_simd<T>& normalize();
};

template <typename T>
vec4_simd<T> operator+(vec4_simd<T> const& v, T _s);
template <typename T>
vec4_simd<T> operator+(T _s, vec4_simd<T> const& v);
template <typename T>
vec4_simd<T> operator+(vec4_simd<T> const& v1, vec4_simd<T> const& v2);

template <typename T>
vec4_simd<T> operator-(vec4_simd<T> const& v, T _s);
template <typename T>
vec4_simd<T> operator-(T _s, vec4_simd<T> const& v);
template <typename T>
vec4_simd<T> operator-(vec4_simd<T> const& v1, vec4_simd<T> const& v2);

template <typename T>
vec4_simd<T> operator*(vec4_simd<T> const& v, T _s);
template <typename T>
vec4_simd<T> operator*(T _s, vec4_simd<T> const& v);
template <typename T>
vec4_simd<T> operator*(vec4_simd<T> const& v1, vec4_simd<T> const& v2);

template <typename T>
vec4_simd<T> operator/(vec4_simd<T> const& v, T _s);
template <typename T>
vec4_simd<T> operator/(T _s, vec4_simd<T> const& v);
template <typename T>
vec4_simd<T> operator/(vec4_simd<T> const& v1, vec4_simd<T> const& v2);

template <typename T>
vec4_simd<T> operator-(vec4_simd<T> const& v);

template <typename T>
vec4_simd<T> operator<(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
template <typename T>
vec4_simd<T> operator<=(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
template <typename T>
vec4_simd<T> operator>(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
template <typename T>
vec4_simd<T> operator>=(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
template <typename T>
vec4_simd<T> operator==(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
template <typename T>
vec4_simd<T> operator!=(vec4_simd<T> const& v1, vec4_simd<T> const& v2);

template <typename T>
vec4_simd<T> operator&(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
template <typename T>
vec4_simd<T> operator|(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
template <typename T>
vec4_simd<T> operator^(vec4_simd<T> const& v1, vec4_simd<T> const& v2);

template <typename T>
vec4_simd<T> lt(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
template <typename T>
vec4_simd<T> le(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
template <typename T>
vec4_simd<T> gt(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
template <typename T>
vec4_simd<T> ge(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
template <typename T>
vec4_simd<T> eq(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
template <typename T>
vec4_simd<T> neq(vec4_simd<T> const& v1, vec4_simd<T> const& v2);

template <typename T>
vec4_simd<T> abs(vec4_simd<T> const& v);
template <typename T>
vec4_simd<T> square(vec4_simd<T> const& v);
template <typename T>
vec4_simd<T> sqrt(vec4_simd<T> const& v);
template <typename T>
vec4_simd<T> normalize(vec4_simd<T> const& v);
template <typename T>
vec4_simd<T> length(vec4_simd<T> const& v);
template <typename T>
vec4_simd<T> lengthsquare(vec4_simd<T> const& v);
template <typename T>
vec4_simd<T> dot(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
template <typename T>
vec4_simd<T> max_(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
template <typename T>
vec4_simd<T> min_(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
template <typename T>
bool any(vec4_simd<T> const& v);
template <typename T>
bool all(vec4_simd<T> const& v);

#if defined (__SSE4_2__)
#include "ArrayMath.sse"
#elif defined (__AVX__)
#include "ArrayMath.avx"
#endif








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