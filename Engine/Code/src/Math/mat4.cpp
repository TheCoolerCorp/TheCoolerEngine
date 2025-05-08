#include "Math/mat4.h"

namespace Engine
{
	namespace Math
	{
        void mat4::Print() const
        {
            std::cout << "Matrix 4x4 : " << '\n';
            quat t_line1(mElements[0], mElements[1], mElements[2], mElements[3]);
            t_line1.BlankPrint();
            quat t_line2(mElements[4], mElements[5], mElements[6], mElements[7]);
            t_line2.BlankPrint();
            quat t_line3(mElements[8], mElements[9], mElements[10], mElements[11]);
            t_line3.BlankPrint();
            quat t_line4(mElements[12], mElements[13], mElements[14], mElements[15]);
            t_line4.BlankPrint();
            std::cout << '\n';
        }

        quat mat4::Diagonal() const
        {
            return { mElements[0], mElements[5], mElements[10], mElements[15] };
        }

        void mat4::Opposite()
        {
            for (float& t_element : mElements)
            {
                t_element *= -1;
            }
        }

        void mat4::Inverse()
        {
            const float t_det = Determinant();
            if (t_det == 0.f)
            {
                return;
            }

            mat4 t_cofactorMatrix = Comatrix();
            t_cofactorMatrix.Transpose();

            const float t_invDet = 1.f / t_det;
            mElements = MultiplyNumber(t_cofactorMatrix, t_invDet).mElements;
        }

        void mat4::Transpose()
        {
            for (int i = 0; i < 4; ++i)
            {
                for (int j = i + 1; j < 4; ++j)
                {
                    const float t_temp = mElements[i * 4 + j];
                    mElements[i * 4 + j] = mElements[j * 4 + i];
                    mElements[j * 4 + i] = t_temp;
                }
            }
        }

        float mat4::Determinant() const
        {
            float t_det = 0.f;
            t_det += mElements[0] * Mat3Determinant({
                mElements[5], mElements[6], mElements[7], mElements[9], mElements[10], mElements[11], mElements[13],
                mElements[14], mElements[15]
                });
            t_det -= mElements[1] * Mat3Determinant({
                mElements[4], mElements[6], mElements[7], mElements[8], mElements[10], mElements[11], mElements[12],
                mElements[14], mElements[15]
                });
            t_det += mElements[2] * Mat3Determinant({
                mElements[4], mElements[5], mElements[7], mElements[8], mElements[9], mElements[11], mElements[12],
                mElements[13], mElements[15]
                });
            t_det -= mElements[3] * Mat3Determinant({
                mElements[4], mElements[5], mElements[6], mElements[8], mElements[9], mElements[10], mElements[12],
                mElements[13], mElements[14]
                });
            return t_det;
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

        mat4 mat4::Add(const mat4& a_mat1, const mat4& a_mat2)
        {
            mat4 t_m;
            for (int i = 0; i < 16; i++)
            {
                t_m.mElements[i] = a_mat1[i] + a_mat2[i];
            }
            return t_m;
        }

        mat4 mat4::MultiplyNumber(const mat4& a_mat, const float a_number)
        {
            mat4 t_m;
            for (int i = 0; i < 16; ++i)
            {
                t_m.mElements[i] = a_mat[i] * a_number;
            }
            return t_m;
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

        mat4 mat4::Translate(const vec3& a_v)
        {
            const std::array<float, 16> t_elements = {
                1.f, 0.f, 0.f, a_v.x,
                0.f, 1.f, 0.f, a_v.y,
                0.f, 0.f, 1.f, a_v.z,
                0.f, 0.f, 0.f, 1.f
            };

            const mat4 t_m(t_elements);
            return t_m;
        }

        mat4 mat4::Rotation(const quat& a_q)
        {
            const std::array<float, 16> t_elements = {
                1.f - (2.f * powf(a_q.y, 2.f)) - (2.f * powf(a_q.z, 2.f)), (2.f * a_q.x * a_q.y) - (2.f * a_q.w * a_q.z), (2.f * a_q.x * a_q.z) + (2.f * a_q.w * a_q.y), 0,
                (2.f * a_q.x * a_q.y) + (2.f * a_q.w * a_q.z), 1.f - (2.f * powf(a_q.x, 2.f)) - (2.f * powf(a_q.z, 2.f)), (2.f * a_q.z * a_q.y) - (2.f * a_q.w * a_q.x), 0,
            	(2.f * a_q.x * a_q.z) - (2.f * a_q.w * a_q.y), (2.f * a_q.z * a_q.y) + (2.f * a_q.w * a_q.x), 1.f - (2.f * powf(a_q.x, 2.f)) - (2.f * powf(a_q.y, 2.f)), 0,
                0, 0, 0, 1
            };

            const mat4 t_m(t_elements);
            return t_m;
        }

        mat4 mat4::Scale(const vec3& a_s)
        {
            const std::array<float, 16> t_elements = {
                a_s.x, 0.f, 0.f, 0.f,
                0.f, a_s.y, 0.f, 0.f,
                0.f, 0.f, a_s.z, 0.f,
                0.f, 0.f, 0.f, 1.f
            };

            const mat4 t_m(t_elements);
            return t_m;
        }

        mat4 mat4::TRS(const vec3& a_translation, const quat& a_rotation, const vec3& a_scaling)
        {
            return Translate(a_translation) * Rotation(a_rotation) * Scale(a_scaling);
        }

        mat4 mat4::View(const vec3 a_up, const vec3 a_center, const vec3 a_eye)
        {
            const vec3 t_f = vec3::Normalize(a_center - a_eye);
            const vec3 t_r = vec3::Normalize(vec3::CrossProduct(t_f, a_up));
            const vec3 t_u = vec3::CrossProduct(t_r, t_f);

            const mat4 t_view({
            	t_r.x,  t_r.y,  t_r.z,  -1.f * vec3::DotProduct(t_r, a_eye),
            	t_u.x,  t_u.y,  t_u.z,  -1.f * vec3::DotProduct(t_u, a_eye),
            	-t_f.x, -t_f.y, -t_f.z, vec3::DotProduct(t_f, a_eye),
            	0.f,    0.f,    0.f,    1.f });

            return t_view;
        }

        mat4 mat4::Perspective(const float a_fovY, const float a_aspect, const float a_near, const float a_far)
        {
            return mat4({
                1.f / (a_aspect * tanf(a_fovY / 2.f)), 0.f, 0.f, 0.f,
                0.f, 1.f / (tanf(a_fovY / 2.f)), 0.f, 0.f,
                0.f, 0.f, -1.f * (a_far + a_near) / (a_far - a_near), -1.f * (2.f * a_far * a_near) / (a_far - a_near),
                0.f, 0.f, -1.f, 0.f
                });
        }

        mat4 mat4::Orthographic(const float a_top, const float a_bottom, const float a_right, const float a_left,
            const float a_far, const float a_near)
        {
            return mat4({
                2.f / (a_right - a_left), 0.f, 0.f, -1.f * ((a_right + a_left) / (a_right - a_left)),
                0.f, 2.f / (a_top - a_bottom), 0.f, -1.f * ((a_top + a_bottom) / (a_top - a_bottom)),
                0.f, 0.f, -2.f / (a_far - a_near), -1.f * ((a_far + a_near) / (a_far - a_near)),
                0.f, 0.f, 0.f, 1.f
                });
        }
	}
}
