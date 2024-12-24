#pragma once
#include "Math/MathFwd.h"
#include "Templates/IsInteger.h"
#include <type_traits>

namespace DE
{
	namespace Math
	{
		template<typename T>
		struct TRect
		{
			static_assert(TIsInteger<T>::Value || std::is_floating_point_v<T>, "Integer or floating");

			union
			{
				struct
				{
					T Left;
					T Up;
					T Right;
					T Down;
				};
				T LeftUpRightDown[4];
			};



		};

	}
}
