#include "ArrayMath.hpp"

vec4d::vec4d()
{
}

vec4d::vec4d() : data(_mm256_setzero_pd()) {}
vec4d::vec4d(double s) : data(__mm256_set1_pd(s)) {}