#pragma once

#include <type_traits>
#include <array>

template <class T>
class Copter
{
private:
	Copter() {}; // prevent undesirable inheritance making ctor private
	friend T; // allow inheritance for Derived
public:
	struct StatusStruct
	{
		std::array<double, 3> position;
		std::array<double, 3> angle;
		std::array<double, 3> velocity;
		std::array<double, 3> reaction;
	};
	StatusStruct Status() const
	{
		return static_cast<const T*>(this)->status();
	}
	template<size_t N>
	void Apply(std::array<double, N> thrust)
	{
		static_cast<T*>(this)->apply(thrust);
	}
};

template <class T>
struct is_copter : std::is_base_of<typename Copter<T>, typename T> {};