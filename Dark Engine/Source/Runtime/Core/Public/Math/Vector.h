#pragma once
#include <Core.h>
#include <CoreTypes.h>
#include <Math/MathFwd.h>
#include <Math/Vector2D.h>





namespace DE
{
	namespace Math
	{
		template<typename T>
		struct TVector
		{
			static_assert(std::is_floating_point_v<T>, "T must be floating point");

		public:
			using FDark = T;
			union
			{
				struct
				{
					T X;
					T Y;
					T Z;
				};
				T XYZ[3];
			};

			/** A zero vector (0, 0, 0) */
			DENGINE_API static const TVector<T> ZeroVector;

			/** A zero vector (0, 0, 0) */
			DENGINE_API static const TVector<T> OneVector;

			/** A zero vector (0, 0, 0) */
			DENGINE_API static const TVector<T> UpVector;

			/** A zero vector (0, 0, 0) */
			DENGINE_API static const TVector<T> DownVector;

			/** A zero vector (0, 0, 0) */
			DENGINE_API static const TVector<T> ForwardVector;

			/** A zero vector (0, 0, 0) */
			DENGINE_API static const TVector<T> BackwardVector;

			/** A zero vector (0, 0, 0) */
			DENGINE_API static const TVector<T> RightVector;

			/** A zero vector (0, 0, 0) */
			DENGINE_API static const TVector<T> LeftVector;

			/** A zero vector (0, 0, 0) */
			DENGINE_API static const TVector<T> XAxisVector;

			/** A zero vector (0, 0, 0) */
			DENGINE_API static const TVector<T> YAxisVector;

			/** A zero vector (0, 0, 0) */
			DENGINE_API static const TVector<T> ZAxisVector;


			static inline TVector<T> Zero() { return ZeroVector; }
			static inline TVector<T> One() { return OneVector; }
			static inline TVector<T> UnitX() { return XAxisVector; }
			static inline TVector<T> UnitY() { return YAxisVector; }
			static inline TVector<T> UnitZ() { return ZAxisVector; }

		public:
			/** Default constructor (no initialization). */
			TVector();

			/**
			* Constructor initializing all components to a single T value.
			*
			* @param InF Value to set all components to.
			*/
			explicit TVector(T InF);

			/**
			 * Constructor using initial values for each component.
			 *
			 * @param InX X Coordinate.
			 * @param InY Y Coordinate.
			 * @param InZ Z Coordinate.
			 */

			explicit TVector(T InX, T InY, T InZ);

			/**
		 * Constructs a vector from an TVector2<T> and Z value.
		 *
		 * @param V Vector to copy from.
		 * @param InZ Z Coordinate.
		 */

			explicit TVector(const TVector2<T> V, T InZ);

			explicit TVector(const TVector2<T> V1, TVector2<T> V2);


		};

	}
}



typedef DE::Math::TVector<float> FVector;