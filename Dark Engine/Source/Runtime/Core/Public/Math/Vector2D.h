#pragma once

#include <CoreTypes.h>
#include <Core.h>


namespace DE
{
	namespace Math
	{

		template<typename T>
		struct TVector2
		{
			static_assert(std::is_floating_point_v<T>, "T must be floating point");

		public:
			using FDark = T;

			union
			{
				struct
				{
					T X;
					T Y;
				};

				T XY[2];
			};



		};

	}
}