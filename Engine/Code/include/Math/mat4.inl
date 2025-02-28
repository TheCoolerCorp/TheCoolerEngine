namespace TheCoolerMath
{
    inline float Mat3Determinant(const std::array<float, 9>& a_elements)
    {
        const float t_det = a_elements[0] * (a_elements[4] * a_elements[8] - a_elements[5] * a_elements[7])
            - a_elements[1] * (a_elements[3] * a_elements[8] - a_elements[5] * a_elements[6])
            + a_elements[2] * (a_elements[3] * a_elements[7] - a_elements[4] * a_elements[6]);
        return t_det;
    }

    inline mat4 operator+(const mat4& a_mat1, const mat4& a_mat2)
    {
        return mat4::Add(a_mat1, a_mat2);
    }

    inline mat4 operator-(const mat4& a_mat1, const mat4& a_mat2)
    {
        return mat4::Add(a_mat1, mat4::MultiplyNumber(a_mat2, -1.f));
    }

    inline mat4 operator*(const mat4& a_mat1, const mat4& a_mat2)
    {
        return mat4::Multiply(a_mat1, a_mat2);
    }

    inline mat4 operator*(const mat4& a_m, const float& a_f)
    {
        return mat4::MultiplyNumber(a_m, a_f);
    }

    inline mat4 operator*(const float& a_f, const mat4& a_m)
    {
        return mat4::MultiplyNumber(a_m, a_f);
    }

    inline vec3 operator*(const mat4& a_m, const vec3& a_v)
    {
        vec3 t_vR;
        t_vR.x = (a_m.mElements[0] * a_v.x) + (a_m.mElements[1] * a_v.y) + (a_m.mElements[2] * a_v.z) + a_m.mElements[
            3];
        t_vR.y = (a_m.mElements[4] * a_v.x) + (a_m.mElements[5] * a_v.y) + (a_m.mElements[6] * a_v.z) + a_m.mElements[
            7];
        t_vR.z = (a_m.mElements[8] * a_v.x) + (a_m.mElements[9] * a_v.y) + (a_m.mElements[10] * a_v.z) + a_m.mElements[
            11];
        return t_vR;
    }

    inline quat operator*(const mat4& a_m, const quat& a_q)
    {
        quat t_qR;
        t_qR.x = (a_m.mElements[0] * a_q.x) + (a_m.mElements[1] * a_q.y) + (a_m.mElements[2] * a_q.z) + (a_m.mElements[
            3] * a_q.w);
        t_qR.y = (a_m.mElements[4] * a_q.x) + (a_m.mElements[5] * a_q.y) + (a_m.mElements[6] * a_q.z) + (a_m.mElements[
            7] * a_q.w);
        t_qR.z = (a_m.mElements[8] * a_q.x) + (a_m.mElements[9] * a_q.y) + (a_m.mElements[10] * a_q.z) + (a_m.mElements[
            11] * a_q.w);
        t_qR.w = (a_m.mElements[12] * a_q.x) + (a_m.mElements[13] * a_q.y) + (a_m.mElements[14] * a_q.z) + (a_m.
            mElements[15] * a_q.w);
        return t_qR;
    }

    inline mat4 operator+=(const mat4& a_mat1, const mat4& a_mat2)
    {
        return a_mat1 + a_mat2;
    }

    inline mat4 operator-=(const mat4& a_mat1, const mat4& a_mat2)
    {
        return a_mat1 - a_mat2;
    }

    inline mat4 operator*=(const mat4& a_mat1, const mat4& a_mat2)
    {
        return a_mat1 * a_mat2;
    }

    inline mat4 operator*=(const mat4& a_m, const float& a_f)
    {
        return a_m * a_f;
    }

    inline mat4::mat4(const bool a_identity)
    {
        if (a_identity)
        {
            mElements = { 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
                         0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f };
            return;
        }
        mElements = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f,
                     0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
    }


    inline mat4::mat4(const std::array<float, 16>& a_elements)
    {
        if (a_elements.size() > 16)
            abort();

        mElements = a_elements;
    }

    inline float mat4::Trace() const
    {
        return mElements[0] + mElements[5] + mElements[10] + mElements[15];
    }

    inline float mat4::operator[](const int a_index) const
    {
        if (a_index < 0 || a_index > 15)
        {
            std::cout << "Error : overflow\n";
            return 0.f;
        }
        return mElements[a_index];
    }
} // namespace TheCoolerMath