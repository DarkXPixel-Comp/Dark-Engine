#pragma once
#include <Core.h>
#include <CoreTypes.h>
#include <Math/MathFwd.h>
#include <Math/Vector2D.h>
#include <Math/DarkMathUtility.h>
#include "Containers/String/DarkString.h"
#include "Templates/DarkTypeTraits.h"
#include <type_traits>





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

#if ENABLE_NAN_DIAGNOSTIC
			FORCEINLINE void DiagnosticCheckNaN() const
			{
				if (ContainsNaN())
				{
					return;
				}
			}
#else
			FORCEINLINE void DiagnosticCheckNaN() const {}


#endif

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

			/**
			* Calculate cross product between this and another vector.
			*
			* @param V The other vector.
			* @return The cross product.
			*/
			FORCEINLINE	TVector<T> operator^(const TVector<T>& V) const;

			/**
			 * Calculate cross product between this and another vector.
			 *
			 * @param V The other vector.
			 * @return The cross product.
			 */

			FORCEINLINE TVector<T> Cross(const TVector<T>& V2) const;

			/**
			* Calculate the cross product of two vectors.
			*
			* @param A The first vector.
			* @param B The second vector.
			* @return The cross product.
			*/
			FORCEINLINE static TVector<T> CrossProduct(const TVector<T>& A, const TVector<T>& B);

			/**
			 * Calculate the dot product between this and another vector.
			 *
			 * @param V The other vector.
			 * @return The dot product.
			 */
			FORCEINLINE T operator|(const TVector<T>& V) const;

			/**
			 * Calculate the dot product between this and another vector.
			 *
			 * @param V The other vector.
			 * @return The dot product.
			 */
			FORCEINLINE T Dot(const TVector<T>& V) const;

			FORCEINLINE static T DotProduct(const TVector<T>& A, const TVector<T>& B);

			/**
			 * Gets the result of component-wise addition of this and another vector.
			 *
			 * @param V The vector to add to this.
			 * @return The result of vector addition.
			 */
			FORCEINLINE TVector<T> operator+(const TVector<T>& V) const;

			FORCEINLINE TVector<T> operator-(const TVector<T>& V) const;

			template<typename FArg, TEMPLATE_REQUIRES(std::is_arithmetic<FArg>::value)>
			FORCEINLINE TVector<T> operator-(FArg Bias)
			{
				return TVector<T>(X - (T)Bias, Y - (T)Bias, Z - T(Bias));
			}


			template<typename FArg, TEMPLATE_REQUIRES(std::is_arithmetic<FArg>::value)>
			FORCEINLINE TVector<T> operator*(FArg Scale) const
			{
				return TVector<T>(X * (T)Scale, Y * (T)Scale, Z * (T)Scale);
			}	
			template<typename FArg, TEMPLATE_REQUIRES(std::is_arithmetic<FArg>::value)>
			FORCEINLINE TVector<T> operator/(FArg Scale) const
			{
				const T RScale = T(1) / Scale;
				return TVector<T>(X * (T)RScale, Y * (T)RScale, Z * (T)RScale);
			}

			FORCEINLINE TVector<T> operator*(const TVector<T>& V) const;

			FORCEINLINE TVector<T> operator/(const TVector<T>& V) const;

			bool operator==(const TVector<T>& V) const;

			bool operator!=(const TVector<T>& V) const;

			bool Equals(const TVector<T>& V, T Tolerance = DE_KINDA_SMALL_NUMBER) const;

			bool AllComponentEqual(T Tolerance = DE_KINDA_SMALL_NUMBER) const;

			FORCEINLINE TVector<T> operator+=(const TVector<T>& V);
			
			FORCEINLINE TVector<T> operator-=(const TVector<T>& V);

			T& operator[](int32 Index);

			T operator[](int32 Index) const;

			T& Component(int32 Index);

			T Component(int32 Index) const;



		public:
			bool ContainsNaN() const;


			FString	ToString() const;





			template<typename FArg, TEMPLATE_REQUIRES(std::is_arithmetic<FArg>::value)>
			FORCEINLINE TVector<T> operator*=(FArg Scale)
			{
				X *= Scale; Y *= Scale; Z *= Scale;
				DiagnosticCheckNaN();
				return *this;
			}
			template<typename FArg, TEMPLATE_REQUIRES(std::is_arithmetic<FArg>::value)>
			FORCEINLINE TVector<T> operator/=(FArg Scale)
			{
				const T RV = (T)1 / Scale;
				X *= Scale; Y *= Scale; Z *= Scale;
				DiagnosticCheckNaN();
				return *this;
			}




		};

		template<typename T>
		FORCEINLINE TVector<T>::TVector()
		{}
		
		template<typename T>
		FORCEINLINE TVector<T>::TVector(T InX, T InY, T InZ)
			: X(InX), Y(InY), Z(InZ)
		{
			DiagnosticCheckNaN();
		}

		template<typename T>
		FORCEINLINE DE::Math::TVector<T>::TVector(const TVector2<T> V, T InZ)
			: X(V.X), Y(V.Y), Z(InZ)
		{
			DiagnosticCheckNaN();
		}

		template<typename T>
		FORCEINLINE DE::Math::TVector<T>::TVector(T InF)
			: X(InF), Y(InF), Z(InF)
		{
		}


		template<typename T>
		FORCEINLINE TVector<T> TVector<T>::operator+(const TVector<T>& V) const
		{
			return TVector<T>(X + V.X, Y + V.Y, Z + V.Z);
		}


		template <typename T>
		FORCEINLINE bool TVector<T>::ContainsNaN() const
		{
			return	(!FMath::IsFinite(X) ||
					 !FMath::IsFinite(Y) ||
					 !FMath::IsFinite(Z));
		}



	}

}


