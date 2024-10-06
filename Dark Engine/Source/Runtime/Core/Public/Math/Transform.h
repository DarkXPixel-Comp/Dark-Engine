#pragma once
#include "MathSSE.h"
#include "Vector.h"
#include "Rotator.h"
#include "DirectXMath.h"



namespace DE
{
	namespace Math
	{
		template <typename T>
		struct TTransform
		{
		public:
			static const TTransform<T> Identity;


		protected:
			TPersistentVectorRegister<T> Rotation;
			TPersistentVectorRegister<T> Location;
			TPersistentVectorRegister<T> Scale;

		public:
			TTransform()
			{
				Rotation = { 0, 0, 0, 1 };
				Location = { 0, 0, 0, 1 };
				Scale = { 1, 1, 1, 0 };
			}

			TTransform(const TVector<T>& InLocation)
			{
				Rotation = { 0, 0, 0, 1 };
				Location = { InLocation.X, InLocation.Y, InLocation.Z, 1 };
				Scale = { 1, 1, 1, 0 };
			}

		/*	TTransform(const TVector<T>& InScale)
			{
				Rotation = { 0, 0, 0, 1 };
				Location = { 0, 0, 0, 1 };
				Scale = { InScale.X, InScale.Y, InScale.Z, 0 };
			}*/

			TTransform(const TVector<T>& InLocation, const TRotator<T>& InRotation, const TVector<T>& InScale)
			{
				/*Location = { (T)InLocation.X, (T)InLocation.Y, (T)InLocation.Z, (T)1 };
				Rotation = { (T)InRotation.Roll, (T)InRotation.Pitch, (T)InRotation.Yaw, 1 };
				Scale = { (T)InScale.X, (T)InScale.Y, (T)InScale.Z, 0 };*/
			}


			FORCEINLINE void SetLocation(const TVector<T>& NewLocation)
			{
				Location = VectorLoadFloat3(NewLocation.XYZ);
			}

			FORCEINLINE void SetRotation(const TRotator<T>& NewRotation)
			{
				Rotation = VectorLoadFloat3(NewRotation.XYZ);
			}
			
			FORCEINLINE void SetScale(const TVector<T>& NewScale)
			{
				Scale = VectorLoadFloat3(NewScale.XYZ);
			}


			FORCEINLINE TVector<T> InverseTransformPosition(const TVector<T>& Other) const
			{
				const TPersistentVectorRegister<T> InputVector = VectorLoadFloat3(Other.XYZ);

				const TPersistentVectorRegister<T> TranslatedVec = VectorSet_W0(VectorSubtract(InputVector, Location));


				TVector<T> Result;

				VectorStoreFloat3(TranslatedVec, Result.XYZ);

				return Result;
			}


			FORCEINLINE TMatrix<T> ToMatrixWithScale() const
			{
				TMatrix<T> OutMatrix;

				TMatrix<T> TranslationMatrix = TMatrix<T>::Identity;
				TMatrix<T> RotationMatrix = TMatrix<T>::Identity;
				TMatrix<T> ScaleMatrix = TMatrix<T>::Identity;

				TVector4<T>	Tr = Location.GetVector();
				TVector4<T>	R = Rotation.GetVector();
				TVector4<T>	S = Scale.GetVector();


				TranslationMatrix.M[3][0] = Tr.X;
				TranslationMatrix.M[3][1] = Tr.Y;
				TranslationMatrix.M[3][2] = Tr.Z;

				float cp = FMath::Cos(FMath::DegreesToRadians(R.X));
				float sp = FMath::Sin(FMath::DegreesToRadians(R.X));

				float cy = FMath::Cos(FMath::DegreesToRadians(R.Y));
				float sy = FMath::Sin(FMath::DegreesToRadians(R.Y));

				float cr = FMath::Cos(FMath::DegreesToRadians(R.Z));
				float sr = FMath::Sin(FMath::DegreesToRadians(R.Z));

				RotationMatrix.M[0][0] = cr * cy + sr * sp * sy;
				RotationMatrix.M[0][1] = sr * cp;
				RotationMatrix.M[0][2] = sr * sp * cy - cr * sy;

				RotationMatrix.M[1][0] = cr * sp * sy - sr * cy;
				RotationMatrix.M[1][1] = cr * cp;
				RotationMatrix.M[1][2] = sr * sy + cr * sp * cy;

				RotationMatrix.M[2][0] = cp * sy;
				RotationMatrix.M[2][1] = -sp;
				RotationMatrix.M[2][2] = cp * cy;

				
				ScaleMatrix.M[0][0] = S.X;
				ScaleMatrix.M[1][1] = S.Y;
				ScaleMatrix.M[2][2] = S.Z;

				OutMatrix = RotationMatrix * ScaleMatrix * TranslationMatrix;

				return OutMatrix;
			}

		};

	}
}

