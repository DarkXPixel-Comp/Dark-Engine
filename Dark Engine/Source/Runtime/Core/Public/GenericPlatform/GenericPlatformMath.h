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


	template <class T1, class T2>
	static constexpr T1 Pow(T1 A, T2 B)
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


	static FORCEINLINE float Tan(float Value) { return tanf(Value); }
	static FORCEINLINE double Tan(double Value) { return tan(Value); }

	static FORCEINLINE float CoTan(float Value) { return 1.f / Tan(Value); }
	static FORCEINLINE double CoTan(double Value) { return 1.0 / Tan(Value); }

	static FORCEINLINE float Sin(float Value) { return sinf(Value); }
	static FORCEINLINE double Sin(double Value) { return sin(Value); }

	static FORCEINLINE float Cos(float Value) { return cosf(Value); }
	static FORCEINLINE double Cos(double Value) { return cos(Value); }
};