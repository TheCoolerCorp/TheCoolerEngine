#include "quatUT.h"

#include "glm/detail/type_quat.hpp"
#include "Math/TheCoolerMath.h"

using namespace TheCoolerMath;

namespace quatUT
{
	/*------------------------------------------CONSTRUCTORS-----------------------------------------------------*/
	TEST(Quaternion, DefaultConstructor)
	{
		quat q1;
		EXPECT_FLOAT_EQ(q1.x, 0.f);
		EXPECT_FLOAT_EQ(q1.y, 0.f);
		EXPECT_FLOAT_EQ(q1.z, 0.f);
		EXPECT_FLOAT_EQ(q1.w, 1.f);
	}

	TEST(Quaternion, BasicConstructor)
	{
		quat q2 = quat(1.f, 2.f, 3.f, 4.f);
		EXPECT_FLOAT_EQ(q2.x, 1.f);
		EXPECT_FLOAT_EQ(q2.y, 2.f);
		EXPECT_FLOAT_EQ(q2.z, 3.f);
		EXPECT_FLOAT_EQ(q2.w, 4.f);
	}

	TEST(Quaternion, SingleValueConstructor)
	{
		quat q3 = quat(10.f);
		EXPECT_FLOAT_EQ(q3.x, 10.f);
		EXPECT_FLOAT_EQ(q3.y, 10.f);
		EXPECT_FLOAT_EQ(q3.z, 10.f);
		EXPECT_FLOAT_EQ(q3.w, 10.f);
	}

	TEST(Quaternion, EulerAnglesConstructor)
	{
		glm::vec3 glmEulerAngles = glm::vec3(1.23f, 3.10f, 2.86f);
		glm::quat qTest1 = glm::quat(glmEulerAngles);
		vec3 eulerAngles = vec3(1.23f, 3.10f, 2.86f);
		quat q4 = quat(eulerAngles);
		EXPECT_NEAR(q4.x, qTest1.x, 0.0001f);
		EXPECT_NEAR(q4.y, qTest1.y, 0.0001f);
		EXPECT_NEAR(q4.z, qTest1.z, 0.0001f);
		EXPECT_NEAR(q4.w, qTest1.w, 0.0001f);
	}

	/*------------------------------------------Functions-----------------------------------------------------*/
	TEST(Quaternion, Invert)
	{
		quat q1 = quat(1.f, 2.f, 3.f, 4.f);
		q1.Invert();
		EXPECT_FLOAT_EQ(q1.x, 1.f);
		EXPECT_FLOAT_EQ(q1.y, 1.f / 2.f);
		EXPECT_FLOAT_EQ(q1.z, 1.f / 3.f);
		EXPECT_FLOAT_EQ(q1.w, 1.f / 4.f);
	}

	TEST(Quaternion, AddNumber)
	{
		quat q2 = quat(5.f, 89.f, 41.f, 3.f);
		q2.AddNumber(5.f);
		EXPECT_FLOAT_EQ(q2.x, 5.f + 5.f);
		EXPECT_FLOAT_EQ(q2.y, 89.f + 5.f);
		EXPECT_FLOAT_EQ(q2.z, 41.f + 5.f);
		EXPECT_FLOAT_EQ(q2.w, 3.f + 5.f);
	}

	TEST(Quaternion, MultiplyNumber)
	{
		quat q3 = quat(15.f, 22.f, 57.f, 39.f);
		q3.MultiplyNumber(3.5f);
		EXPECT_FLOAT_EQ(q3.x, 15.f * 3.5f);
		EXPECT_FLOAT_EQ(q3.y, 22.f * 3.5f);
		EXPECT_FLOAT_EQ(q3.z, 57.f * 3.5f);
		EXPECT_FLOAT_EQ(q3.w, 39.f * 3.5f);
	}

	TEST(Quaternion, ToEulerAngles)
	{
		quat q4 = quat(vec3(2.f, 1.5f, 1.f));
		vec3 eulerAngles = quat::ToEulerAngles(q4);
		EXPECT_NEAR(eulerAngles.x, 2.f, 0.0001f);
		EXPECT_NEAR(eulerAngles.y, 1.5f, 0.0001f);
		EXPECT_NEAR(eulerAngles.z, 1.f, 0.0001f);
	}

