#pragma once
#include "HAL/Platform.h"


#if PLATFORM_64BITS
#pragma pack(push, 16)
#else
#pragma pack(push, 8)
#endif

#define INITGUID

//extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 613; }
//
//extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = ".\\D3D12\\"; }

#include "D3D12/d3d12.h"
#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include "WinPixEventRuntime/pix3.h"



#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "dxcompiler.lib")

#undef DrawText

#pragma pack(pop)
#pragma warning(pop)
