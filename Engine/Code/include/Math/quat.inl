namespace TheCoolerMath
{
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

    inline quat operator+=(quat& a_q, const float& a_f)
    {
        return a_q = a_q + a_f;
    }

    inline quat operator-=(quat& a_q, const float& a_f)
    {
        return a_q = a_q - a_f;
    }

    inline quat operator*=(quat& a_q, const float& a_f)
    {
        return a_q = a_q * a_f;
    }

    inline quat operator/=(quat& a_q, const float& a_f)
    {
        return a_q = a_q / a_f;
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

    inline quat quat::Add(const quat a_q1, const quat a_q2)
    {
        quat t_q;

        t_q.x = a_q1.x + a_q2.x;
        t_q.y = a_q1.y + a_q2.y;
        t_q.z = a_q1.z + a_q2.z;
        t_q.w = a_q1.w + a_q2.w;

        return t_q;
    }
} // namespace TheCoolerMath