	TEST(Quaternion, Conjugate)
	{
		quat q5 = quat(45.f, 1128.f, 2.f, 89.f);
		quat conjQ5 = quat::Conjugate(q5);
		EXPECT_FLOAT_EQ(conjQ5.x, -q5.x);
		EXPECT_FLOAT_EQ(conjQ5.y, -q5.y);
		EXPECT_FLOAT_EQ(conjQ5.z, -q5.z);
		EXPECT_FLOAT_EQ(conjQ5.w, q5.w);
	}

	TEST(Quaternion, Add)
	{
		quat q6 = quat(26.f, 486.f, 87.f, 1.f);
		quat q7 = quat(3.f, 78.f, 123.f, 69.f);
		quat q8 = quat::Add(q6, q7);
		EXPECT_FLOAT_EQ(q8.x, q6.x + q7.x);
		EXPECT_FLOAT_EQ(q8.y, q6.y + q7.y);
		EXPECT_FLOAT_EQ(q8.z, q6.z + q7.z);
		EXPECT_FLOAT_EQ(q8.w, q6.w + q7.w);
	}

	TEST(Quaternion, MultiplyQuat)
	{
		glm::quat testQ1 = glm::quat(48.f, 59.f, 4156.f, 165.f);
		glm::quat testQ2 = glm::quat(32.f, 4561.f, 6.f, 54896.f);
		glm::quat testQ3 = testQ1 * testQ2;
		quat q9 = quat(59.f, 4156.f, 165.f, 48.f);
		quat q10 = quat(4561.f, 6.f, 54896.f, 32.f);
		quat q11 = quat::Multiply(q9, q10);
		EXPECT_FLOAT_EQ(q11.x, testQ3.x);
		EXPECT_FLOAT_EQ(q11.y, testQ3.y);
		EXPECT_FLOAT_EQ(q11.z, testQ3.z);
		EXPECT_FLOAT_EQ(q11.w, testQ3.w);
	}

	TEST(Quaternion, MultiplyVector)
	{
		glm::quat testQV1 = glm::quat(0.f, 5.f, 69.f, 999.f);
		glm::quat testQ4 = glm::quat(78.f, 23.f, 154.f, 2.f);
		glm::quat testQ5 = glm::quat(78.f, -23.f, -154.f, -2.f);
		glm::quat testQ6 = testQ4 * testQV1 * testQ5;
		vec3 v1 = vec3(5.f, 69.f, 999.f);
		quat q12 = quat(23.f, 154.f, 2.f, 78.f);
		vec3 v2 = quat::Multiply(q12, v1);
		EXPECT_FLOAT_EQ(v2.x, testQ6.x);
		EXPECT_FLOAT_EQ(v2.y, testQ6.y);
		EXPECT_FLOAT_EQ(v2.z, testQ6.z);
	}

	TEST(Quaternion, SquaredNorm)
	{
		quat q12 = quat(23.f, 154.f, 2.f, 78.f);
		float sqrNormQ12 = quat::SquaredNorm(q12);
		EXPECT_FLOAT_EQ(sqrNormQ12, q12.x * q12.x + q12.y * q12.y + q12.z * q12.z + q12.w * q12.w);
	}

	TEST(Quaternion, Norm)
	{
		quat q12 = quat(23.f, 154.f, 2.f, 78.f);
		float sqrNormQ12 = quat::SquaredNorm(q12);
		float normQ12 = quat::Norm(q12);
		EXPECT_FLOAT_EQ(normQ12, sqrtf(sqrNormQ12));
	}

