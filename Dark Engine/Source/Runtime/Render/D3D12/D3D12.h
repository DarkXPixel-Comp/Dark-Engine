#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include "d3dx12.h"
#include "D3D12MemoryAllocator/D3D12MemAlloc.h"
#include <DTK12/DescriptorHeap.h>
#include <DDSTextureLoader.h>
#include <BufferHelpers.h>
#include <VertexTypes.h>
#include <CommonStates.h>
#include <GraphicsMemory.h>
#include <ResourceUploadBatch.h>
#include <Model.h>






#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "DirectXTK12.lib")
//#pragma comment(lib, "D3Dcompiler_47.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

typedef ComPtr<ID3D12Resource> Resource;

#ifndef _DEBUG
#define DXCall(x) x
#else
#define DXCall(x) if(FAILED(x)) {throw;}
#endif // _DEBUG

#define checked(x) if(!x) {	throw;	}

#define DXAlloc(x, y) new x(y)



