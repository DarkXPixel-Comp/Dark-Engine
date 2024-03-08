#include "DarkMemory.h"
#include "PlatformMath.h"


struct FAllocationData
{
	/** Pointer to the full allocation. Needed so the OS knows what to free. */
	void* FullAllocationPointer;
	/** Full size of the allocation including the extra page. */
	SIZE_T	FullSize;
	/** Size of the allocation requested. */
	SIZE_T	Size;

	SIZE_T	Sentinel;
}



void* FMemory::Realloc(void* Ptr, SIZE_T Count, uint32 Alignment)
{
	void* ptr = Malloc(Count, Alignment);
	if (!ptr)
	{
		return nullptr;
	}

	if (Ptr)
	{
		SIZE_T OriginalCount = GetAllocSize(Ptr);
		SIZE_T CopyCount = FPlatformMath::Min(Count, OriginalCount);
		Memcpy(ptr, Ptr, CopyCount);
		Free(Ptr);
	}

	return ptr;
}

void FMemory::Free(void* Ptr)
{
	if (!Ptr)
	{
		return;
	}

	SystemFree(Ptr);
}

inline CORE_API SIZE_T FMemory::GetAllocSize(void* Ptr)
{
	SIZE_T Result;

	if (Ptr == nullptr)
	{
		Result = 0;
	}
	else
	{
		FAllocationData* AllocDataPtr = reinterpret_cast<FAllocationData*>(Ptr);
		AllocDataPtr--;
		Result = AllocDataPtr->Size;
	}
	return Result;
}

