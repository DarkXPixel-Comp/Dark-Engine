#include "D3D12RHIPrivate.h"
#include <Misc/AssertionMacros.h>
#include "RHICommandList.h"
#include "D3D12CommandContext.h"
#include "imgui_impl_dx12.h"
#include <D3D12Util.h>


FD3D12DynamicRHI* FD3D12DynamicRHI::SingleD3D12RHI = nullptr;

FD3D12DynamicRHI::FD3D12DynamicRHI(const TArray<TSharedPtr<FD3D12Adapter>>& InChosenAdapters)
	: ChosenAdapters(InChosenAdapters)
{
	if (SingleD3D12RHI)
	{
		return;
	}

	SingleD3D12RHI = this;

	FeatureLevel = GetAdapter().GetFeatureLevel();




}

FD3D12DynamicRHI::~FD3D12DynamicRHI()
{
}

void FD3D12DynamicRHI::Init()
{
	DE_LOG(D3D12RHI, Log, TEXT("Start init"));

	for (auto& Adapter : ChosenAdapters)
	{
		check(Adapter->GetDesc().IsValid());
		Adapter->InitializeDevices();
	}

	FD3D12Adapter& Adapter = GetAdapter();

	ImGuiDescriptorHeap = CreateDescriptorHeap(
		Adapter.GetDevice(),
		TEXT("ImGuiDescriptorHeap"),
		ERHIDescriptorHeapType::Standart,
		1,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
	);
	DE_LOG(D3D12RHI, Log, TEXT("Create IMGUI Heap"));


	GRHICommandList.GetImmediateCommandList().InitializeContexts();

	DE_LOG(D3D12RHI, Log, TEXT("Initialize Contexts"));

	DE_LOG(D3D12RHI, Log, TEXT("Finish init"));
}

TSharedPtr<FRHIViewport> FD3D12DynamicRHI::RHICreateViewport(void* WindowHandle, uint32 SizeX, uint32 SizeY, bool bIsFullscreen)
{
	FD3D12Viewport* RenderingViewport = new FD3D12Viewport(&GetAdapter(), HWND(WindowHandle), SizeX, SizeY,
		bIsFullscreen, EPixelFormat::PF_R8G8B8A8);
	RenderingViewport->Init();

#ifdef IMGUI
	ImGui_ImplDX12_Init(GetAdapter().GetD3DDevice(), RenderingViewport->GetCountBackBuffers(),
		FD3D12Viewport::GetRenderTargetFormat(
			EPixelFormat::PF_R8G8B8A8),
			ImGuiDescriptorHeap->GetHeap(),
			ImGuiDescriptorHeap->GetCPUSlotHandle(ImGuiDescriptorHandle),
			ImGuiDescriptorHeap->GetGPUSlotHandle(ImGuiDescriptorHandle));
#endif

	return MakeShareble(RenderingViewport);
}

FD3D12CommandContext* FD3D12DynamicRHI::CreateCommandContext(FD3D12Device* InParent, ED3D12QueueType InQueueType, bool InIsDefaultContext)
{
	return new FD3D12CommandContext(InParent, InQueueType, InIsDefaultContext);
}

FRHITexture* FD3D12DynamicRHI::RHIGetViewportBackBuffer(FRHIViewport* Viewport)
{
	FD3D12Viewport* D3DViewport = static_cast<FD3D12Viewport*>(Viewport);
	FRHITexture* Result = static_cast<FRHITexture*>(D3DViewport->GetCurrentBackBuffer());
	return Result;
}

TRefCountPtr<FRHITexture> FD3D12DynamicRHI::RHICreateTexture(const FRHITextureCreateDesc& CreateDesc)
{
	return CreateD3D12Texture(CreateDesc, GetAdapter().GetDevice());
}

FD3D12Texture* FD3D12DynamicRHI::CreateEmptyD3D12Texture(const FRHITextureCreateDesc& CreateDesc, FD3D12Device* Device)
{
	return new FD3D12Texture(CreateDesc, Device);
}

FD3D12Texture* FD3D12DynamicRHI::CreateD3D12Texture(const FRHITextureCreateDesc& CreateDesc, FD3D12Device* Device)
{
	FD3D12Texture* NewTexture = CreateEmptyD3D12Texture(CreateDesc, Device);

	D3D12_HEAP_PROPERTIES TextureHeapProperties = {};
	TextureHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_RESOURCE_DESC TextureDesc = {};



	//Device->GetDevice()->CreateCommittedResource(&TextureHeapProperties, D3D12_HEAP_FLAG_NONE,
	//	)

	switch (CreateDesc.Dimension)
	{
	case ETextureDimension::Texture2D:
		TextureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		TextureDesc.Format = FD3D12Viewport::GetRenderTargetFormat(CreateDesc.Format);
		TextureDesc.MipLevels = CreateDesc.NumMips;
		TextureDesc.DepthOrArraySize = CreateDesc.Depth;
		TextureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		TextureDesc.Width = CreateDesc.Extent.X;
		TextureDesc.Height = CreateDesc.Extent.Y;
		TextureDesc.SampleDesc = { 1, 0 };
		break;
	case ETextureDimension::Texture2DArray:
		break;
	case ETextureDimension::Texture3D:
		break;
	case ETextureDimension::TextureCube:
		break;
	case ETextureDimension::TextureCubeArray:
		break;
	default:
		break;
	}

	TRefCountPtr<ID3D12Resource> D3DTextureResource;

	DXCall(Device->GetDevice()->CreateCommittedResource(&TextureHeapProperties, D3D12_HEAP_FLAG_NONE, &TextureDesc,
		D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&D3DTextureResource)));

	FD3D12Resource* TextureResource = new FD3D12Resource(Device, D3DTextureResource.Get(),
		D3D12_RESOURCE_STATE_COMMON, TextureDesc);
	NewTexture->SetResource(TextureResource);

	//NewTexture->




	return NewTexture;
}



IRHIComputeContext* FD3D12DynamicRHI::RHIGetCommandContext()
{
	return CreateCommandContext(GetAdapter().GetDevice(), ED3D12QueueType::Direct, false);
}

IRHICommandContext* FD3D12DynamicRHI::RHIGetDefaultContext()
{
	IRHICommandContext* DefaultCommandContext = nullptr;

	FD3D12Device* Device = GetAdapter().GetDevice();

	DefaultCommandContext = static_cast<IRHICommandContext*>(&Device->GetDefaultCommandContext());
	check(DefaultCommandContext);
	return DefaultCommandContext;
}




void FD3D12DynamicRHI::PostInit()
{
	FD3D12Adapter& Adapter = GetAdapter();
}