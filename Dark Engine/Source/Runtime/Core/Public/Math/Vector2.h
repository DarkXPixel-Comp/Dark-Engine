#pragma once
#include "Math/MathFwd.h"
#include <type_traits>


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

			/// 2D zero vector (0, 0)
			CORE_API static const TVector2<T> ZeroVector;

			/// 2D unit vector (1, 1)
			CORE_API static const TVector2<T> UnitVector;
		public:
			TVector2(T InF) : X(InF), Y(InF) {}
			TVector2() : X(0), Y(0) {}
			TVector2(T InX, T InY) : X(InX), Y(InY) {}

			CORE_API TVector2<T> operator*(const TVector2<T>& V) const
			{
				return TVector2<T>(X * V.X, Y * V.Y);
			}

			CORE_API TVector2<T> operator*=(const TVector2<T>& V)
			{
				X *= V.X;
				Y *= V.Y;
				return *this;
			}

			CORE_API bool operator ==(const TVector2<T>& V) const
			{
				return X == V.X && Y == V.Y;
			}
		};

	}
}