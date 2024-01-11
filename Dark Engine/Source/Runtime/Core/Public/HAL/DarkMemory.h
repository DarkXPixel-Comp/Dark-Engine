#pragma once

#include "CoreTypes.h"
#include "Windows/WindowsPlatformMemory.h"
#include "Templates/IsPointer.h"



enum
{
	DEFAULT_ALIGNMENT = 0,
	MIN_ALIGNMENT = 8
};



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



	static void* Memmove(void* Dest, const void* Src, SIZE_T Count)
	{
		return FPlatformMemory::Memmove(Dest, Src, Count);
	}

	static int32 Memcmp(const void* Buf1, const void* Buf2, SIZE_T Count)
	{
		return FPlatformMemory::Memcmp(Buf1, Buf2, Count);

	}
	static void* Memset(void* Dest, uint8 Char, SIZE_T Count)
	{
		return FPlatformMemory::Memset(Dest, Char, Count);
	}

	static void* Memzero(void* Dest, SIZE_T Count)
	{
		return FPlatformMemory::Memzero(Dest, Count);
	}

	static void* Memcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return FPlatformMemory::Memcpy(Dest, Src, Count);
	}

	bool MemIsZero(const void* Ptr, SIZE_T Count)
	{
		uint8* Start = (uint8*)Ptr;
		uint8* End = Start + Count;

		while (Start < End)
		{
			if (*Start++ != 0)
			{
				return false;
			}
		}
		return true;
	}
	
	static void Memswap(void* Ptr1, void* Ptr2, SIZE_T Count)
	{
		return FPlatformMemory::Memswap(Ptr1, Ptr2, Count);
	}

	static void* SystemMalloc(SIZE_T Size)
	{
		return malloc(Size);
	}
	
	static void SystemFree(void* Ptr)
	{
		free(Ptr);
	}
	

	static CORE_API void* Malloc(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	static CORE_API void* Realloc(void* Ptr, SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT);
	static CORE_API void Free(void* Ptr);
	static CORE_API	SIZE_T GetAllocSize(void* Ptr);

	static void* MallocZero(SIZE_T Count, uint32 Alignment = DEFAULT_ALIGNMENT)
	{
		void* Ptr = Malloc(Count, Alignment);
		Memzero(Ptr, Count);
		return Ptr;
	}



};