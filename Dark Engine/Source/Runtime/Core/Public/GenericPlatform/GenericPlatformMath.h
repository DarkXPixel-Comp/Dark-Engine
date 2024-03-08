#pragma once
#include <CoreMinimal.h>

#include <cstdlib>



struct FGenericPlatformMath
{
	static constexpr FORCEINLINE int32 TruncToInt32(float F)
	{
		return (int32)F;
	}
	static constexpr FORCEINLINE int32 TruncToInt32(double F)
	{
		return (int32)F;
	}
	static constexpr FORCEINLINE int64 TruncToInt64(double F)
	{
		return (int64)F;
	}

	static constexpr FORCEINLINE int32 TruncToInt(float F) { return TruncToInt32(F); }
	static constexpr FORCEINLINE int64 TruncToInt(double F) { return TruncToInt64(F); }



	template <class T>
	static constexpr FORCEINLINE T Max(const T A, const T B)
	{
		return B < A ? A : B;
	}

	template <class T>
	static constexpr FORCEINLINE T AbsMax(const T A, const T B)
	{
		return Max(Abs(A), Abs(B));
	}
	
	template <class T> 
	static constexpr FORCEINLINE T Abs(const T V)
	{
		return V < 0 ? -V : V;
	}
	
	template <class T>
	static constexpr FORCEINLINE T Min(const T A, const T B)
	{
		return A < B ? A : B;
	}
	template <class T>
	static constexpr FORCEINLINE T AbsMin(const T A, const T B)
	{
		return Min(Abs(A), Abs(B));
	}

	template <class T>
	static constexpr T Sqrt(T V)
	{
		return sqrt(V);
	}

	template <class T>
	static constexpr T InvSqrt(T V)
	{
		return 1 / sqrt(V);
	}
	
	template <class T>
	static constexpr T Pow(T A, T B)
	{
		return pow(A, B);
	}


	static FORCEINLINE int32 Rand() { return rand(); }

	static FORCEINLINE void RandInit(int32 Seed) { srand(Seed); }

	static FORCEINLINE float FRand()
	{
		constexpr int32 RandMax = 0x00ffffff < RAND_MAX ? 0x00ffffff : RAND_MAX;
		return (Rand() & RandMax) / (float)RandMax;
	}



};