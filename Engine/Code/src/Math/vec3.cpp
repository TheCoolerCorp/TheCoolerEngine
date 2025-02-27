#include "Math/vec3.h"

namespace TheCoolerMath
{
    float vec3::operator[](const int a_index) const
    {
        if (a_index < 0 || a_index > 2)
        {
            std::cout << "Error : overflow" << std::endl;
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
        if (a_index == 2)
        {
            return z;
        }
        return 0.f;
    }

    float vec3::GetAngle(const vec3 a_v1, const vec3 a_v2)
    {
        const float t_dotProduct = DotProduct(a_v1, a_v2);
        const float t_v1Norm = Norm(a_v1);
        const float t_v2Norm = Norm(a_v2);

        if (t_v1Norm * t_v2Norm <= 0.0f)
        {
            return 0.f;
        }

        const float t_angle = acosf(t_dotProduct / (t_v1Norm * t_v2Norm));

        return t_angle;
    }
} // namespace TheCoolerMath
