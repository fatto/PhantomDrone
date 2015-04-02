#include "ArrayMath.hpp"

vec4d::vec4d() :
	data(_mm256_setzero_pd()) {}
vec4d::vec4d(simd_type v) :
	data(v) {}
vec4d::vec4d(double _s) :
	data(_mm256_set1_pd(_s)) {}
vec4d::vec4d(double _a, double _b, double _c,double _d) :
	data(_mm256_setr_pd(_a, _b, _c, _d)) {}

vec4d& vec4d::operator=(vec4d const& v)
{
	data = v.data;
	return *this;
}

vec4d& vec4d::operator+=(double _s)
{
	data = _mm256_add_pd(data, _mm256_set1_pd(_s));
	return *this;
}
vec4d& vec4d::operator+=(vec4d const& v)
{
	data = _mm256_add_pd(data, v.data);
	return *this;
}

vec4d& vec4d::operator-=(double _s)
{
	data = _mm256_sub_pd(data, _mm256_set1_pd(_s));
	return *this;
}
vec4d& vec4d::operator-=(vec4d const& v)
{
	data = _mm256_sub_pd(data, v.data);
	return *this;
}

vec4d& vec4d::operator*=(double _s)
{
	data = _mm256_mul_pd(data, _mm256_set1_pd(_s));
	return *this;
}
vec4d& vec4d::operator*=(vec4d const& v)
{
	data = _mm256_mul_pd(data, v.data);
	return *this;
}

vec4d& vec4d::operator/=(double _s)
{
	data = _mm256_div_pd(data, _mm256_set1_pd(_s));
	return *this;
}
vec4d& vec4d::operator/=(vec4d const& v)
{
	data = _mm256_div_pd(data, v.data);
	return *this;
}

vec4d& vec4d::normalize()
{
	// x^2, y^2, z^2, w^2
	vec4d res = square(*this);
	// x^2 + y^2 + z^2 + w^2
	res.data = _mm256_hadd_pd(res.data, _mm256_permute2f128_pd(res.data, res.data, 1));
	res.data = _mm256_hadd_pd(res.data, res.data);
	// sqrt(x^2 + y^2 + z^2 + w^2)
	res = sqrt(res);
	// (x, y, z, w)/sqrt(x^2 + y^2 + z^2 + w^2)
	/*data = data/res;*/
	*this /= res;

	return *this;
}


vec4d operator+(vec4d const& v, double _s)
{
	return _mm256_add_pd(v.data, _mm256_set1_pd(_s));
}
vec4d operator+(double _s, vec4d const& v)
{
	return _mm256_add_pd(v.data, _mm256_set1_pd(_s));
}
vec4d operator+(vec4d const& v1, vec4d const& v2)
{
	return _mm256_add_pd(v1.data, v2.data);
}

vec4d operator-(vec4d const& v, double _s)
{
	return _mm256_sub_pd(v.data, _mm256_set1_pd(_s));
}
vec4d operator-(double _s, vec4d const& v)
{
	return _mm256_sub_pd(v.data, _mm256_set1_pd(_s));
}
vec4d operator-(vec4d const& v1, vec4d const& v2)
{
	return _mm256_sub_pd(v1.data, v2.data);
}

vec4d operator*(vec4d const& v, double _s)
{
	return _mm256_mul_pd(v.data, _mm256_set1_pd(_s));
}
vec4d operator*(double _s, vec4d const& v)
{
	return _mm256_mul_pd(v.data, _mm256_set1_pd(_s));
}
vec4d operator*(vec4d const& v1, vec4d const& v2)
{
	return _mm256_mul_pd(v1.data, v2.data);
}

vec4d operator/(vec4d const& v, double _s)
{
	return _mm256_div_pd(v.data, _mm256_set1_pd(_s));
}
vec4d operator/(double _s, vec4d const& v)
{
	return _mm256_div_pd(v.data, _mm256_set1_pd(_s));
}
vec4d operator/(vec4d const& v1, vec4d const& v2)
{
	return _mm256_div_pd(v1.data, v2.data);
}

vec4d operator-(vec4d const& v)
{
	union
	{
		uint64_t i;
		double d;
	} u;
	u.i = 0x8000000000000000; // sign_mask
	return _mm256_and_pd(_mm256_set1_pd(u.d), v.data);
}


