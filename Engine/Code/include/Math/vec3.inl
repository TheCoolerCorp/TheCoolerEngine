namespace TheCoolerMath
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

    inline vec3 operator+=(const vec3& a_v, const float& a_f)
    {
        return a_v + a_f;
    }

    inline vec3 operator-=(const vec3& a_v, const float& a_f)
    {
        return a_v - a_f;
    }

    inline vec3 operator*=(const vec3& a_v, const float& a_f)
    {
        return a_v * a_f;
    }

    inline vec3 operator/=(const vec3& a_v, const float& a_f)
    {
        return a_v / a_f;
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

    inline vec3 vec3::MidPoint(const vec3 a_v1, const vec3 a_v2)
    {
        vec3 t_v3;

        t_v3.x = (a_v1.x + a_v2.x) / 2;
        t_v3.y = (a_v1.y + a_v2.y) / 2;
        t_v3.z = (a_v1.z + a_v2.z) / 2;

        return t_v3;
    }

    inline float vec3::Distance(const vec3 a_p1, const vec3 a_p2)
    {
        const float t_xSquared = powf(a_p1.x - a_p2.x, 2.f);
        const float t_ySquared = powf(a_p1.y - a_p2.y, 2.f);
        const float t_zSquared = powf(a_p1.z - a_p2.z, 2.f);
        const float t_dist = sqrtf(t_xSquared + t_ySquared + t_zSquared);

        return t_dist;
    }

    inline float vec3::SquaredNorm(const vec3 a_v)
    {
        const float t_xSquared = powf(a_v.x, 2.f);
        const float t_ySquared = powf(a_v.y, 2.f);
        const float t_zSquared = powf(a_v.z, 2.f);
        const float t_squaredNorm = t_xSquared + t_ySquared + t_zSquared;

        return t_squaredNorm;
    }

    inline float vec3::Norm(const vec3 a_v)
    {
        return sqrtf(SquaredNorm(a_v));
    }

    inline vec3 vec3::Normalize(const vec3 a_v)
    {
        const float t_norm = Norm(a_v);
        return { a_v.x / t_norm, a_v.y / t_norm, a_v.z / t_norm };
    }

    inline float vec3::DotProduct(const vec3 a_v1, const vec3 a_v2)
    {
        return (a_v1.x * a_v2.x) + (a_v1.y * a_v2.y) + (a_v1.z * a_v2.z);
    }

    inline vec3 vec3::CrossProduct(const vec3 a_v1, const vec3 a_v2)
    {
        vec3 t_v3;

        t_v3.x = (a_v1.y * a_v2.z) - (a_v1.z * a_v2.y);
        t_v3.y = (a_v1.z * a_v2.x) - (a_v1.x * a_v2.z);
        t_v3.z = (a_v1.x * a_v2.y) - (a_v1.y * a_v2.x);

        return t_v3;
    }
} // namespace TheCoolerMath