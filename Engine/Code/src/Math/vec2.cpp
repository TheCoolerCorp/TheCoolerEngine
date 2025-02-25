#include "Math/vec2.h"

namespace TheCoolerMath
{
	float vec2::operator[](const int a_index) const
	{
		if (a_index < 0 || a_index > 1)
		{
			std::cout << "Error : overflow (vec2)\n";
			return 0.f;
		}

		if (a_index == 0)
		{
			return x;
		}
		if (a_index == 1)
		{
			return y;
		}

		return 0.f;
	}

	float vec2::GetAngle(const vec2 a_v1, const vec2 a_v2)
	{
		float dotProduct = DotProduct(a_v1, a_v2);
		float v1Norm = Norm(a_v1);
		float v2Norm = Norm(a_v2);

		if ((v1Norm * v2Norm) <= 0.0f)
		{
			return 0.0f;
		}

		float angle = acosf(dotProduct / (v1Norm * v2Norm));

		return angle;
	}
}