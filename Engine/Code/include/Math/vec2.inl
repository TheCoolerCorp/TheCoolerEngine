#ifndef VEC2_INL
#define VEC2_INL

namespace Engine
{
    namespace Math
    {
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

        inline vec2& operator+=(vec2& a_v, const float& a_f)
        {
            return a_v = a_v + a_f;
        }

        inline vec2& operator-=(vec2& a_v, const float& a_f)
        {
            return a_v = a_v - a_f;
        }

        inline vec2& operator*=(vec2& a_v, const float& a_f)
        {
            return a_v = a_v * a_f;
        }

        inline vec2& operator/=(vec2& a_v, const float& a_f)
        {
            return a_v = a_v / a_f;
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
    }
}
#endif // VEC2_INL
