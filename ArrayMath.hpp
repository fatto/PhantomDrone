#pragma once
#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcd.lib")

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
//#ifdef  _WIN64
//#define __AVX__
//#endif

inline void* operator new(size_t size)
{
		return _mm_malloc(size, 64);
}

inline void* operator new[](size_t size)
{
		return _mm_malloc(size, 64);
}

inline void operator delete(void *mem)
{
		_mm_free(mem);
}

inline void operator delete[](void *mem)
{
		_mm_free(mem);
}


//dot4
//__m256d xy0 = _mm256_mul_pd(x[0], y[0]);
//__m256d xy1 = _mm256_mul_pd(x[1], y[1]);
//__m256d xy2 = _mm256_mul_pd(x[2], y[2]);
//__m256d xy3 = _mm256_mul_pd(x[3], y[3]);
//
//// low to high: xy00+xy01 xy10+xy11 xy02+xy03 xy12+xy13
//__m256d temp01 = _mm256_hadd_pd(xy0, xy1);
//
//// low to high: xy20+xy21 xy30+xy31 xy22+xy23 xy32+xy33
//__m256d temp23 = _mm256_hadd_pd(xy2, xy3);
//
//// low to high: xy02+xy03 xy12+xy13 xy20+xy21 xy30+xy31
//__m256d swapped = _mm256_permute2f128_pd(temp01, temp23, 0x21);
//
//// low to high: xy00+xy01 xy10+xy11 xy22+xy23 xy32+xy33
//__m256d blended = _mm256_blend_pd(temp01, temp23, 0b1100);
//
//__m256d dotproduct = _mm256_add_pd(swapped, blended);
//
//
//dot2
//__m256d xy = _mm256_mul_pd(x, y);
//__m256d zw = _mm256_mul_pd(z, w);
//__m256d temp = _mm256_hadd_pd(xy, zw);
//__m128d hi128 = _mm256_extractf128_pd(temp, 1);
//__m128d dotproduct = _mm_add_pd((__m128d)temp, hi128);

#if defined (__AVX__)

struct alignas(__m256d) vector4
{
	union
	{
		struct { double x, y, z, w; };
		struct { double r, g, b, a; };
		struct { double s, t, p, q; };
		__m256d data;
	};
	inline vector4() : data(_mm256_setzero_pd()) {}
	inline vector4(double _s) : data(_mm256_set1_pd(_s)) {}
	inline vector4(double _a, double _b, double _c, double _d) : data(_mm256_setr_pd(_a, _b, _c, _d)) {}
	inline vector4(__m256d _v) : data(_v) {}

	// arithmetic operators with vector4
	inline vector4 operator+(const vector4& _b) const { return _mm256_add_pd(data, _b.data); }
	inline vector4 operator-(const vector4& _b) const { return _mm256_sub_pd(data, _b.data); }
	inline vector4 operator*(const vector4& _b) const { return _mm256_mul_pd(data, _b.data); }
	inline vector4 operator/(const vector4& _b) const { return _mm256_div_pd(data, _b.data); }

	// op= operators
	inline vector4& operator+=(const vector4& _b) { data = _mm256_add_pd(data, _b.data); return *this; }
	inline vector4& operator-=(const vector4& _b) { data = _mm256_sub_pd(data, _b.data); return *this; }
	inline vector4& operator*=(const vector4& _b) { data = _mm256_mul_pd(data, _b.data); return *this; }
	inline vector4& operator/=(const vector4& _b) { data = _mm256_div_pd(data, _b.data); return *this; }

	// arithmetic operators with double
	inline vector4 operator+(double _b) const { return _mm256_add_pd(data, _mm256_set1_pd(_b)); }
	inline vector4 operator-(double _b) const { return _mm256_sub_pd(data, _mm256_set1_pd(_b)); }
	inline vector4 operator*(double _b) const { return _mm256_mul_pd(data, _mm256_set1_pd(_b)); }
	inline vector4 operator/(double _b) const { return _mm256_div_pd(data, _mm256_set1_pd(_b)); }

