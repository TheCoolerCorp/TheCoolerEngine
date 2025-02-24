#include "Math/vec2.h"

float vec2::operator[](const int index) const
{
	if (index < 0 || index > 1)
	{
		std::cout << "Error : overflow (vec2)\n";
		return 0.f;
	}

	if (index == 0)
	{
		return x;
	}
	if (index == 1)
	{
		return y;
	}

	return 0.f;
}

float vec2::GetAngle(const vec2 v1, const vec2 v2)
{
	float dotProduct = DotProduct(v1, v2);
	float v1Norm = Norm(v1);
	float v2Norm = Norm(v2);

	if ((v1Norm * v2Norm) <= 0.0f)
	{
		return 0.0f;
	}

	float angle = acosf(dotProduct / (v1Norm * v2Norm));

	return angle;
}