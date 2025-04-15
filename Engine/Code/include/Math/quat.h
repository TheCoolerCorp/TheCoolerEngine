#ifndef QUAT_H
#define QUAT_H

#include <cmath>
#include <iostream>

#include "vec3.h"
#include "EngineExport.h"

namespace Engine
{
    namespace Math
    {
        // Class for Quaternion
        class quat
        {
        public:
            // quaternion components
            float x, y, z, w;

            // different ways of initializing a quaternion
            ENGINE_API quat() : x(0.f), y(0.f), z(0.f), w(1.f) {}
            ENGINE_API quat(const float a_x, const float a_y, const float a_z, const float a_w) : x(a_x), y(a_y), z(a_z), w(a_w) {}
            ENGINE_API explicit quat(const float a_coords) : x(a_coords), y(a_coords), z(a_coords), w(a_coords) {}
            ENGINE_API explicit quat(vec3 a_eulerAngles);
            ENGINE_API ~quat() = default;

            // different print methods
            ENGINE_API void Print() const;
            ENGINE_API void BlankPrint() const;

            ENGINE_API void Invert(); // returns the quaternion's invert
            ENGINE_API void AddNumber(float a_number); // adds a number to the quaternion
            ENGINE_API void MultiplyNumber(float a_number); // multiplies the quaternion by a number

            ENGINE_API float operator[](int a_index) const; // get a component with an index

            ENGINE_API static vec3 ToEulerAngles(quat a_q);
            ENGINE_API static quat Conjugate(const quat& a_q); // returns the quaternion's opposite
            ENGINE_API static quat Add(quat a_q1, quat a_q2); // adds two quaternions
            ENGINE_API static quat Multiply(quat a_q1, quat a_q2); // multiplies two quaternions
            ENGINE_API static vec3 Multiply(quat a_q, vec3 a_v);
            ENGINE_API static quat Slerp(quat a_q1, quat a_q2, float a_t);
            ENGINE_API static float SquaredNorm(quat a_q); // returns the squared norm of a quaternion
            ENGINE_API  static float Norm(quat a_q); // returns the norm of a quaternion
            ENGINE_API static quat Normalize(quat a_q); //returns normalized quaternion
            ENGINE_API static float DotProduct(quat a_q1, quat a_q2); // returns the dot product of two quaternions
            ENGINE_API static quat GetRotation(const mat4& a_m);
        };
    }
}

#include "quat.inl"

#endif