	TEST(Quaternion, Normalize)
	{
		quat q12 = quat(23.f, 154.f, 2.f, 78.f);
		glm::quat testQ4 = glm::quat(78.f, 23.f, 154.f, 2.f);
		quat q13 = quat::Normalize(q12);
		glm::quat testQ7 = glm::normalize(testQ4);
		EXPECT_FLOAT_EQ(q13.x, testQ7.x);
		EXPECT_FLOAT_EQ(q13.y, testQ7.y);
		EXPECT_FLOAT_EQ(q13.z, testQ7.z);
		EXPECT_FLOAT_EQ(q13.w, testQ7.w);
		EXPECT_TRUE(quat::Norm(q13) == 1.f);
	}

	TEST(Quaternion, DotProduct)
	{
		quat q12 = quat(23.f, 154.f, 2.f, 78.f);
		quat q13 = quat::Normalize(q12);
		float dotProduct = quat::DotProduct(q12, q13);
		EXPECT_FLOAT_EQ(dotProduct, q12.x * q13.x + q12.y * q13.y + q12.z * q13.z + q12.w * q13.w);
	}

	/*------------------------------------------Operators-----------------------------------------------------*/
	TEST(Quaternion, OperatorBrackets)
	{
		quat q = quat(15.f, 22.f, 57.f, 39.f);
		EXPECT_FLOAT_EQ(q.x, q[0]);
		EXPECT_FLOAT_EQ(q.y, q[1]);
		EXPECT_FLOAT_EQ(q.z, q[2]);
		EXPECT_FLOAT_EQ(q.w, q[3]);
	}

	TEST(Quaternion, OperatorEqual)
	{
		quat q1 = quat(5.f, 89.f, 45.f, 3.f);
		quat q2 = quat(5.f, 89.f, 45.f, 3.f);
		quat q3 = quat(4.f, 89.f, 45.f, 3.f);
		EXPECT_FALSE(q1 == q3);
		EXPECT_TRUE(q1 == q2);
	}

	TEST(Quaternion, OperatorAddNumber)
	{
		quat q1 = quat(65.f, 47.f, 52.f, 1.f);
		float nb = 5.f;
		quat q = q1;
		q.AddNumber(nb);
		quat q2 = q1 + nb;
		EXPECT_TRUE(q2 == q);

		quat q3 = nb + q1;
		EXPECT_TRUE(q3 == q);
	}

	TEST(Quaternion, OperatorSubtractNumber)
	{
		quat q1 = quat(56.f, 2.f, 789.f, 12.f);
		float nb = 6.f;
		quat q = q1;
		q.AddNumber(-nb);
		quat q2 = q1 - nb;
		EXPECT_TRUE(q2 == q);
	}

	TEST(Quaternion, OperatorMultiplyNumber)
	{
		quat q1 = quat(4615.f, 541.f, 52.f, 4.f);
		float nb = 45.f;
		quat q = q1;
		q.MultiplyNumber(nb);
		quat q2 = q1 * nb;
		EXPECT_TRUE(q2 == q);

		quat q3 = nb * q1;
		EXPECT_TRUE(q3 == q);
	}

	TEST(Quaternion, OperatorDivideNumber)
	{
		quat q1 = quat(78.f, 41.f, 152.f, 46.f);
		float nb = 3.f;
		quat q = q1;
		q.MultiplyNumber(1.f / nb);
		quat q2 = q1 / nb;
		EXPECT_TRUE(q2 == q);
	}

	TEST(Quaternion, OperatorMultiplyVector)
	{
		quat q = quat(5.f, 456.f, 845613.f, 1.f);
		vec3 v1 = vec3(12.f, 120.f, 56.f);
		vec3 v2 = q * v1;
		vec3 v3 = quat::Multiply(q, v1);
		EXPECT_FLOAT_EQ(v2.x, v3.x);
		EXPECT_FLOAT_EQ(v2.y, v3.y);
		EXPECT_FLOAT_EQ(v2.z, v3.z);
	}

	TEST(Quaternion, OperatorAdd)
	{
		quat q1 = ::quat(78.f, 23.f, 154.f, 2.f);
		quat q2 = quat(5.f, 89.f, 41.f, 3.f);
		quat q3 = q1 + q2;
		quat q4 = quat::Add(q1, q2);
		EXPECT_TRUE(q3 == q4);
	}

