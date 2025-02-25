#include "Math/quat.h"

namespace TheCoolerMath
{
	float quat::operator[](int index) const
	{
		if (index < 0 || index > 3)
		{
			std::cout << "Error : overflow" << '\n';
			return 0.f;
		}

		if (index == 0)
		{
			return x;
		}
		if (index == 1)
		{
			return y;
		}
		if (index == 2)
		{
			return z;
		}
		if (index == 3)
		{
			return w;
		}
		return 0.f;
	}
}