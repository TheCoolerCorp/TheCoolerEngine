#include "mat4UT.h"

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/detail/type_quat.hpp"
#include "glm/gtx/quaternion.hpp"

using namespace Engine::Math;

namespace mat4UT
{
	/*------------------------------------------CONSTRUCTORS-----------------------------------------------------*/
	TEST(Matrix4, DefaultConstructor)
	{
		mat4 m1(false);
		mat4 m2(true);

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(m1[i], 0.f);
		}

		EXPECT_FLOAT_EQ(m2[0], 1.f);
		EXPECT_FLOAT_EQ(m2[1], 0.f);
		EXPECT_FLOAT_EQ(m2[2], 0.f);
		EXPECT_FLOAT_EQ(m2[3], 0.f);

		EXPECT_FLOAT_EQ(m2[4], 0.f);
		EXPECT_FLOAT_EQ(m2[5], 1.f);
		EXPECT_FLOAT_EQ(m2[6], 0.f);
		EXPECT_FLOAT_EQ(m2[7], 0.f);

		EXPECT_FLOAT_EQ(m2[8], 0.f);
		EXPECT_FLOAT_EQ(m2[9], 0.f);
		EXPECT_FLOAT_EQ(m2[10], 1.f);
		EXPECT_FLOAT_EQ(m2[11], 0.f);

