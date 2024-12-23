#pragma once
#include "Math/MathFwd.h"


namespace DE
{
	namespace Math
	{
		template <typename T>
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

		public:
			TVector2(T InF) : X(InF), Y(InF) {}
			TVector2() : X(0), Y(0) {}
			TVector2(T InX, T InY) : X(InX), Y(InY) {}
		};

	}
}