	// op= operators with double
	inline vector4& operator+=(double _b) { data = _mm256_add_pd(data, _mm256_set1_pd(_b)); return *this; }
	inline vector4& operator-=(double _b) { data = _mm256_sub_pd(data, _mm256_set1_pd(_b)); return *this; }
	inline vector4& operator*=(double _b) { data = _mm256_mul_pd(data, _mm256_set1_pd(_b)); return *this; }
	inline vector4& operator/=(double _b) { data = _mm256_div_pd(data, _mm256_set1_pd(_b)); return *this; }

	// cross product
	// inline vector4 cross(const vector4& b) const
	// {
	// return _mm_sub_ps(
	// _mm_mul_ps(_mm_shuffle_ps(data, data, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b.data, b.data, _MM_SHUFFLE(3, 1, 0, 2))), 
	// _mm_mul_ps(_mm_shuffle_ps(data, data, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(b.data, b.data, _MM_SHUFFLE(3, 0, 2, 1)))
	// );
	// }

	// dot product with another vector
	inline vector4 dot(const vector4& _b) const
	{
		vector4 res = (*this)*_b;
		res.data = _mm256_hadd_pd(res.data, _mm256_permute2f128_pd(res.data, res.data, 1));
		res.data = _mm256_hadd_pd(res.data, res.data);
		return res;
	}
	// length of the vector
	inline vector4 length() const { return vector4(_mm256_sqrt_pd(dot(*this).data)); }
	// returns the vector scaled to unit length
	inline vector4 normalize() const { return (*this)/length(); }
	inline vector4 abs() const { union { uint64_t i; double d; } u; u.i = 0x7fffffffffffffff; /*abs_mask*/ return vector4(_mm256_and_pd(_mm256_set1_pd(u.d), data)); }

	inline vector4 shufflePermute(const vector4& _b) { return vector4(_mm256_permute_pd(_mm256_shuffle_pd(data, _b.data, 0b1010),0b0101)); } 
	// inline vector4 permute() const { return vector4(_mm256_permute_pd(data, 0b0101)); }
	// inline vector4 blend(const vector4& _b) const { return vector4(_mm256_blend_pd(data, _b.data, 0b0101)); }

	// bool mask
	inline int mask() const { return _mm256_movemask_pd(data); }
};

 // union{uint64_t i; T d;} u;	u.i = 0x8000000000000000;/*sign_mask*/ return _mm256_xor_pd(_mm256_set1_pd(u.d), v.data);
inline vector4 operator-(const vector4& _a) { return vector4(_mm256_xor_pd(_mm256_set1_pd(-0.0), _a.data)); }

inline vector4 operator+(double _a, const vector4& _b) { return _b + _a; }
inline vector4 operator-(double _a, const vector4& _b) { return vector4(_mm256_set1_pd(_a)) - _b; }
inline vector4 operator*(double _a, const vector4& _b) { return _b * _a; }
inline vector4 operator/(double _a, const vector4& _b) { return vector4(_mm256_set1_pd(_a)) / _b; }

inline vector4 lt(const vector4& v1, const vector4& v2) { return vector4(_mm256_cmp_pd(v1.data, v2.data, _CMP_LT_OS)); }
inline vector4 le(const vector4& v1, const vector4& v2) { return vector4(_mm256_cmp_pd(v1.data, v2.data, _CMP_LE_OS)); }
inline vector4 gt(const vector4& v1, const vector4& v2) { return vector4(_mm256_cmp_pd(v1.data, v2.data, _CMP_NLE_US)); }
inline vector4 ge(const vector4& v1, const vector4& v2) { return vector4(_mm256_cmp_pd(v1.data, v2.data, _CMP_NLT_US)); }
inline vector4 eq(const vector4& v1, const vector4& v2) { return vector4(_mm256_cmp_pd(v1.data, v2.data, _CMP_EQ_OQ)); }
inline vector4 neq(const vector4& v1, const vector4& v2) { return vector4(_mm256_cmp_pd(v1.data, v2.data, _CMP_NEQ_UQ)); }

