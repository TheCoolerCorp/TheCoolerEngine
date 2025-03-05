#include "Math/vec2.h"

namespace Math
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

    vec2 vec2::MidPoint(const vec2 a_v1, const vec2 a_v2)
    {
        vec2 t_v3;

        t_v3.x = (a_v1.x + a_v2.x) / 2;
        t_v3.y = (a_v1.y + a_v2.y) / 2;

        return t_v3;
    }

    float vec2::Distance(const vec2 a_p1, const vec2 a_p2)
    {
        const float t_xSquared = powf(a_p1.x - a_p2.x, 2.f);
        const float t_ySquared = powf(a_p1.y - a_p2.y, 2.f);
        const float t_dist = sqrtf(t_xSquared + t_ySquared);

        return t_dist;
    }

    float vec2::SquaredNorm(const vec2 a_v)
    {
        const float t_xSquared = powf(a_v.x, 2.f);
        const float t_ySquared = powf(a_v.y, 2.f);
        const float t_squaredNorm = t_xSquared + t_ySquared;

        return t_squaredNorm;
    }

    float vec2::Norm(const vec2 a_v)
    {
        return sqrtf(SquaredNorm(a_v));
    }

    vec2 vec2::Normalize(const vec2 a_v)
    {
        const float t_norm = Norm(a_v);
        return { a_v.x / t_norm, a_v.y / t_norm };
    }

    float vec2::DotProduct(const vec2 a_v1, const vec2 a_v2)
    {
        return (a_v1.x * a_v2.x) + (a_v1.y * a_v2.y);
    }

    float vec2::CrossProduct(const vec2 a_v1, const vec2 a_v2)
    {
        return (a_v1.x * a_v2.y) - (a_v2.x * a_v1.y);
    }

    float vec2::GetAngle(const vec2 a_v1, const vec2 a_v2)
    {
        const float t_dotProduct = DotProduct(a_v1, a_v2);
        const float t_v1Norm = Norm(a_v1);
        const float t_v2Norm = Norm(a_v2);

        if ((t_v1Norm * t_v2Norm) <= 0.0f)
        {
            return 0.0f;
        }

        const float t_angle = acosf(t_dotProduct / (t_v1Norm * t_v2Norm));

        return t_angle;
    }
} // namespace Math
