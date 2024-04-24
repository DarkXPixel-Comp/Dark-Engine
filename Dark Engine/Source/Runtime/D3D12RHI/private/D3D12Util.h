#pragma once
#include "D3D12Resources.h"
#include <d3d12.h>
#include <comdef.h>


static DXGI_FORMAT GDXGIFormats[EPixelFormat::Count] = 
{
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_R32G32B32A32_UINT,
	DXGI_FORMAT_B8G8R8A8_UNORM
	//DXGI_FORMAT_DE
};

static D3D12_CULL_MODE TranslateCullMode(ERasterizerCullMode CullMode)
{
	switch (CullMode)
	{
	case RCM_Back: return D3D12_CULL_MODE_BACK;
	case RCM_Forward: return D3D12_CULL_MODE_FRONT;
	case RCM_None: return D3D12_CULL_MODE_NONE;
	}
	return D3D12_CULL_MODE_NONE;
}

static D3D12_FILL_MODE TranslateFillMode(ERasterizerFillMode FillMode)
{
	switch (FillMode)
	{
	case RFM_Solid:	return D3D12_FILL_MODE_SOLID;
	case RFM_Wireframe:	return D3D12_FILL_MODE_WIREFRAME;
	}
	return D3D12_FILL_MODE_SOLID;
}


static FORCEINLINE D3D_PRIMITIVE_TOPOLOGY GetD3D12PrimitiveTopology(uint32 PrimitiveTopology)
{
	static const uint8 D3D12PrimitiveTopology[] =
	{
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		D3D_PRIMITIVE_TOPOLOGY_LINELIST,
		0,
		D3D_PRIMITIVE_TOPOLOGY_POINTLIST
	};

	return (D3D_PRIMITIVE_TOPOLOGY)D3D12PrimitiveTopology[PrimitiveTopology];
}



FORCEINLINE D3D12_RESOURCE_STATES GetD3D12ResourceState(ERHIAccess InRHIAccess)
{
	D3D12_RESOURCE_STATES State = D3D12_RESOURCE_STATE_COMMON;
	switch (InRHIAccess)
	{
	case ERHIAccess::CpuRead: return State |D3D12_RESOURCE_STATE_COPY_SOURCE;
	case ERHIAccess::Present: return State | D3D12_RESOURCE_STATE_PRESENT;
	case ERHIAccess::VertexOrIndexBuffer: return State | D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER;
	case ERHIAccess::SRVGraphics: return State | D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
	case ERHIAccess::UAVCompute:
	case ERHIAccess::UAVGraphics: return State | D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	case ERHIAccess::RTV: return State | D3D12_RESOURCE_STATE_RENDER_TARGET;
	case ERHIAccess::CopyDesc: return State | D3D12_RESOURCE_STATE_COPY_DEST;
	case ERHIAccess::DSVWrite: return State | D3D12_RESOURCE_STATE_DEPTH_WRITE;
	}

	return D3D12_RESOURCE_STATE_COMMON;
}



FORCEINLINE DXGI_FORMAT GetDXGIFormat(EPixelFormat InFormat)
{
	return GDXGIFormats[InFormat];
}
	
FORCEINLINE uint32 CalcSubresource(uint32 MipSlice, uint32 ArraySlice, uint32 MipLevels)
{
	return MipSlice + ArraySlice * MipLevels;
}


FORCEINLINE FString GetDX12ErrorMessage(HRESULT hr)
{
	return _com_error(hr).ErrorMessage();
}


FORCEINLINE FString ParseDX12Error(HRESULT hr)
{
	_com_error error(hr);
	return FString(error.ErrorMessage());
}


FORCEINLINE void VerifyD3D12Result(HRESULT hr, const ANSICHAR* Code, const ANSICHAR* Filename, uint32 Line)
{
	FString Error = Code;
	DE_LOG(D3D12RHI, Error, TEXT("%s = %s \t in %s(%i)"), *GetDX12ErrorMessage(hr), *Error, *FString(Filename), Line);
}




#define DXCall(x) {HRESULT Result = x; if(FAILED(Result)) {VerifyD3D12Result(Result, #x, __FILE__, __LINE__);}}



FORCEINLINE void SetName(ID3D12Object* const Object, const TCHAR* const Name)
{
	if (Object && Name)
	{
		Object->SetName(Name);
		return;
	}
	Object->SetName(L"NULL");



}

FORCEINLINE void SetName(FD3D12Resource* const Resource, const TCHAR* const Name)
{
	if (Resource && Name)
	{
		Resource->GetResource()->SetName(Name);
	}
}

namespace DX12
{
	FORCEINLINE void SetName(ID3D12Object* const Object, const TCHAR* const Name)
	{
		if (Object && Name)
		{
			Object->SetName(Name);
			return;
		}
		Object->SetName(L"NULL");



	}

	FORCEINLINE void SetName(FD3D12Resource* const Resource, const TCHAR* const Name)
	{
		if (Resource && Name)
		{
			Resource->GetResource()->SetName(Name);
		}
	}

}

