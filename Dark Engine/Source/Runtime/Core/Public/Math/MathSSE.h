#pragma once
#include "Core.h"

//#include "Math/MathFwd.h"
#include <xmmintrin.h>
#include <emmintrin.h>

#include "Vector4.h"


typedef __m128 VectorRegister4Float;
typedef __m128d	VectorRegister2Double;




#define SHUFFLEMASK(A0,A1,B2,B3) ( (A0) | ((A1)<<2) | ((B2)<<4) | ((B3)<<6) )

#define SHUFFLEMASK2(A0,A1) ((A0) | ((A1)<<1))


struct alignas(16) VectorRegister4Double
{
	__m128d XY;
	__m128d ZW;

	FORCEINLINE VectorRegister4Double() = default;

	FORCEINLINE VectorRegister4Double(const __m128d& InXY, const __m128d& InZW)
	{
		XY = InXY;
		ZW = InZW;
	}

	FORCEINLINE VectorRegister4Double(const VectorRegister4Float& FloatVector)
	{
		XY = _mm_cvtps_pd(FloatVector);
		ZW = _mm_cvtps_pd(_mm_movehl_ps(FloatVector, FloatVector));
	}

	FORCEINLINE VectorRegister4Double& operator=(const VectorRegister4Float& FloatVector)
	{
		XY = _mm_cvtps_pd(FloatVector);
		ZW = _mm_cvtps_pd(_mm_movehl_ps(FloatVector, FloatVector));
		return *this;
	}

	FORCEINLINE FVector4d GetVector() const
	{
		const FVector4d* Result = reinterpret_cast<const FVector4d*>(this);
		return *Result;
	}

};


template<typename T>
//using TPersistentVectorRegister = VectorRegister4Double;
using TPersistentVectorRegister = std::conditional_t<std::is_same_v<T, float>, VectorRegister4Float, std::conditional_t<std::is_same_v<T, double>, VectorRegister4Double, void>>;




template <int Index>
FORCEINLINE VectorRegister2Double VectorReplicateImpl2(const VectorRegister2Double& Vec)
{
	// Note: 2 doubles (VectorRegister2Double / m128d)
	return _mm_shuffle_pd(Vec, Vec, SHUFFLEMASK2(Index, Index));
}

template <int Index>
FORCEINLINE VectorRegister4Double VectorReplicateImpl4(const VectorRegister4Double& Vec)
{
	if constexpr (Index <= 1)
	{
		VectorRegister2Double Temp = VectorReplicateImpl2<Index>(Vec.XY);
		return VectorRegister4Double(Temp, Temp);
	}
	else
	{
		VectorRegister2Double Temp = VectorReplicateImpl2<Index - 2>(Vec.ZW);
		return VectorRegister4Double(Temp, Temp);
	}
}


FORCEINLINE VectorRegister4Float VectorLoad(const float* Ptr)
{
	return _mm_loadu_ps((float*)(Ptr));
}

FORCEINLINE VectorRegister4Double VectorLoad(const double* Ptr)
{
	VectorRegister4Double Result;
	Result.XY = _mm_loadu_pd((double*)(Ptr));
	Result.ZW = _mm_loadu_pd((double*)(Ptr + 2));
	return Result;
}

FORCEINLINE void VectorStoreFloat3(const VectorRegister4Float& Vec, float Ptr[3])
{
	float* F = (float*)&Vec;

	Ptr[0] = F[0];
	Ptr[1] = F[1];
	Ptr[2] = F[2];
}

FORCEINLINE void VectorStoreFloat3(const VectorRegister4Double& Vec, double Ptr[3])
{
	double* F = (double*)&Vec;

	Ptr[0] = F[0];
	Ptr[1] = F[1];
	Ptr[2] = F[2];
}



FORCEINLINE VectorRegister4Float MakeVectorRegisterFloat(float X, float Y, float Z, float W)
{
	return _mm_setr_ps(X, Y, Z, W);
}

FORCEINLINE VectorRegister4Float VectorLoadFloat3(float Arr[3])
{
	return MakeVectorRegisterFloat(Arr[0], Arr[1], Arr[2], 0);
}

FORCEINLINE VectorRegister4Float VectorSubtract(const VectorRegister4Float& Vec1, const VectorRegister4Float& Vec2)
{
	return _mm_sub_ps(Vec1, Vec2);
}


FORCEINLINE VectorRegister4Double VectorSubtract(const VectorRegister4Double& Vec1, const VectorRegister4Double& Vec2)
{
	VectorRegister4Double Result;
	Result.XY = _mm_sub_pd(Vec1.XY, Vec2.XY);
	Result.ZW = _mm_sub_pd(Vec1.ZW, Vec2.ZW);

	return Result;
}


FORCEINLINE VectorRegister4Double VectorSet_W0(const VectorRegister4Double& Vec)
{
	VectorRegister4Double Result;
	Result.XY = Vec.XY;
	Result.ZW = _mm_move_sd(_mm_setzero_pd(), Vec.ZW);

	return Result;
}

FORCEINLINE VectorRegister4Double VectorLoadFloat3(const double Arr[3])
{
	VectorRegister4Double Result;
	Result.XY = _mm_loadu_pd(Arr);
	Result.ZW = _mm_load_sd(Arr + 2);
	return Result;
}




FORCEINLINE void VectorStore(const VectorRegister4Double& Vec, double* Dest)
{
	_mm_storeu_pd(Dest, Vec.XY);
	_mm_storeu_pd(Dest + 2, Vec.ZW);
}

FORCEINLINE VectorRegister4Double VectorMultiply(const VectorRegister4Double& Vec1, const VectorRegister4Double& Vec2)
{
	VectorRegister4Double Result;
	Result.XY = _mm_mul_pd(Vec1.XY, Vec2.XY);
	Result.ZW = _mm_mul_pd(Vec1.ZW, Vec2.ZW);
	return Result;
}

FORCEINLINE VectorRegister4Double VectorAdd(const VectorRegister4Double& Vec1, const VectorRegister4Double& Vec2)
{
	VectorRegister4Double Result;
	Result.XY = _mm_add_pd(Vec1.XY, Vec2.XY);
	Result.ZW = _mm_add_pd(Vec1.ZW, Vec2.ZW);
	return Result;
}

FORCEINLINE VectorRegister4Double VectorMultiplyAdd(const VectorRegister4Double& A, const VectorRegister4Double& B, const VectorRegister4Double& C)
{
	return VectorAdd(VectorMultiply(A, B), C);
}


template<typename T>
void MatrixMultiply(T* Result, const T* Matrix1, const T* Matrix2);

template<typename T>
void MatrixTranspose(T* Matrix);

//__m128 VectorMultiply(const __m128& Vec1, const __m128& Vec2)
//{
//	return _mm_mul_ps(Vec1, Vec2);
//}
