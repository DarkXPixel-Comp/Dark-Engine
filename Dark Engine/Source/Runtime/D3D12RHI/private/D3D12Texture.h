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


	
	void AddSRV(const D3D12_SHADER_RESOURCE_VIEW_DESC& SRVDesc);

	void SetNumRTV(int32 Num)
	{
		RenderTargetViews.Resize(Num);
	}

	void EmplaceRTV(D3D12_RENDER_TARGET_VIEW_DESC const& RTVDesc, int32 Index);
	void EmplaceDSV(D3D12_DEPTH_STENCIL_VIEW_DESC const& DSVDesc, int32 Index);
	void EmplaceSRV(D3D12_SHADER_RESOURCE_VIEW_DESC const& SRVDesc);



	FD3D12Resource* GetResource() const { return Resource.Get(); }

	void SetResource(FD3D12Resource* InResource);

	void CreateViews();

	static D3D12_RESOURCE_DESC GetResourceDescFromTextureDesc(const FRHITextureCreateDesc& InDesc);

	virtual void* GetNativeShaderResourceView() const override;

public:
	TArray<TSharedPtr<class FD3D12RenderTargetView>> RenderTargetViews;

	//TSharedPtr<class FD3D12RenderTargetView> RenderTargetView;
	TSharedPtr<class FD3D12ShaderResourceView> ShaderResourceView;
	/*TSharedPtr<FD3D12DepthStencilView> DepthStencilView;
	TSharedPtr<FD3D12ShaderResoutrceView> ShaderResourceView;*/


private:
	D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress;
	TRefCountPtr<FD3D12Resource> Resource;



};