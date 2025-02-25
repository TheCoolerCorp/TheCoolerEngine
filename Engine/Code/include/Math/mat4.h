#pragma once
#include <array>
#include <cmath>

#include "vec3.h"
#include "quat.h"

namespace TheCoolerMath
{
    // Class for 4x4 matrices
    class mat4
    {
    public:
        std::array<float, 16> mElements{}; //vector to save the matrix's elements

        // Different ways of initializing a matrix
        explicit mat4(bool a_identity = false);
        explicit mat4(const std::array<float, 16>& a_elements);
        ~mat4() = default;

        void Print() const; // Displays the matrix

        [[nodiscard]] quat Diagonal() const; // method that returns the diagonal
        [[nodiscard]] float Trace() const; // method that returns the trace

        float operator[](int a_index) const; // operator to get any element of the matirx with the index

        void Opposite(); // returns the opposite of a matrix
        void Inverse();
        void Transpose(); // transposes a matrix and returns it
        [[nodiscard]] float Determinant() const; // returns the determinant of a matrix
        [[nodiscard]] mat4 Comatrix() const;

        static mat4 Add(const mat4& a_mat1, const mat4& a_mat2); // adds two matrices
        static mat4 MultiplyNumber(const mat4& a_mat, float a_number); // multiplies a matrix by a number
        static mat4 Multiply(const mat4& a_mat1, const mat4& a_mat2); // multiplies two matrices
        static mat4 Translate(const vec3& a_v);
        static mat4 Rotation(const quat& a_q);
        static mat4 Scale(const vec3& a_s);
        static mat4 TRS(const vec3& a_translation, const quat& a_rotation, const vec3& a_scaling);
        // returns a TRS matrix
        static mat4 View(vec3 a_up, vec3 a_center, vec3 a_eye);
        static mat4 Perspective(float a_fovY, float a_aspect, float a_near, float a_far);
        static mat4 Orthographic(float a_top, float a_bottom, float a_right, float a_left, float a_far, float a_near);
    };

    inline float Mat3Determinant(const std::array<float, 9>& a_elements)
    {
        const float t_det = a_elements[0] * (a_elements[4] * a_elements[8] - a_elements[5] * a_elements[7])
            - a_elements[1] * (a_elements[3] * a_elements[8] - a_elements[5] * a_elements[6])
            + a_elements[2] * (a_elements[3] * a_elements[7] - a_elements[4] * a_elements[6]);
        return t_det;
    }

    inline mat4 operator+(const mat4& a_mat1, const mat4& a_mat2)
    {
        return mat4::Add(a_mat1, a_mat2);
    }

    inline mat4 operator-(const mat4& a_mat1, const mat4& a_mat2)
    {
        return mat4::Add(a_mat1, mat4::MultiplyNumber(a_mat2, -1.f));
    }

    inline mat4 operator*(const mat4& a_mat1, const mat4& a_mat2)
    {
        return mat4::Multiply(a_mat1, a_mat2);
    }

    inline mat4 operator*(const mat4& a_m, const float& a_f)
    {
        return mat4::MultiplyNumber(a_m, a_f);
    }

    inline mat4 operator*(const float& a_f, const mat4& a_m)
    {
        return mat4::MultiplyNumber(a_m, a_f);
    }

    inline vec3 operator*(const mat4& a_m, const vec3& a_v)
    {
        vec3 t_vR;
        t_vR.x = (a_m.mElements[0] * a_v.x) + (a_m.mElements[1] * a_v.y) + (a_m.mElements[2] * a_v.z) + a_m.mElements[
            3];
        t_vR.y = (a_m.mElements[4] * a_v.x) + (a_m.mElements[5] * a_v.y) + (a_m.mElements[6] * a_v.z) + a_m.mElements[
            7];
        t_vR.z = (a_m.mElements[8] * a_v.x) + (a_m.mElements[9] * a_v.y) + (a_m.mElements[10] * a_v.z) + a_m.mElements[
            11];
        return t_vR;
    }

    inline quat operator*(const mat4& a_m, const quat& a_q)
    {
        quat t_qR;
        t_qR.x = (a_m.mElements[0] * a_q.x) + (a_m.mElements[1] * a_q.y) + (a_m.mElements[2] * a_q.z) + (a_m.mElements[
            3] * a_q.w);
        t_qR.y = (a_m.mElements[4] * a_q.x) + (a_m.mElements[5] * a_q.y) + (a_m.mElements[6] * a_q.z) + (a_m.mElements[
            7] * a_q.w);
        t_qR.z = (a_m.mElements[8] * a_q.x) + (a_m.mElements[9] * a_q.y) + (a_m.mElements[10] * a_q.z) + (a_m.mElements[
            11] * a_q.w);
        t_qR.w = (a_m.mElements[12] * a_q.x) + (a_m.mElements[13] * a_q.y) + (a_m.mElements[14] * a_q.z) + (a_m.
            mElements[15] * a_q.w);
        return t_qR;
    }

