#pragma once
#include <cstddef>
#include <array>

struct alignas(double[4]) vector4
{
	union
	{
		struct { double x, y, z, w; };
		struct { double r, g, b, a; };
		struct { double s, t, p, q; };
		std::array<double, 4> data;
	};
	inline vector4() : data(std::array<double, 4>{0.0, 0.0, 0.0, 0.0}) {}
	inline vector4(double _s) : data(std::array<double, 4>{_s, _s, _s, _s}) {}
	inline vector4(double _a, double _b, double _c, double _d) : data(std::array<double, 4>{_a, _b, _c, _d}) {}
	inline vector4(std::array<double, 4> _v) : data(_v) {}

	// arithmetic operators with vector4
	inline vector4 operator+(const vector4& _b) const { return {x+_b.x, y+_b.y, z+_b.z , w+_b.w}; }
	inline vector4 operator-(const vector4& _b) const { return {x-_b.x, y-_b.y, z-_b.z , w-_b.w}; }
	inline vector4 operator*(const vector4& _b) const { return {x*_b.x, y*_b.y, z*_b.z , w*_b.w}; }
	inline vector4 operator/(const vector4& _b) const { return {x/_b.x, y/_b.y, z/_b.z , w/_b.w}; }

	// op= operators
	inline vector4& operator+=(const vector4& _b) { *this = (*this)+_b; return *this; }
	inline vector4& operator-=(const vector4& _b) { *this = (*this)+_b; return *this; }
	inline vector4& operator*=(const vector4& _b) { *this = (*this)+_b; return *this; }
	inline vector4& operator/=(const vector4& _b) { *this = (*this)+_b; return *this; }

	// arithmetic operators with double
	inline vector4 operator+(double _b) const { return (*this) + vector4(_b); }
	inline vector4 operator-(double _b) const { return (*this) - vector4(_b); }
	inline vector4 operator*(double _b) const { return (*this) * vector4(_b); }
	inline vector4 operator/(double _b) const { return (*this) / vector4(_b); }

	// op= operators with double
	inline vector4& operator+=(double _b) { *this = (*this)+vector4(_b); return *this; }
	inline vector4& operator-=(double _b) { *this = (*this)-vector4(_b); return *this; }
	inline vector4& operator*=(double _b) { *this = (*this)*vector4(_b); return *this; }
	inline vector4& operator/=(double _b) { *this = (*this)/vector4(_b); return *this; }

	// cross product
	inline vector4 cross(const vector4& _b) const
	{
		vector4 u1(y, z, x, w), u2(z, x, y, w);
		vector4 w1(_b.z, _b.x, _b.y, _b.w), w2(_b.y, _b.z, _b.x, _b.w);

		return (u1*w1) - (u2*w2);
	}

	// dot product with another vector
	inline double dot(const vector4& _b) const
	{
		vector4 res = (*this)*_b;
		return res.x + res.y + res.z; // dot is in 3 dimension
	}
	// length of the vector
	inline double length() const { return std::sqrt(dot(*this)); }
	// returns the vector scaled to unit length
	inline vector4 normalize() const { return (*this)/length(); }
	inline vector4 abs() const { return { std::abs(x), std::abs(y), std::abs(z), std::abs(w)}; }

	// inline vector4 shufflePermute(const vector4& _b) { return vector4(_mm256_permute_pd(_mm256_shuffle_pd(data, _b.data, 0b1010),0b0101)); } 
	// inline vector4 permute() const { return vector4(_mm256_permute_pd(data, 0b0101)); }
	// inline vector4 blend(const vector4& _b) const { return vector4(_mm256_blend_pd(data, _b.data, 0b0101)); }

	// bool mask
	// inline int mask() const { return _mm256_movemask_pd(data); }
};

 // union{uint64_t i; T d;} u;	u.i = 0x8000000000000000;/*sign_mask*/ return _mm256_xor_pd(_mm256_set1_pd(u.d), v.data);
inline vector4 operator-(const vector4& _a) { return {-_a.x, -_a.y, -_a.z, -_a.w}; }

inline vector4 operator+(double _a, const vector4& _b) { return vector4(_a) + _b; }
inline vector4 operator-(double _a, const vector4& _b) { return vector4(_a) - _b; }
inline vector4 operator*(double _a, const vector4& _b) { return vector4(_a) * _b; }
inline vector4 operator/(double _a, const vector4& _b) { return vector4(_a) / _b; }

