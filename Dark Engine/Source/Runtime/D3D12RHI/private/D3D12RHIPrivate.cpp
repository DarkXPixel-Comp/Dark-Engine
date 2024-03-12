#include "D3D12RHIPrivate.h"
#include <Misc/AssertionMacros.h>
#include "RHICommandList.h"
#include "D3D12CommandContext.h"
#include "imgui_impl_dx12.h"


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

	GRHICommandList.GetImmediateCommandList().InitializeContexts();


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