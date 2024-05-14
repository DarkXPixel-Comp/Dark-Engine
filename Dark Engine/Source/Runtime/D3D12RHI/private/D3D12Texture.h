#pragma once


#include "D3D12Resources.h"
#include "RHIResources.h"
#include "Containers/UnordoredMap.h"





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
	void EmplaceUAV(D3D12_UNORDERED_ACCESS_VIEW_DESC const& UAVDesc);

	FD3D12Resource* GetResource() const { return Resource.Get(); }
	void SetResource(FD3D12Resource* InResource);
	void CreateViews();

	virtual FRHIUnorderedAccessView* GetUnorderedAccessView() const
	{
		return reinterpret_cast<FRHIUnorderedAccessView*>(UnorderedAccessView.get());
	}

	virtual FIntPoint GetSize() const override;
	virtual void* GetNativeShaderResourceView() const override;

	FRHIDescriptorHandle GetBindlessHandle() const;

	class FD3D12ShaderResourceView* GetShaderResourceView() const { return ShaderResourceView.get(); }

	void* Lock(class FRHICommandListImmediate* RHICmdList, uint32 MipIndex, uint32 ArrayIndex, EResourceLockMode LockMode,
		uint32& DestStride, uint64* OutLockedByteCount = nullptr);
	void Unlock(class FRHICommandListImmediate* RHICmdList, uint32 MipIndex, uint32 ArrayIndex);
	void UpdateTexture(uint32 MipIndex, uint32 DestX, uint32 DestY, uint32 DestZ,
		const D3D12_TEXTURE_COPY_LOCATION& SourceCopyLocation);

public:
	static D3D12_RESOURCE_DESC GetResourceDescFromTextureDesc(const FRHITextureCreateDesc& InDesc);
public:
	TArray<TSharedPtr<class FD3D12RenderTargetView>> RenderTargetViews;
	TSharedPtr<class FD3D12ShaderResourceView> ShaderResourceView;
	TSharedPtr<class FD3D12UnorderedAccessView> UnorderedAccessView;


private:
	D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress;
	TRefCountPtr<FD3D12Resource> Resource;

	TMap<uint32, FD3D12LockedResource*>	LockedMap;



};