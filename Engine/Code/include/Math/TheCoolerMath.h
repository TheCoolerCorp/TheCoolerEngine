#pragma once
#include <numbers>

#include "vec2.h"
#include "vec3.h"
#include "mat4.h"
#include "quat.h"

namespace TheCoolerMath
{
	constexpr float PI = std::numbers::pi_v<float>;

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
