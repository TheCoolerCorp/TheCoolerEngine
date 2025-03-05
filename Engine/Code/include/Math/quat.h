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
        class ENGINE_API quat
        {
        public:
            // quaternion components
            float x, y, z, w;

            // different ways of initializing a quaternion
            quat() : x(0.f), y(0.f), z(0.f), w(1.f) {}
            quat(const float a_x, const float a_y, const float a_z, const float a_w) : x(a_x), y(a_y), z(a_z), w(a_w) {}
            explicit quat(const float a_coords) : x(a_coords), y(a_coords), z(a_coords), w(a_coords) {}
            explicit quat(vec3 a_eulerAngles);
            ~quat() = default;

            // different print methods
            void Print() const;
            void BlankPrint() const;

            void Invert(); // returns the quaternion's invert
            void AddNumber(float a_number); // adds a number to the quaternion
            void MultiplyNumber(float a_number); // multiplies the quaternion by a number

            float operator[](int a_index) const; // get a component with an index

            static vec3 ToEulerAngles(quat a_q);
            static quat Conjugate(const quat& a_q); // returns the quaternion's opposite
            static quat Add(quat a_q1, quat a_q2); // adds two quaternions
            static quat Multiply(quat a_q1, quat a_q2); // multiplies two quaternions
            static vec3 Multiply(quat a_q, vec3 a_v);
            static quat Slerp(quat a_q1, quat a_q2, float a_t);
            static float SquaredNorm(quat a_q); // returns the squared norm of a quaternion
            static float Norm(quat a_q); // returns the norm of a quaternion
            static quat Normalize(quat a_q); //returns normalized quaternion
            static float DotProduct(quat a_q1, quat a_q2); // returns the dot product of two quaternions
        };
    }
}

#include "quat.inl"

#endif