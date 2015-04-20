#pragma once

#include "ArrayMath.hpp"
#include <limits>
#include <cstddef>
#include <cassert>
#include <vector>
#include <algorithm>
#include <utility>

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
		vector4 x_axis, y_axis, z_axis;
		vector4 half_size;
		std::vector<vector4> compute_verticies() const
		{
			return {
				position + (x_axis * half_size.x) + (y_axis * half_size.y) + (z_axis * half_size.z),
				position + (x_axis * half_size.x) + (y_axis * half_size.y) - (z_axis * half_size.z),
				position + (x_axis * half_size.x) - (y_axis * half_size.y) + (z_axis * half_size.z),
				position + (x_axis * half_size.x) - (y_axis * half_size.y) - (z_axis * half_size.z),
				position - (x_axis * half_size.x) + (y_axis * half_size.y) + (z_axis * half_size.z),
				position - (x_axis * half_size.x) + (y_axis * half_size.y) - (z_axis * half_size.z),
				position - (x_axis * half_size.x) - (y_axis * half_size.y) + (z_axis * half_size.z),
				position - (x_axis * half_size.x) - (y_axis * half_size.y) - (z_axis * half_size.z)
			};
		}
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




	// inline Response<Point, Circle> testIn(const Point& point, const Circle& circle)
	// {
	// 	Response<Point, Circle> response(point, circle);
	// 	vector4 diff = point.position - circle.position;
	// 	vector4 distance = diff.length();
	// 	if (le(distance, circle.radius).mask() & 0b0011)
	// 	{
	// 		response.collide = true;
	// 		response.direction = (circle.radius - distance) * diff.normalize();
	// 	}
	// 	return response;
	// }
	// inline Response<Circle, Circle> testIn(const Circle& c1, const Circle& c2)
	// {
	// 	Response<Circle, Circle> response(c1, c2);
	// 	vector4 diff = c1.position - c2.position;
	// 	vector4 distance = diff.length();
	// 	if (le(distance, c1.radius + c2.radius).mask() & 0b0001)
	// 	{
	// 		response.collide = true;
	// 		response.direction = ((c1.radius + c2.radius) - distance) * diff.normalize();
	// 	}
	// 	return response;
	// }
	// inline Response<Polygon, Polygon> testIn(const Polygon& p1, const Polygon& p2)
	// {
	// 	Response<Polygon, Polygon> response(p1, p2);
	// 	assert(p1.vertices.size() > 2 && p2.vertices.size() > 2);
	// 	vector4 diff = p1.position - p2.position;
	// 	vector4 distance = diff.length();
	// 	vector4 min_dist(std::numeric_limits<double>::max());
	// 	vector4 move_axis;

	// 	auto p1_size = p1.vertices.size();
	// 	auto p2_size = p2.vertices.size();
	// 	for(decltype(p1.vertices)::size_type i = 0; i < p1_size; ++i)
	// 	{
	// 		vector4 axis = p1.vertices[(i+1)%p1_size] - p1.vertices[i]; // x = -(y2-y1), y = (x2-x1)
	// 		axis = axis.shufflePermute(-axis).normalize();
	// 		vector4 min1 = axis.dot(p1.vertices[0]);
	// 		vector4 max1 = min1;
	// 		for(decltype(p1.vertices)::size_type j = 1; j < p1_size; ++j)
	// 		{
	// 			vector4 dt = axis.dot(p1.vertices[j]); // change to dot4
	// 			min1 = min_(min1, dt);
	// 			max1 = max_(max1, dt);
	// 		}

	// 		vector4 min2 = axis.dot(p2.vertices[0]);
	// 		vector4 max2 = min2;
	// 		for (decltype(p2.vertices)::size_type j = 1; j < p2_size; ++j)
	// 		{
	// 			vector4 dt = axis.dot(p2.vertices[j]); // change to dot4
	// 			min2 = min_(min2, dt);
	// 			max2 = max_(max2, dt);
	// 		}

	// 		//vector4 offset = axis.dot(diff);
	// 		//min1 += offset;
	// 		//max1 += offset;

	// 		vector4 test = lt(min1, min2);
	// 		vector4 not_test = ge(min1, min2);
	// 		vector4 dist = ((min2 - max1)&test) + ((min1 - max2)&not_test);
	// 		//vector4 test1 = min1 - max2;
	// 		//vector4 test2 = min2 - max1;
	// 		if (gt(dist, vector4()).mask() & 0b0001)
	// 		{
	// 			return response;
	// 		}

	// 		dist = dist.abs();
	// 		test = lt(dist, min_dist);
	// 		not_test = ge(dist, min_dist);
	// 		min_dist = (dist&test) + (min_dist&not_test); // min_dit = dist < min_dist ? dist : min_dist;
	// 		move_axis = (axis&test) + (move_axis&not_test);
	// 	}

	// 	for (decltype(p2.vertices)::size_type i = 0; i < p2_size; ++i)
	// 	{
	// 		vector4 axis = p2.vertices[(i + 1) % p2_size] - p2.vertices[i]; // x = -(y2-y1), y = (x2-x1)
	// 		axis = axis.shufflePermute(-axis).normalize();
	// 		vector4 min1 = axis.dot(p2.vertices[0]);
	// 		vector4 max1 = min1;
	// 		for (decltype(p2.vertices)::size_type j = 1; j < p2_size; ++j)
	// 		{
	// 			vector4 dt = axis.dot(p2.vertices[j]); // change to dot4
	// 			min1 = min_(min1, dt);
	// 			max1 = max_(max1, dt);
	// 		}

	// 		vector4 min2 = axis.dot(p1.vertices[0]);
	// 		vector4 max2 = min2;
	// 		for (decltype(p1.vertices)::size_type j = 1; j < p1_size; ++j)
	// 		{
	// 			vector4 dt = axis.dot(p1.vertices[j]); // change to dot4
	// 			min2 = min_(min2, dt);
	// 			max2 = max_(max2, dt);
	// 		}

	// 		//vector4 offset = axis.dot(diff);
	// 		//min1 += offset;
	// 		//max1 += offset;

	// 		vector4 test = lt(min1, min2);
	// 		vector4 not_test = ge(min1, min2);
	// 		vector4 dist = ((min2 - max1)&test) + ((min1 - max2)&not_test);
	// 		//vector4 test1 = min1 - max2;
	// 		//vector4 test2 = min2 - max1;
	// 		if (gt(dist, vector4()).mask() & 0b0001)
	// 		{
	// 			return response;
	// 		}

	// 		dist = dist.abs();
	// 		test = lt(dist, min_dist);
	// 		not_test = ge(dist, min_dist);
	// 		min_dist = (dist&test) + (min_dist&not_test); // min_dit = dist < min_dist ? dist : min_dist;
	// 		move_axis = (axis&test) + (move_axis&not_test);
	// 	}
	// 	// response.collide = true;
	// 	// response.direction = 

	// 	// if (le(distance, c1.radius + c2.radius).mask() & 0b001)
	// 	// {
	// 	// 	response.collide = true;
	// 	// 	response.direction = ((c1.radius + c2.radius) - distance) * diff.normalize();
	// 	// }

	// 	vector4 d_axis = diff.dot(move_axis);
	// 	vector4 test = lt(d_axis, vector4());
	// 	vector4 not_test = ge(d_axis, vector4());

	// 	response.collide = true;
	// 	vector4 min_axis = move_axis * min_dist;
	// 	response.direction = (min_axis&not_test) + (-min_axis&test);
	// 	return response;
	// }
	inline std::pair<double, double> min_max_projection(const vector4& axe, const std::vector<vector4>& vertices)
	{
		double min_proj = std::numeric_limits<double>::max();
		double max_proj = std::numeric_limits<double>::min();

		for(auto& v : vertices)
		{
			double proj = axe.dot(v);
			min_proj = proj < min_proj ? proj : min_proj;
			max_proj = proj > max_proj ? proj : max_proj;
		}

		return{ min_proj, max_proj };
	}
	inline Response<Box, Box> testIn(const Box& b1, const Box& b2)
	{
		Response<Box, Box> response(b1, b2);

		vector4 T = b2.position - b1.position;
		double Rxx = b1.x_axis.dot(b2.x_axis);
		double Rxy = b1.x_axis.dot(b2.y_axis);
		double Rxz = b1.x_axis.dot(b2.z_axis);

		double Ryx = b1.y_axis.dot(b2.x_axis);
		double Ryy = b1.y_axis.dot(b2.y_axis);
		double Ryz = b1.y_axis.dot(b2.z_axis);

		double Rzx = b1.z_axis.dot(b2.x_axis);
		double Rzy = b1.z_axis.dot(b2.y_axis);
		double Rzz = b1.z_axis.dot(b2.z_axis);
		std::array<std::tuple<vector4, double>, 15> proj {
			std::tuple<vector4, double>{ b1.x_axis, /*std::abs(*/std::abs(T.dot(b1.x_axis)) - (b1.half_size.x + std::abs(b2.half_size.x*Rxx) + std::abs(b2.half_size.y*Rxy) + std::abs(b2.half_size.z*Rxz))/*)*/ },
			std::tuple<vector4, double>{ b1.y_axis, /*std::abs(*/std::abs(T.dot(b1.y_axis)) - (b1.half_size.y + std::abs(b2.half_size.x*Ryx) + std::abs(b2.half_size.y*Ryy) + std::abs(b2.half_size.z*Ryz))/*)*/ },
			std::tuple<vector4, double>{ b1.z_axis, /*std::abs(*/std::abs(T.dot(b1.z_axis)) - (b1.half_size.z + std::abs(b2.half_size.x*Rzx) + std::abs(b2.half_size.y*Rzy) + std::abs(b2.half_size.z*Rzz))/*)*/ },
			
			std::tuple<vector4, double>{ b2.x_axis, /*std::abs(*/std::abs(T.dot(b2.x_axis)) - (b2.half_size.x + std::abs(b1.half_size.x*Rxx) + std::abs(b1.half_size.y*Ryx) + std::abs(b1.half_size.z*Rzx))/*)*/ },
			std::tuple<vector4, double>{ b2.y_axis, /*std::abs(*/std::abs(T.dot(b2.y_axis)) - (b2.half_size.y + std::abs(b1.half_size.x*Rxy) + std::abs(b1.half_size.y*Ryy) + std::abs(b1.half_size.z*Rzy))/*)*/ },
			std::tuple<vector4, double>{ b2.z_axis, /*std::abs(*/std::abs(T.dot(b2.z_axis)) - (b2.half_size.z + std::abs(b1.half_size.x*Rxz) + std::abs(b1.half_size.y*Ryz) + std::abs(b1.half_size.z*Rzz))/*)*/ },
			
			std::tuple<vector4, double>{ b1.x_axis.cross(b2.x_axis)/*.normalize()*/, /*std::abs(*/std::abs(T.dot(b1.z_axis)*Ryx - T.dot(b1.y_axis)*Rzx) - (std::abs(b1.half_size.y*Rzx) + std::abs(b1.half_size.z*Ryx) + std::abs(b2.half_size.y*Rxz) + std::abs(b2.half_size.z*Rxy))/*)*/ },
			std::tuple<vector4, double>{ b1.x_axis.cross(b2.y_axis)/*.normalize()*/, /*std::abs(*/std::abs(T.dot(b1.z_axis)*Ryy - T.dot(b1.y_axis)*Rzy) - (std::abs(b1.half_size.y*Rzy) + std::abs(b1.half_size.z*Ryy) + std::abs(b2.half_size.x*Rxz) + std::abs(b2.half_size.z*Rxx))/*)*/ },
			std::tuple<vector4, double>{ b1.x_axis.cross(b2.z_axis)/*.normalize()*/, /*std::abs(*/std::abs(T.dot(b1.z_axis)*Ryz - T.dot(b1.y_axis)*Rzz) - (std::abs(b1.half_size.y*Rzz) + std::abs(b1.half_size.z*Ryz) + std::abs(b2.half_size.x*Rxy) + std::abs(b2.half_size.y*Rxx))/*)*/ },
			
			std::tuple<vector4, double>{ b1.y_axis.cross(b2.x_axis)/*.normalize()*/, /*std::abs(*/std::abs(T.dot(b1.x_axis)*Rzx - T.dot(b1.z_axis)*Rxx) - (std::abs(b1.half_size.x*Rzx) + std::abs(b1.half_size.z*Rxx) + std::abs(b2.half_size.y*Ryz) + std::abs(b2.half_size.z*Ryy))/*)*/ },
			std::tuple<vector4, double>{ b1.y_axis.cross(b2.y_axis)/*.normalize()*/, /*std::abs(*/std::abs(T.dot(b1.x_axis)*Rzy - T.dot(b1.z_axis)*Rxy) - (std::abs(b1.half_size.x*Rzy) + std::abs(b1.half_size.z*Rxy) + std::abs(b2.half_size.x*Ryz) + std::abs(b2.half_size.z*Ryx))/*)*/ },
			std::tuple<vector4, double>{ b1.y_axis.cross(b2.z_axis)/*.normalize()*/, /*std::abs(*/std::abs(T.dot(b1.x_axis)*Rzz - T.dot(b1.z_axis)*Rxz) - (std::abs(b1.half_size.x*Rzz) + std::abs(b1.half_size.z*Rxz) + std::abs(b2.half_size.x*Ryy) + std::abs(b2.half_size.y*Ryx))/*)*/ },

			std::tuple<vector4, double>{ b1.z_axis.cross(b2.x_axis)/*.normalize()*/, /*std::abs(*/std::abs(T.dot(b1.y_axis)*Rxx - T.dot(b1.x_axis)*Ryx) - (std::abs(b1.half_size.x*Ryx) + std::abs(b1.half_size.y*Rxx) + std::abs(b2.half_size.y*Rzz) + std::abs(b2.half_size.z*Rzy))/*)*/ },
			std::tuple<vector4, double>{ b1.z_axis.cross(b2.y_axis)/*.normalize()*/, /*std::abs(*/std::abs(T.dot(b1.y_axis)*Rxy - T.dot(b1.x_axis)*Ryy) - (std::abs(b1.half_size.x*Ryy) + std::abs(b1.half_size.y*Rxy) + std::abs(b2.half_size.x*Rzz) + std::abs(b2.half_size.z*Rzx))/*)*/ },
			std::tuple<vector4, double>{ b1.z_axis.cross(b2.z_axis)/*.normalize()*/, /*std::abs(*/std::abs(T.dot(b1.y_axis)*Rxz - T.dot(b1.x_axis)*Ryz) - (std::abs(b1.half_size.x*Ryz) + std::abs(b1.half_size.y*Rxz) + std::abs(b2.half_size.x*Rzy) + std::abs(b2.half_size.y*Rzx))/*)*/ }
		};

		auto vs1 = b1.compute_verticies();
		auto vs2 = b2.compute_verticies();

		vector4 ax;
		double min_dist = std::numeric_limits<double>::max();
		for(auto& t : proj)
		{
			if(std::get<0>(t).length() < 0.01)
				continue;
			auto b1_min_max = min_max_projection(std::get<0>(t).normalize(), vs1);
			auto b2_min_max = min_max_projection(std::get<0>(t).normalize(), vs2);
			double dist = std::get<0>(b1_min_max) < std::get<0>(b2_min_max) ? std::get<0>(b2_min_max) - std::get<1>(b1_min_max) : std::get<0>(b1_min_max) - std::get<1>(b2_min_max);
			if(dist > 0.0)
				return response;
			dist = std::abs(dist);

			bool change = dist < min_dist;
			min_dist = change ? dist : min_dist;
			ax = change ? std::get<0>(t).normalize() : ax;
		}

		response.collide = true;
		response.direction = ax * min_dist;
		return response;

		// vector4 test = lt(min1, min2);
		// 	vector4 not_test = ge(min1, min2);
		// 	vector4 dist = ((min2 - max1)&test) + ((min1 - max2)&not_test);
		// 	//vector4 test1 = min1 - max2;
		// 	//vector4 test2 = min2 - max1;
		// 	if (gt(dist, vector4()).mask() & 0b0001)
		// 	{
		// 		return response;
		// 	}

		// 	dist = dist.abs();
		// 	test = lt(dist, min_dist);
		// 	not_test = ge(dist, min_dist);
		// 	min_dist = (dist&test) + (min_dist&not_test); // min_dit = dist < min_dist ? dist : min_dist;
		// 	move_axis = (axis&test) + (move_axis&not_test);
		// }

		auto res = std::find_if(std::begin(proj), std::end(proj), [](const std::tuple<vector4, double>& p) { return std::get<1>(p) > 0.0; } );
		if (res!= std::end(proj)) // there's a separation axis
		{
			return response;
		}

		auto collision = std::min_element(std::begin(proj), std::end(proj), [](const std::tuple<vector4, double>& p1, const std::tuple<vector4, double>& p2) { return std::abs(std::get<1>(p1)) < std::abs(std::get<1>(p2)) && std::abs(std::get<1>(p1)) > 0.0; } );

		response.collide = true;
		response.direction = std::get<0>(*collision) * std::abs(std::get<1>(*collision));

		return response;

		// // L = b1.x_axis x b2.z_axis
		// if(gt((T.dot(b1.z_axis)*Ryz - T.dot(b1.y_axis)*Rzz).abs(), (b1.half_size.y*Rzz).abs() + (b1.half_size.z*Ryz).abs() + (b2.half_size.x*Rxy).abs() + (b2.half_size.y*Rxx).abs())
		// };

		// // L = b1.x_axis
		// if(gt(T.dot(b1.x_axis).abs(), vector4(b1.half_size.x) + (b2.half_size.x*Rxx).abs() + (b2.half_size.y*Rxy).abs() + (b2.half_size.z*Rxz).abs()).mask())
		// {
		// 	return response;
		// }
		// // L = b1.y_axis
		// if(gt(T.dot(b1.y_axis).abs(), vector4(b1.half_size.y) + (b2.half_size.x*Ryx).abs() + (b2.half_size.y*Ryy).abs() + (b2.half_size.z*Ryz).abs()).mask())
		// {
		// 	return response;
		// }
		// // L = b1.z_axis
		// if(gt(T.dot(b1.z_axis).abs(), vector4(b1.half_size.z) + (b2.half_size.x*Rzx).abs() + (b2.half_size.y*Rzy).abs() + (b2.half_size.z*Rzz).abs()).mask())
		// {
		// 	return response;
		// }
		// // L = b2.x_axis
		// if(gt(T.dot(b2.x_axis).abs(), vector4(b2.half_size.x) + (b1.half_size.x*Rxx).abs() + (b1.half_size.y*Ryx).abs() + (b1.half_size.z*Rzx).abs()).mask())
		// {
		// 	return response;
		// }
		// // L = b2.y_axis
		// if(gt(T.dot(b2.y_axis).abs(), vector4(b2.half_size.y) + (b1.half_size.x*Rxy).abs() + (b1.half_size.y*Ryy).abs() + (b1.half_size.z*Rzy).abs()).mask())
		// {
		// 	return response;
		// }
		// // L = b2.z_axis
		// if(gt(T.dot(b2.z_axis).abs(), vector4(b2.half_size.z) + (b1.half_size.x*Rxz).abs() + (b1.half_size.y*Ryz).abs() + (b1.half_size.z*Rzz).abs()).mask())
		// {
		// 	return response;
		// }

		// // L = b1.x_axis x b2.x_axis
		// if(gt((T.dot(b1.z_axis)*Ryx - T.dot(b1.y_axis)*Rzx).abs(), (b1.half_size.y*Rzx).abs() + (b1.half_size.z*Ryx).abs() + (b2.half_size.y*Rxz).abs() + (b2.half_size.z*Rxy).abs()).mask())
		// {
		// 	return response;
		// }

		// // L = b1.x_axis x b2.y_axis
		// if(gt((T.dot(b1.z_axis)*Ryy - T.dot(b1.y_axis)*Rzy).abs(), (b1.half_size.y*Rzy).abs() + (b1.half_size.z*Ryy).abs() + (b2.half_size.x*Rxz).abs() + (b2.half_size.z*Rxx).abs()).mask())
		// {
		// 	return response;
		// }

		// // L = b1.x_axis x b2.z_axis
		// if(gt((T.dot(b1.z_axis)*Ryz - T.dot(b1.y_axis)*Rzz).abs(), (b1.half_size.y*Rzz).abs() + (b1.half_size.z*Ryz).abs() + (b2.half_size.x*Rxy).abs() + (b2.half_size.y*Rxx).abs()).mask())
		// {
		// 	return response;
		// }

		// // L = b1.y_axis x b2.x_axis
		// if(gt((T.dot(b1.x_axis)*Rzx - T.dot(b1.z_axis)*Rxx).abs(), (b1.half_size.x*Rzx).abs() + (b1.half_size.z*Rxx).abs() + (b2.half_size.y*Ryz).abs() + (b2.half_size.z*Ryy).abs()).mask())
		// {
		// 	return response;
		// }

		// // L = b1.y_axis x b2.y_axis
		// if(gt((T.dot(b1.x_axis)*Rzy - T.dot(b1.z_axis)*Rxy).abs(), (b1.half_size.x*Rzy).abs() + (b1.half_size.z*Rxy).abs() + (b2.half_size.x*Ryz).abs() + (b2.half_size.z*Ryx).abs()).mask())
		// {
		// 	return response;
		// }

		// // L = b1.y_axis x b2.z_axis
		// if(gt((T.dot(b1.x_axis)*Rzz - T.dot(b1.z_axis)*Rxz).abs(), (b1.half_size.x*Rzz).abs() + (b1.half_size.z*Rxz).abs() + (b2.half_size.x*Ryy).abs() + (b2.half_size.y*Ryx).abs()).mask())
		// {
		// 	return response;
		// }

		// // L = b1.z_axis x b2.x_axis
		// if(gt((T.dot(b1.y_axis)*Rxx - T.dot(b1.x_axis)*Ryx).abs(), (b1.half_size.x*Ryx).abs() + (b1.half_size.y*Rxx).abs() + (b2.half_size.y*Rzz).abs() + (b2.half_size.z*Rzy).abs()).mask())
		// {
		// 	return response;
		// }

		// // L = b1.z_axis x b2.y_axis
		// if(gt((T.dot(b1.y_axis)*Rxy - T.dot(b1.x_axis)*Ryy).abs(), (b1.half_size.x*Ryy).abs() + (b1.half_size.y*Rxy).abs() + (b2.half_size.x*Rzz).abs() + (b2.half_size.z*Rzx).abs()).mask())
		// {
		// 	return response;
		// }

		// // L = b1.z_axis x b2.z_axis
		// if(gt((T.dot(b1.y_axis)*Rxz - T.dot(b1.x_axis)*Ryz).abs(), (b1.half_size.x*Ryz).abs() + (b1.half_size.y*Rxz).abs() + (b2.half_size.x*Rzy).abs() + (b2.half_size.y*Rzx).abs()).mask())
		// {
		// 	return response;
		// }

		// response.collide = true;
		// return response;
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
