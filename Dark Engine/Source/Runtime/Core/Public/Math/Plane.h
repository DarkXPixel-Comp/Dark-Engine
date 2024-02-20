#pragma once 

#include <CoreTypes.h>
#include <Math/MathFwd.h>
#include <Math/Vector.h>

namespace DE
{
	namespace Math
	{
		template <typename T>
		struct TPlane : public TVector<T>
		{
			T W;

			TPlane(T InX, T InY, T InZ, T InW):
				X(InX),
				Y(InY),
				Z(InZ),
				W(InW)
			{}


		};

	}
}