inline vector4 dot4(const vector4& v1, const vector4& v2, const vector4& v3, const vector4& v4, const vector4& v5, const vector4& v6, const vector4& v7, const vector4& v8)
{
	vector4 v12 = v1*v2;
	vector4 v34 = v3*v4;
	vector4 v56 = v5*v6;
	vector4 v78 = v7*v8;

	vector4 temp1 = vector4(_mm256_hadd_pd(v12.data, v34.data));
	vector4 temp2 = vector4(_mm256_hadd_pd(v56.data, v78.data));
	
	vector4 swapped = vector4(_mm256_permute2f128_pd(temp1.data, temp2.data, 0x21));
	vector4 blended = vector4(_mm256_blend_pd(temp1.data, temp2.data, 0b1100));

	return vector4(_mm256_add_pd(swapped.data, blended.data));
}

inline vector4 max_(const vector4& v1,const vector4& v2)
{
	return _mm256_max_pd(v1.data, v2.data);
}
inline vector4 min_(const vector4& v1, const vector4& v2)
{
	return _mm256_min_pd(v1.data, v2.data);
}
inline vector4 operator&(const vector4& v1, const vector4& v2)
{
	return _mm256_and_pd(v1.data, v2.data);
}
// inline vector4 operator|(const vector4& v1, const vector4& v2)
// {
// 	return _mm256_or_pd(v1.data, v2.data);
// }
// inline vector4 operator^(const vector4& v1, const vector4& v2)
// {
// 	return _mm256_xor_pd(v1.data, v2.data);
// }
#elif defined (__SSE4_2__)

// struct alignas(__m128) vector4
// {
// 	union
// 	{
// 		struct { double x, y, z, w; };
// 		struct { double r, g, b, a; };
// 		struct { double s, t, p, q; };
// 		__m128 data;
// 	};
// 	inline vector4() : mmvalue(_mm_setzero_ps()) {}
// 	inline vector4(float x, float y, float z) : mmvalue(_mm_set_ps(0, z, y, x)) {}
// 	inline vector4(__m128 m) : mmvalue(m) {}

// 	// arithmetic operators with vector4
// 	inline vector4 operator+(const vector4& b) const { return _mm_add_ps(mmvalue, b.mmvalue); }
// 	inline vector4 operator-(const vector4& b) const { return _mm_sub_ps(mmvalue, b.mmvalue); }
// 	inline vector4 operator*(const vector4& b) const { return _mm_mul_ps(mmvalue, b.mmvalue); }
// 	inline vector4 operator/(const vector4& b) const { return _mm_div_ps(mmvalue, b.mmvalue); }

// 	// op= operators
// 	inline vector4& operator+=(const vector4& b) { mmvalue = _mm_add_ps(mmvalue, b.mmvalue); return *this; }
// 	inline vector4& operator-=(const vector4& b) { mmvalue = _mm_sub_ps(mmvalue, b.mmvalue); return *this; }
// 	inline vector4& operator*=(const vector4& b) { mmvalue = _mm_mul_ps(mmvalue, b.mmvalue); return *this; }
// 	inline vector4& operator/=(const vector4& b) { mmvalue = _mm_div_ps(mmvalue, b.mmvalue); return *this; }

// 	// arithmetic operators with float
// 	inline vector4 operator+(float b) const { return _mm_add_ps(mmvalue, _mm_set1_ps(b)); }
// 	inline vector4 operator-(float b) const { return _mm_sub_ps(mmvalue, _mm_set1_ps(b)); }
// 	inline vector4 operator*(float b) const { return _mm_mul_ps(mmvalue, _mm_set1_ps(b)); }
// 	inline vector4 operator/(float b) const { return _mm_div_ps(mmvalue, _mm_set1_ps(b)); }

