#pragma once
#include "Math/DarkMathUtility.h"
#include "templates/IsFloatingPoint.h"
#include "Math/Vector.h"
#include "Math/MathFwd.h"




namespace DE
{

	/**
	* Implements a container for rotation information.
	*
	* All rotation values are stored in degrees.
	*
	* The angles are interpreted as intrinsic rotations applied in the order Yaw, then Pitch, then Roll. I.e., an object would be rotated
	* first by the specified yaw around its up axis (with positive angles interpreted as clockwise when viewed from above, along -Z),
	* then pitched around its (new) right axis (with positive angles interpreted as 'nose up', i.e. clockwise when viewed along +Y),
	* and then finally rolled around its (final) forward axis (with positive angles interpreted as clockwise rotations when viewed along +X).
	*
	* Note that these conventions differ from quaternion axis/angle. UE Quat always considers a positive angle to be a left-handed rotation,
	* whereas Rotator treats yaw as left-handed but pitch and roll as right-handed.
	*
	*/

	namespace Math
	{
		template <typename T>
		struct TRotator
		{
			static_assert(TIsFloatingPoint<T>::Value, "TRotator only supports float and double types");

		public:
			using FReal = T;

			/** Rotation around the right axis (around Y axis), Looking up and down (0=Straight Ahead, +Up, -Down) */
			T Pitch;

			/** Rotation around the up axis (around Z axis), Turning around (0=Forward, +Right, -Left)*/
			T Yaw;

			/** Rotation around the forward axis (around X axis), Tilting your head, (0=Straight, +Clockwise, -CCW) */
			T Roll;

		public:
			DENGINE_API static const TRotator<T> ZeroRotator;



			TVector<T> GetForwardVector() const
			{
				TVector<T> Result;
				Result.X = -(FMath::Cos(FMath::DegreesToRadians(Yaw))) * FMath::Cos(FMath::DegreesToRadians(Pitch));
				Result.Y = -FMath::Sin(FMath::DegreesToRadians(Pitch));
				Result.Z = FMath::Sin(FMath::DegreesToRadians(Yaw)) * FMath::Cos(FMath::DegreesToRadians(Pitch));
				return Result;
			}



		};



	}
}