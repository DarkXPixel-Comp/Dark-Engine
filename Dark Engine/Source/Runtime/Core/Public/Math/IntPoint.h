#pragma once
#include "Platform/Platform.h"
#include "Math/MathFwd.h"
#include "Math/Vector2.h"
#include <type_traits>

namespace DE
{
	namespace Math
	{
		template<typename T>
		struct TIntPoint
		{
			static_assert(std::is_integral_v<T>, "T must be integral");
			union
			{
				struct
				{
					T X;
					T Y;
				};
				T XY[2];
			};
			FORCEINLINE TIntPoint() : X(0), Y(0) {}
			FORCEINLINE TIntPoint(T InX, T InY) : X(InX), Y(InY) {}
			FORCEINLINE TIntPoint(const TVector2<T>& V) : X(static_cast<T>(V.X)), Y(static_cast<T>(V.Y)) {}


			FORCEINLINE bool operator==(const TIntPoint& Other) const
			{
				return X == Other.X && Y == Other.Y;
			}
		};
	}
}