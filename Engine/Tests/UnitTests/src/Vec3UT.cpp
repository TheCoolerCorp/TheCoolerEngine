#include <iostream>
#include "vec3UT.h"

using namespace Math;

namespace vec3UT
{
	/*------------------------------------------CONSTRUCTORS-----------------------------------------------------*/
	TEST(Vector3, DefaultConstructor)
	{
		vec3 v;

		EXPECT_FLOAT_EQ(v.x, 0.f);
		EXPECT_FLOAT_EQ(v.y, 0.f);
		EXPECT_FLOAT_EQ(v.z, 0.f);
	}

	TEST(Vector3, BasicConstructor)
	{
		float t_value1 = 0.f;
		float t_value2 = 1.f;
		float t_value3 = 2.f;

		vec3 v = vec3(t_value1, t_value2, t_value3);

		EXPECT_FLOAT_EQ(v.x, t_value1);
		EXPECT_FLOAT_EQ(v.y, t_value2);
		EXPECT_FLOAT_EQ(v.z, t_value3);
	}

	TEST(Vector3, SingleValueConstructor)
	{
		float t_value = 1.f;

		vec3 v = vec3(t_value);

		EXPECT_FLOAT_EQ(v.x, t_value);
		EXPECT_FLOAT_EQ(v.y, t_value);
	}

	/*------------------------------------------Functions-----------------------------------------------------*/
	TEST(Vector3, Opposite)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float t_value3 = 8.f;

		vec3 v = vec3(t_value1, t_value2, t_value3);
		v.Opposite();

