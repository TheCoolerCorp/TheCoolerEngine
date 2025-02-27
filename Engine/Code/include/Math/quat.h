#pragma once
#include <cmath>
#include <iostream>

#include "vec3.h"

namespace TheCoolerMath
{
    // Class for Quaternion
    class quat
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
        static float SquaredNorm(quat a_q); // returns the squared norm of a quaternion
        static float Norm(quat a_q); // returns the norm of a quaternion
        static quat Normalize(quat a_q); //returns normalized quaternion
        static float DotProduct(quat a_q1, quat a_q2); // returns the dot product of two quaternions
    };

    inline bool operator==(const quat& a_q1, const quat& a_q2)
    {
        return fabsf(a_q1.x - a_q2.x) < std::numeric_limits<float>::epsilon()
            && fabsf(a_q1.y - a_q2.y) < std::numeric_limits<float>::epsilon()
            && fabsf(a_q1.z - a_q2.z) < std::numeric_limits<float>::epsilon()
            && fabsf(a_q1.w - a_q2.w) < std::numeric_limits<float>::epsilon();
    }

    inline quat operator+(const quat& a_q, const float& a_f)
    {
        quat t_q = a_q;
        t_q.AddNumber(a_f);
        return t_q;
    }

    inline quat operator+(const float& a_f, const quat& a_q)
    {
        return a_q + a_f;
    }

    inline quat operator-(const quat& a_q, const float& a_f)
    {
        return a_q + (-a_f);
    }

    inline quat operator*(const quat& a_q, const float& a_f)
    {
        quat t_q = a_q;
        t_q.MultiplyNumber(a_f);
        return t_q;
    }

    inline quat operator*(const float& a_f, const quat& a_q)
    {
        return a_q * a_f;
    }

    inline quat operator/(const quat& a_q, const float& a_f)
    {
        quat t_q = a_q;
        t_q.MultiplyNumber(1.f / a_f);
        return t_q;
    }

    inline vec3 operator*(const quat& a_q, const vec3& a_v)
    {
        return quat::Multiply(a_q, a_v);
    }

    inline quat operator+(const quat& a_q1, const quat& a_q2)
    {
        return quat::Add(a_q1, a_q2);
    }

    inline quat operator-(const quat& a_q1, const quat& a_q2)
    {
        return quat::Add(a_q1, a_q2 * -1.f);
    }

    inline quat operator*(const quat& a_q1, const quat& a_q2)
    {
        return quat::Multiply(a_q1, a_q2);
    }

    inline quat operator/(const quat& a_q1, const quat& a_q2)
    {
        quat t_q3 = a_q2;
        t_q3.Invert();
        return quat::Multiply(a_q1, t_q3);
    }

    inline quat operator+=(const quat& a_q, const float& a_f)
    {
        return a_q + a_f;
    }

    inline quat operator-=(const quat& a_q, const float& a_f)
    {
        return a_q - a_f;
    }

    inline quat operator*=(const quat& a_q, const float& a_f)
    {
        return a_q * a_f;
    }

    inline quat operator/=(const quat& a_q, const float& a_f)
    {
        return a_q / a_f;
    }

    inline quat& operator+=(quat& a_q1, const quat& a_q2)
    {
        return a_q1 = a_q1 + a_q2;
    }

    inline quat& operator-=(quat& a_q1, const quat& a_q2)
    {
        return a_q1 = a_q1 - a_q2;
    }

    inline quat& operator*=(quat& a_q1, const quat& a_q2)
    {
        return a_q1 = a_q1 * a_q2;
    }

    inline quat& operator/=(quat& a_q1, const quat& a_q2)
    {
        return a_q1 = a_q1 / a_q2;
    }

    inline void quat::Print() const
    {
        std::cout << "quat : " << "x = " << x << ", y = " << y << ", z = " << z << ", w = " << w << '\n';
    }

    inline void quat::BlankPrint() const
    {
        std::cout << x << "  " << y << "  " << z << "  " << w << '\n';
    }

    inline void quat::Invert()
    {
        x = 1.f / x;
        y = 1.f / y;
        z = 1.f / z;
        w = 1.f / w;
    }

    inline void quat::AddNumber(const float a_number)
    {
        x += a_number;
        y += a_number;
        z += a_number;
        w += a_number;
    }

    inline void quat::MultiplyNumber(const float a_number)
    {
        x *= a_number;
        y *= a_number;
        z *= a_number;
        w *= a_number;
    }

    inline quat quat::Conjugate(const quat& a_q)
    {
        quat t_q = a_q;
        t_q.x *= -1.f;
        t_q.y *= -1.f;
        t_q.z *= -1.f;
        return t_q;
    }

    inline quat quat::Add(const quat a_q1, const quat a_q2)
    {
        quat t_q;

        t_q.x = a_q1.x + a_q2.x;
        t_q.y = a_q1.y + a_q2.y;
        t_q.z = a_q1.z + a_q2.z;
        t_q.w = a_q1.w + a_q2.w;

        return t_q;
    }

    inline quat quat::Multiply(const quat a_q1, const quat a_q2)
    {
        const float t_x = a_q1.w * a_q2.x + a_q1.x * a_q2.w + a_q1.y * a_q2.z - a_q1.z * a_q2.y;
        const float t_y = a_q1.w * a_q2.y - a_q1.x * a_q2.z + a_q1.y * a_q2.w + a_q1.z * a_q2.x;
        const float t_z = a_q1.w * a_q2.z + a_q1.x * a_q2.y - a_q1.y * a_q2.x + a_q1.z * a_q2.w;
        const float t_w = a_q1.w * a_q2.w - a_q1.x * a_q2.x - a_q1.y * a_q2.y - a_q1.z * a_q2.z;
        return quat{t_x, t_y, t_z, t_w};
    }

    inline vec3 quat::Multiply(quat a_q, vec3 a_v)
    {
        quat t_quatVec(a_v.x, a_v.y, a_v.z, 0.f);
        quat t_conjQ = Conjugate(a_q);
        quat t_rot = (a_q * t_quatVec) * t_conjQ;
        return {t_rot.x, t_rot.y, t_rot.z};
    }

    inline float quat::SquaredNorm(const quat a_q)
    {
        const float t_xSquared = powf(a_q.x, 2.f);
        const float t_ySquared = powf(a_q.y, 2.f);
        const float t_zSquared = powf(a_q.z, 2.f);
        const float t_wSquared = powf(a_q.w, 2.f);
        const float t_squaredNorm = t_xSquared + t_ySquared + t_zSquared + t_wSquared;

        return t_squaredNorm;
    }

    inline float quat::Norm(const quat a_q)
    {
        return sqrtf(SquaredNorm(a_q));
    }

    inline quat quat::Normalize(const quat a_q)
    {
        const float t_norm = Norm(a_q);
        return {a_q.x / t_norm, a_q.y / t_norm, a_q.z / t_norm, a_q.w / t_norm};
    }

    inline float quat::DotProduct(const quat a_q1, const quat a_q2)
    {
        return (a_q1.x * a_q2.x) + (a_q1.y * a_q2.y) + (a_q1.z * a_q2.z) + (a_q1.w * a_q2.w);
    }
} // namespace TheCoolerMath
