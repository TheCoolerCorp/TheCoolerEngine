#pragma once
#include <iostream>

namespace TheCoolerMath
{
	// Class for Quaternion
	class quat
	{
	public:
		// vector components
		float x, y, z, w;

		// different ways of initializing a vector
		quat() : x(0.f), y(0.f), z(0.f), w(0.f) {}
		quat(const float a_x, const float a_y, const float a_z, const float a_w) : x(a_x), y(a_y), z(a_z), w(a_w) {}
		explicit quat(const float a_coords) : x(a_coords), y(a_coords), z(a_coords), w(a_coords) {}
		~quat() = default;

		// different print methods
		void Print() const;
		void BlankPrint() const;
	};

	inline void quat::Print() const
	{
		std::cout << "quat : " << "x = " << x << ", y = " << y << ", z = " << z << ", w = " << w << '\n';
	}

	inline void quat::BlankPrint() const
	{
		std::cout << x << "  " << y << "  " << z << "  " << w << '\n';
	}
}