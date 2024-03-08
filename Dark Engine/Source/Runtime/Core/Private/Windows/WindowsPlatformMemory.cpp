#include "Windows/WindowsPlatformMemory.h"
#include <Windows.h>



void* FWindowsPlatformMemory::BinnedAllocFromOS(SIZE_T Size)
{
	void* Ptr = VirtualAlloc(NULL, Size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	return Ptr;
}