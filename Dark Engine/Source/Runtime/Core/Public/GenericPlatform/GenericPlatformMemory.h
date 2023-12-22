#pragma once


#include "CoreFwd.h"

#include "CoreTypes.h"
#include "HAL/PlatformString.h"

#include <string>
#include <wchar.h>

class FMalloc;
class FString;


struct FGenericPlatformMemory
{
	static DENGINE_API bool bIsOOM;

	/** Set to true if we encounters out of memory. */
	static DENGINE_API bool bIsOOM;

	/** Set to size of allocation that triggered out of memory, zero otherwise. */
	static DENGINE_API uint64 OOMAllocationSize;

	/** Set to alignment of allocation that triggered out of memory, zero otherwise. */
	static DENGINE_API uint32 OOMAllocationAlignment;

	/** Preallocated buffer to delete on out of memory. Used by OOM handling and crash reporting. */
	static DENGINE_API void* BackupOOMMemoryPool;

	/** Size of BackupOOMMemoryPool in bytes. */
	static DENGINE_API uint32 BackupOOMMemoryPoolSize;

	enum EMemoryCounterRegion
	{
		MCR_Invalid, // not memory
		MCR_Physical, // main system memory
		MCR_GPU, // memory directly a GPU (graphics card, etc)
		MCR_GPUSystem, // system memory directly accessible by a GPU
		MCR_TexturePool, // presized texture pools
		MCR_StreamingPool, // amount of texture pool available for streaming.
		MCR_UsedStreamingPool, // amount of texture pool used for streaming.
		MCR_GPUDefragPool, // presized pool of memory that can be defragmented.
		MCR_PhysicalLLM, // total physical memory including CPU and GPU
		MCR_MAX
	};

	enum EMemoryAllocatorToUse
	{
		Ansi, // Default C allocator
		Stomp, // Allocator to check for memory stomping
		TBB, // Thread Building Blocks malloc
		Jemalloc, // Linux/FreeBSD malloc
		Binned, // Older binned malloc
		Binned2, // Newer binned malloc
		Binned3, // Newer VM-based binned malloc, 64 bit only
		Platform, // Custom platform specific allocator
		Mimalloc, // mimalloc
	};


};