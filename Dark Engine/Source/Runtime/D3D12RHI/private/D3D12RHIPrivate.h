#pragma once
#include "D3D12ThirdParty.h"
#include "D3D12RHI.h"
#include "D3D12Adapter.h"
#include "D3D12Viewport.h"
#include "D3D12Descriptors.h"
#include "DynamicRHI.h"
#include "RHIContext.h"
#include "D3D12Queue.h"
#include "Containers/UnordoredMap.h"




class FD3D12DynamicRHI final :public ID3D12PlatformDynamicRHI
{
	static FD3D12DynamicRHI* SingleD3D12RHI;

public:
	static FD3D12DynamicRHI* GetD3D12RHI() { return SingleD3D12RHI; }

	FD3D12DynamicRHI(const TArray<TSharedPtr<FD3D12Adapter>>& InChosenAdapters);

	~FD3D12DynamicRHI();

	FD3D12Adapter& GetAdapter(int32 Index = 0) { return *ChosenAdapters[Index]; }
	const FD3D12Adapter& GetAdapter(int32 Index = 0) const { return *(ChosenAdapters[Index]); }

	const uint64 GetNumAdapters() const { return /*ChosenAdapters.GetSize()*/0; }

	void Init() override;

	void PostInit() override;

	//virtual void PostInit();
	virtual void Shutdown() override;

	virtual TSharedPtr<FRHIViewport> RHICreateViewport(void* WindowHandle, uint32 SizeX, uint32 SizeY, bool bIsFullscreen) override;


	virtual IRHIComputeContext* RHIGetCommandContext() override;
	virtual IRHICommandContext* RHIGetDefaultContext() override;

	class FD3D12CommandContext* CreateCommandContext(FD3D12Device* InParent, ED3D12QueueType InQueueType, bool InIsDefaultContext);;

	virtual FRHITexture* RHIGetViewportBackBuffer(FRHIViewport* Viewport) override;

	virtual TRefCountPtr<FRHIComputeShader>	RHICreateComputeShader(const TArray<uint8>& Code, const FShaderBounds& Bounds);
	virtual TRefCountPtr<FRHIVertexDeclaration>	RHICreateVertexDeclaration(const FVertexDeclarationElementList& Elements) override;
	virtual TRefCountPtr<FRHIVertexShader> RHICreateVertexShader(const TArray<uint8>& Code, const FShaderBounds& Bounds) override;
	virtual TRefCountPtr<FRHIPixelShader> RHICreatePixelShader(const TArray<uint8>& Code, const FShaderBounds& Bounds) override;
	virtual TRefCountPtr<FRHIRasterizerState> RHICreateRasterizerState(const FRasterizerStateInitializer& Initializer) override;


	virtual ID3D12CommandQueue* RHIGetCommandQueue() override { return nullptr; }
	virtual ID3D12Device* RHIGetDevice() { return nullptr; }
	virtual ID3D12GraphicsCommandList* RHIGetGraphicsCommandList(uint32 InDeviceIndex) { return nullptr; }
	virtual DXGI_FORMAT RHIGetSwapChainFormat() const { return DXGI_FORMAT_UNKNOWN; }


	virtual TRefCountPtr<FRHIBuffer> RHICreateBuffer(const FRHIBufferDesc& CreateDesc, const TCHAR* DebugName, ERHIAccess InitialState);
	virtual void* RHILockBuffer(FRHIBuffer* BufferRHI, uint32 Offset, uint32 Size, EResourceLockMode LockMode);
	virtual void RHIUnlockBuffer(FRHIBuffer* BufferRHI);

	virtual TRefCountPtr<FRHIUniformBuffer>	RHICreateUniformBuffer(const void* Contents, uint32 Size, EUniformBufferUsage Usage);
	virtual void RHIUpdateUniformBuffer(FRHIUniformBuffer* BufferRHI, const void* Contents, uint32 Size);

	virtual TRefCountPtr<FRHITexture> RHICreateTexture(const FRHITextureCreateDesc& CreateDesc);
	virtual void* RHILockTexture2D(FRHITexture* TextureRHI, uint32 MipIndex, EResourceLockMode LockMode,
		uint32& DestStride, bool bLockWithinMiptail, uint64* OutLockedByteCount);
	virtual void RHIUnlockTexture2D(FRHITexture* Texture, uint32 MipIndex, bool bLockWithinMiptail);

	virtual TRefCountPtr<FRHIGraphicsPipelineState> RHICreateGraphicsPipelineState(const class FGraphicsPipelineStateInitializer& Initalizer) override;
	virtual TRefCountPtr<FRHIComputePipelineState>	RHICreateComputePipelineState(class FRHIComputeShader* InComputeShader);


#ifdef IMGUI
	FD3D12DescriptorHeap* ImGuiDescriptorHeap;
	//uint32 ImGuiDescriptorHandle = 0;
	FRHIDescriptorHandle ImGuiDescriptorHandle;
#endif

public:
	FD3D12Texture* CreateEmptyD3D12Texture(const FRHITextureCreateDesc& CreateDesc, FD3D12Device* Device);
	FD3D12Texture* CreateD3D12Texture(const FRHITextureCreateDesc& CreateDesc, FD3D12Device* Device);

	FD3D12Buffer* CreateD3D12Buffer(class FRHICommandListBase* RHICmdList, const FRHIBufferDesc& BufferDesc, ERHIAccess ResourceState, const TCHAR* DebugName);

private:
	TArray<TSharedPtr<FD3D12Adapter>> ChosenAdapters;
	D3D_FEATURE_LEVEL FeatureLevel;
	TMap<uint64, class FRHIVertexDeclaration*> VertexDeclarationMap;
	TUnordoredMap<class FGraphicsPipelineStateInitializer, TRefCountPtr<class FRHIGraphicsPipelineState>> GraphicsPipelineStateCache;
	TMap<uint64, TRefCountPtr<class FD3D12ComputePipelineState>> ComputePipelineStateCache;

};

