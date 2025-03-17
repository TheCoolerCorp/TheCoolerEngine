#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <iostream>

#include "EngineExport.h"

namespace Engine
{
    namespace Math
    {
        // Class for Vector2
        class vec2
        {
        public:
            // vector components
            float x, y;

            // different ways of initializing a vector
            ENGINE_API vec2() : x(0.f), y(0.f) {}
            ENGINE_API vec2(const float a_x, const float a_y) : x(a_x), y(a_y) {}
            ENGINE_API explicit vec2(const float a_coords) : x(a_coords), y(a_coords) {}
            ENGINE_API ~vec2() = default;

            // different print methods
            ENGINE_API void Print() const;
            ENGINE_API void BlankPrint() const;

            ENGINE_API void Opposite(); // returns the vector's opposite
            ENGINE_API void Invert(); // returns the vector's invert
            ENGINE_API void AddNumber(float a_number); // adds a number to the vector
            ENGINE_API void MultiplyNumber(float a_number); // multiplies the vector by a number

            ENGINE_API float operator[](int a_index) const; // get a component with an index

            ENGINE_API static vec2 Add(vec2 a_v1, vec2 a_v2); // adds two vectors
            ENGINE_API static vec2 Multiply(vec2 a_v1, vec2 a_v2); // multiplies two vectors
            ENGINE_API  static vec2 MidPoint(vec2 a_v1, vec2 a_v2); // get the mid-point of two points
            ENGINE_API static float Distance(vec2 a_p1, vec2 a_p2); // get the distance between two points
            ENGINE_API static float SquaredNorm(vec2 a_v); // returns the squared norm of a vector
            ENGINE_API static float Norm(vec2 a_v); // returns the norm of a vector
            ENGINE_API static vec2 Normalize(vec2 a_v); //returns normalized vector
            ENGINE_API static float DotProduct(vec2 a_v1, vec2 a_v2); // returns the dot product of two vectors
            ENGINE_API static float CrossProduct(vec2 a_v1, vec2 a_v2); // returns the cross product of two vectors
            ENGINE_API static float GetAngle(vec2 a_v1, vec2 a_v2); // returns the angle between two vectors
        };
    }
}

#include "vec2.inl"

#endif