	TEST(Quaternion, OperatorSubtract)
	{
		quat q1 = ::quat(78.f, 23.f, 154.f, 2.f);
		quat q2 = quat(5.f, 89.f, 41.f, 3.f);
		quat q3 = q1 - q2;
		quat q4 = quat::Add(q1, q2 * -1.f);
		EXPECT_TRUE(q3 == q4);
	}

	TEST(Quaternion, OperatorMultiply)
	{
		quat q1 = ::quat(78.f, 23.f, 154.f, 2.f);
		quat q2 = quat(5.f, 89.f, 41.f, 3.f);
		quat q3 = q1 * q2;
		quat q4 = quat::Multiply(q1, q2);
		EXPECT_TRUE(q3 == q4);
	}

	TEST(Quaternion, OperatorDivide)
	{
		quat q1 = ::quat(78.f, 23.f, 154.f, 2.f);
		quat q2 = quat(5.f, 89.f, 41.f, 3.f);
		quat q3 = q1 / q2;
		q2.Invert();
		quat q4 = quat::Multiply(q1, q2);
		EXPECT_TRUE(q3 == q4);
	}

	TEST(Quaternion, OperatorEqualAddNumber)
	{
		quat q1 = quat(32.f, 4156.f, 45.f, 8.f);
		quat q3 = quat(32.f, 4156.f, 45.f, 8.f);
		float nb = 8.f;
		q1 += nb;
		q3.AddNumber(nb);
		EXPECT_TRUE(q1 == q3);
	}

	TEST(Quaternion, OperatorEqualSubtractNumber)
	{
		quat q1 = quat(32.f, 4156.f, 45.f, 8.f);
		quat q3 = quat(32.f, 4156.f, 45.f, 8.f);
		float nb = 8.f;
		q1 -= nb;
		q3.AddNumber(-nb);
		EXPECT_TRUE(q1 == q3);
	}

	TEST(Quaternion, OperatorEqualMultiplyNumber)
	{
		quat q1 = quat(32.f, 416.f, 459.f, 8.f);
		quat q3 = quat(32.f, 416.f, 459.f, 8.f);
		float nb = 65.f;
		q1 *= nb;
		q3.MultiplyNumber(nb);
		EXPECT_TRUE(q1 == q3);
	}

	TEST(Quaternion, OperatorEqualDivideNumber)
	{
		quat q1 = quat(302.f, 41.f, 459.f, 87.f);
		quat q3 = quat(302.f, 41.f, 459.f, 87.f);
		float nb = 5.f;
		q1 /= nb;
		q3.MultiplyNumber(1.f / nb);
		EXPECT_TRUE(q1 == q3);
	}

	TEST(Quaternion, OperatorEqualAdd)
	{
		quat q1 = quat(56.f, 2.f, 789.f, 12.f);
		quat q2 = quat(5.f, 89.f, 41.f, 3.f);
		quat q3 = quat::Add(q1, q2);
		q1 += q2;
		EXPECT_TRUE(q1 == q3);
	}

	TEST(Quaternion, OperatorEqualSubtract)
	{
		quat q1 = quat(56.f, 2.f, 789.f, 12.f);
		quat q2 = quat(5.f, 89.f, 41.f, 3.f);
		quat q3 = quat::Add(q1, q2 * -1.f);
		q1 -= q2;
		EXPECT_TRUE(q1 == q3);
	}

	TEST(Quaternion, OperatorEqualMultiply)
	{
		quat q1 = quat(56.f, 2.f, 789.f, 12.f);
		quat q2 = quat(5.f, 89.f, 41.f, 3.f);
		quat q3 = quat::Multiply(q1, q2);
		q1 *= q2;
		EXPECT_TRUE(q1 == q3);
	}

	TEST(Quaternion, OperatorEqualDivide)
	{
		quat q1 = quat(56.f, 2.f, 789.f, 12.f);
		quat q2 = quat(5.f, 89.f, 41.f, 3.f);
		quat q3 = q2;
		q2.Invert();
		quat q4 = quat::Multiply(q1, q2);
		q1 /= q3;
		EXPECT_TRUE(q1 == q4);
	}
}