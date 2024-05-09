#pragma once
#include <CoreMinimal.h>
#include <Math/MathFwd.h>
#include "Math/Matrix.h"
#include "Rotator.h"
#include "Plane.h"



namespace DE
{
	namespace Math
	{
		template <typename T>
		struct TInverseRotationMatrix : public TMatrix<T>
		{
		public:
			TInverseRotationMatrix(const TRotator<T>& Rotator);


		};



		template<typename T>
		FORCEINLINE TInverseRotationMatrix<T>::TInverseRotationMatrix(const TRotator<T>& Rotator): TMatrix<T>
			(
				TMatrix<T>
				(
					TPlane<T>(+FMath::Cos(Rotator.Yaw * DE_PI / 180.f), -FMath::Sin(Rotator.Yaw * DE_PI / 180.f),	0.f, 0.f),
					TPlane<T>(+FMath::Sin(Rotator.Yaw * DE_PI / 180.f), +FMath::Cos(Rotator.Yaw * DE_PI / 180.f),	0.f, 0.f),
					TPlane<T>(0.f,										0.f,										1.f, 0.f),
					TPlane<T>(0.f,										0.f,										0.f, 1.f)
				)
			)
		{


		}
	}
}