// 	// op= operators with float
// 	inline vector4& operator+=(float b) { mmvalue = _mm_add_ps(mmvalue, _mm_set1_ps(b)); return *this; }
// 	inline vector4& operator-=(float b) { mmvalue = _mm_sub_ps(mmvalue, _mm_set1_ps(b)); return *this; }
// 	inline vector4& operator*=(float b) { mmvalue = _mm_mul_ps(mmvalue, _mm_set1_ps(b)); return *this; }
// 	inline vector4& operator/=(float b) { mmvalue = _mm_div_ps(mmvalue, _mm_set1_ps(b)); return *this; }

// 	// cross product
// 	inline vector4 cross(const vector4& b) const
// 	{
// 	return _mm_sub_ps(
// 	_mm_mul_ps(_mm_shuffle_ps(mmvalue, mmvalue, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b.mmvalue, b.mmvalue, _MM_SHUFFLE(3, 1, 0, 2))), 
// 	_mm_mul_ps(_mm_shuffle_ps(mmvalue, mmvalue, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(b.mmvalue, b.mmvalue, _MM_SHUFFLE(3, 0, 2, 1)))
// 	);
// 	}

// 	// dot product with another vector
// 	inline float dot(const vector4& b) const { return _mm_cvtss_f32(_mm_dp_ps(mmvalue, b.mmvalue, 0x71)); }
// 	// length of the vector
// 	inline float length() const { return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(mmvalue, mmvalue, 0x71))); }
// 	// 1/length() of the vector
// 	inline float rlength() const { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_dp_ps(mmvalue, mmvalue, 0x71))); }
// 	// returns the vector scaled to unit length
// 	inline vector4 normalize() const { return _mm_mul_ps(mmvalue, _mm_rsqrt_ps(_mm_dp_ps(mmvalue, mmvalue, 0x7F))); }

// 	// overloaded operators that ensure alignment
// 	inline void* operator new[](size_t x) { return _aligned_malloc(x, 16); }
// 	inline void operator delete[](void* x) { if (x) _aligned_free(x); }
// };

// inline vector4 operator+(float a, const vector4& b) { return b + a; }
// inline vector4 operator-(float a, const vector4& b) { return vector4(_mm_set1_ps(a)) - b; }
// inline vector4 operator*(float a, const vector4& b) { return b * a; }
// inline vector4 operator/(float a, const vector4& b) { return vector4(_mm_set1_ps(a)) / b; }
#endif

// template <typename T>
// struct simd;

// template <>
// struct simd<float>
// {
// 	typedef __m128 type;
// 	static constexpr size_t size = sizeof(float)*4;
// };

// //template <>
// //struct simd<double>
// //{
// //	typedef __m256d type;
// //	static constexpr size_t size = sizeof(float)*4;
// //};


// template <typename T>
// struct vec4_simd
// {
// 	typedef typename simd<T>::type simd_type;
// 	typedef vec4_simd<T> type;
// 	typedef T value_type;
// 	typedef aligned_allocator<T, simd<T>::size> allocator;

// 	 union
// 	 {
// 	 	struct { T x, y, z, w; };
// 	 	struct { T r, g, b, a; };
// 	 	struct { T s, t, p, q; };
// 		simd_type data;
// 	 };

// 	// typedef size_t size_type;
// 	// size_type size() const { return 4; }
// 	// double& operator[](size_type i);
// 	// double const& operator[](size_type i) const;

// 	vec4_simd<T>();
// 	vec4_simd<T>(typename simd<T>::type v);
// 	explicit vec4_simd<T>(T _s);
// 	explicit vec4_simd<T>(T _a, T _b, T _c,T _d);
// 	~vec4_simd<T>() {}

// 	vec4_simd<T>& operator=(vec4_simd<T> const& v);

// 	vec4_simd<T>& operator+=(T _s);
// 	vec4_simd<T>& operator+=(vec4_simd<T> const& v);
	
// 	vec4_simd<T>& operator-=(T _s);
// 	vec4_simd<T>& operator-=(vec4_simd<T> const& v);

