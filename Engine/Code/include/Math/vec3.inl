namespace Math
{
    inline bool operator==(const vec3& a_v1, const vec3& a_v2)
    {
        return fabsf(a_v1.x - a_v2.x) < std::numeric_limits<float>::epsilon()
            && fabsf(a_v1.y - a_v2.y) < std::numeric_limits<float>::epsilon()
            && fabsf(a_v1.z - a_v2.z) < std::numeric_limits<float>::epsilon();
    }

    inline vec3 operator+(const vec3& a_v, const float& a_f)
    {
        vec3 t_v = a_v;
        t_v.AddNumber(a_f);
        return t_v;
    }

    inline vec3 operator+(const float& a_f, const vec3& a_v)
    {
        return a_v + a_f;
    }

    inline vec3 operator-(const vec3& a_v, const float& a_f)
    {
        vec3 t_v = a_v;
        t_v.AddNumber(-a_f);
        return t_v;
    }

    inline vec3 operator*(const vec3& a_v, const float& a_f)
    {
        vec3 t_v = a_v;
        t_v.MultiplyNumber(a_f);
        return t_v;
    }

    inline vec3 operator*(const float& a_f, const vec3& a_v)
    {
        return a_v * a_f;
    }

    inline vec3 operator/(const vec3& a_v, const float& a_f)
    {
        vec3 t_v = a_v;
        t_v.MultiplyNumber(1.f / a_f);
        return t_v;
    }

    inline vec3 operator+(const vec3& a_v1, const vec3& a_v2)
    {
        return vec3::Add(a_v1, a_v2);
    }

    inline vec3 operator-(const vec3& a_v1, const vec3& a_v2)
    {
        return vec3::Add(a_v1, a_v2 * -1.f);
    }

    inline vec3 operator*(const vec3& a_v1, const vec3& a_v2)
    {
        return vec3::Multiply(a_v1, a_v2);
    }

    inline vec3 operator/(const vec3& a_v1, const vec3& a_v2)
    {
        vec3 t_v3 = a_v2;
        t_v3.Invert();
        return vec3::Multiply(a_v1, t_v3);
    }

    inline vec3 operator+=(vec3& a_v, const float& a_f)
    {
        return a_v = a_v + a_f;
    }

    inline vec3 operator-=(vec3& a_v, const float& a_f)
    {
        return a_v = a_v - a_f;
    }

    inline vec3 operator*=(vec3& a_v, const float& a_f)
    {
        return a_v = a_v * a_f;
    }

    inline vec3 operator/=(vec3& a_v, const float& a_f)
    {
        return a_v = a_v / a_f;
    }

    inline vec3& operator+=(vec3& a_v1, const vec3& a_v2)
    {
        return a_v1 = a_v1 + a_v2;
    }

    inline vec3& operator-=(vec3& a_v1, const vec3& a_v2)
    {
        return a_v1 = a_v1 - a_v2;
    }

    inline vec3& operator*=(vec3& a_v1, const vec3& a_v2)
    {
        return a_v1 = a_v1 * a_v2;
    }

    inline vec3& operator/=(vec3& a_v1, const vec3& a_v2)
    {
        return a_v1 = a_v1 / a_v2;
    }

    inline void vec3::Print() const
    {
        std::cout << "vec3 : " << "x = " << x << ", y = " << y << ", z = " << z << '\n';
    }

    inline void vec3::BlankPrint() const
    {
        std::cout << x << "  " << y << "  " << z << '\n';
    }

    inline void vec3::Opposite()
    {
        x *= -1.f;
        y *= -1.f;
        z *= -1.f;
    }

    inline void vec3::Invert()
    {
        x = 1 / x;
        y = 1 / y;
        z = 1 / z;
    }

    inline void vec3::AddNumber(const float a_number)
    {
        x += a_number;
        y += a_number;
        z += a_number;
    }

    inline void vec3::MultiplyNumber(const float a_number)
    {
        x *= a_number;
        y *= a_number;
        z *= a_number;
    }

    inline vec3 vec3::Add(const vec3 a_v1, const vec3 a_v2)
    {
        vec3 t_v;

        t_v.x = a_v1.x + a_v2.x;
        t_v.y = a_v1.y + a_v2.y;
        t_v.z = a_v1.z + a_v2.z;

        return t_v;
    }

    inline vec3 vec3::Multiply(const vec3 a_v1, const vec3 a_v2)
    {
        vec3 t_v;

        t_v.x = a_v1.x * a_v2.x;
        t_v.y = a_v1.y * a_v2.y;
        t_v.z = a_v1.z * a_v2.z;

        return t_v;
    }
} // namespace Math