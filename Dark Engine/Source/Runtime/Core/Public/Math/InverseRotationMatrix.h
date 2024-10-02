#pragma once
#include "Math/Matrix.h"
#include "Rotator.h"
#include "Plane.h"
#include <Math/MathFwd.h>



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
				TMatrix<T>	// yaw
				(
					TPlane<T>(+FMath::Cos(Rotator.Yaw * DE_PI / 180.f), -FMath::Sin(Rotator.Yaw * DE_PI / 180.f),	0.f, 0.f),
					TPlane<T>(+FMath::Sin(Rotator.Yaw * DE_PI / 180.f), +FMath::Cos(Rotator.Yaw * DE_PI / 180.f),	0.f, 0.f),
					TPlane<T>(0.f,										0.f,										1.f, 0.f),
					TPlane<T>(0.f,										0.f,										0.f, 1.f)
				) * 
				TMatrix<T>  // pitch
				(
					TPlane<T>(+FMath::Cos(Rotator.Yaw * DE_PI / 180.f), 0.f, -FMath::Sin(Rotator.Yaw * DE_PI / 180.f), 0.f),
					TPlane<T>(0.f, 1.f, 0.f, 0.f),
					TPlane<T>(+FMath::Sin(Rotator.Pitch * DE_PI / 180.f), 0.f, +FMath::Cos(Rotator.Pitch * DE_PI / 180.f), 0.f),
					TPlane<T>(0.f, 0.f, 0.f, 1.f)
				) *
				TMatrix<T> // roll
				(
					TPlane<T>(1.f, 0.f, 0.f, 0.f),
					TPlane<T>(0.f, +FMath::Cos(Rotator.Roll * DE_PI / 180.f), +FMath::Sin(Rotator.Roll * DE_PI / 180.f), 0.f),
					TPlane<T>(0.f, -FMath::Sin(Rotator.Roll * DE_PI / 180.f), +FMath::Cos(Rotator.Roll * DE_PI / 180.f), 0.f),
					TPlane<T>(0.f, 0.f, 0.f, 1.f)
				)
			)
		{


		}
	}
}