// 	vec4_simd<T>& operator*=(T _s);
// 	vec4_simd<T>& operator*=(vec4_simd<T> const& v);

// 	vec4_simd<T>& operator/=(T _s);
// 	vec4_simd<T>& operator/=(vec4_simd<T> const& v);

// 	vec4_simd<T>& normalize();
// };

// template <typename T>
// vec4_simd<T> operator+(vec4_simd<T> const& v, T _s);
// template <typename T>
// vec4_simd<T> operator+(T _s, vec4_simd<T> const& v);
// template <typename T>
// vec4_simd<T> operator+(vec4_simd<T> const& v1, vec4_simd<T> const& v2);

// template <typename T>
// vec4_simd<T> operator-(vec4_simd<T> const& v, T _s);
// template <typename T>
// vec4_simd<T> operator-(T _s, vec4_simd<T> const& v);
// template <typename T>
// vec4_simd<T> operator-(vec4_simd<T> const& v1, vec4_simd<T> const& v2);

// template <typename T>
// vec4_simd<T> operator*(vec4_simd<T> const& v, T _s);
// template <typename T>
// vec4_simd<T> operator*(T _s, vec4_simd<T> const& v);
// template <typename T>
// vec4_simd<T> operator*(vec4_simd<T> const& v1, vec4_simd<T> const& v2);

// template <typename T>
// vec4_simd<T> operator/(vec4_simd<T> const& v, T _s);
// template <typename T>
// vec4_simd<T> operator/(T _s, vec4_simd<T> const& v);
// template <typename T>
// vec4_simd<T> operator/(vec4_simd<T> const& v1, vec4_simd<T> const& v2);

// template <typename T>
// vec4_simd<T> operator-(vec4_simd<T> const& v);

// template <typename T>
// vec4_simd<T> operator<(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
// template <typename T>
// vec4_simd<T> operator<=(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
// template <typename T>
// vec4_simd<T> operator>(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
// template <typename T>
// vec4_simd<T> operator>=(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
// template <typename T>
// vec4_simd<T> operator==(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
// template <typename T>
// vec4_simd<T> operator!=(vec4_simd<T> const& v1, vec4_simd<T> const& v2);

// template <typename T>
// vec4_simd<T> operator&(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
// template <typename T>
// vec4_simd<T> operator|(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
// template <typename T>
// vec4_simd<T> operator^(vec4_simd<T> const& v1, vec4_simd<T> const& v2);

// template <typename T>
// vec4_simd<T> lt(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
// template <typename T>
// vec4_simd<T> le(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
// template <typename T>
// vec4_simd<T> gt(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
// template <typename T>
// vec4_simd<T> ge(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
// template <typename T>
// vec4_simd<T> eq(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
// template <typename T>
// vec4_simd<T> neq(vec4_simd<T> const& v1, vec4_simd<T> const& v2);

// template <typename T>
// vec4_simd<T> abs(vec4_simd<T> const& v);
// template <typename T>
// vec4_simd<T> square(vec4_simd<T> const& v);
// template <typename T>
// vec4_simd<T> sqrt(vec4_simd<T> const& v);
// template <typename T>
// vec4_simd<T> normalize(vec4_simd<T> const& v);
// template <typename T>
// vec4_simd<T> length(vec4_simd<T> const& v);
// template <typename T>
// vec4_simd<T> lengthsquare(vec4_simd<T> const& v);
// template <typename T>
// vec4_simd<T> dot(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
// template <typename T>
// vec4_simd<T> max_(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
// template <typename T>
// vec4_simd<T> min_(vec4_simd<T> const& v1, vec4_simd<T> const& v2);
// template <typename T>
// bool any(vec4_simd<T> const& v);
// template <typename T>
// bool all(vec4_simd<T> const& v);

// #if defined (__AVX__)
// #include "ArrayMath.sse"
// #elif defined (__SSE4_2__)
// #include "ArrayMath.avx"
// #endif








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