		EXPECT_FLOAT_EQ(v.x, -t_value1);
		EXPECT_FLOAT_EQ(v.y, -t_value2);
		EXPECT_FLOAT_EQ(v.z, -t_value3);
	}

	TEST(Vector3, Invert)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float t_value3 = 8.f;

		vec3 v = vec3(t_value1, t_value2, t_value3);
		v.Invert();

		EXPECT_FLOAT_EQ(v.x, 1.f / t_value1);
		EXPECT_FLOAT_EQ(v.y, 1.f / t_value2);
		EXPECT_FLOAT_EQ(v.z, 1.f / t_value3);
	}

	TEST(Vector3, AddNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float t_value3 = 8.f;
		float add = 2.f;

		vec3 v = vec3(t_value1, t_value2, t_value3);
		v.AddNumber(add);

		EXPECT_FLOAT_EQ(v.x, t_value1 + add);
		EXPECT_FLOAT_EQ(v.y, t_value2 + add);
		EXPECT_FLOAT_EQ(v.y, t_value2 + add);
	}

	TEST(Vector3, MultiplyNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float t_value3 = 8.f;
		float multiply = 2.f;

		vec3 v = vec3(t_value1, t_value2, t_value3);
		v.MultiplyNumber(multiply);

		EXPECT_FLOAT_EQ(v.x, t_value1 * multiply);
		EXPECT_FLOAT_EQ(v.y, t_value2 * multiply);
		EXPECT_FLOAT_EQ(v.z, t_value3 * multiply);
	}

	TEST(Vector3, Add)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		float t_value4 = 1.f;
		float t_value5 = 2.f;
		float t_value6 = 3.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);
		vec3 v2 = vec3(t_value4, t_value5, t_value6);
		vec3 v3 = vec3::Add(v1, v2);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);
		glm::vec3 v2g = glm::vec3(t_value4, t_value5, t_value6);
		glm::vec3 v3g = v1g + v2g;

		EXPECT_FLOAT_EQ(v3.x, v3g.x);
		EXPECT_FLOAT_EQ(v3.y, v3g.y);
		EXPECT_FLOAT_EQ(v3.z, v3g.z);
	}

	TEST(Vector3, Multiply)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		float t_value4 = 1.f;
		float t_value5 = 2.f;
		float t_value6 = 3.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);
		vec3 v2 = vec3(t_value4, t_value5, t_value6);
		vec3 v3 = vec3::Multiply(v1, v2);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);
		glm::vec3 v2g = glm::vec3(t_value4, t_value5, t_value6);
		glm::vec3 v3g = v1g * v2g;

		EXPECT_FLOAT_EQ(v3.x, v3g.x);
		EXPECT_FLOAT_EQ(v3.y, v3g.y);
		EXPECT_FLOAT_EQ(v3.z, v3g.z);
	}

	TEST(Vector3, MidPoint)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		float t_value4 = 1.f;
		float t_value5 = 2.f;
		float t_value6 = 3.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);
		vec3 v2 = vec3(t_value4, t_value5, t_value6);
		vec3 v3 = vec3::MidPoint(v1, v2);

		vec3 v4 = vec3::MidPoint(v1,v2);

		EXPECT_FLOAT_EQ(v3.x, v4.x);
		EXPECT_FLOAT_EQ(v3.y, v4.y);
		EXPECT_FLOAT_EQ(v3.z, v4.z);
	}

	TEST(Vector3, Distance)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		float t_value4 = 1.f;
		float t_value5 = 2.f;
		float t_value6 = 3.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);
		vec3 v2 = vec3(t_value4, t_value5, t_value6);

		float distance = vec3::Distance(v1, v2);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);
		glm::vec3 v2g = glm::vec3(t_value4, t_value5, t_value6);

		float gDistance = glm::distance(v1g, v2g);

		EXPECT_FLOAT_EQ(distance, gDistance);
	}

	TEST(Vector3, SquaredNorm)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);

		float squaredNorm = vec3::SquaredNorm(v1);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);

		float gSquaredNorm = glm::dot(v1g, v1g);

		EXPECT_FLOAT_EQ(squaredNorm, gSquaredNorm);
	}

	TEST(Vector3, Norm)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);

		float Norm = vec3::Norm(v1);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);

		float gNorm = glm::length(v1g);

		EXPECT_FLOAT_EQ(Norm, gNorm);
	}

	TEST(Vector3, Normalize)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);

		vec3 nV1 = vec3::Normalize(v1);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);

		glm::vec3 nV1g = glm::normalize(v1g);

		EXPECT_FLOAT_EQ(nV1.x, nV1g.x);
		EXPECT_FLOAT_EQ(nV1.y, nV1g.y);
	}

	TEST(Vector3, DotProduct)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		float t_value4 = 1.f;
		float t_value5 = 2.f;
		float t_value6 = 3.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);
		vec3 v2 = vec3(t_value4, t_value5, t_value6);

		float dotProduct = vec3::DotProduct(v1, v2);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);
		glm::vec3 v2g = glm::vec3(t_value4, t_value5, t_value6);

		float gDotProduct = glm::dot(v1g, v2g);

		EXPECT_FLOAT_EQ(dotProduct, gDotProduct);
	}

	TEST(Vector3, CrossProduct)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		float t_value4 = 1.f;
		float t_value5 = 2.f;
		float t_value6 = 3.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);
		vec3 v2 = vec3(t_value4, t_value5, t_value6);

		vec3 crossProduct = vec3::CrossProduct(v1, v2);

		vec3 otherCrossProduct = vec3(
			(v1.y * v2.z) - (v1.z * v2.y),
			(v1.z * v2.x) - (v1.x * v2.z),
			(v1.x * v2.y) - (v1.y * v2.x)
		);

		EXPECT_FLOAT_EQ(crossProduct.x, otherCrossProduct.x);
		EXPECT_FLOAT_EQ(crossProduct.y, otherCrossProduct.y);
		EXPECT_FLOAT_EQ(crossProduct.z, otherCrossProduct.z);
	}

	TEST(Vector3, GetAngle)
	{
		float t_value1 = 1.f;
		float t_value2 = 0.f;
		float t_value3 = 0.f;

		float t_value4 = 0.995f;
		float t_value5 = 0.0998f;
		float t_value6 = 0.0098f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);
		vec3 v2 = vec3(t_value4, t_value5, t_value6);

		float angle = vec3::GetAngle(v1, v2);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);
		glm::vec3 v2g = glm::vec3(t_value4, t_value5, t_value6);

		float gAngle = glm::angle(v1g, v2g);
		EXPECT_NEAR(angle, gAngle, 0.03f);
	}


	/*------------------------------------------Operators-----------------------------------------------------*/
	TEST(Vector3, OperatorBrackets)
	{
		float t_value1 = 1.f;
		float t_value2 = 2.f;
		float t_value3 = 3.f;

		vec3 v = vec3(t_value1, t_value2, t_value3);

		EXPECT_FLOAT_EQ(v[0], t_value1);
		EXPECT_FLOAT_EQ(v[1], t_value2);
		EXPECT_FLOAT_EQ(v[1], t_value2);
	}

	TEST(Vector3, OperatorEqual)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float t_value3 = 8.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);
		vec3 v2 = vec3(t_value1, t_value2, t_value3);

		bool equal = (v1 == v2);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);
		glm::vec3 v2g = glm::vec3(t_value1, t_value2, t_value3);

		bool gEqual = (v1g == v2g);

		EXPECT_FLOAT_EQ(equal, gEqual);
	}

	TEST(Vector3, OperatorAddNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float t_value3 = 8.f;
		float add = 2.f;

		vec3 vO1 = vec3(t_value1, t_value2, t_value3);
		vO1 = vO1 + add;

		vec3 vO2 = vec3(t_value1, t_value2, t_value3);
		vO2 = add + vO2;

		glm::vec3 vO1g = glm::vec3(t_value1, t_value2, t_value3);
		vO1g = vO1g + add;

		EXPECT_FLOAT_EQ(vO1g.x, vO1.x);
		EXPECT_FLOAT_EQ(vO1g.y, vO1.y);
		EXPECT_FLOAT_EQ(vO1g.z, vO1.z);

		EXPECT_FLOAT_EQ(vO1g.x, vO2.x);
		EXPECT_FLOAT_EQ(vO1g.y, vO2.y);
		EXPECT_FLOAT_EQ(vO1g.z, vO2.z);
	}

	TEST(Vector3, OperatorSubtractNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float t_value3 = 8.f;
		float sub = 2.f;

		vec3 vO1 = vec3(t_value1, t_value2, t_value3);
		vO1 = vO1 - sub;

		glm::vec3 vO1g = glm::vec3(t_value1, t_value2, t_value3);
		vO1g = vO1g - sub;

		EXPECT_FLOAT_EQ(vO1g.x, vO1.x);
		EXPECT_FLOAT_EQ(vO1g.y, vO1.y);
		EXPECT_FLOAT_EQ(vO1g.z, vO1.z);
	}

	TEST(Vector3, OperatorMultiplyNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float t_value3 = 8.f;
		float multiply = 2.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);

		vec3 v2 = v1 * multiply;
		glm::vec3 v2g = v1g * multiply;

		EXPECT_FLOAT_EQ(v2.x, v2g.x);
		EXPECT_FLOAT_EQ(v2.y, v2g.y);
		EXPECT_FLOAT_EQ(v2.z, v2g.z);
	}

	TEST(Vector3, OperatorDivideNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float t_value3 = 8.f;
		float divide = 2.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);
		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);

		vec3 v2 = v1 / divide;
		glm::vec3 v2g = v1g / divide;

		EXPECT_FLOAT_EQ(v2.x, v2g.x);
		EXPECT_FLOAT_EQ(v2.y, v2g.y);
		EXPECT_FLOAT_EQ(v2.z, v2g.z);
	}

	TEST(Vector3, OperatorAdd)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		float t_value4 = 1.f;
		float t_value5 = 2.f;
		float t_value6 = 3.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);
		vec3 v2 = vec3(t_value4, t_value5, t_value6);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);
		glm::vec3 v2g = glm::vec3(t_value4, t_value5, t_value6);

		vec3 v3 = v1 + v2;
		glm::vec3 v3g = v1g + v2g;

		EXPECT_FLOAT_EQ(v3.x, v3g.x);
		EXPECT_FLOAT_EQ(v3.y, v3g.y);
		EXPECT_FLOAT_EQ(v3.z, v3g.z);
	}

	TEST(Vector3, OperatorSubtract)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		float t_value4 = 1.f;
		float t_value5 = 2.f;
		float t_value6 = 3.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);
		vec3 v2 = vec3(t_value4, t_value5, t_value6);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);
		glm::vec3 v2g = glm::vec3(t_value4, t_value5, t_value6);

		vec3 v3 = v1 - v2;
		glm::vec3 v3g = v1g - v2g;

		EXPECT_FLOAT_EQ(v3.x, v3g.x);
		EXPECT_FLOAT_EQ(v3.y, v3g.y);
		EXPECT_FLOAT_EQ(v3.z, v3g.z);
	}

	TEST(Vector3, OperatorMultiply)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		float t_value4 = 1.f;
		float t_value5 = 2.f;
		float t_value6 = 3.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);
		vec3 v2 = vec3(t_value4, t_value5, t_value6);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);
		glm::vec3 v2g = glm::vec3(t_value4, t_value5, t_value6);

		vec3 v3 = v1 * v2;
		glm::vec3 v3g = v1g * v2g;

		EXPECT_FLOAT_EQ(v3.x, v3g.x);
		EXPECT_FLOAT_EQ(v3.y, v3g.y);
		EXPECT_FLOAT_EQ(v3.z, v3g.z);
	}

	TEST(Vector3, OperatorDivide)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		float t_value4 = 1.f;
		float t_value5 = 2.f;
		float t_value6 = 3.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);
		vec3 v2 = vec3(t_value4, t_value5, t_value6);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);
		glm::vec3 v2g = glm::vec3(t_value4, t_value5, t_value6);

		vec3 v3 = v1 / v2;
		glm::vec3 v3g = v1g / v2g;

		EXPECT_FLOAT_EQ(v3.x, v3g.x);
		EXPECT_FLOAT_EQ(v3.y, v3g.y);
		EXPECT_FLOAT_EQ(v3.z, v3g.z);
	}

	TEST(Vector3, OperatorEqualAdd)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		float t_value4 = 1.f;
		float t_value5 = 2.f;
		float t_value6 = 3.f;

		vec3 v1(t_value1, t_value2, t_value3);
		vec3 v2(t_value4, t_value5, t_value6);

		glm::vec3 v1g(t_value1, t_value2, t_value3);
		glm::vec3 v2g(t_value4, t_value5, t_value6);

		v1 += v2;
		v1g += v2g;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);
	}

	TEST(Vector3, OperatorEqualSubtract)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		float t_value4 = 1.f;
		float t_value5 = 2.f;
		float t_value6 = 3.f;

		vec3 v1(t_value1, t_value2, t_value3);
		vec3 v2(t_value4, t_value5, t_value6);

		glm::vec3 v1g(t_value1, t_value2, t_value3);
		glm::vec3 v2g(t_value4, t_value5, t_value6);

		v1 -= v2;
		v1g -= v2g;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);
		EXPECT_FLOAT_EQ(v1.z, v1g.z);
	}

	TEST(Vector3, OperatorEqualMultiply)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		float t_value4 = 1.f;
		float t_value5 = 2.f;
		float t_value6 = 3.f;

		vec3 v1(t_value1, t_value2, t_value3);
		vec3 v2(t_value4, t_value5, t_value6);

		glm::vec3 v1g(t_value1, t_value2, t_value3);
		glm::vec3 v2g(t_value4, t_value5, t_value6);

		v1 *= v2;
		v1g *= v2g;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);
		EXPECT_FLOAT_EQ(v1.z, v1g.z);
	}

	TEST(Vector3, OperatorEqualDivide)
	{
		float t_value1 = 3.f;
		float t_value2 = 4.f;
		float t_value3 = 5.f;

		float t_value4 = 1.f;
		float t_value5 = 2.f;
		float t_value6 = 3.f;

		vec3 v1(t_value1, t_value2, t_value3);
		vec3 v2(t_value4, t_value5, t_value6);

		glm::vec3 v1g(t_value1, t_value2, t_value3);
		glm::vec3 v2g(t_value4, t_value5, t_value6);

		v1 /= v2;
		v1g /= v2g;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);
		EXPECT_FLOAT_EQ(v1.z, v1g.z);
	}

	TEST(Vector3, OperatorEqualAddNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float t_value3 = 8.f;
		float add = 2.f;

		vec3 v1(t_value1, t_value2, t_value3);

		glm::vec3 v1g(t_value1, t_value2, t_value3);

		v1 += add;
		v1g += add;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);
		EXPECT_FLOAT_EQ(v1.z, v1g.z);

	}

	TEST(Vector3, OperatorEqualSubtractNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float t_value3 = 8.f;
		float sub = 2.f;

		vec3 v1(t_value1, t_value2, t_value3);

		glm::vec3 v1g(t_value1, t_value2, t_value3);

		v1 -= sub;
		v1g -= sub;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);
		EXPECT_FLOAT_EQ(v1.z, v1g.z);
	}

	TEST(Vector3, OperatorEqualDivideNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float t_value3 = 8.f;
		float divide = 2.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);

		v1 /= divide;
		v1g /= divide;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);
		EXPECT_FLOAT_EQ(v1.z, v1g.z);
	}

	TEST(Vector3, OperatorEqualMutliplyNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float t_value3 = 8.f;
		float multiply = 2.f;

		vec3 v1 = vec3(t_value1, t_value2, t_value3);

		glm::vec3 v1g = glm::vec3(t_value1, t_value2, t_value3);

		v1 *= multiply;
		v1g *= multiply;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);
		EXPECT_FLOAT_EQ(v1.z, v1g.z);
	}
}