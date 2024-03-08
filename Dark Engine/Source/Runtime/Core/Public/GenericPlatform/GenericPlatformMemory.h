#pragma once


#include "CoreFwd.h"

#include "CoreTypes.h"
#include "HAL/PlatformString.h"

#include <string>
#include <wchar.h>

class FMalloc;
//class FString;


struct FGenericPlatformMemory
{
public:
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





	static void Init();

	static void OnOutOfMemory(uint64 Size, uint32 Alignmemnt);

	static void SetupMemoryPools();



	static CORE_API void* BinnedAllocFromOS(SIZE_T Size);



	static void* Memzero(void* Dst, SIZE_T Count) { return Memset(Dst, 0, Count); }


	static FORCEINLINE void* Memmove(void* Dst, const void* Src, SIZE_T Count) { return memmove(Dst, Src, Count); }
	static int32 Memcmp(const void* Buf1, const void* Buf2, SIZE_T Count) { return memcmp(Buf1, Buf2, Count); }
	static void* Memset(void* Dst, uint8 Char, SIZE_T Count) { return memset(Dst, Char, Count); }
	static void* Memcpy(void* Dst, const void* Src, SIZE_T Count) { return memcpy(Dst, Src, Count); }


	static void Memswap(void* Ptr1, void* Ptr2, SIZE_T Size)
	{
		switch (Size)
		{
		case 0:
			break;
		case 1:
			Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
			break;
		case 2:
			Valswap(*(uint16*)Ptr1, *(uint16*)Ptr2);
			break;
		case 3:
			Valswap(*((uint16*&)Ptr1)++, *((uint16*&)Ptr2)++);
			Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
			break;
		case 4:
			Valswap(*(uint32*)Ptr1, *(uint32*)Ptr2);
			break;

		case 5:
			Valswap(*((uint32*&)Ptr1)++, *((uint32*&)Ptr2)++);
			Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
			break;

		case 6:
			Valswap(*((uint32*&)Ptr1)++, *((uint32*&)Ptr2)++);
			Valswap(*(uint16*)Ptr1, *(uint16*)Ptr2);
			break;

		case 7:
			Valswap(*((uint32*&)Ptr1)++, *((uint32*&)Ptr2)++);
			Valswap(*((uint16*&)Ptr1)++, *((uint16*&)Ptr2)++);
			Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
			break;

		case 8:
			Valswap(*(uint64*)Ptr1, *(uint64*)Ptr2);
			break;

		case 16:
			Valswap(((uint64*)Ptr1)[0], ((uint64*)Ptr2)[0]);
			Valswap(((uint64*)Ptr1)[1], ((uint64*)Ptr2)[1]);
			break;
		default:
			break;
		}

	}


private:
	template<typename T>
	static void Valswap(T& A, T& B)
	{
		T Tmp = A;
		A = B;
		B = Tmp;
	}
	




};