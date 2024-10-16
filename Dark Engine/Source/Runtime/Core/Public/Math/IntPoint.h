#pragma once
#include "Templates/IsInteger.h"
#include <CoreTypes.h>
#include <Math/MathFwd.h>
#include <Math/Vector2D.h>


namespace DE
{
	namespace Math
	{

		template<typename IntType>
		struct TIntPoint
		{
			static_assert(TIsInteger<IntType>::Value, "Only integer");
			union
			{
				struct
				{
					IntType X;
					IntType Y;
				};
				IntType XY[2];
			};

			static const TIntPoint ZeroValue;

			TIntPoint() = default;

			TIntPoint(IntType InX, IntType InY) :
				X(InX),
				Y(InY)
			{}
			TIntPoint(IntType InXY) :
				X(InXY),
				Y(InXY)
			{}
			TIntPoint(IntType InXY[2]) :
				X(InXY[0]),
				Y(InXY[1])
			{}

			template<typename T>
			bool operator==(const TIntPoint<T>& Val)
			{
				return X == Val.X && Y == Val.Y;
			}

			template<typename T>
			TIntPoint<T> operator+(const TIntPoint<T>& Other)
			{
				return TIntPoint<T>(Other.X + X, Other.Y + Y);
			}

			template<typename T>
			bool operator!=(const TIntPoint<T>& Val)
			{
				return X != Val.X || Y != Val.Y;
			}


		};

	}
}