    inline mat4 operator+=(const mat4& a_mat1, const mat4& a_mat2)
    {
        return a_mat1 + a_mat2;
    }

    inline mat4 operator-=(const mat4& a_mat1, const mat4& a_mat2)
    {
        return a_mat1 - a_mat2;
    }

    inline mat4 operator*=(const mat4& a_mat1, const mat4& a_mat2)
    {
        return a_mat1 * a_mat2;
    }

    inline mat4 operator*=(const mat4& a_m, const float& a_f)
    {
        return a_m * a_f;
    }

    inline mat4::mat4(const std::array<float, 16>& a_elements)
    {
        if (a_elements.size() > 16)
            abort();

        mElements = a_elements;
    }

    inline void mat4::Print() const
    {
        std::cout << "Matrix 4x4 : " << '\n';
        for (int i = 0; i < 4; ++i)
        {
            quat t_line(mElements[i], mElements[i + 1], mElements[i + 2], mElements[i + 3]);
            t_line.BlankPrint();
        }
        std::cout << '\n';
    }

    inline quat mat4::Diagonal() const
    {
        return {mElements[0], mElements[5], mElements[10], mElements[15]};
    }

    inline float mat4::Trace() const
    {
        return mElements[0] + mElements[5] + mElements[10] + mElements[15];
    }

    inline float mat4::operator[](const int a_index) const
    {
        if (a_index < 0 || a_index > 15)
        {
            std::cout << "Error : overflow\n";
            return 0.f;
        }
        return mElements[a_index];
    }

    inline void mat4::Opposite()
    {
        for (float& t_element : mElements)
        {
            t_element *= -1;
        }
    }

    inline void mat4::Transpose()
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

    inline float mat4::Determinant() const
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

    inline mat4 mat4::Add(const mat4& a_mat1, const mat4& a_mat2)
    {
        mat4 t_m;
        for (int i = 0; i < 16; i++)
        {
            t_m.mElements[i] = a_mat1[i] + a_mat2[i];
        }
        return t_m;
    }

    inline mat4 mat4::MultiplyNumber(const mat4& a_mat, const float a_number)
    {
        mat4 t_m;
        for (int i = 0; i < 16; ++i)
        {
            t_m.mElements[i] = a_mat[i] * a_number;
        }
        return t_m;
    }

    inline mat4 mat4::Translate(const vec3& a_v)
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

    inline mat4 mat4::Rotation(const quat& a_q)
    {
        const std::array<float, 16> t_elements = {
            1.f - (2.f * powf(a_q.y, 2.f)) - (2.f * powf(a_q.z, 2.f)), (2.f * a_q.x * a_q.y) - (2.f * a_q.w * a_q.z),
            (2.f * a_q.x * a_q.z) + (2.f * a_q.w * a_q.y), 0,
            (2.f * a_q.x * a_q.y) + (2.f * a_q.w * a_q.z), 1.f - (2.f * powf(a_q.x, 2.f)) - (2.f * powf(a_q.z, 2.f)),
            (2.f * a_q.z * a_q.y) - (2.f * a_q.w * a_q.x), 0,
            (2.f * a_q.x * a_q.z) - (2.f * a_q.w * a_q.y), (2.f * a_q.z * a_q.y) + (2.f * a_q.w * a_q.x),
            1.f - (2.f * powf(a_q.x, 2.f)) - (2.f * powf(a_q.y, 2.f)), 0,
            0, 0, 0, 1
        };

        const mat4 t_m(t_elements);
        return t_m;
    }

    inline mat4 mat4::Scale(const vec3& a_s)
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

    inline mat4 mat4::TRS(const vec3& a_translation, const quat& a_rotation, const vec3& a_scaling)
    {
        return Translate(a_translation) * Rotation(a_rotation) * Scale(a_scaling);
    }

    inline mat4 mat4::Perspective(const float a_fovY, const float a_aspect, const float a_near, const float a_far)
    {
        return mat4({
            1.f / (a_aspect * tanf(a_fovY / 2.f)), 0.f, 0.f, 0.f,
            0.f, 1.f / (tanf(a_fovY / 2.f)), 0.f, 0.f,
            0.f, 0.f, -1.f * (a_far + a_near) / (a_far - a_near), -1.f * (2.f * a_far * a_near) / (a_far - a_near),
            0.f, 0.f, -1.f, 0.f
        });
    }

    inline mat4 mat4::Orthographic(const float a_top, const float a_bottom, const float a_right, const float a_left,
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
