#include "mat4UT.h"

namespace mat4UT
{
	/*------------------------------------------CONSTRUCTORS-----------------------------------------------------*/
	TEST(Matrix4, DefaultConstructor)
	{
		TheCoolerMath::mat4 m1(false);
		TheCoolerMath::mat4 m2(true);

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_EQ(m1[i], 0.f);
		}

		EXPECT_EQ(m2[0], 1.f);
		EXPECT_EQ(m2[1], 0.f);
		EXPECT_EQ(m2[2], 0.f);
		EXPECT_EQ(m2[3], 0.f);

		EXPECT_EQ(m2[4], 0.f);
		EXPECT_EQ(m2[5], 1.f);
		EXPECT_EQ(m2[6], 0.f);
		EXPECT_EQ(m2[7], 0.f);

		EXPECT_EQ(m2[8], 0.f);
		EXPECT_EQ(m2[9], 0.f);
		EXPECT_EQ(m2[10], 1.f);
		EXPECT_EQ(m2[11], 0.f);

		EXPECT_EQ(m2[12], 0.f);
		EXPECT_EQ(m2[13], 0.f);
		EXPECT_EQ(m2[14], 0.f);
		EXPECT_EQ(m2[15], 1.f);

	}

	TEST(Matrix4, BasicConstructor)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };
		TheCoolerMath::mat4 m1(array);

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_EQ(m1[i], (float)i);
		}
	}

	/*------------------------------------------FUNCTIONS-----------------------------------------------------*/
	TEST(Matrix4, Diagonal)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f, 
										4.f, 5.f, 6.f, 7.f, 
										8.f, 9.f, 10.f, 11.f, 
										12.f, 13.f, 14.f, 15.f };

		TheCoolerMath::mat4 m1(array);

		TheCoolerMath::quat diagonal = m1.Diagonal();

		EXPECT_EQ(m1[0], 0.f);
		EXPECT_EQ(m1[5], 5.f);
		EXPECT_EQ(m1[10], 10.f);
		EXPECT_EQ(m1[15], 15.f);
	}

	TEST(Matrix4, Trace)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		TheCoolerMath::mat4 m1(array);

		float trace = m1.Trace();
		
		EXPECT_EQ(trace, 30.f);
	}

	TEST(Matrix4, Opposite)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		TheCoolerMath::mat4 m1(array);
		m1.Opposite();

		EXPECT_EQ(m1[3], -3.f);
		EXPECT_EQ(m1[6], -6.f);
		EXPECT_EQ(m1[10], -10.f);
		EXPECT_EQ(m1[15], -15.f);
	}

	TEST(Matrix4, Inverse)
	{
		std::array<float, 16> array = { 4.f, 1.f, 2.f, 3.f,
										4.f, 7.f, 6.f, 7.f,
										8.f, 9.f, 2.f, 11.f,
										12.f, 11.f, 14.f, 15.f };

		TheCoolerMath::mat4 m1(array);

		m1.Inverse();

		glm::mat4 m1g = { 4.f, 1.f, 2.f, 3.f,
							4.f, 7.f, 6.f, 7.f,
							8.f, 9.f, 2.f, 11.f,
							12.f, 11.f, 14.f, 15.f };
		m1g = glm::inverse(m1g);

		float array1[16];
		memcpy(array1, &m1g, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_EQ(m1[i], array1[i]);
		}
	}

	TEST(Matrix4, Transpose)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		TheCoolerMath::mat4 m1(array);

		glm::mat4 m1g = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		float array1[16];
		memcpy(array1, &m1g, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_EQ(m1[i], array1[i]);
		}
	}
	TEST(Matrix4, Determinant)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		TheCoolerMath::mat4 m1(array);

		float det = m1.Determinant();

		glm::mat4 m1g = { 0.f, 1.f, 2.f, 3.f,
						  4.f, 5.f, 6.f, 7.f,
			   			  8.f, 9.f, 10.f, 11.f,
						  12.f, 13.f, 14.f, 15.f };

		m1g = glm::transpose(m1g);

		float gDet = glm::determinant(m1g);

		EXPECT_EQ(det, gDet);
	}

	TEST(Matrix4, Add)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		TheCoolerMath::mat4 m1(array);
		TheCoolerMath::mat4 m2(array);

		TheCoolerMath::mat4 result = TheCoolerMath::mat4::Add(m1, m2);

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
			EXPECT_EQ(result[i], array1[i]);
		}
	}

	TEST(Matrix4, MultiplyNumber)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		TheCoolerMath::mat4 m1(array);

		float value = 2.f;

		m1 = TheCoolerMath::mat4::MultiplyNumber(m1, value);

		TheCoolerMath::mat4 m1c(array);

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_EQ(m1[i], (m1c[i] * value));
		}
	}

	/*TEST(Matrix4, Multiply)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		TheCoolerMath::mat4 m1(array);
		TheCoolerMath::mat4 m2(array);

		TheCoolerMath::mat4 result = TheCoolerMath::mat4::Multiply(m1, m2);

		glm::mat4 m1g = { 0.f, 1.f, 2.f, 3.f,
						  4.f, 5.f, 6.f, 7.f,
						  8.f, 9.f, 10.f, 11.f,
						  12.f, 13.f, 14.f, 15.f };

		glm::mat4 m2g = { 0.f, 1.f, 2.f, 3.f,
				  4.f, 5.f, 6.f, 7.f,
				  8.f, 9.f, 10.f, 11.f,
				  12.f, 13.f, 14.f, 15.f };

		glm::mat4 resultg = m1g * m2g;
		resultg = glm::transpose(resultg);

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_EQ(result[i], resultg[i]);
		}
	}*/

	TEST(Matrix4, Translate)
	{
		TheCoolerMath::mat4 translate = TheCoolerMath::mat4::Translate(TheCoolerMath::vec3(1.f, 2.f, 3.f));

		glm::mat4 identity = glm::mat4(1.f);
		glm::mat4 translateg = glm::translate(identity, glm::vec3(1.f, 2.f, 3.f));
		translateg = glm::transpose(translateg);

		float array1[16];
		memcpy(array1, &translateg, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_EQ(translate[i], array1[i]);
		}
	}

	// Do later
	TEST(Matrix4, Rotation)
	{
		/*float angleRadians = glm::radians(45.0f);
		TheCoolerMath::quat q = TheCoolerMath::quat::FromAxisAngle(TheCoolerMath::vec3(0.f, 0.f, 1.f), angleRadians);

		TheCoolerMath::mat4 rotation = TheCoolerMath::mat4::Rotation(q);

		glm::quat qg = glm::angleAxis(angleRadians, glm::vec3(0.f, 0.f, 1.f));
		glm::mat4 rotationg = glm::toMat4(qg);
		rotationg = glm::transpose(rotationg);

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_NEAR(rotation[i], rotationg[i], 1e-5);
		}*/
	}

	TEST(Matrix4, Scale)
	{
		TheCoolerMath::mat4 scale = TheCoolerMath::mat4::Scale(TheCoolerMath::vec3(1.f, 1.f, 1.f));

		glm::mat4 identity = glm::mat4(1.f);
		glm::mat4 scaleg = glm::scale(identity, glm::vec3(1.f, 1.f, 1.f));
		scaleg = glm::transpose(scaleg);

		float array1[16];
		memcpy(array1, &scaleg, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_EQ(scale[i], array1[i]);
		}
	}

	// Do later
	//TEST(Matrix4, TRS)
	//{
	//	/*TheCoolerMath::mat4 scale = TheCoolerMath::mat4::Scale(TheCoolerMath::vec3(1.f, 1.f, 1.f));

	//	glm::mat4 scaleg = glm::scale(glm::vec3(1.f, 1.f, 1.f));
	//	scaleg = glm::transpose(scaleg);

	//	for (int i = 0; i < 16; ++i)
	//	{
	//		EXPECT_EQ(scale[i], scaleg[i]);
	//	}*/
	//}

	TEST(Matrix4, View)
	{
		TheCoolerMath::mat4 view = TheCoolerMath::mat4::View(TheCoolerMath::vec3(0.f, 0.f, 1.f), TheCoolerMath::vec3(0.f, 0.f, 0.f), TheCoolerMath::vec3(1.f, 0.f, 0.f));

		glm::mat4 viewg = glm::lookAt(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
		viewg = glm::transpose(viewg);

		float array1[16];
		memcpy(array1, &viewg, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_EQ(view[i], array1[i]);
		}
	}

	TEST(Matrix4, Perspective)
	{
		TheCoolerMath::mat4 perspective = TheCoolerMath::mat4::Perspective(45.f, 16.f / 9.f, 0.1f, 100.f);

		glm::mat4 perspectiveg = glm::perspective(45.f, 16.f / 9.f, 0.1f, 100.f);
		perspectiveg = glm::transpose(perspectiveg);

		float array1[16];
		memcpy(array1, &perspectiveg, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_EQ(perspective[i], array1[i]);
		}
	}

	TEST(Matrix4, Orthographice)
	{
		TheCoolerMath::mat4 ortho = TheCoolerMath::mat4::Orthographic(5.f, -5.f, 10.f, -10.f, 100.f, 0.1f);

		glm::mat4 orthog = glm::ortho(5.f, -5.f, 10.f, -10.f, 100.f, 0.1f);
		orthog = glm::transpose(orthog);

		float array1[16];
		memcpy(array1, &orthog, 16 * sizeof(float));

		for (int i = 0; i < 16; ++i)
		{
			EXPECT_EQ(ortho[i], array1[i]);
		}
	}

	/*------------------------------------------Operators-----------------------------------------------------*/
	TEST(Matrix4, OperatorBrackets)
	{
		std::array<float, 16> array = { 0.f, 1.f, 2.f, 3.f,
										4.f, 5.f, 6.f, 7.f,
										8.f, 9.f, 10.f, 11.f,
										12.f, 13.f, 14.f, 15.f };

		TheCoolerMath::mat4 m1(array);

		EXPECT_EQ(m1[3], 3.f);
		EXPECT_EQ(m1[6], 6.f);
		EXPECT_EQ(m1[10], 10.f);
		EXPECT_EQ(m1[15], 15.f);
	}

}