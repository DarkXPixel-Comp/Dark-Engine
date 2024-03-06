#include "D3D12RHIPrivate.h"
#include <Misc/AssertionMacros.h>
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

}

TSharedPtr<FRHIViewport> FD3D12DynamicRHI::RHICreateViewport(void* WindowHandle, uint32 SizeX, uint32 SizeY, bool bIsFullscreen)
{
	FD3D12Viewport* RenderingViewport = new FD3D12Viewport(&GetAdapter(), HWND(WindowHandle), SizeX, SizeY,
		bIsFullscreen, EPixelFormat::PF_R8G8B8A8);
	RenderingViewport->Init();

	return MakeShareble(RenderingViewport);
}




void FD3D12DynamicRHI::PostInit()
{
#ifdef IMGUI
	//ImGui_ImplDX12_Init()
#endif

}