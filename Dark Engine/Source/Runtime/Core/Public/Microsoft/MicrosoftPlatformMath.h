#pragma once

#include <CoreTypes.h>
#include <float.h>
#include <intrin0.inl.h>
#include "Math/DarkPlatformMathSSE4.h"
#include "GenericPlatform/GenericPlatformMath.h"



struct FMicrosoftPlatformMathBase : public TDarkPlatformMathSSE4Base<FGenericPlatformMath>
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
	static FORCEINLINE bool IsNaN(float A) { return _isnan(A) != 0; }
	static FORCEINLINE bool IsNaN(double A) { return _isnan(A) != 0; }
	static FORCEINLINE bool IsFinite(float A) { return _finite(A) != 0; }
	static FORCEINLINE bool IsFinite(double A) { return _finite(A) != 0; }



#pragma intrinsic(_BitScanReverse)

	static FORCEINLINE uint32 FloorLog2(uint32 Value)
	{
		// Use BSR to return the log2 of the integer
		// return 0 if value is 0
		unsigned long BitIndex;
		return _BitScanReverse(&BitIndex, Value) ? BitIndex : 0;
   }

	static FORCEINLINE uint8 CountLeadingZeros8(uint8 Value)
	{
		unsigned long BitIndex;
		_BitScanReverse(&BitIndex, uint32(Value) * 2 + 1);
		return uint8(8 - BitIndex);
	}

#endif

};