#ifndef THECOOLERMATH_H
#define THECOOLERMATH_H

#include "EngineExport.h"

namespace TheCoolerMath
{
    constexpr float PI = 3.14159265358979323846f;

    inline float ENGINE_API Lerp(const float& a_f1, const float& a_f2, const float& a_t)
    {
        return a_f2 * a_t + a_f1 * (1 - a_t);
    }

    inline float ENGINE_API ToRadians(const float& a_deg)
    {
        return a_deg * (PI / 180.f);
    }

    inline float ENGINE_API ToDegrees(const float& a_rad)
    {
        return a_rad * (180.f / PI);
    }
} // namespace TheCoolerMath

#endif