		EXPECT_FLOAT_EQ(m2[12], 0.f);
		EXPECT_FLOAT_EQ(m2[13], 0.f);
		EXPECT_FLOAT_EQ(m2[14], 0.f);
		EXPECT_FLOAT_EQ(m2[15], 1.f);

	}

	TEST(Matrix4, BasicConstructor)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };
		mat4 m1(array);

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(m1[i], static_cast<float>(i));
		}
	}

	/*------------------------------------------FUNCTIONS-----------------------------------------------------*/
	TEST(Matrix4, Diagonal)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);

		quat diagonal = m1.Diagonal();

		EXPECT_FLOAT_EQ(m1[0], 0.f);
		EXPECT_FLOAT_EQ(m1[5], 5.f);
		EXPECT_FLOAT_EQ(m1[10], 10.f);
		EXPECT_FLOAT_EQ(m1[15], 15.f);
	}

	TEST(Matrix4, Trace)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);

		float trace = m1.Trace();

		EXPECT_FLOAT_EQ(trace, 30.f);
	}

	TEST(Matrix4, Opposite)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);
		m1.Opposite();

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(m1[i], -array[i]);
		}
	}

	TEST(Matrix4, Inverse)
	{
		std::array<float, 16> array = { 1.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 67.f, 7.f,
										8.f, 95.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 1.f };

		mat4 m1(array);
		m1.Inverse();

		glm::mat4 mg1 = glm::mat4({ 1.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 67.f, 7.f,
										8.f, 95.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 1.f });
		mg1 = glm::inverse(mg1);
		float array1[16];
		memcpy(array1, &mg1, 16 * sizeof(float));
		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(m1[i], array1[i]);
		}
	}

	TEST(Matrix4, Transpose)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);
		m1.Transpose();

		std::array<float, 16> array1 = { 0.f, 4.f, 8.f, 12.f,
										1.f, 5.f, 9.f, 13.f,
										2.f, 6.f, 10.f, 14.f,
										3.f, 7.f, 11.f, 15.f };

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(m1[i], array1[i]);
		}
	}

	TEST(Matrix4, Determinant)
	{
		std::array<float, 16> array = { 1.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);

		float det = m1.Determinant();

		glm::mat4 m1g = { 1.f, 1.f, 2.f, 3.f,
						  4.f, 5.f, 6.f, 7.f,
						  8.f, 9.f, 10.f, 11.f,
						  12.f, 13.f, 14.f, 15.f };

		float gDet = glm::determinant(m1g);

		EXPECT_FLOAT_EQ(det, gDet);
	}

	TEST(Matrix4, Add)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);
		mat4 m2(array);

		mat4 result = mat4::Add(m1, m2);

		glm::mat4 m1g = { 0.f, 1.f, 2.f, 3.f,
						  4.f, 5.f, 6.f, 7.f,
						  8.f, 9.f, 10.f, 11.f,
						  12.f, 13.f, 14.f, 15.f };

		glm::mat4 m2g = { 0.f, 1.f, 2.f, 3.f,
				  4.f, 5.f, 6.f, 7.f,
				  8.f, 9.f, 10.f, 11.f,
				  12.f, 13.f, 14.f, 15.f };

		glm::mat4 resultg = m1g + m2g;
		float array1[16];
		memcpy(array1, &resultg, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(result[i], array1[i]);
		}
	}

	TEST(Matrix4, MultiplyNumber)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);

		float value = 2.f;

		m1 = mat4::MultiplyNumber(m1, value);

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(m1[i], array[i] * value);
		}
	}

	TEST(Matrix4, Multiply)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);
		mat4 m2(array);

		mat4 result = mat4::Multiply(m1, m2);

		glm::mat4 m1g = { 0.f, 1.f, 2.f, 3.f,
						  4.f, 5.f, 6.f, 7.f,
						  8.f, 9.f, 10.f, 11.f,
						  12.f, 13.f, 14.f, 15.f };

		glm::mat4 m2g = { 0.f, 1.f, 2.f, 3.f,
				  4.f, 5.f, 6.f, 7.f,
				  8.f, 9.f, 10.f, 11.f,
				  12.f, 13.f, 14.f, 15.f };

		glm::mat4 resultg = m1g * m2g;
		float array1[16];
		memcpy(array1, &resultg, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(result[i], array1[i]);
		}
	}

	TEST(Matrix4, Translate)
	{
		mat4 translate = mat4::Translate(vec3(1.f, 2.f, 3.f));

		glm::mat4 identity = glm::mat4(1.f);
		glm::mat4 translateg = glm::translate(identity, glm::vec3(1.f, 2.f, 3.f));
		translateg = glm::transpose(translateg);
		float array1[16];
		memcpy(array1, &translateg, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(translate[i], array1[i]);
		}
	}

	TEST(Matrix4, Rotation)
	{
		quat q = quat(vec3(0.f, 0.f, 1.f));

		mat4 rotation = mat4::Rotation(q);

		glm::quat qg = glm::quat(glm::vec3(0.f, 0.f, 1.f));
		glm::mat4 rotationg = glm::toMat4(qg);
		rotationg = glm::transpose(rotationg);
		float array1[16];
		memcpy(array1, &rotationg, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_NEAR(rotation[i], array1[i], 1e-5);
		}
	}

	TEST(Matrix4, Scale)
	{
		mat4 scale = mat4::Scale(vec3(2.f, 1.f, 46.3f));

		glm::mat4 identity = glm::mat4(1.f);
		glm::mat4 scaleg = glm::scale(identity, glm::vec3(2.f, 1.f, 46.3f));
		scaleg = glm::transpose(scaleg);
		float array1[16];
		memcpy(array1, &scaleg, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(scale[i], array1[i]);
		}
	}

	TEST(Matrix4, TRS)
	{
		vec3 translate = vec3(1.f, 2.f, 3.f);
		quat rotation = quat(vec3(0.f, 0.f, 1.f));
		vec3 scale = vec3(2.f, 1.f, 46.3f);

		glm::mat4 identity = glm::mat4(1.f);
		glm::mat4 translateg = glm::translate(identity, glm::vec3(1.f, 2.f, 3.f));
		glm::quat qg = glm::quat(glm::vec3(0.f, 0.f, 1.f));
		glm::mat4 rotationg = glm::toMat4(qg);
		glm::mat4 scaleg = glm::scale(identity, glm::vec3(2.f, 1.f, 46.3f));

		mat4 TRS = mat4::TRS(translate, rotation, scale);
		glm::mat4 TRSg = translateg * rotationg * scaleg;
		TRSg = glm::transpose(TRSg);
		float array1[16];
		memcpy(array1, &TRSg, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_NEAR(TRS[i], array1[i], 1e-5);
		}
	}

	TEST(Matrix4, View)
	{
		mat4 view = mat4::View(vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, 0.f), vec3(0.f, 0.f, -3.f));

		glm::mat4 viewg = glm::lookAt(glm::vec3(0.f, 0.f, -3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		viewg = glm::transpose(viewg);
		float array1[16];
		memcpy(array1, &viewg, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(view[i], array1[i]);
		}
	}

	TEST(Matrix4, Perspective)
	{
		mat4 perspective = mat4::Perspective(45.f, 16.f / 9.f, 0.1f, 100.f);

		glm::mat4 perspectiveg = glm::perspective(45.f, 16.f / 9.f, 0.1f, 100.f);
		perspectiveg = glm::transpose(perspectiveg);
		float array1[16];
		memcpy(array1, &perspectiveg, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(perspective[i], array1[i]);
		}
	}

	TEST(Matrix4, Orthographic)
	{
		mat4 ortho = mat4::Orthographic(4.5f, -4.5f, 8.f, -8.f, 100.f, 0.1f);

		glm::mat4 orthog = glm::ortho(-8.f, 8.f, -4.5f, 4.5f, 0.1f, 100.f);
		orthog = glm::transpose(orthog);
		float array1[16];
		memcpy(array1, &orthog, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(ortho[i], array1[i]);
		}
	}

	TEST(Matrix4, Matrix3Determinant)
	{
		float det = Mat3Determinant({ 0.f, 1.f, 2.f,
										3.f, 4.f, 5.f,
										6.f, 7.f, 8.f });

		float detg = glm::determinant(glm::mat3({ 0.f, 1.f, 2.f,
										3.f, 4.f, 5.f,
										6.f, 7.f, 8.f }));

		EXPECT_FLOAT_EQ(det, detg);

	}

	///*------------------------------------------Operators-----------------------------------------------------*/
	TEST(Matrix4, OperatorBrackets)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(m1[i], m1.mElements[i]);
		}
	}

	TEST(Matrix4, OperatorAdd)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);
		mat4 m2(array);
		mat4 m3(array);
		mat4 m4(array);

		mat4 result = mat4::Add(m1, m2);
		mat4 result1 = m3 + m4;

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(result1[i], result[i]);
		}
	}

	TEST(Matrix4, OperatorSubtract)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);
		mat4 m2(array);
		mat4 m3(array);
		mat4 m4(array);

		m2.Opposite();

		mat4 result = mat4::Add(m1, m2);
		mat4 result1 = m3 - m4;

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(result1[i], result[i]);
		}
	}

	TEST(Matrix4, OperatorMultiply)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);
		mat4 m2(array);
		mat4 m3(array);
		mat4 m4(array);

		mat4 result = mat4::Multiply(m1, m2);
		mat4 result1 = m3 * m4;

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(result1[i], result[i]);
		}
	}

	TEST(Matrix4, OperatorMultiplyNumber)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);
		mat4 m2(array);
		float nb = 5;

		mat4 result = mat4::MultiplyNumber(m1, nb);
		mat4 result1 = m2 * nb;

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(result1[i], result[i]);
		}

		mat4 result2 = nb * m2;
		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(result2[i], result[i]);
		}
	}

	TEST(Matrix4, OperatorMultiplyVector)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);
		vec3 v1 = vec3(56.f, 3.f, 12.f);
		vec3 v2 = m1 * v1;

		glm::mat4 mg1 = glm::mat4({ 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f });
		mg1 = glm::transpose(mg1);
		glm::vec4 vg1 = glm::vec4(56.f, 3.f, 12.f, 1.f);
		glm::vec4 vg2 = mg1 * vg1;

		EXPECT_FLOAT_EQ(v2.x, vg2.x);
		EXPECT_FLOAT_EQ(v2.y, vg2.y);
		EXPECT_FLOAT_EQ(v2.z, vg2.z);
	}

	TEST(Matrix4, OperatorMultiplyQuaternion)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);
		quat q1 = quat(56.f, 3.f, 12.f, 1.f);
		quat q2 = m1 * q1;

		glm::mat4 mg1 = glm::mat4({ 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f });
		mg1 = glm::transpose(mg1);
		glm::vec4 vg1 = glm::vec4(56.f, 3.f, 12.f, 1.f);
		glm::vec4 vg2 = mg1 * vg1;

		EXPECT_FLOAT_EQ(q2.x, vg2.x);
		EXPECT_FLOAT_EQ(q2.y, vg2.y);
		EXPECT_FLOAT_EQ(q2.z, vg2.z);
		EXPECT_FLOAT_EQ(q2.w, vg2.w);
	}

	TEST(Matrix4, OperatorEqualAdd)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);
		mat4 m2(array);
		mat4 m3 = m1 + m2;
		mat4 m4(array);

		m4 += m2;

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(m3[i], m4[i]);
		}
	}

	TEST(Matrix4, OperatorEqualSubtract)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);
		mat4 m2(array);
		mat4 m3 = m1 - m2;
		mat4 m4(array);

		m4 -= m2;

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(m3[i], m4[i]);
		}
	}

	TEST(Matrix4, OperatorEqualMultiply)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);
		mat4 m2(array);
		mat4 m3 = m1 * m2;
		mat4 m4(array);

		m4 *= m2;

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(m3[i], m4[i]);
		}
	}

	TEST(Matrix4, OperatorEqualMultiplyNumber)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		mat4 m1(array);
		float nb = 89.f;
		mat4 m3 = m1 * nb;
		mat4 m4(array);

		m4 *= nb;

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_FLOAT_EQ(m3[i], m4[i]);
		}
	}
}