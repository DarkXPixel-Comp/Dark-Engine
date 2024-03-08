#pragma once
#include "GenericPlatform/GenericPlatformMemory.h"



struct FWindowsPlatformMemory : public FGenericPlatformMemory
{
public :
	static CORE_API void Init();
	static uint32 GetBackMemoryPoolSize() { return 32 * 1024 * 1024; }
	static CORE_API class FMalloc* BaseAllocator();

	static CORE_API void* BinnedAllocFromOS(SIZE_T Size);











};


typedef FWindowsPlatformMemory FPlatformMemory;