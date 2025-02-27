#pragma once
#include <cmath>
#include <iostream>

namespace TheCoolerMath
{
    // Class for Vector2
    class vec2
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

    inline bool operator==(const vec2& a_v1, const vec2& a_v2)
    {
        return fabsf(a_v1.x - a_v2.x) < std::numeric_limits<float>::epsilon()
            && fabsf(a_v1.y - a_v2.y) < std::numeric_limits<float>::epsilon();
    }

    inline vec2 operator+(const vec2& a_v, const float& a_f)
    {
        vec2 t_vR = a_v;
        t_vR.AddNumber(a_f);
        return t_vR;
    }

    inline vec2 operator+(const float& a_f, const vec2& a_v)
    {
        return a_v + a_f;
    }

    inline vec2 operator-(const vec2& a_v, const float& a_f)
    {
        vec2 t_vR = a_v;
        t_vR.AddNumber(-a_f);
        return t_vR;
    }

    inline vec2 operator*(const vec2& a_v, const float& a_f)
    {
        vec2 t_vR = a_v;
        t_vR.MultiplyNumber(a_f);
        return t_vR;
    }

    inline vec2 operator*(const float& a_f, const vec2& a_v)
    {
        return a_v * a_f;
    }

    inline vec2 operator/(const vec2& a_v, const float& a_f)
    {
        vec2 t_vR = a_v;
        t_vR.MultiplyNumber(1.f / a_f);
        return t_vR;
    }

    inline vec2 operator+(const vec2& a_v1, const vec2& a_v2)
    {
        return vec2::Add(a_v1, a_v2);
    }

    inline vec2 operator-(const vec2& a_v1, const vec2& a_v2)
    {
        vec2 t_v3 = a_v2;
        t_v3.Opposite();
        return vec2::Add(a_v1, t_v3);
    }

    inline vec2 operator*(const vec2& a_v1, const vec2& a_v2)
    {
        return vec2::Multiply(a_v1, a_v2);
    }

    inline vec2 operator/(const vec2& a_v1, const vec2& a_v2)
    {
        vec2 t_v3 = a_v2;
        t_v3.Invert();
        return vec2::Multiply(a_v1, t_v3);
    }

    inline vec2 operator+=(const vec2& a_v, const float& a_f)
    {
        return a_v + a_f;
    }

    inline vec2 operator-=(const vec2& a_v, const float& a_f)
    {
        return a_v - a_f;
    }

    inline vec2 operator*=(const vec2& a_v, const float& a_f)
    {
        return a_v * a_f;
    }

    inline vec2 operator/=(const vec2& a_v, const float& a_f)
    {
        return a_v / a_f;
    }

    inline vec2& operator+=(vec2& a_v1, const vec2& a_v2)
    {
        return a_v1 = a_v1 + a_v2;
    }

    inline vec2& operator-=(vec2& a_v1, const vec2& a_v2)
    {
        return a_v1 = a_v1 - a_v2;
    }

    inline vec2& operator*=(vec2& a_v1, const vec2& a_v2)
    {
        return a_v1 = a_v1 * a_v2;
    }

    inline vec2& operator/=(vec2& a_v1, const vec2& a_v2)
    {
        return a_v1 = a_v1 / a_v2;
    }

    inline void vec2::Print() const
    {
        std::cout << "vec2 : " << "x = " << x << ", y = " << y << '\n';
    }

    inline void vec2::BlankPrint() const
    {
        std::cout << x << "  " << y << '\n';
    }

    inline void vec2::Opposite()
    {
        x *= -1.f;
        y *= -1.f;
    }

    inline void vec2::Invert()
    {
        x = 1 / x;
        y = 1 / y;
    }

    inline void vec2::AddNumber(const float a_number)
    {
        x += a_number;
        y += a_number;
    }

    inline void vec2::MultiplyNumber(const float a_number)
    {
        x *= a_number;
        y *= a_number;
    }

    inline vec2 vec2::Add(const vec2 a_v1, const vec2 a_v2)
    {
        vec2 t_v;

        t_v.x = a_v1.x + a_v2.x;
        t_v.y = a_v1.y + a_v2.y;

        return t_v;
    }

    inline vec2 vec2::Multiply(const vec2 a_v1, const vec2 a_v2)
    {
        vec2 t_v;

        t_v.x = a_v1.x * a_v2.x;
        t_v.y = a_v1.y * a_v2.y;

        return t_v;
    }

    inline vec2 vec2::MidPoint(const vec2 a_v1, const vec2 a_v2)
    {
        vec2 t_v3;

        t_v3.x = (a_v1.x + a_v2.x) / 2;
        t_v3.y = (a_v1.y + a_v2.y) / 2;

        return t_v3;
    }

    inline float vec2::Distance(const vec2 a_p1, const vec2 a_p2)
    {
        const float t_xSquared = powf(a_p1.x - a_p2.x, 2.f);
        const float t_ySquared = powf(a_p1.y - a_p2.y, 2.f);
        const float t_dist = sqrtf(t_xSquared + t_ySquared);

        return t_dist;
    }

    inline float vec2::SquaredNorm(const vec2 a_v)
    {
        const float t_xSquared = powf(a_v.x, 2.f);
        const float t_ySquared = powf(a_v.y, 2.f);
        const float t_squaredNorm = t_xSquared + t_ySquared;

        return t_squaredNorm;
    }

    inline float vec2::Norm(const vec2 a_v)
    {
        return sqrtf(SquaredNorm(a_v));
    }

    inline vec2 vec2::Normalize(const vec2 a_v)
    {
        const float t_norm = Norm(a_v);
        return {a_v.x / t_norm, a_v.y / t_norm};
    }

    inline float vec2::DotProduct(const vec2 a_v1, const vec2 a_v2)
    {
        return (a_v1.x * a_v2.x) + (a_v1.y * a_v2.y);
    }

    inline float vec2::CrossProduct(const vec2 a_v1, const vec2 a_v2)
    {
        return (a_v1.x * a_v2.y) - (a_v2.x * a_v1.y);
    }
} // namespace TheCoolerMath
