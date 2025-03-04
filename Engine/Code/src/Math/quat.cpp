#include "Math/quat.h"
#include "Math/TheCoolerMath.h"

namespace Math
{
    quat::quat(const vec3 a_eulerAngles)
    {
        const float t_cosX = cosf(a_eulerAngles.x * 0.5f);
        const float t_sinX = sinf(a_eulerAngles.x * 0.5f);
        const float t_cosY = cosf(a_eulerAngles.y * 0.5f);
        const float t_sinY = sinf(a_eulerAngles.y * 0.5f);
        const float t_cosZ = cosf(a_eulerAngles.z * 0.5f);
        const float t_sinZ = sinf(a_eulerAngles.z * 0.5f);

        x = t_sinX * t_cosY * t_cosZ - t_cosX * t_sinY * t_sinZ;
        y = t_cosX * t_sinY * t_cosZ + t_sinX * t_cosY * t_sinZ;
        z = t_cosX * t_cosY * t_sinZ - t_sinX * t_sinY * t_cosZ;
        w = t_cosX * t_cosY * t_cosZ + t_sinX * t_sinY * t_sinZ;
    }

    float quat::operator[](const int a_index) const
    {
        if (a_index < 0 || a_index > 3)
        {
            std::cout << "Error : overflow" << '\n';
            return 0.f;
        }

        if (a_index == 0)
        {
            return x;
        }
        if (a_index == 1)
        {
            return y;
        }
        if (a_index == 2)
        {
            return z;
        }
        if (a_index == 3)
        {
            return w;
        }
        return 0.f;
    }

    vec3 quat::ToEulerAngles(const quat a_q)
    {
        vec3 t_eulerAngles;
        const quat t_q = Normalize(a_q);

        const float t_sinXCosY = 2.f * (t_q.w * t_q.x + t_q.y * t_q.z);
        const float t_cosXCosY = 1.f - 2.f * (t_q.x * t_q.x + t_q.y * t_q.y);
        t_eulerAngles.x = atan2f(t_sinXCosY, t_cosXCosY);

        const float t_sinY = 2.f * (t_q.w * t_q.y - t_q.x * t_q.z);
        t_eulerAngles.y = fabs(t_sinY) >= 1.f ? copysignf(PI / 2.f, t_sinY) : asinf(t_sinY);

        const float t_sinZCosY = 2.f * (t_q.w * t_q.z + t_q.x * t_q.y);
        const float t_cosZCosY = 1.f - 2.f * (t_q.y * t_q.y + t_q.z * t_q.z);
        t_eulerAngles.z = atan2f(t_sinZCosY, t_cosZCosY);

        return t_eulerAngles;
    }

    quat quat::Conjugate(const quat& a_q)
    {
        quat t_q = a_q;
        t_q.x *= -1.f;
        t_q.y *= -1.f;
        t_q.z *= -1.f;
        return t_q;
    }

    quat quat::Multiply(const quat a_q1, const quat a_q2)
    {
        const float t_x = a_q1.w * a_q2.x + a_q1.x * a_q2.w + a_q1.y * a_q2.z - a_q1.z * a_q2.y;
        const float t_y = a_q1.w * a_q2.y - a_q1.x * a_q2.z + a_q1.y * a_q2.w + a_q1.z * a_q2.x;
        const float t_z = a_q1.w * a_q2.z + a_q1.x * a_q2.y - a_q1.y * a_q2.x + a_q1.z * a_q2.w;
        const float t_w = a_q1.w * a_q2.w - a_q1.x * a_q2.x - a_q1.y * a_q2.y - a_q1.z * a_q2.z;
        return quat(t_x, t_y, t_z, t_w);
    }

    vec3 quat::Multiply(quat a_q, vec3 a_v)
    {
        quat t_quatVec(a_v.x, a_v.y, a_v.z, 0.f);
        quat t_conjQ = Conjugate(a_q);
        quat t_rot = (a_q * t_quatVec) * t_conjQ;
        return { t_rot.x, t_rot.y, t_rot.z };
    }

    quat quat::Slerp(quat a_q1, quat a_q2, float a_t)
    {
        quat t_qT;

        float t_cosHalfTheta = a_q1.x * a_q2.x + a_q1.y * a_q2.y + a_q1.z * a_q2.z + a_q1.w * a_q2.w;

        quat a_q2Adjusted = a_q2;
        if (t_cosHalfTheta < 0.f) {
            a_q2Adjusted.x = -a_q2.x;
            a_q2Adjusted.y = -a_q2.y;
            a_q2Adjusted.z = -a_q2.z;
            a_q2Adjusted.w = -a_q2.w;
            t_cosHalfTheta = -t_cosHalfTheta;
        }

        if (t_cosHalfTheta > 0.9995f) {
            t_qT.x = a_q1.x + a_t * (a_q2Adjusted.x - a_q1.x);
            t_qT.y = a_q1.y + a_t * (a_q2Adjusted.y - a_q1.y);
            t_qT.z = a_q1.z + a_t * (a_q2Adjusted.z - a_q1.z);
            t_qT.w = a_q1.w + a_t * (a_q2Adjusted.w - a_q1.w);

            t_qT = Normalize(t_qT);
            return t_qT;
        }

        const float t_halfTheta = acosf(t_cosHalfTheta);
        const float t_sinHalfTheta = sqrtf(1.f - t_cosHalfTheta * t_cosHalfTheta);

        const float t_ratioA = sinf((1.f - a_t) * t_halfTheta) / t_sinHalfTheta;
        const float t_ratioB = sinf(a_t * t_halfTheta) / t_sinHalfTheta;

        t_qT.x = a_q1.x * t_ratioB + a_q2Adjusted.x * t_ratioA;
        t_qT.y = a_q1.y * t_ratioB + a_q2Adjusted.y * t_ratioA;
        t_qT.z = a_q1.z * t_ratioB + a_q2Adjusted.z * t_ratioA;
        t_qT.w = a_q1.w * t_ratioB + a_q2Adjusted.w * t_ratioA;

        t_qT = Normalize(t_qT);
        return t_qT;
    }

    float quat::SquaredNorm(const quat a_q)
    {
        const float t_xSquared = powf(a_q.x, 2.f);
        const float t_ySquared = powf(a_q.y, 2.f);
        const float t_zSquared = powf(a_q.z, 2.f);
        const float t_wSquared = powf(a_q.w, 2.f);
        const float t_squaredNorm = t_xSquared + t_ySquared + t_zSquared + t_wSquared;

        return t_squaredNorm;
    }

    float quat::Norm(const quat a_q)
    {
        return sqrtf(SquaredNorm(a_q));
    }

    quat quat::Normalize(const quat a_q)
    {
        const float t_norm = Norm(a_q);
        return { a_q.x / t_norm, a_q.y / t_norm, a_q.z / t_norm, a_q.w / t_norm };
    }

    float quat::DotProduct(const quat a_q1, const quat a_q2)
    {
        return (a_q1.x * a_q2.x) + (a_q1.y * a_q2.y) + (a_q1.z * a_q2.z) + (a_q1.w * a_q2.w);
    }
} // namespace Math
