#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include <limits>

namespace TheCoolerMath
{
    //Class for Vector3
    class vec3
    {
    public:
        // vector components
        float x;
        float y;
        float z;

        // different ways of initializing a vector
        vec3() : x(0.f), y(0.f), z(0.f) {}
        vec3(const float a_x, const float a_y, const float a_z) : x(a_x), y(a_y), z(a_z) {}
        explicit vec3(const float a_coords) : x(a_coords), y(a_coords), z(a_coords) {}
        ~vec3() = default;

        // different print methods
        void Print() const;
        void BlankPrint() const;

        void Opposite(); // returns the vector's opposite
        void Invert(); // returns the vector's invert
        void AddNumber(float a_number); // adds a number to the vector
        void MultiplyNumber(float a_number); // multiplies the vector by a number

        float operator[](int a_index) const; // get a component with an index

        static vec3 Add(vec3 a_v1, vec3 a_v2); // adds two vectors
        static vec3 Multiply(vec3 a_v1, vec3 a_v2); // multiplies two vectors
        static vec3 MidPoint(vec3 a_v1, vec3 a_v2); // get the mid-point of two points
        static float Distance(vec3 a_p1, vec3 a_p2); // get the distance between two points
        static float SquaredNorm(vec3 a_v); // returns the squared norm of a vector
        static float Norm(vec3 a_v); // returns the norm of a vector
        static vec3 Normalize(vec3 a_v); //returns normalized vector
        static float DotProduct(vec3 a_v1, vec3 a_v2); // returns the dot product of two vectors
        static vec3 CrossProduct(vec3 a_v1, vec3 a_v2); // returns the cross product of two vectors
        static float GetAngle(vec3 a_v1, vec3 a_v2); // returns the angle between two vectors
    };
} // namespace TheCoolerMath

#include "vec3.inl"

#endif