#include "ArrayMath.hpp"
#include <limits>
#include <cstddef>

#include <vector>

namespace sat
{
	template <typename T, typename U>
	struct Response
	{
		vector4 direction;
		T const& a;
		U const& b;
		bool collide;
		Response(T const& _a, U const& _b) : a(_a), b(_b), collide(false) { }
	};
	struct Point
	{
		vector4 position;
	};
	struct Circle
	{
		vector4 position;
		vector4 radius;
	};
	struct Polygon
	{
		vector4 position;
		std::vector<vector4> vertices;
	};
	struct Quad
	{
		vector4 position;
		vector4 bottom_left, top_right;
	};
	struct Box
	{
		vector4 position;
		vector4 bottom_left_front, top_right_back;
		// Polygon toPolygon()
		// {
		// 	Polygon res{position, {vec4d(), }};
		// 	return Polygon(njsagiefnsgraejn);
		// }
	};
	// struct Circle
	// {
		// vec4 position;
		// vec4 radius;
		// Polygon getAABB()
		// {
		// 	vec4d corner = position - radius;
		// 	return Box(corner, radius).toPolygon();
		// }
	// };
	// struct Sphere
	// {
		// vec4 position;
		// vec4 radius;
		// Polygon getAABB()
		// {
		// 	vec4d corner = position - radius;
		// 	return Box(corner, radius).toPolygon();
		// }
	// };

	// struct Mesh
	// {
	// 	vec4 position;
	// 	std::vector<vec4, vec4::allocator> points;
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
	// };

	// std::array<vec4,2> project2D(std::vector<vec4, vec4::allocator> const& points, vec4 normal)
	// {
	// 	vec4 dotproduct = dot(points[0], normal);
	// 	vec4 minval = dotproduct;
	// 	vec4 maxval = dotproduct;
	// 	for(std::vector<vec4, vec4::allocator>::size_type i = 1; i < points.size(); ++i)
	// 	{
	// 		dotproduct = dot(points[i], normal);
	// 		minval = min_(minval, dotproduct);
	// 		maxval = max_(maxval, dotproduct);
	// 	}
	// 	return {minval, maxval};
	// }
	// std::array<vec4,2> project3D(std::vector<vec4, vec4::allocator> const& points, vec4 normal)
	// {
	// 	std::vector<vec4, vec4::allocator> v;
	// }




