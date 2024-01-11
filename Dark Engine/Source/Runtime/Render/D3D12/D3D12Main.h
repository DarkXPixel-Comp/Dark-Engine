#pragma once

#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <wrl.h>


#include <d3d12.h>

#include "d3dx12.h"





#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "DirectXTK12.lib")
#pragma comment(lib, "assimp-vc143-mtd.lib")
#pragma comment(lib, "dxcompiler.lib")
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



