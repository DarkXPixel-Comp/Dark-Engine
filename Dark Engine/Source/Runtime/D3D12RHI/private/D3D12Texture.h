#pragma once


#include "D3D12Resources.h"
#include "RHIResources.h"





class FD3D12Texture : public FRHITexture, public FD3D12DeviceChild
{
public:
	FD3D12Texture() = delete;

	FD3D12Texture(const FRHITextureCreateDesc& InDesc,
		class FD3D12Device* InParent):
		FRHITexture(InDesc),
		FD3D12DeviceChild(InParent)
	{}


private:
	D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress;
	FD3D12Resource* Resource;

};