	Response<Point, Circle> testIn(const Point& point, const Circle& circle)
	{
		Response<Point, Circle> response(point, circle);
		vector4 diff = point.position - circle.position;
		vector4 distance = diff.length();
		if (le(distance, circle.radius).mask() & 0b0011)
		{
			response.collide = true;
			response.direction = (circle.radius - distance) * diff.normalize();
		}
		return response;
	}
	Response<Circle, Circle> testIn(const Circle& c1, const Circle& c2)
	{
		Response<Circle, Circle> response(c1, c2);
		vector4 diff = c1.position - c2.position;
		vector4 distance = diff.length();
		if (le(distance, c1.radius + c2.radius).mask() & 0b0001)
		{
			response.collide = true;
			response.direction = ((c1.radius + c2.radius) - distance) * diff.normalize();
		}
		return response;
	}
	Response<Polygon, Polygon> testIn(const Polygon& p1, const Polygon& p2)
	{
		Response<Polygon, Polygon> response(p1, p2);
		assert(p1.vertices.size() > 2 && p2.vertices.size() > 2);
		vector4 diff = p1.position - p2.position;
		vector4 distance = diff.length();
		vector4 min_dist(std::numeric_limits<double>::max());
		vector4 move_axis;

		auto p1_size = p1.vertices.size();
		auto p2_size = p2.vertices.size();
		for(decltype(p1.vertices)::size_type i = 0; i < p1_size; ++i)
		{
			vector4 axis = p1.vertices[(i+1)%p1_size] - p1.vertices[i]; // x = -(y2-y1), y = (x2-x1)
			axis = axis.shufflePermute(-axis).normalize();
			vector4 min1 = axis.dot(p1.vertices[0]);
			vector4 max1 = min1;
			for(decltype(p1.vertices)::size_type j = 1; j < p1_size; ++j)
			{
				vector4 dt = axis.dot(p1.vertices[j]); // change to dot4
				min1 = min_(min1, dt);
				max1 = max_(max1, dt);
			}

			vector4 min2 = axis.dot(p2.vertices[0]);
			vector4 max2 = min2;
			for (decltype(p2.vertices)::size_type j = 1; j < p2_size; ++j)
			{
				vector4 dt = axis.dot(p2.vertices[j]); // change to dot4
				min2 = min_(min2, dt);
				max2 = max_(max2, dt);
			}

			//vector4 offset = axis.dot(diff);
			//min1 += offset;
			//max1 += offset;

			vector4 test = lt(min1, min2);
			vector4 not_test = ge(min1, min2);
			vector4 dist = ((min2 - max1)&test) + ((min1 - max2)&not_test);
			//vector4 test1 = min1 - max2;
			//vector4 test2 = min2 - max1;
			if (gt(dist, vector4()).mask() & 0b0001)
			{
				return response;
			}

			dist = dist.abs();
			test = lt(dist, min_dist);
			not_test = ge(dist, min_dist);
			min_dist = (dist&test) + (min_dist&not_test); // min_dit = dist < min_dist ? dist : min_dist;
			move_axis = (axis&test) + (move_axis&not_test);
		}

		for (decltype(p2.vertices)::size_type i = 0; i < p2_size; ++i)
		{
			vector4 axis = p2.vertices[(i + 1) % p2_size] - p2.vertices[i]; // x = -(y2-y1), y = (x2-x1)
			axis = axis.shufflePermute(-axis).normalize();
			vector4 min1 = axis.dot(p2.vertices[0]);
			vector4 max1 = min1;
			for (decltype(p2.vertices)::size_type j = 1; j < p2_size; ++j)
			{
				vector4 dt = axis.dot(p2.vertices[j]); // change to dot4
				min1 = min_(min1, dt);
				max1 = max_(max1, dt);
			}

			vector4 min2 = axis.dot(p1.vertices[0]);
			vector4 max2 = min2;
			for (decltype(p1.vertices)::size_type j = 1; j < p1_size; ++j)
			{
				vector4 dt = axis.dot(p1.vertices[j]); // change to dot4
				min2 = min_(min2, dt);
				max2 = max_(max2, dt);
			}

			//vector4 offset = axis.dot(diff);
			//min1 += offset;
			//max1 += offset;

			vector4 test = lt(min1, min2);
			vector4 not_test = ge(min1, min2);
			vector4 dist = ((min2 - max1)&test) + ((min1 - max2)&not_test);
			//vector4 test1 = min1 - max2;
			//vector4 test2 = min2 - max1;
			if (gt(dist, vector4()).mask() & 0b0001)
			{
				return response;
			}

			dist = dist.abs();
			test = lt(dist, min_dist);
			not_test = ge(dist, min_dist);
			min_dist = (dist&test) + (min_dist&not_test); // min_dit = dist < min_dist ? dist : min_dist;
			move_axis = (axis&test) + (move_axis&not_test);
		}
		// response.collide = true;
		// response.direction = 

		// if (le(distance, c1.radius + c2.radius).mask() & 0b001)
		// {
		// 	response.collide = true;
		// 	response.direction = ((c1.radius + c2.radius) - distance) * diff.normalize();
		// }

		vector4 d_axis = diff.dot(move_axis);
		vector4 test = lt(d_axis, vector4());
		vector4 not_test = ge(d_axis, vector4());

		response.collide = true;
		vector4 min_axis = move_axis * min_dist;
		response.direction = (min_axis&not_test) + (-min_axis&test);
		return response;
	}
	// Response<vec4, Sphere> testIn(vec4 const& point, Sphere const& sphere)
	// {
	// 	vec4 sq_distance = lengthsquare(point-sphere.position);
	// 	vec4 sq_radius = square(sphere.radius);
	// 	Response<vec4, Sphere> res(point, sphere);
	// 	res.collide = nonzero(le(sq_distance, sq_radius));
	// 	return res;
	// }

	//Response<Sphere, Sphere>
}