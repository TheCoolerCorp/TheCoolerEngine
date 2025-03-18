#ifndef MAT4_H
#define MAT4_H

#include <array>
#include <cmath>

#include "quat.h"
#include "vec3.h"
#include "EngineExport.h"

namespace Engine
{
    namespace Math
    {
        // Class for 4x4 matrices
        class mat4
        {
        public:
            std::array<float, 16> mElements{}; //vector to save the matrix's elements

            // Different ways of initializing a matrix
            ENGINE_API explicit mat4(bool a_identity = false);
            ENGINE_API explicit mat4(const std::array<float, 16>& a_elements);
            ENGINE_API ~mat4() = default;

            ENGINE_API void Print() const; // Displays the matrix

            ENGINE_API [[nodiscard]] quat Diagonal() const; // method that returns the diagonal
            ENGINE_API [[nodiscard]] float Trace() const; // method that returns the trace

            ENGINE_API float operator[](int a_index) const; // operator to get any element of the matrix with the index

            ENGINE_API void Opposite(); // returns the opposite of a matrix
            ENGINE_API void Inverse();
            ENGINE_API void Transpose(); // transposes a matrix and returns it
            ENGINE_API [[nodiscard]] float Determinant() const; // returns the determinant of a matrix
            ENGINE_API [[nodiscard]] mat4 Comatrix() const;

            ENGINE_API static mat4 Add(const mat4& a_mat1, const mat4& a_mat2); // adds two matrices
            ENGINE_API static mat4 MultiplyNumber(const mat4& a_mat, float a_number); // multiplies a matrix by a number
            ENGINE_API static mat4 Multiply(const mat4& a_mat1, const mat4& a_mat2); // multiplies two matrices
            ENGINE_API  static mat4 Translate(const vec3& a_v);
            ENGINE_API static mat4 Rotation(const quat& a_q);
            ENGINE_API static mat4 Scale(const vec3& a_s);
            ENGINE_API static mat4 TRS(const vec3& a_translation, const quat& a_rotation, const vec3& a_scaling);
            // returns a TRS matrix
            ENGINE_API static mat4 View(vec3 a_up, vec3 a_center, vec3 a_eye);
            ENGINE_API static mat4 Perspective(float a_fovY, float a_aspect, float a_near, float a_far);
            ENGINE_API  static mat4 Orthographic(float a_top, float a_bottom, float a_right, float a_left, float a_far, float a_near);
        };

        float ENGINE_API Mat3Determinant(const std::array<float, 9>& a_elements);
    }
}

#include "mat4.inl"

#endif