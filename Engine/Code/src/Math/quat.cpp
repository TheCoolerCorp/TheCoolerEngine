#include "Math/quat.h"
#include "Math/TheCoolerMath.h"

namespace TheCoolerMath
{
    quat::quat(const vec3 a_eulerAngles)
    {
        const float t_cosX = cosf(a_eulerAngles.x * 0.5f);
        const float t_sinX = sinf(a_eulerAngles.x * 0.5f);
        const float t_cosY = cosf(a_eulerAngles.y * 0.5f);
        const float t_sinY = sinf(a_eulerAngles.y * 0.5f);
        const float t_cosZ = cosf(a_eulerAngles.z * 0.5f);
        const float t_sinZ = sinf(a_eulerAngles.z * 0.5f);

        x = t_sinX * t_cosY * t_cosZ - t_cosX * t_sinY * t_sinZ;
        y = t_cosX * t_sinY * t_cosZ + t_sinX * t_cosY * t_sinX;
        z = t_cosX * t_cosY * t_sinZ - t_sinX * t_sinY * t_cosZ;
        w = t_cosX * t_cosY * t_cosZ + t_sinX * t_sinY * t_sinZ;
    }

    float quat::operator[](const int a_index) const
    {
        if (a_index < 0 || a_index > 3)
        {
            std::cout << "Error : overflow" << '\n';
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
        if (a_index == 3)
        {
            return w;
        }
        return 0.f;
    }

    vec3 quat::ToEulerAngles(const quat a_q)
    {
        vec3 t_eulerAngles;
        const quat t_q = Normalize(a_q);

        const float t_sinXCosY = 2.f * (t_q.w * t_q.x + t_q.y * t_q.z);
        const float t_cosXCosY = 1.f - 2.f * (t_q.x * t_q.x + t_q.y * t_q.y);
        t_eulerAngles.x = atan2f(t_sinXCosY, t_cosXCosY);

        const float t_sinY = sqrtf(1.f + 2.f * (t_q.w * t_q.y - t_q.x * t_q.z));
        const float t_cosY = sqrtf(1.f - 2.f * (t_q.w * t_q.y - t_q.x * t_q.z));
        t_eulerAngles.y = 2.f * atan2f(t_sinY, t_cosY) - (PI / 2.f);

        const float t_sinZCosY = 2.f * (t_q.w * t_q.z + t_q.x * t_q.y);
        const float t_cosZCosY = 1.f - 2.f * (t_q.y * t_q.y + t_q.z * t_q.z);
        t_eulerAngles.z = atan2f(t_sinZCosY, t_cosZCosY);

        return t_eulerAngles;
    }
} // namespace TheCoolerMath
