#include "Math/vec3.h"
#include "Math/mat4.h"
#include "Math/TheCoolerMath.h"

namespace Engine
{
    namespace Math
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

        vec3 vec3::Lerp(vec3 a_v1, vec3 a_v2, const float a_alpha)
        {
            vec3 t_result;
            t_result.x = Math::Lerp(a_v1.x, a_v2.x, a_alpha);
            t_result.y = Math::Lerp(a_v1.y, a_v2.y, a_alpha);
            t_result.z = Math::Lerp(a_v1.z, a_v2.z, a_alpha);
            return t_result;
        }

        vec3 vec3::MidPoint(const vec3 a_v1, const vec3 a_v2)
        {
            vec3 t_v3;

            t_v3.x = (a_v1.x + a_v2.x) / 2;
            t_v3.y = (a_v1.y + a_v2.y) / 2;
            t_v3.z = (a_v1.z + a_v2.z) / 2;

            return t_v3;
        }

        float vec3::Distance(const vec3 a_p1, const vec3 a_p2)
        {
            const float t_xSquared = powf(a_p1.x - a_p2.x, 2.f);
            const float t_ySquared = powf(a_p1.y - a_p2.y, 2.f);
            const float t_zSquared = powf(a_p1.z - a_p2.z, 2.f);
            const float t_dist = sqrtf(t_xSquared + t_ySquared + t_zSquared);

            return t_dist;
        }

        float vec3::SquaredNorm(const vec3 a_v)
        {
            const float t_xSquared = powf(a_v.x, 2.f);
            const float t_ySquared = powf(a_v.y, 2.f);
            const float t_zSquared = powf(a_v.z, 2.f);
            const float t_squaredNorm = t_xSquared + t_ySquared + t_zSquared;

            return t_squaredNorm;
        }

        float vec3::Norm(const vec3 a_v)
        {
            return sqrtf(SquaredNorm(a_v));
        }

        vec3 vec3::Normalize(const vec3 a_v)
        {
            const float t_norm = Norm(a_v);
            return { a_v.x / t_norm, a_v.y / t_norm, a_v.z / t_norm };
        }

        float vec3::DotProduct(const vec3 a_v1, const vec3 a_v2)
        {
            return (a_v1.x * a_v2.x) + (a_v1.y * a_v2.y) + (a_v1.z * a_v2.z);
        }

        vec3 vec3::CrossProduct(const vec3 a_v1, const vec3 a_v2)
        {
            vec3 t_v3;

            t_v3.x = (a_v1.y * a_v2.z) - (a_v1.z * a_v2.y);
            t_v3.y = (a_v1.z * a_v2.x) - (a_v1.x * a_v2.z);
            t_v3.z = (a_v1.x * a_v2.y) - (a_v1.y * a_v2.x);

            return t_v3;
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

        vec3 vec3::GetPosition(const mat4& a_m)
        {
            return { a_m.mElements[3], a_m.mElements[7], a_m.mElements[11] };
        }

        vec3 vec3::GetScale(const mat4& a_m)
        {
            vec3 t_scale;
            t_scale.x = Norm({ a_m.mElements[0], a_m.mElements[4], a_m.mElements[8] });
            t_scale.y = Norm({ a_m.mElements[1], a_m.mElements[5], a_m.mElements[9] });
            t_scale.z = Norm({ a_m.mElements[2], a_m.mElements[6], a_m.mElements[10] });
            return t_scale;
        }
    }
}
