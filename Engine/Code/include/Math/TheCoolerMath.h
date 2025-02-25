#pragma once
#include "vec2.h"
#include "mat4.h"

namespace TheCoolerMath
{
	constexpr float PI = 3.14159265358979323846f;

	inline float Lerp(const float &f1, const float &f2, const float &t)
	{
		return f2 * t + f1 * (1 - t);
	}

	inline float ToRadians(const float& deg)
	{
		return deg * (PI / 180.f);
	}

	inline float ToDegrees(const float& rad)
	{
		return rad * (180.f / PI);
	}
}
