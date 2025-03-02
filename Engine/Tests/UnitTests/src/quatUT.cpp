#include "quatUT.h"

#include "glm/detail/type_quat.hpp"
#include "Math/TheCoolerMath.h"

using namespace TheCoolerMath;

namespace quatUT
{
	TEST(Quaternion, Constructors)
	{
		quat q1;
		EXPECT_FLOAT_EQ(q1.x, 0.f);
		EXPECT_FLOAT_EQ(q1.y, 0.f);
		EXPECT_FLOAT_EQ(q1.z, 0.f);
		EXPECT_FLOAT_EQ(q1.w, 1.f);

		quat q2 = quat(1.f, 2.f, 3.f, 4.f);
		EXPECT_FLOAT_EQ(q2.x, 1.f);
		EXPECT_FLOAT_EQ(q2.y, 2.f);
		EXPECT_FLOAT_EQ(q2.z, 3.f);
		EXPECT_FLOAT_EQ(q2.w, 4.f);

		quat q3 = quat(10.f);
		EXPECT_FLOAT_EQ(q3.x, 10.f);
		EXPECT_FLOAT_EQ(q3.y, 10.f);
		EXPECT_FLOAT_EQ(q3.z, 10.f);
		EXPECT_FLOAT_EQ(q3.w, 10.f);

		glm::vec3 glmEulerAngles = glm::vec3(1.23f, 3.10f, 2.86f);
		glm::quat qTest1 = glm::quat(glmEulerAngles);
		vec3 eulerAngles = vec3(1.23f, 3.10f, 2.86f);
		quat q4 = quat(eulerAngles);
		EXPECT_NEAR(q4.x, qTest1.x, 0.0001f);
		EXPECT_NEAR(q4.y, qTest1.y, 0.0001f);
		EXPECT_NEAR(q4.z, qTest1.z, 0.0001f);
		EXPECT_NEAR(q4.w, qTest1.w, 0.0001f);
	}

	TEST(Quaternion, Methods)
	{
		quat q1 = quat(1.f, 2.f, 3.f, 4.f);
		q1.Invert();
		EXPECT_FLOAT_EQ(q1.x, 1.f);
		EXPECT_FLOAT_EQ(q1.y, 1.f / 2.f);
		EXPECT_FLOAT_EQ(q1.z, 1.f / 3.f);
		EXPECT_FLOAT_EQ(q1.w, 1.f / 4.f);

		quat q2 = quat(5.f, 89.f, 41.f, 3.f);
		q2.AddNumber(5.f);
		EXPECT_FLOAT_EQ(q2.x, 5.f + 5.f);
		EXPECT_FLOAT_EQ(q2.y, 89.f + 5.f);
		EXPECT_FLOAT_EQ(q2.z, 41.f + 5.f);
		EXPECT_FLOAT_EQ(q2.w, 3.f + 5.f);

		quat q3 = quat(15.f, 22.f, 57.f, 39.f);
		q3.MultiplyNumber(3.5f);
		EXPECT_FLOAT_EQ(q3.x, 15.f * 3.5f);
		EXPECT_FLOAT_EQ(q3.y, 22.f * 3.5f);
		EXPECT_FLOAT_EQ(q3.z, 57.f * 3.5f);
		EXPECT_FLOAT_EQ(q3.w, 39.f * 3.5f);

		EXPECT_FLOAT_EQ(q3.y, q3[1]);

		quat q4 = quat(vec3(2.f, 1.5f, 1.f));
		vec3 eulerAngles = quat::ToEulerAngles(q4);
		EXPECT_NEAR(eulerAngles.x, 2.f, 0.0001f);
		EXPECT_NEAR(eulerAngles.y, 1.5f, 0.0001f);
		EXPECT_NEAR(eulerAngles.z, 1.f, 0.0001f);

		quat q5 = quat(45.f, 1128.f, 2.f, 89.f);
		quat conjQ5 = quat::Conjugate(q5);
		EXPECT_FLOAT_EQ(conjQ5.x, -q5.x);
		EXPECT_FLOAT_EQ(conjQ5.y, -q5.y);
		EXPECT_FLOAT_EQ(conjQ5.z, -q5.z);
		EXPECT_FLOAT_EQ(conjQ5.w, q5.w);

		quat q6 = quat(26.f, 486.f, 87.f, 1.f);
		quat q7 = quat(3.f, 78.f, 123.f, 69.f);
		quat q8 = quat::Add(q6, q7);
		EXPECT_FLOAT_EQ(q8.x, q6.x + q7.x);
		EXPECT_FLOAT_EQ(q8.y, q6.y + q7.y);
		EXPECT_FLOAT_EQ(q8.z, q6.z + q7.z);
		EXPECT_FLOAT_EQ(q8.w, q6.w + q7.w);

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

		float sqrNormQ12 = quat::SquaredNorm(q12);
		EXPECT_FLOAT_EQ(sqrNormQ12, q12.x * q12.x + q12.y * q12.y + q12.z * q12.z + q12.w * q12.w);

		float normQ12 = quat::Norm(q12);
		EXPECT_FLOAT_EQ(normQ12, sqrtf(sqrNormQ12));

		quat q13 = quat::Normalize(q12);
		glm::quat testQ7 = glm::normalize(testQ4);
		EXPECT_FLOAT_EQ(q13.x, testQ7.x);
		EXPECT_FLOAT_EQ(q13.y, testQ7.y);
		EXPECT_FLOAT_EQ(q13.z, testQ7.z);
		EXPECT_FLOAT_EQ(q13.w, testQ7.w);

		float dotProduct = quat::DotProduct(q12, q13);
		EXPECT_FLOAT_EQ(dotProduct, q12.x * q13.x + q12.y * q13.y + q12.z * q13.z + q12.w * q13.w);
	}

	TEST(Quaternion, Operators)
	{
		
	}
}