#include "MathUtilsUT.h"

using namespace Engine::Math;

namespace MathUtilsUT
{
	TEST(MathUtils, Lerp)
	{
		float nb1 = 0.f;
		float nb2 = 1.f;
		float t = 0.5f;
		float result = Lerp(nb1, nb2, t);
		EXPECT_FLOAT_EQ(result, nb1 + t * (nb2 - nb1));
	}

	TEST(MathUtils, ToRadians)
	{
		float degrees = 180.f;
		float radians = ToRadians(degrees);
		EXPECT_FLOAT_EQ(radians, PI);
	}

	TEST(MathUtils, ToDegrees)
	{
		float radians = PI;
		float degrees = ToDegrees(radians);
		EXPECT_FLOAT_EQ(degrees, 180.f);
	}
}