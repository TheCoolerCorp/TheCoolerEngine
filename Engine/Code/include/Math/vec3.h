#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include "EngineExport.h"

namespace Engine
{
    namespace Math
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
            ENGINE_API vec3() : x(0.f), y(0.f), z(0.f) {}
            ENGINE_API vec3(const float a_x, const float a_y, const float a_z) : x(a_x), y(a_y), z(a_z) {}
            ENGINE_API explicit vec3(const float a_coords) : x(a_coords), y(a_coords), z(a_coords) {}
            ENGINE_API ~vec3() = default;

            // different print methods
            ENGINE_API void Print() const;
            ENGINE_API void BlankPrint() const;

            ENGINE_API void Opposite(); // returns the vector's opposite
            ENGINE_API  void Invert(); // returns the vector's invert
            ENGINE_API void AddNumber(float a_number); // adds a number to the vector
            ENGINE_API  void MultiplyNumber(float a_number); // multiplies the vector by a number

            ENGINE_API float operator[](int a_index) const; // get a component with an index

            ENGINE_API static vec3 Add(vec3 a_v1, vec3 a_v2); // adds two vectors
            ENGINE_API  static vec3 Multiply(vec3 a_v1, vec3 a_v2); // multiplies two vectors
            ENGINE_API static vec3 MidPoint(vec3 a_v1, vec3 a_v2); // get the mid-point of two points
            ENGINE_API  static float Distance(vec3 a_p1, vec3 a_p2); // get the distance between two points
            ENGINE_API static float SquaredNorm(vec3 a_v); // returns the squared norm of a vector
            ENGINE_API static float Norm(vec3 a_v); // returns the norm of a vector
            ENGINE_API static vec3 Normalize(vec3 a_v); //returns normalized vector
            ENGINE_API  static float DotProduct(vec3 a_v1, vec3 a_v2); // returns the dot product of two vectors
            ENGINE_API  static vec3 CrossProduct(vec3 a_v1, vec3 a_v2); // returns the cross product of two vectors
            ENGINE_API static float GetAngle(vec3 a_v1, vec3 a_v2); // returns the angle between two vectors
        };
    }
}

#include "vec3.inl"

#endif