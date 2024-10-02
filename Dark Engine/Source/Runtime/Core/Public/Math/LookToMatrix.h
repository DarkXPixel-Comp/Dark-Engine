#pragma once
#include <CoreMinimal.h>
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include <Math/MathFwd.h>

namespace DE
{
	namespace Math
	{
		template<typename T>
		struct TLookToMatrix : public TMatrix<T>
		{
		public:
			TLookToMatrix(const TVector<T>& EyePosition, const TVector<T>& EyeDirection, const TVector<T>& UpDirection)
			{
				TVector<T> R2 = EyeDirection.GetUnsafeNormal();
				TVector<T> R0 = UpDirection.Cross(R2);
				R0.Normalize();

				TVector<T> R1 = R2.Cross(R0);

				TVector<T> NegativeEyePosition = EyePosition.GetNegative();

				TVector<T> D0(R0.Dot(NegativeEyePosition));
				TVector<T> D1(R1.Dot(NegativeEyePosition));
				TVector<T> D2(R2.Dot(NegativeEyePosition));

				
				auto M = this->M;

				M[0][0] = R0[0]; M[0][1] = R0[1]; M[0][2] = R0[2]; M[0][3] = 0;
				M[1][0] = R1[0]; M[1][1] = R1[1]; M[1][2] = R1[2]; M[1][3] = 0;
				M[2][0] = R2[0]; M[2][1] = R2[1]; M[2][2] = R2[2]; M[2][3] = 0;
				M[3][0] = 0;	 M[3][1] = 0;	  M[3][2] = 0;	   M[3][3] = 1;

				this->Transpose();
			}


		};
	}
}