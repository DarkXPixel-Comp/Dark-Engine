#pragma once 

//#include "Vector.h"
#include <CoreTypes.h>
#include <Math/MathFwd.h>



namespace DE
{
	namespace Math
	{
		template <typename T>
		struct TPlane
		{
			using FDark = T;
			union
			{
				struct
				{
					T X; ///<- X Element
					T Y; ///<- Y Element
					T Z; ///<- Z Element
					T W; ///<- Z Element

				};
				T XYZW[4]; ///<- XYZW Array
			};

			explicit TPlane()
			{
				FMemory::Memzero(this, sizeof(*this));
			}

			explicit TPlane(T InX, T InY, T InZ, T InW): X(InX), Y(InY), Z(InZ), W(InW)
			{}


		};

	}
}