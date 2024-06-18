#pragma once
#include "MathSSE.h"
#include "Vector.h"
#include "Rotator.h"



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
				Location = { 0, 0, 0, 0 };
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

			FORCEINLINE TMatrix<T> ToMatrixWithScale() const
			{
				TMatrix<T> OutMatrix;



				return OutMatrix;
			}
		};
	}
}

