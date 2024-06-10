#pragma once
#include "HAL/Platform.h"


#if PLATFORM_64BITS
#pragma pack(push, 16)
#else
#pragma pack(push, 8)
#endif

#define INITGUID


#include "D3D12/d3d12.h"
#if D3D12_USING_MEMORY_ALLOCATOR
#include "D3D12MA/D3D12MemAlloc.h"
#endif
//#include <d3d12.h>
#include "d3dx12/d3dx12.h"
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include "WinPixEventRuntime/pix3.h"

#if D3D12_USING_DIRECTSR
#include "D3D12/directsr.h"
#endif // D3D12_USING_DIRECTSR




#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "dxcompiler.lib")

#undef DrawText

#pragma pack(pop)
#pragma warning(pop)
