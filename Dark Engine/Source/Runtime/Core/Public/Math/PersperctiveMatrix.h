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
			//TPerspectiveMatrix(T HalfFov, T Width, T Height, T MinZ);
			TPerspectiveMatrix(T FovY, T AspectRatio, T MinZ, T MaxZ) :
				TMatrix<T>
				(
					TPlane<T>((1 / FMath::Tan(FovY / 2)) / AspectRatio, 0, 0, 0),
					TPlane<T>(0, 1 / FMath::Tan(FovY / 2), 0, 0),
					TPlane<T>(0, 0, MaxZ / (MaxZ - MinZ), 1),
					TPlane<T>(0, 0,  (-(MaxZ / (MaxZ - MinZ))) * MinZ, 0)
				)
			{
			}

		};


		template <typename T>
		struct TReversedZPerspectiveMatrix : public TMatrix<T>
		{
			TReversedZPerspectiveMatrix(T HalfFOVX, T HalfFOVY, T MultFOVX, T MultFOVY, T MinZ, T MaxZ);
			TReversedZPerspectiveMatrix(T HalfFOV, T Width, T Height, T MinZ, T MaxZ);
			TReversedZPerspectiveMatrix(T HalfFOV, T Width, T Height, T MinZ);

		};

		template<typename T>
		FORCEINLINE TReversedZPerspectiveMatrix<T>::TReversedZPerspectiveMatrix(T HalfFOV, T Width, T Height, T MinZ) :
			TMatrix<T>
			(
				TPlane<T>(1.f / FMath::Tan(HalfFOV),	0.f,									0.f,	0.f),
				TPlane<T>(0.f,							Width / FMath::Tan(HalfFOV) / Height,	0.f,	0.f),
				TPlane<T>(0.f,							0.f,									0.f,	1.f),
				TPlane<T>(0.f,							0.f,									MinZ,	0.f)
			)
		{
		}

	}
}