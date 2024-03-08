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


	
	void AddRTV(const D3D12_RENDER_TARGET_VIEW_DESC& RTVDesc, int32 OutIndex);
	FD3D12Resource* GetResource() const { return Resource; }
	void SetResource(FD3D12Resource* InResource);

public:
	TSharedPtr<class FD3D12RenderTargetView> RenderTargetView;
	/*TSharedPtr<FD3D12DepthStencilView> DepthStencilView;
	TSharedPtr<FD3D12ShaderResoutrceView> ShaderResourceView;*/


private:
	D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress;
	FD3D12Resource* Resource;



};