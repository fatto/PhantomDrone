#include "ArrayMath.hpp"

#include <vector>

namespace sat
{
	struct Box
	{
		vec4d position;
		vec4d size;
		// Polygon toPolygon()
		// {
		// 	Polygon res{position, {vec4d(), }};
		// 	return Polygon(njsagiefnsgraejn);
		// }
	};
	struct Circle
	{
		vec4d position;
		vec4d radius;
		// Polygon getAABB()
		// {
		// 	vec4d corner = position - radius;
		// 	return Box(corner, radius).toPolygon();
		// }
	};
	struct Sphere
	{
		vec4d position;
		vec4d radius;
		// Polygon getAABB()
		// {
		// 	vec4d corner = position - radius;
		// 	return Box(corner, radius).toPolygon();
		// }
	};

	struct Mesh
	{
		vec4d position;
		std::vector<vec4d, simd_allocator> points;
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
		vec4d direction;
		vec4d magnitude;
		T const& a;
		U const& b;
		bool collide;
		Response(T const& _a, U const& _b) : a(_a), b(_b) { }
	};

	std::array<vec4d,2> project2D(std::vector<vec4d, simd_allocator> const& points, vec4d normal)
	{
		vec4d dotproduct = dot(points[0], normal);
		vec4d minval = dotproduct;
		vec4d maxval = dotproduct;
		for(std::vector<vec4d, simd_allocator>::size_type i = 1; i < points.size(); ++i)
		{
			dotproduct = dot(points[i], normal);
			minval = min_(minval, dotproduct);
			maxval = max_(maxval, dotproduct);
		}
		return {minval, maxval};
	}
	std::array<vec4d,2> project3D(std::vector<vec4d, simd_allocator> const& points, vec4d normal)
	{
		std::vector<vec4d, simd_allocator> v;
	}

	Response<vec4d, Sphere> testIn(vec4d const& point, Sphere const& sphere)
	{
		vec4d sq_distance = lengthsquare(point-sphere.position);
		vec4d sq_radius = square(sphere.radius);
		Response<vec4d, Sphere> res(point);
		res.collide = nonzero(sq_distance <= sq_radius);
		return res;
	}

	Response<Sphere, Sphere>
}