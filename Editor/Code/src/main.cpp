#include <iostream>
#include "Math/vec3.h"

using namespace TheCoolerMath;

int main()
{
    const vec3 t_vec = vec3(0.f, 1.f, 2.f);
    const vec3 t_vec2 = vec3(0.f, 3.f, 5.f);
    vec3 t_ve3 = vec3::CrossProduct(t_vec, t_vec2);
    t_ve3.Print();
    return 0;
}
