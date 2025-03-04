#include <iostream>
#include "vec2UT.h"

using namespace Math;

namespace vec2UT
{
	/*------------------------------------------CONSTRUCTORS-----------------------------------------------------*/ 
	TEST(Vector2, DefaultConstructor)
	{
		vec2 v;

		EXPECT_FLOAT_EQ(v.x, 0.f);
		EXPECT_FLOAT_EQ(v.y, 0.f);
	}

	TEST(Vector2, BasicConstructor)
	{
		float t_value1 = 0.f;
		float t_value2 = 1.f;

		vec2 v = vec2(t_value1, t_value2);

		EXPECT_FLOAT_EQ(v.x, t_value1);
		EXPECT_FLOAT_EQ(v.y, t_value2);
	}

	TEST(Vector2, SingleValueConstructor)
	{
		float t_value = 1.f;

		vec2 v = vec2(t_value);

		EXPECT_FLOAT_EQ(v.x, t_value);
		EXPECT_FLOAT_EQ(v.y, t_value);
	}

	/*------------------------------------------Functions-----------------------------------------------------*/
	TEST(Vector2, Opposite)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		vec2 v = vec2(t_value1, t_value2);
		v.Opposite();

		EXPECT_FLOAT_EQ(v.x, -t_value1);
		EXPECT_FLOAT_EQ(v.y, -t_value2);
	}

	TEST(Vector2, Invert)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		vec2 v = vec2(t_value1, t_value2);
		v.Invert();

		EXPECT_FLOAT_EQ(v.x, 1.f / t_value1);
		EXPECT_FLOAT_EQ(v.y, 1.f / t_value2);
	}

	TEST(Vector2, AddNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float add = 2.f;

		vec2 v = vec2(t_value1, t_value2);
		v.AddNumber(add);
		
		EXPECT_FLOAT_EQ(v.x, t_value1 + add);
		EXPECT_FLOAT_EQ(v.y, t_value2 + add);
	}

	TEST(Vector2, MultiplyNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float multiply = 2.f;

		vec2 v = vec2(t_value1, t_value2);
		v.MultiplyNumber(multiply);

		EXPECT_FLOAT_EQ(v.x, t_value1 * multiply);
		EXPECT_FLOAT_EQ(v.y, t_value2 * multiply);
	}

	TEST(Vector2, Add)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		float t_value3 = 4.f;
		float t_value4 = 8.f;

		vec2 v1 = vec2(t_value1, t_value2);
		vec2 v2 = vec2(t_value3, t_value4);
		vec2 v3 = vec2::Add(v1, v2);

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);
		glm::vec2 v2g = glm::vec2(t_value3, t_value4);
		glm::vec2 v3g = v1g + v2g;

		EXPECT_FLOAT_EQ(v3.x, v3g.x);
		EXPECT_FLOAT_EQ(v3.y, v3g.y);
	}

	TEST(Vector2, Multiply)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		float t_value3 = 4.f;
		float t_value4 = 8.f;

		vec2 v1 = vec2(t_value1, t_value2);
		vec2 v2 = vec2(t_value3, t_value4);
		vec2 v3 = vec2::Multiply(v1, v2);

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);
		glm::vec2 v2g = glm::vec2(t_value3, t_value4);
		glm::vec2 v3g = v1g * v2g;

		EXPECT_FLOAT_EQ(v3.x, v3g.x);
		EXPECT_FLOAT_EQ(v3.y, v3g.y);
	}

	TEST(Vector2, MidPoint)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		float t_value3 = 4.f;
		float t_value4 = 8.f;

		vec2 v1 = vec2(t_value1, t_value2);
		vec2 v2 = vec2(t_value3, t_value4);
		vec2 v3 = vec2::MidPoint(v1, v2);

		vec2 v4 = (v1 + v2) / 2;

		EXPECT_FLOAT_EQ(v3.x, v4.x);
		EXPECT_FLOAT_EQ(v3.y, v4.y);
	}

	TEST(Vector2, Distance)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		float t_value3 = 4.f;
		float t_value4 = 8.f;

		vec2 v1 = vec2(t_value1, t_value2);
		vec2 v2 = vec2(t_value3, t_value4);

		float distance = vec2::Distance(v1, v2);

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);
		glm::vec2 v2g = glm::vec2(t_value3, t_value4);

		float gDistance = glm::distance(v1g, v2g);

		EXPECT_FLOAT_EQ(distance, gDistance);
	}

	TEST(Vector2, SquaredNorm)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		vec2 v1 = vec2(t_value1, t_value2);

		float squaredNorm = vec2::SquaredNorm(v1);

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);

		float gSquaredNorm = glm::dot(v1g, v1g);

		EXPECT_FLOAT_EQ(squaredNorm, gSquaredNorm);
	}

	TEST(Vector2, Norm)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		vec2 v1 = vec2(t_value1, t_value2);

		float Norm = vec2::Norm(v1);

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);

		float gNorm = glm::length(v1g);

		EXPECT_FLOAT_EQ(Norm, gNorm);
	}

	TEST(Vector2, Normalize)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		vec2 v1 = vec2(t_value1, t_value2);

		vec2 nV1 = vec2::Normalize(v1);

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);

		glm::vec2 nV1g = glm::normalize(v1g);

		EXPECT_FLOAT_EQ(nV1.x, nV1g.x);
		EXPECT_FLOAT_EQ(nV1.y, nV1g.y);
	}

	TEST(Vector2, DotProduct)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		float t_value3 = 4.f;
		float t_value4 = 8.f;

		vec2 v1 = vec2(t_value1, t_value2);
		vec2 v2 = vec2(t_value3, t_value4);

		float dotProduct = vec2::DotProduct(v1, v2);

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);
		glm::vec2 v2g = glm::vec2(t_value3, t_value4);

		float gDotProduct = glm::dot(v1g, v2g);

		EXPECT_FLOAT_EQ(dotProduct, gDotProduct);
	}

	TEST(Vector2, CrossProduct)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		float t_value3 = 4.f;
		float t_value4 = 8.f;

		vec2 v1 = vec2(t_value1, t_value2);
		vec2 v2 = vec2(t_value3, t_value4);

		float crossProduct = vec2::CrossProduct(v1, v2);

		float otherCrossProduct = (v1.x * v2.y) - (v2.x * v1.y);

		EXPECT_FLOAT_EQ(crossProduct, otherCrossProduct);
	}

	TEST(Vector2, GetAngle)
	{
		float t_value1 = 1.f;
		float t_value2 = 0.f;

		float t_value3 = 0.995f;
		float t_value4 = 0.0998f;

		vec2 v1 = vec2(t_value1, t_value2);
		vec2 v2 = vec2(t_value3, t_value4);

		float angle = vec2::GetAngle(v1, v2);

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);
		glm::vec2 v2g = glm::vec2(t_value3, t_value4);

		float gAngle = glm::angle(v1g, v2g);
		EXPECT_NEAR(angle, gAngle, 0.0001f);
	}


	/*------------------------------------------Operators-----------------------------------------------------*/
	TEST(Vector2, OperatorBrackets)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		vec2 v = vec2(t_value1, t_value2);

		EXPECT_FLOAT_EQ(v[0], t_value1);
		EXPECT_FLOAT_EQ(v[1], t_value2);
	}

	TEST(Vector2, OperatorEqual)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		vec2 v1 = vec2(t_value1, t_value2);
		vec2 v2 = vec2(t_value1, t_value2);

		bool equal = v1 == v2;

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);
		glm::vec2 v2g = glm::vec2(t_value1, t_value2);

		bool gEqual = v1g == v2g;

		EXPECT_FLOAT_EQ(equal, gEqual);
	}

	TEST(Vector2, OperatorAddNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float add = 2.f;

		vec2 vO1 = vec2(t_value1, t_value2);
		vO1 = vO1 + add;

		vec2 vO2 = vec2(t_value1, t_value2);
		vO2 = add + vO2;

		glm::vec2 vO1g = glm::vec2(t_value1, t_value2);
		vO1g = vO1g + add;

		EXPECT_FLOAT_EQ(vO1g.x, vO1.x);
		EXPECT_FLOAT_EQ(vO1g.y, vO1.y);

		EXPECT_FLOAT_EQ(vO1g.x, vO2.x);
		EXPECT_FLOAT_EQ(vO1g.y, vO2.y);
	}

	TEST(Vector2, OperatorSubtractNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float sub = 2.f;

		vec2 vO1 = vec2(t_value1, t_value2);
		vO1 = vO1 - sub;

		glm::vec2 vO1g = glm::vec2(t_value1, t_value2);
		vO1g = vO1g - sub;

		EXPECT_FLOAT_EQ(vO1g.x, vO1.x);
		EXPECT_FLOAT_EQ(vO1g.y, vO1.y);
	}
	
	TEST(Vector2, OperatorMultiplyNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float multiply = 2.f;

		vec2 v1 = vec2(t_value1, t_value2);

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);

		vec2 v2 = v1 * multiply;
		glm::vec2 v2g = v1g * multiply;

		EXPECT_FLOAT_EQ(v2.x, v2g.x);
		EXPECT_FLOAT_EQ(v2.y, v2g.y);
	}

	TEST(Vector2, OperatorDivideNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float divide = 2.f;

		vec2 v1 = vec2(t_value1, t_value2);
		glm::vec2 v1g = glm::vec2(t_value1, t_value2);

		vec2 v2 = v1 / divide;
		glm::vec2 v2g = v1g / divide;

		EXPECT_FLOAT_EQ(v2.x, v2g.x);
		EXPECT_FLOAT_EQ(v2.y, v2g.y);
	}

	TEST(Vector2, OperatorAdd)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float t_value3 = 2.f;
		float t_value4 = 4.f;

		vec2 v1 = vec2(t_value1, t_value2);
		vec2 v2 = vec2(t_value3, t_value4);

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);
		glm::vec2 v2g = glm::vec2(t_value3, t_value4);

		vec2 v3 = v1 + v2;
		glm::vec2 v3g = v1g + v2g;

		EXPECT_FLOAT_EQ(v3.x, v3g.x);
		EXPECT_FLOAT_EQ(v3.y, v3g.y);
	}

	TEST(Vector2, OperatorSubtract)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		float t_value3 = 2.f;
		float t_value4 = 4.f;

		vec2 v1 = vec2(t_value1, t_value2);
		vec2 v2 = vec2(t_value3, t_value4);

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);
		glm::vec2 v2g = glm::vec2(t_value3, t_value4);

		vec2 v3 = v1 - v2;
		glm::vec2 v3g = v1g - v2g;

		EXPECT_FLOAT_EQ(v3.x, v3g.x);
		EXPECT_FLOAT_EQ(v3.y, v3g.y);
	}

	TEST(Vector2, OperatorMultiply)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		float t_value3 = 2.f;
		float t_value4 = 4.f;

		vec2 v1 = vec2(t_value1, t_value2);
		vec2 v2 = vec2(t_value3, t_value4);

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);
		glm::vec2 v2g = glm::vec2(t_value3, t_value4);

		vec2 v3 = v1 * v2;
		glm::vec2 v3g = v1g * v2g;

		EXPECT_FLOAT_EQ(v3.x, v3g.x);
		EXPECT_FLOAT_EQ(v3.y, v3g.y);
	}

	TEST(Vector2, OperatorDivide)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		float t_value3 = 2.f;
		float t_value4 = 4.f;

		vec2 v1 = vec2(t_value1, t_value2);
		vec2 v2 = vec2(t_value3, t_value4);

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);
		glm::vec2 v2g = glm::vec2(t_value3, t_value4);

		vec2 v3 = v1 / v2;
		glm::vec2 v3g = v1g / v2g;

		EXPECT_FLOAT_EQ(v3.x, v3g.x);
		EXPECT_FLOAT_EQ(v3.y, v3g.y);
	}

	TEST(Vector2, OperatorEqualAdd)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		float t_value3 = 2.f;
		float t_value4 = 4.f;

		vec2 v1(t_value1, t_value2);
		vec2 v2(t_value3, t_value4);

		glm::vec2 v1g(t_value1, t_value2);
		glm::vec2 v2g(t_value3, t_value4);

		v1 += v2;
		v1g += v2g;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);
	}

	TEST(Vector2, OperatorEqualSubtract)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		float t_value3 = 2.f;
		float t_value4 = 4.f;

		vec2 v1(t_value1, t_value2);
		vec2 v2(t_value3, t_value4);

		glm::vec2 v1g(t_value1, t_value2);
		glm::vec2 v2g(t_value3, t_value4);

		v1 -= v2;
		v1g -= v2g;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);
	}

	TEST(Vector2, OperatorEqualMultiply)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		float t_value3 = 2.f;
		float t_value4 = 4.f;

		vec2 v1(t_value1, t_value2);
		vec2 v2(t_value3, t_value4);

		glm::vec2 v1g(t_value1, t_value2);
		glm::vec2 v2g(t_value3, t_value4);

		v1 *= v2;
		v1g *= v2g;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);
	}

	TEST(Vector2, OperatorEqualDivide)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;

		float t_value3 = 2.f;
		float t_value4 = 4.f;

		vec2 v1(t_value1, t_value2);
		vec2 v2(t_value3, t_value4);

		glm::vec2 v1g(t_value1, t_value2);
		glm::vec2 v2g(t_value3, t_value4);

		v1 /= v2;
		v1g /= v2g;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);
	}

	TEST(Vector2, OperatorEqualAddNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float add = 2.f;

		vec2 v1(t_value1, t_value2);

		glm::vec2 v1g(t_value1, t_value2);

		v1 += add;
		v1g += add;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);

	}

	TEST(Vector2, OperatorEqualSubtractNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float sub = 2.f;

		vec2 v1(t_value1, t_value2);

		glm::vec2 v1g(t_value1, t_value2);

		v1 -= sub;
		v1g -= sub;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);
	}

	TEST(Vector2, OperatorEqualDivideNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float divide = 2.f;

		vec2 v1 = vec2(t_value1, t_value2);

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);

		v1 /= divide;
		v1g /= divide;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);
	}

	TEST(Vector2, OperatorEqualMutliplyNumber)
	{
		float t_value1 = 3.f;
		float t_value2 = 5.f;
		float multiply = 2.f;

		vec2 v1 = vec2(t_value1, t_value2);

		glm::vec2 v1g = glm::vec2(t_value1, t_value2);

		v1 *= multiply;
		v1g *= multiply;

		EXPECT_FLOAT_EQ(v1.x, v1g.x);
		EXPECT_FLOAT_EQ(v1.y, v1g.y);
	}
}