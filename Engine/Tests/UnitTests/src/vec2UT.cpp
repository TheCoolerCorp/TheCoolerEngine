#include <iostream>
#include "vec2UT.h"

namespace vec2UT
{
	TEST(Vector2, DefaultConstructor)
	{
		TheCoolerMath::vec2 v;
		EXPECT_EQ(v.x, 0.f);
		EXPECT_EQ(v.y, 0.f);
	}

	TEST(Vector2, CopyConstructor)
	{
		TheCoolerMath::vec2 v;
		EXPECT_EQ(v.x, 1.f);
		EXPECT_EQ(v.y, 0.f);
	}
}