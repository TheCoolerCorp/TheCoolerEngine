#include "Math/mat4.h"

namespace TheCoolerMath
{
    void mat4::Inverse()
    {
        const float t_det = Determinant();
        if (t_det == 0.f)
        {
            std::cout << "Cannot invert 4x4 matrix" << '\n';
            return;
        }

        mat4 t_cofactorMatrix = Comatrix();
        t_cofactorMatrix.Transpose();

        const float t_invDet = 1.f / t_det;
        mElements = MultiplyNumber(t_cofactorMatrix, t_invDet).mElements;
    }

    mat4 mat4::Comatrix() const
    {
        mat4 t_solution;

        for (size_t i = 0; i < 4; ++i)
        {
            for (size_t j = 0; j < 4; ++j)
            {
                std::array<float, 9> t_subMatrix{};
                int p = 0, q = 0;
                for (size_t x = 0; x < 4; ++x)
                {
                    if (x == i) continue;
                    q = 0;
                    for (size_t y = 0; y < 4; ++y)
                    {
                        if (y == j) continue;
                        t_subMatrix[p * 3 + q] = mElements[x * 4 + y];
                        ++q;
                    }
                    ++p;
                }
                const float t_cofactorSign = ((i + j) % 2 == 0) ? 1.f : -1.f;
                t_solution.mElements[i * 4 + j]
                    = t_cofactorSign * Mat3Determinant(t_subMatrix);
            }
        }

        return t_solution;
    }

    mat4 mat4::Multiply(const mat4& a_mat1, const mat4& a_mat2)
    {
        std::array<float, 16> t_result{};

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                float t_sum = 0;
                for (int k = 0; k < 4; ++k)
                {
                    t_sum += a_mat1.mElements[i * 4 + k]
                        * a_mat2.mElements[k * 4 + j];
                }
                t_result[i * 4 + j] = t_sum;
            }
        }
        const mat4 t_m(t_result);
        return t_m;
    }

    mat4 mat4::View(const vec3 a_up, const vec3 a_center, const vec3 a_eye)
    {
        const vec3 t_f = vec3::Normalize(a_center - a_eye);
        const vec3 t_r = vec3::Normalize(vec3::CrossProduct(t_f, a_up));
        const vec3 t_u = vec3::CrossProduct(t_r, t_f);

        const mat4 t_view({t_r.x, t_r.y, t_r.z,
                           -1.f * vec3::DotProduct(t_r, a_eye), t_u.x, t_u.y,
                           t_u.z, -1.f * vec3::DotProduct(t_u, a_eye), -t_f.x,
                           -t_f.y, -t_f.z, vec3::DotProduct(t_f, a_eye), 0.f,
                           0.f, 0.f, 1.f});

        return t_view;
    }
} // namespace TheCoolerMath
