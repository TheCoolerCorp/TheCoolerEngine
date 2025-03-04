#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <iostream>

#include "EngineExport.h"

namespace Math
{
    // Class for Vector2
    class ENGINE_API vec2
    {
    public:
        // vector components
        float x, y;

        // different ways of initializing a vector
        vec2() : x(0.f), y(0.f) {}
        vec2(const float a_x, const float a_y) : x(a_x), y(a_y) {}
        explicit vec2(const float a_coords) : x(a_coords), y(a_coords) {}
        ~vec2() = default;

        // different print methods
        void Print() const;
        void BlankPrint() const;

        void Opposite(); // returns the vector's opposite
        void Invert(); // returns the vector's invert
        void AddNumber(float a_number); // adds a number to the vector
        void MultiplyNumber(float a_number); // multiplies the vector by a number

        float operator[](int a_index) const; // get a component with an index

        static vec2 Add(vec2 a_v1, vec2 a_v2); // adds two vectors
        static vec2 Multiply(vec2 a_v1, vec2 a_v2); // multiplies two vectors
        static vec2 MidPoint(vec2 a_v1, vec2 a_v2); // get the mid-point of two points
        static float Distance(vec2 a_p1, vec2 a_p2); // get the distance between two points
        static float SquaredNorm(vec2 a_v); // returns the squared norm of a vector
        static float Norm(vec2 a_v); // returns the norm of a vector
        static vec2 Normalize(vec2 a_v); //returns normalized vector
        static float DotProduct(vec2 a_v1, vec2 a_v2); // returns the dot product of two vectors
        static float CrossProduct(vec2 a_v1, vec2 a_v2); // returns the cross product of two vectors
        static float GetAngle(vec2 a_v1, vec2 a_v2); // returns the angle between two vectors
    };
} // namespace Math

#include "vec2.inl"

#endif