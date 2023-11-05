#pragma once

#include "CoreTypes.h"
//#include "GenericPlatform/GenericPlatformMemory"
#include "Templates/IsPointer.h"



struct FMemory
{
	enum AllocationHints
	{
		None = -1,
		Default,
		Temporary,
		SmallPool,

		Max
	};


	static FORCEINLINE void* Memmove(void* Dest, const void* Src, SIZE_T Count)
	{
		FPlatformMemory::Memmove(Dest, Src, Count);

	}

	static FORCEINLINE int32 Memcmp(const void* Buf1, const void* Buf2, SIZE_T Count)
	{

	}
	static FORCEINLINE void* Memset(const void* Dest, uint8 Char, SIZE_T Count)
	{

	}








};