inline std::array<bool,4> lt(const vector4& v1, const vector4& v2) { return {v1.x < v2.x, v1.y < v2.y, v1.z < v2.z, v1.w < v2.w}; }
inline std::array<bool,4> le(const vector4& v1, const vector4& v2) { return {v1.x <= v2.x, v1.y <= v2.y, v1.z <= v2.z, v1.w <= v2.w}; }
inline std::array<bool,4> gt(const vector4& v1, const vector4& v2) { return {v1.x > v2.x, v1.y > v2.y, v1.z > v2.z, v1.w > v2.w}; }
inline std::array<bool,4> ge(const vector4& v1, const vector4& v2) { return {v1.x >= v2.x, v1.y >= v2.y, v1.z >= v2.z, v1.w >= v2.w}; }
inline std::array<bool,4> eq(const vector4& v1, const vector4& v2) { return {v1.x == v2.x, v1.y == v2.y, v1.z == v2.z, v1.w == v2.w}; }
inline std::array<bool,4> neq(const vector4& v1, const vector4& v2) { return {v1.x != v2.x, v1.y != v2.y, v1.z != v2.z, v1.w != v2.w}; }

// inline vector4 dot4(const vector4& v1, const vector4& v2, const vector4& v3, const vector4& v4, const vector4& v5, const vector4& v6, const vector4& v7, const vector4& v8)
// {
// 	vector4 v12 = v1*v2;
// 	vector4 v34 = v3*v4;
// 	vector4 v56 = v5*v6;
// 	vector4 v78 = v7*v8;

// 	vector4 temp1 = vector4(_mm256_hadd_pd(v12.data, v34.data));
// 	vector4 temp2 = vector4(_mm256_hadd_pd(v56.data, v78.data));
	
// 	vector4 swapped = vector4(_mm256_permute2f128_pd(temp1.data, temp2.data, 0x21));
// 	vector4 blended = vector4(_mm256_blend_pd(temp1.data, temp2.data, 0b1100));

// 	return vector4(_mm256_add_pd(swapped.data, blended.data));
// }

// inline vector4 max_(const vector4& v1,const vector4& v2)
// {
// 	return _mm256_max_pd(v1.data, v2.data);
// }
// inline vector4 min_(const vector4& v1, const vector4& v2)
// {
// 	return _mm256_min_pd(v1.data, v2.data);
// }
// inline vector4 operator&(const vector4& v1, const vector4& v2)
// {
// 	return _mm256_and_pd(v1.data, v2.data);
// }
// inline vector4 operator|(const vector4& v1, const vector4& v2)
// {
// 	return _mm256_or_pd(v1.data, v2.data);
// }
// inline vector4 operator^(const vector4& v1, const vector4& v2)
// {
// 	return _mm256_xor_pd(v1.data, v2.data);
// }
inline std::array<vector4, 4> lookAt(const vector4& eye, const vector4& center, const vector4& up)
{
	vector4 f = (center - eye).normalize();
	vector4 s = f.cross(up).normalize();
	vector4 u = s.cross(f);
	std::array<vector4, 4> res;
	res[0].x = s.x;
	res[1].x = s.y;
	res[2].x = s.z;
	
	res[0].y = u.x;
	res[1].y = u.y;
	res[2].y = u.z;

	res[0].z = -f.x;
	res[1].z = -f.y;
	res[2].z = -f.z;

	res[3].x = -s.dot(eye);
	res[3].y = -u.dot(eye);
	res[3].z = f.dot(eye);

	res[3].w = 1.0;

	return res;
}

// param.x = fovy, param.y = aspect, param.z = znear, param.w = zfar
inline std::array<vector4, 4> perspective(const vector4& param)
{
	double half_fov = tan(param.x / 2.0);

	std::array<vector4, 4> res;
	res[0].x = 1.0/(param.y*half_fov);
	res[1].y = 1.0/(half_fov);
	res[2].z = (param.w+param.z)/(param.w-param.z);
	res[2].w = -1.0;
	res[3].z = (2.0*param.w*param.z)/(param.w-param.z);
	
	return res;
}

template < typename T>
T clamp(T val, T low, T high)
{
	return val > high ? val : val < low ? low : val;
}