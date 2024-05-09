#pragma once
#include "Templates/IsFloatingPoint.h"
#include "Plane.h"



namespace DE
{
	namespace Math
	{
		template <typename T>
		struct alignas(16) TMatrix
		{
			static_assert(TIsFloatingPoint<T>::Value, "T must be float or double");

		public:
			using FReal = T;

			union
			{
				alignas(16)	T Matrix[4][4];
				alignas(16)	T M[4][4];
			};

			static const TMatrix Identity;

			FORCEINLINE TMatrix()
			{}

			explicit TMatrix   (T M00, T M01, T M02, T M03,
								T M10, T M11, T M12, T M13,
								T M20, T M21, T M22, T M23,
								T M30, T M31, T M32, T M33)
			{
				M[0][0] = M00; M[0][1] = M01; M[0][2] = M02; M[0][3] = M03;
				M[1][0] = M10; M[1][1] = M11; M[1][2] = M12; M[1][3] = M13;
				M[2][0] = M20; M[2][1] = M21; M[2][2] = M22; M[2][3] = M23;
				M[3][0] = M30; M[3][1] = M31; M[3][2] = M32; M[3][3] = M33;
			}

			FORCEINLINE TMatrix(const TPlane<T>& InX, const TPlane<T>& InY, const TPlane<T>& InZ, const TPlane<T>& InW)
			{
				M[0][0] = InX.X; M[0][1] = InX.Y; M[0][2] = InX.Z; M[0][3] = InX.W;
				M[1][0] = InY.X; M[1][1] = InY.Y; M[1][2] = InY.Z; M[1][3] = InY.W;
				M[2][0] = InZ.X; M[2][1] = InZ.Y; M[2][2] = InZ.Z; M[2][3] = InZ.W;
				M[3][0] = InW.X; M[3][1] = InW.Y; M[3][2] = InW.Z; M[3][3] = InW.W;
			}
			


			FORCEINLINE TMatrix<T> operator*(const TMatrix<T>& Other) const
			{
				TMatrix<T> Result;


				return Result;
			}


		};

	}
}