#pragma once
namespace TheCoolerMath
{
    constexpr float PI = 3.14159265358979323846f;

    inline float Lerp(const float& a_f1, const float& a_f2, const float& a_t)
    {
        return a_f2 * a_t + a_f1 * (1 - a_t);
    }

    inline float ToRadians(const float& a_deg)
    {
        return a_deg * (PI / 180.f);
    }

    inline float ToDegrees(const float& a_rad)
    {
        return a_rad * (180.f / PI);
    }
}
