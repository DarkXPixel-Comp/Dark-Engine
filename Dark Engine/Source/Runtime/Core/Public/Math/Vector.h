#pragma once
#include <Core.h>
#include <CoreTypes.h>
#include <Math/MathFwd.h>
#include <Math/Vector2D.h>
#include <Math/DarkMathUtility.h>
#include "Containers/DarkString.h"
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
		public:
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


			TVector<T> operator*=(const TVector<T>& V);

			TVector<T> operator/=(const TVector<T>& V);


			T& operator[](int32 Index);

			T operator[](int32 Index) const;

			T& Component(int32 Index);

			T Component(int32 Index) const;

			/** Get a specific component of the vector, given a specific axis by enum */
			//T GetComponentForAxis(EAxis::Type Axis) const;

			 /** Set a specified componet of the vector, given a specific axis by enum */
			//void SetComponentForAxis(EAxis::Type Axis, T Component);

		public:

			void Set(T InX, T InY, T InZ);

			T GetMax() const;

			T GetAbsMax() const;

			T GetMin() const;

			T GetAbsMin() const;

			/** Gets the component-wise min of two vectors. */
			TVector<T> ComponentMin(const TVector<T>& Other) const;

			/** Gets the component-wise max of two vectors. */
			TVector<T> ComponentMax(const TVector<T>& Other) const;


			/**
* Get a copy of this vector with absolute value of each component.
*
* @return A copy of this vector with absolute value of each component.
*/
			TVector<T> GetAbs() const;

			/**
			 * Get the length (magnitude) of this vector.
			 *
			 * @return The length of this vector.
			 */
			T Size() const;

			/**
			 * Get the length (magnitude) of this vector.
			 *
			 * @return The length of this vector.
			 */
			T Length() const;

			/**
			 * Get the squared length of this vector.
			 *
			 * @return The squared length of this vector.
			 */
			T SizeSquared() const;

			/**
			 * Get the squared length of this vector.
			 *
			 * @return The squared length of this vector.
			 */
			T SquaredLength() const;

			/**
			 * Get the length of the 2D components of this vector.
			 *
			 * @return The 2D length of this vector.
			 */
			T Size2D() const;

			/**
			 * Get the squared length of the 2D components of this vector.
			 *
			 * @return The squared 2D length of this vector.
			 */
			T SizeSquared2D() const;

			/**
			 * Checks whether vector is near to zero within a specified tolerance.
			 *
			 * @param Tolerance Error tolerance.
			 * @return true if the vector is near to zero, false otherwise.
			 */
			bool IsNearlyZero(T Tolerance = DE_KINDA_SMALL_NUMBER) const;

			/**
			 * Checks whether all components of the vector are exactly zero.
			 *
			 * @return true if the vector is exactly zero, false otherwise.
			 */
			bool IsZero() const;

			/**
			 * Check if the vector is of unit length, with specified tolerance.
			 *
			 * @param LengthSquaredTolerance Tolerance against squared length.
			 * @return true if the vector is a unit vector within the specified tolerance.
			 */
			FORCEINLINE bool IsUnit(T LengthSquaredTolerance = DE_KINDA_SMALL_NUMBER) const;

			/**
			 * Checks whether vector is normalized.
			 *
			 * @return true if normalized, false otherwise.
			 */
			bool IsNormalized() const;

			/**
			 * Normalize this vector in-place if it is larger than a given tolerance. Leaves it unchanged if not.
			 *
			 * @param Tolerance Minimum squared length of vector for normalization.
			 * @return true if the vector was normalized correctly, false otherwise.
			 */
			bool Normalize(T Tolerance = DE_SMALL_NUMBER);

			/**
			 * Calculates normalized version of vector without checking for zero length.
			 *
			 * @return Normalized version of vector.
			 * @see GetSafeNormal()
			 */
			FORCEINLINE TVector<T> GetUnsafeNormal() const;

			/**
			 * Gets a normalized copy of the vector, checking it is safe to do so based on the length.
			 * Returns zero vector by default if vector length is too small to safely normalize.
			 *
			 * @param Tolerance Minimum squared vector length.
			 * @return A normalized copy if safe, ResultIfZero otherwise.
			 */
			TVector<T> GetSafeNormal(T Tolerance = DE_SMALL_NUMBER, const TVector<T>& ResultIfZero = ZeroVector) const;

			/**
			 * Gets a normalized copy of the 2D components of the vector, checking it is safe to do so. Z is set to zero.
			 * Returns zero vector by default if vector length is too small to normalize.
			 *
			 * @param Tolerance Minimum squared vector length.
			 * @return Normalized copy if safe, otherwise returns ResultIfZero.
			 */
			TVector<T> GetSafeNormal2D(T Tolerance = DE_SMALL_NUMBER, const TVector<T>& ResultIfZero = ZeroVector) const;

			/**
			 * Util to convert this vector into a unit direction vector and its original length.
			 *
			 * @param OutDir Reference passed in to store unit direction vector.
			 * @param OutLength Reference passed in to store length of the vector.
			 */
			void ToDirectionAndLength(TVector<T>& OutDir, double& OutLength) const;
			void ToDirectionAndLength(TVector<T>& OutDir, float& OutLength) const;

			/**
			 * Get a copy of the vector as sign only.
			 * Each component is set to +1 or -1, with the sign of zero treated as +1.
			 *
			 * @param A copy of the vector with each component set to +1 or -1
			 */
			FORCEINLINE TVector<T> GetSignVector() const;

			/**
			 * Projects 2D components of vector based on Z.
			 *
			 * @return Projected version of vector based on Z.
			 */
			TVector<T> Projection() const;

			/**
			* Calculates normalized 2D version of vector without checking for zero length.
			*
			* @return Normalized version of vector.
			* @see GetSafeNormal2D()
			*/
			FORCEINLINE TVector<T> GetUnsafeNormal2D() const;

			/**
			 * Gets a copy of this vector snapped to a grid.
			 *
			 * @param GridSz Grid dimension.
			 * @return A copy of this vector snapped to a grid.
			 * @see FMath::GridSnap()
			 */
			TVector<T> GridSnap(const T& GridSz) const;

			/**
			 * Get a copy of this vector, clamped inside of a cube.
			 *
			 * @param Radius Half size of the cube.
			 * @return A copy of this vector, bound by cube.
			 */
			TVector<T> BoundToCube(T Radius) const;

			/** Get a copy of this vector, clamped inside of a cube. */
			TVector<T> BoundToBox(const TVector<T>& Min, const TVector<T>& Max) const;

			/** Create a copy of this vector, with its magnitude clamped between Min and Max. */
			TVector<T> GetClampedToSize(T Min, T Max) const;

			/** Create a copy of this vector, with the 2D magnitude clamped between Min and Max. Z is unchanged. */
			TVector<T> GetClampedToSize2D(T Min, T Max) const;

			/** Create a copy of this vector, with its maximum magnitude clamped to MaxSize. */
			TVector<T> GetClampedToMaxSize(T MaxSize) const;

			/** Create a copy of this vector, with the maximum 2D magnitude clamped to MaxSize. Z is unchanged. */
			TVector<T> GetClampedToMaxSize2D(T MaxSize) const;

			/**
			 * Add a vector to this and clamp the result in a cube.
			 *
			 * @param V Vector to add.
			 * @param Radius Half size of the cube.
			 */
			void AddBounded(const TVector<T>& V, T Radius = MAX_int16);

			/**
			 * Gets the reciprocal of this vector, avoiding division by zero.
			 * Zero components are set to BIG_NUMBER.
			 *
			 * @return Reciprocal of this vector.
			 */
			TVector<T> Reciprocal() const;

			/**
			 * Check whether X, Y and Z are nearly equal.
			 *
			 * @param Tolerance Specified Tolerance.
			 * @return true if X == Y == Z within the specified tolerance.
			 */
			bool IsUniform(T Tolerance = DE_KINDA_SMALL_NUMBER) const;

			/**
			 * Mirror a vector about a normal vector.
			 *
			 * @param MirrorNormal Normal vector to mirror about.
			 * @return Mirrored vector.
			 */
			TVector<T> MirrorByVector(const TVector<T>& MirrorNormal) const;

			/**
			 * Mirrors a vector about a plane.
			 *
			 * @param Plane Plane to mirror about.
			 * @return Mirrored vector.
			 */
			//TVector<T> MirrorByPlane(const TPlane<T>& Plane) const;

			/**
			 * Rotates around Axis (assumes Axis.Size() == 1).
			 *
			 * @param AngleDeg Angle to rotate (in degrees).
			 * @param Axis Axis to rotate around.
			 * @return Rotated Vector.
			 */
			TVector<T> RotateAngleAxis(const T AngleDeg, const TVector<T>& Axis) const;

			/**
			 * Rotates around Axis (assumes Axis.Size() == 1).
			 *
			 * @param AngleRad Angle to rotate (in radians).
			 * @param Axis Axis to rotate around.
			 * @return Rotated Vector.
			 */
			TVector<T> RotateAngleAxisRad(const T AngleRad, const TVector<T>& Axis) const;

			/**
			 * Returns the cosine of the angle between this vector and another projected onto the XY plane (no Z).
			 *
			 * @param B the other vector to find the 2D cosine of the angle with.
			 * @return The cosine.
			 */
			FORCEINLINE T CosineAngle2D(TVector<T> B) const;

			/**
			 * Gets a copy of this vector projected onto the input vector.
			 *
			 * @param A	Vector to project onto, does not assume it is normalized.
			 * @return Projected vector.
			 */
			FORCEINLINE TVector<T> ProjectOnTo(const TVector<T>& A) const;

			/**
			 * Gets a copy of this vector projected onto the input vector, which is assumed to be unit length.
			 *
			 * @param  Normal Vector to project onto (assumed to be unit length).
			 * @return Projected vector.
			 */
			FORCEINLINE TVector<T> ProjectOnToNormal(const TVector<T>& Normal) const;

			/**
			 * Return the TRotator orientation corresponding to the direction in which the vector points.
			 * Sets Yaw and Pitch to the proper numbers, and sets Roll to zero because the roll can'T be determined from a vector.
			 *
			 * @return TRotator from the Vector's direction, without any roll.
			 * @see ToOrientationQuat()
			 */


			bool ContainsNaN() const;

			FString	ToString() const;



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


