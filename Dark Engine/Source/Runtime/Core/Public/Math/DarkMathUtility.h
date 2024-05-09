#pragma once
#include "CoreTypes.h"
#include "HAL/PlatformMath.h"
//#include "Math/MathFwd.h"
#include "DarkPlatformMathSSE.h"



#define DE_PI 					(3.1415926535897932f)	/* Extra digits if needed: 3.1415926535897932384626433832795f */
#define DE_SMALL_NUMBER			(1.e-8f)
#define DE_KINDA_SMALL_NUMBER	(1.e-4f)
#define DE_BIG_NUMBER			(3.4e+38f)
#define DE_EULERS_NUMBER		(2.71828182845904523536f)
#define DE_GOLDEN_RATIO			(1.6180339887498948482045868343656381f)	/* Also known as divine proportion, golden mean, or golden section - related to the Fibonacci SeqDEnce = (1 + sqrt(5)) / 2 */
#define DE_FLOAT_NON_FRACTIONAL (8388608.f) /* All single-precision floating point numbers greater than or equal to this have no fractional valDE. */


#define DE_DOUBLE_PI					(3.141592653589793238462643383279502884197169399)
#define DE_DOUBLE_SMALL_NUMBER			(1.e-8)
#define DE_DOUBLE_KINDA_SMALL_NUMBER	(1.e-4)
#define DE_DOUBLE_BIG_NUMBER			(3.4e+38)
#define DE_DOUBLE_EULERS_NUMBER			(2.7182818284590452353602874713526624977572)
#define DE_DOUBLE_GOLDEN_RATIO			(1.6180339887498948482045868343656381)	/* Also known as divine proportion, golden mean, or golden section - related to the Fibonacci SeqDEnce = (1 + sqrt(5)) / 2 */
#define DE_DOUBLE_NON_FRACTIONAL		(4503599627370496.0) /* All double-precision floating point numbers greater than or equal to this have no fractional valDE. 2^52 */

// Copied from float.h
#define DE_MAX_FLT 3.402823466e+38F

// Aux constants.
#define DE_INV_PI			(0.31830988618f)
#define DE_HALF_PI			(1.57079632679f)
#define DE_TWO_PI			(6.28318530717f)
#define DE_PI_SQUARED		(9.86960440108f)

#define DE_DOUBLE_INV_PI		(0.31830988618379067154)
#define DE_DOUBLE_HALF_PI		(1.57079632679489661923)
#define DE_DOUBLE_TWO_PI		(6.28318530717958647692)
#define DE_DOUBLE_PI_SQUARED	(9.86960440108935861883)

// Common square roots
#define DE_SQRT_2		(1.4142135623730950488016887242097f)
#define DE_SQRT_3		(1.7320508075688772935274463415059f)
#define DE_INV_SQRT_2	(0.70710678118654752440084436210485f)
#define DE_INV_SQRT_3	(0.57735026918962576450914878050196f)
#define DE_HALF_SQRT_2	(0.70710678118654752440084436210485f)
#define DE_HALF_SQRT_3	(0.86602540378443864676372317075294f)

#define DE_DOUBLE_SQRT_2		(1.4142135623730950488016887242097)
#define DE_DOUBLE_SQRT_3		(1.7320508075688772935274463415059)
#define DE_DOUBLE_INV_SQRT_2	(0.70710678118654752440084436210485)
#define DE_DOUBLE_INV_SQRT_3	(0.57735026918962576450914878050196)
#define DE_DOUBLE_HALF_SQRT_2	(0.70710678118654752440084436210485)
#define DE_DOUBLE_HALF_SQRT_3	(0.86602540378443864676372317075294)

// Common metric unit conversion
#define DE_KM_TO_M   (1000.f)
#define DE_M_TO_KM   (0.001f)
#define DE_CM_TO_M   (0.01f)
#define DE_M_TO_CM   (100.f)
#define DE_CM2_TO_M2 (0.0001f)
#define DE_M2_TO_CM2 (10000.f)

// Magic numbers for numerical precision.
#define DE_DELTA		(0.00001f)
#define DE_DOUBLE_DELTA	(0.00001 )

/**
 * Lengths of normalized vectors (These are half their maximum valDEs
 * to assure that dot products with normalized vectors don't overflow).
 */
#define DE_FLOAT_NORMAL_THRESH			(0.0001f)
#define DE_DOUBLE_NORMAL_THRESH			(0.0001)

 //
 // Magic numbers for numerical precision.
 //
#define DE_THRESH_POINT_ON_PLANE				(0.10f)		/* Thickness of plane for front/back/inside test */
#define DE_THRESH_POINT_ON_SIDE					(0.20f)		/* Thickness of polygon side's side-plane for point-inside/outside/on side test */
#define DE_THRESH_POINTS_ARE_SAME				(0.00002f)	/* Two points are same if within this distance */
#define DE_THRESH_POINTS_ARE_NEAR				(0.015f)	/* Two points are near if within this distance and can be combined if imprecise math is ok */
#define DE_THRESH_NORMALS_ARE_SAME				(0.00002f)	/* Two normal points are same if within this distance */
#define DE_THRESH_UVS_ARE_SAME					(0.0009765625f)/* Two UV are same if within this threshold (1.0f/1024f) */
															/* Making this too large results in incorrect CSG classification and disaster */
