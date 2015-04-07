#include "ArrayMath.hpp"

#include <vector>

using vec4 = std::vec4_simd<float>;

namespace sat
{
	struct Box
	{
		vec4 position;
		vec4 size;
		// Polygon toPolygon()
		// {
		// 	Polygon res{position, {vec4d(), }};
		// 	return Polygon(njsagiefnsgraejn);
		// }
	};
	struct Circle
	{
		vec4 position;
		vec4 radius;
		// Polygon getAABB()
		// {
		// 	vec4d corner = position - radius;
		// 	return Box(corner, radius).toPolygon();
		// }
	};
	struct Sphere
	{
		vec4 position;
		vec4 radius;
		// Polygon getAABB()
		// {
		// 	vec4d corner = position - radius;
		// 	return Box(corner, radius).toPolygon();
		// }
	};

	struct Mesh
	{
		vec4 position;
		std::vector<vec4, vec4::allocator> points;
		// Polygon getAABB()
		// {
		// 	std::vector<vec4d, simd_allocator>::size_type len = points.size();

		// 	vec4d vmin = points[0];
		// 	vec4d vmax = points[0];
		// 	for(std::vector<vec4d, simd_allocator>::size_type i = 1; i < len; ++i)
		// 	{
		// 		vmin = min_(vmin, points[i]);
		// 		vmax = max_(vmax, points[i]);
		// 	}

		// 	return Box(position + vmin, vmax - vmin).toPolygon();
		// }
	};

	template <typename T, typename U>
	struct Response
	{
		vec4 direction;
		vec4 magnitude;
		T const& a;
		U const& b;
		bool collide;
		Response(T const& _a, U const& _b) : a(_a), b(_b) { }
	};

	std::array<vec4,2> project2D(std::vector<vec4, vec4::allocator> const& points, vec4 normal)
	{
		vec4 dotproduct = dot(points[0], normal);
		vec4 minval = dotproduct;
		vec4 maxval = dotproduct;
		for(std::vector<vec4, vec4::allocator>::size_type i = 1; i < points.size(); ++i)
		{
			dotproduct = dot(points[i], normal);
			minval = min_(minval, dotproduct);
			maxval = max_(maxval, dotproduct);
		}
		return {minval, maxval};
	}
	std::array<vec4,2> project3D(std::vector<vec4, vec4::allocator> const& points, vec4 normal)
	{
		std::vector<vec4, vec4::allocator> v;
	}

	Response<vec4, Sphere> testIn(vec4 const& point, Sphere const& sphere)
	{
		vec4 sq_distance = lengthsquare(point-sphere.position);
		vec4 sq_radius = square(sphere.radius);
		Response<vec4, Sphere> res(point, sphere);
		res.collide = nonzero(le(sq_distance, sq_radius));
		return res;
	}

	Response<Sphere, Sphere>
}