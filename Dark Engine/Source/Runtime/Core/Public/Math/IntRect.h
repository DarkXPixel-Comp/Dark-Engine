#pragma once


#include "IntPoint.h"


namespace DE
{
	namespace Math
	{
		template <typename IntType>
		struct TIntRect
		{
			using IntPointType = TIntPoint<IntType>;

			static_assert(TIsInteger<IntType>::Value, "Only Integer");

			union
			{
				struct
				{
					IntPointType LeftUp;
					IntPointType RightDown;
				};
				IntPointType LeftUpRightDown[2];
			};


			TIntRect() = default;

			TIntRect(IntType X0, IntType Y0, IntType X1, IntType Y1) :
				LeftUp(X0, Y0),
				RightDown(X1, Y1)
			{}

			TIntRect(IntPointType InLeftUp, IntPointType InRightDown):
				LeftUp(InLeftUp),
				RightDown(InRightDown)
			{}


			bool IsPointInRect(IntPointType Point)
			{
				if (Point.X > LeftUp.X &&
					Point.X < RightDown.X &&
					Point.Y > LeftUp.Y &&
					Point.Y < RightDown.Y)
				{
					return true;
				}
				return false;
			}



		};

	}
}