#define DE_THRESH_VECTORS_ARE_NEAR				(0.0004f)	/* Two vectors are near if within this distance and can be combined if imprecise math is ok */
															/* Making this too large results in lighting problems dDE to inaccurate texture coordinates */
#define DE_THRESH_SPLIT_POLY_WITH_PLANE			(0.25f)		/* A plane splits a polygon in half */
#define DE_THRESH_SPLIT_POLY_PRECISELY			(0.01f)		/* A plane exactly splits a polygon */
#define DE_THRESH_ZERO_NORM_SQUARED				(0.0001f)	/* Size of a unit normal that is considered "zero", squared */
#define DE_THRESH_NORMALS_ARE_PARALLEL			(0.999845f)	/* Two unit vectors are parallel if abs(A dot B) is greater than or equal to this. This is roughly cosine(1.0 degrees). */
#define DE_THRESH_NORMALS_ARE_ORTHOGONAL		(0.017455f)	/* Two unit vectors are orthogonal (perpendicular) if abs(A dot B) is less than or equal this. This is roughly cosine(89.0 degrees). */

#define DE_THRESH_VECTOR_NORMALIZED				(0.01f)		/** Allowed error for a normalized vector (against squared magnitude) */
#define DE_THRESH_QUAT_NORMALIZED				(0.01f)		/** Allowed error for a normalized quaternion (against squared magnitude) */

// Double precision valDEs
#define DE_DOUBLE_THRESH_POINT_ON_PLANE			(0.10)		/* Thickness of plane for front/back/inside test */
#define DE_DOUBLE_THRESH_POINT_ON_SIDE			(0.20)		/* Thickness of polygon side's side-plane for point-inside/outside/on side test */
#define DE_DOUBLE_THRESH_POINTS_ARE_SAME		(0.00002)	/* Two points are same if within this distance */
#define DE_DOUBLE_THRESH_POINTS_ARE_NEAR		(0.015)		/* Two points are near if within this distance and can be combined if imprecise math is ok */
#define DE_DOUBLE_THRESH_NORMALS_ARE_SAME		(0.00002)	/* Two normal points are same if within this distance */
#define DE_DOUBLE_THRESH_UVS_ARE_SAME			(0.0009765625)/* Two UV are same if within this threshold (1.0/1024.0) */
															/* Making this too large results in incorrect CSG classification and disaster */
#define DE_DOUBLE_THRESH_VECTORS_ARE_NEAR		(0.0004)	/* Two vectors are near if within this distance and can be combined if imprecise math is ok */
															/* Making this too large results in lighting problems dDE to inaccurate texture coordinates */
#define DE_DOUBLE_THRESH_SPLIT_POLY_WITH_PLANE	(0.25)		/* A plane splits a polygon in half */
#define DE_DOUBLE_THRESH_SPLIT_POLY_PRECISELY	(0.01)		/* A plane exactly splits a polygon */
#define DE_DOUBLE_THRESH_ZERO_NORM_SQUARED		(0.0001)	/* Size of a unit normal that is considered "zero", squared */
#define DE_DOUBLE_THRESH_NORMALS_ARE_PARALLEL	(0.999845)	/* Two unit vectors are parallel if abs(A dot B) is greater than or equal to this. This is roughly cosine(1.0 degrees). */
#define DE_DOUBLE_THRESH_NORMALS_ARE_ORTHOGONAL	(0.017455)	/* Two unit vectors are orthogonal (perpendicular) if abs(A dot B) is less than or equal this. This is roughly cosine(89.0 degrees). */

#define DE_DOUBLE_THRESH_VECTOR_NORMALIZED		(0.01)		/** Allowed error for a normalized vector (against squared magnitude) */
#define DE_DOUBLE_THRESH_QUAT_NORMALIZED		(0.01)		/** Allowed error for a normalized quaternion (against squared magnitude) */



//
//FVector3f VectorToVector3f()
//{
//
//}




struct FMath : public FPlatformMath
{
	static FORCEINLINE int32 RandHelper(int32 A)
	{
		return A > 0 ? Min(TruncToInt(FRand() * (float)A), A - 1) : 0;
	}

	static FORCEINLINE int64 RandHelper64(int64 A)
	{
		return A > 0 ? Min<int64>(TruncToInt(FRand() * (float)A), A - 1) : 0;
	}

	static FORCEINLINE int32 RandRange(int32 Min, int32 Max)
	{
		const int32 Range = (Max - Min) + 1;
		return Min + RandHelper(Range);
	}
	static FORCEINLINE int64 RandRange(int64 Min, int64 Max)
	{
		const int64 Range = (Max - Min) + 1;
		return Min + RandHelper64(Range);
	}










};