#include "Math/vec3.h"

namespace TheCoolerMath
{
	float vec3::GetAngle(const vec3 a_v1, const vec3 a_v2)
	{
		const float t_dotProduct = DotProduct(a_v1, a_v2);
		const float t_v1Norm = Norm(a_v1);
		const float t_v2Norm = Norm(a_v2);

		if (t_v1Norm * t_v2Norm <= 0.0f)
		{
			return 0.f;
		}

		const float angle = acosf(t_dotProduct / (t_v1Norm * t_v2Norm));

		return angle;
	}
}