#pragma once


#include <CoreMinimal.h>
#include <Math/MathFwd.h>
#include "Math/Matrix.h"
#include "Plane.h"



namespace DE
{
	namespace Math
	{
		template <typename T>
		struct TPerspectiveMatrix : public TMatrix<T>
		{
			TPerspectiveMatrix(T HalfFovX, T HalfFovY, T MultFovX, T MultFovY,T MinZ, T MaxZ);
			TPerspectiveMatrix(T HalfFov, T Width, T Height, T MinZ, T MaxZ);
			TPerspectiveMatrix(T HalfFov, T Width, T Height, T MinZ);

		};



		//template <typename T>
		//TPerspectiveMatrix<T>::TPerspectiveMatrix(T HalfFovX, T HalfFovY, T MultFovX, T MultFovY, T MinZ, T MaxZ): TMatrix<T>
		//(
		//	TPlane<T>(1.f / FMath::)

		//)



	}
}