vec4d operator<(vec4d const& v1, vec4d const& v2)
{
	return _mm256_and_pd(_mm256_cmp_pd(v1.data, v2.data, _CMP_LT_OS),_mm256_set1_pd(1.0));
}
vec4d operator<=(vec4d const& v1, vec4d const& v2)
{
	return _mm256_and_pd(_mm256_cmp_pd(v1.data, v2.data, _CMP_LE_OS),_mm256_set1_pd(1.0));
}
vec4d operator>(vec4d const& v1, vec4d const& v2)
{
	return _mm256_and_pd(_mm256_cmp_pd(v1.data, v2.data, _CMP_NLE_US),_mm256_set1_pd(1.0));
}
vec4d operator>=(vec4d const& v1, vec4d const& v2)
{
	return _mm256_and_pd(_mm256_cmp_pd(v1.data, v2.data, _CMP_NLT_US),_mm256_set1_pd(1.0));
}
vec4d operator==(vec4d const& v1, vec4d const& v2)
{
	return _mm256_and_pd(_mm256_cmp_pd(v1.data, v2.data, _CMP_EQ_OQ),_mm256_set1_pd(1.0));
}
vec4d operator!=(vec4d const& v1, vec4d const& v2)
{
	return _mm256_and_pd(_mm256_cmp_pd(v1.data, v2.data, _CMP_NEQ_UQ),_mm256_set1_pd(1.0));
}

vec4d operator&(vec4d const& v1, vec4d const& v2)
{
	return _mm256_and_pd(v1.data, v2.data);
}
vec4d operator|(vec4d const& v1, vec4d const& v2)
{
	return _mm256_or_pd(v1.data, v2.data);
}
vec4d operator^(vec4d const& v1, vec4d const& v2)
{
	return _mm256_xor_pd(v1.data, v2.data);
}

vec4d lt(vec4d const& v1, vec4d const& v2)
{
	return _mm256_cmp_pd(v1.data, v2.data, _CMP_LT_OS);
}
vec4d le(vec4d const& v1, vec4d const& v2)
{
	return _mm256_cmp_pd(v1.data, v2.data, _CMP_LE_OS);
}
vec4d gt(vec4d const& v1, vec4d const& v2)
{
	return _mm256_cmp_pd(v1.data, v2.data, _CMP_NLE_US);
}
vec4d ge(vec4d const& v1, vec4d const& v2)
{
	return _mm256_cmp_pd(v1.data, v2.data, _CMP_NLT_US);
}
vec4d eq(vec4d const& v1, vec4d const& v2)
{
	return _mm256_cmp_pd(v1.data, v2.data, _CMP_EQ_OQ);
}
vec4d neq(vec4d const& v1, vec4d const& v2)
{
	return _mm256_cmp_pd(v1.data, v2.data, _CMP_NEQ_UQ);
}


vec4d abs(vec4d const& v)
{
	union
	{
		uint64_t i;
		double d;
	} u;
	u.i = 0x7fffffffffffffff; // abs_mask  -- 0x8000000000000000 sign_mask
	return _mm256_and_pd(_mm256_set1_pd(u.d), v.data);
}
vec4d square(vec4d const& v)
{
	return _mm256_mul_pd(v.data, v.data);
}
vec4d sqrt(vec4d const& v)
{
	return _mm256_sqrt_pd(v.data);
}
vec4d normalize(vec4d const& v)
{
	// x^2, y^2, z^2, w^2
	vec4d res = square(v);
	// x^2 + y^2 + z^2 + w^2
	res.data = _mm256_hadd_pd(res.data, _mm256_permute2f128_pd(res.data, res.data, 1));
	res.data = _mm256_hadd_pd(res.data, res.data);
	// sqrt(x^2 + y^2 + z^2 + w^2)
	res = sqrt(res);
	// (x, y, z, w)/sqrt(x^2 + y^2 + z^2 + w^2)
	return v/res;
}
vec4d length(vec4d const& v)
{
	// x^2, y^2, z^2, w^2
	vec4d res = square(v);
	// x^2 + y^2 + z^2 + w^2
	res.data = _mm256_hadd_pd(res.data, _mm256_permute2f128_pd(res.data, res.data, 1));
	res.data = _mm256_hadd_pd(res.data, res.data);
	// sqrt(x^2 + y^2 + z^2 + w^2)
	res = sqrt(res);

	return res;
}
vec4d lengthsquare(vec4d const& v)
{
	// x^2, y^2, z^2, w^2
	vec4d res = square(v);
	// x^2 + y^2 + z^2 + w^2
	res.data = _mm256_hadd_pd(res.data, _mm256_permute2f128_pd(res.data, res.data, 1));
	res.data = _mm256_hadd_pd(res.data, res.data);

	return res;
}
vec4d dot(vec4d const& v1, vec4d const& v2)
{
	vec4d res = v1*v2;
	res.data = _mm256_hadd_pd(res.data, _mm256_permute2f128_pd(res.data, res.data, 1));
	res.data = _mm256_hadd_pd(res.data, res.data);
	return res;
}
vec4d max_(vec4d const& v1, vec4d const& v2)
{
	return _mm256_max_pd(v1.data, v2.data);
}
vec4d min_(vec4d const& v1, vec4d const& v2)
{
	return _mm256_min_pd(v1.data, v2.data);
}
bool nonzero(vec4d const& v)
{
	return _mm256_movemask_pd(v.data) != 0;
}