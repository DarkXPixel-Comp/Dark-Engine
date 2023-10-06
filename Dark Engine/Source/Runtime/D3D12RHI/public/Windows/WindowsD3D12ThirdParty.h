#pragma once
#include "HAL/Platform.h"


#if PLATFORM_64BITS
#pragma pack(push, 16)
#else
#pragma pack(push, 8)
#endif

#define INITGUID
#include <d3d12.h>
//#include <d3dx12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>

#undef DrawText

#pragma pack(pop)
#pragma warning(pop)