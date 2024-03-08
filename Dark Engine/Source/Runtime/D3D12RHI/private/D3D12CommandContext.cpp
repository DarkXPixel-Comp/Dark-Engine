#include "D3D12CommandContext.h"
#include "imgui_impl_dx12.h"
#include "D3D12Descriptors.h"
#include "D3D12View.h"
#include "RHIResources.h"

FD3D12ContextCommon::FD3D12ContextCommon(FD3D12Device* InDevice, ED3D12QueueType InQueueType, bool InbIsDefaultContext):
	Device(InDevice),
	QueueType(InQueueType),
	bIsDefualtContext(InbIsDefaultContext)
{}

void FD3D12ContextCommon::OpenCommandList()
{
	if (CommandAllocator == nullptr)
	{
		CommandAllocator = Device->GetCommandAllocator(QueueType);
	}

	CommandList = Device->GetCommandList(CommandAllocator);



}

void FD3D12ContextCommon::CloseCommandList()
{
	CommandList->Close();

	FD3D12Queue& Queue = Device->GetQueue(ED3D12QueueType::Direct);
	Queue.ObjectPool.Lists.Add(CommandList);
	Queue.ObjectPool.Allocators.Add(CommandAllocator); //TEMP
	CommandList = nullptr;
	CommandAllocator = nullptr;
}

void FD3D12ContextCommon::SignalManualFence(ID3D12Fence* Fence, uint64 Value)
{
	if (IsOpen())
	{
		CloseCommandList();
	}
	GetPayload(EPhase::Signal)->FencesToSignal.Emplace(Fence, Value);

}

void FD3D12ContextCommon::WaitManualFence(ID3D12Fence* Fence, uint64 Value)
{
	if (IsOpen())
	{
		CloseCommandList();
	}

	GetPayload(EPhase::Wait)->FencesToWait.Emplace(Fence, Value);
}

void FD3D12ContextCommon::Finalize(TArray<FD3D12Payload*>& OutPayloads)
{
	if (IsOpen())
	{
		CloseCommandList();
	}

	if (CommandAllocator)
	{
		GetPayload(EPhase::Signal)->AllocatorsToRelease.Add(CommandAllocator);
	}

	OutPayloads.Append(MoveTemp(Payloads));
}

void FD3D12ContextCommon::FlushCommands(ED3D12FlushFlags Flags)
{
	check(bIsDefualtContext);

	if (IsOpen())
	{
		//CloseCommandList();

		ID3D12CommandList* Lists[] = { CommandList->CommandList.Get() };
		FD3D12Queue& Queue = Device->GetQueue(ED3D12QueueType::Direct);

		CloseCommandList();

		Queue.CommandQueue->ExecuteCommandLists(1, Lists);
		Queue.WaitFrame();

		

	}
}

FD3D12CommandContextBase::FD3D12CommandContextBase(FD3D12Adapter* InParent):
	FD3D12AdapterChild(InParent)
{
}

void FD3D12CommandContextBase::RHIBeginDrawingViewport(FRHIViewport* RHIViewport, FRHITexture* RenderTargetRHI)
{
	FD3D12Viewport* Viewport = reinterpret_cast<FD3D12Viewport*>(RHIViewport);
	Parent->SetDrawingViewport(Viewport);
	


	if (RenderTargetRHI == nullptr)
	{
		RenderTargetRHI = Viewport->GetCurrentBackBuffer();
	}
}

void FD3D12CommandContextBase::RHIEndDrawingViewport(FRHIViewport *Viewport, bool bPresent, bool Vsync)
{
	FD3D12Viewport* ViewportRHI = reinterpret_cast<FD3D12Viewport*>(Viewport);

	Parent->SetDrawingViewport(nullptr);


	ViewportRHI->Present(Vsync);
}

void FD3D12CommandContext::RHIBeginFrame()
{	
	FD3D12CommandList& List = GetCommandList();
	FD3D12Viewport* Viewport = FD3D12AdapterChild::Parent->GetDrawingViewport();
	ID3D12DescriptorHeap* Heaps[] = { FD3D12DynamicRHI::GetD3D12RHI()->ImGuiDescriptorHeap->GetHeap() };
	Viewport->GetCurrentBackBuffer()->RenderTargetView->GetCpuHandle();




	List.GetGraphicsCommandList()->SetDescriptorHeaps(1, Heaps);
	Viewport->GetCurrentBackBuffer();

}

void FD3D12CommandContext::RHIEndFrame()
{
	FlushCommands();
}

void FD3D12CommandContext::RHIBeginImGui()
{
#ifdef IMGUI
	ImGui_ImplDX12_NewFrame();
#endif
}


void FD3D12CommandContext::RHIEndImGui()
{
	FD3D12CommandList& List = GetCommandList();

#ifdef IMGUI
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), List.GetGraphicsCommandList());
#endif
}

void FD3D12CommandContext::RHIBeginRenderPass(FRHIRenderPassInfo& InInfo)
{


}

void FD3D12CommandContext::SetRenderTargets(const FRHIRenderTargetView *RenderTargetsRHI, const FRHIDepthRenderTargetView *DepthStencilViewRHI)
{
	FD3D12Texture* NewDepthTexture = DepthStencilViewRHI ? (FD3D12Texture*)DepthStencilViewRHI->Texture : nullptr;


	//FD3D12DepthStencilView* DepthStencilView;


}

void FD3D12CommandContextBase::RHISetViewport(float MinX, float MinY, float MinZ, float MaxX, float MaxY, float MaxZ)
{
	D3D12_VIEWPORT Viewport = { MinX, MinY, (MaxX - MinX), (MaxY - MinY), MinZ, MaxZ };
	if (Viewport.Width > 0 && Viewport.Height > 0)
	{
	}

}

FD3D12CommandContext::FD3D12CommandContext(FD3D12Device* InParent, ED3D12QueueType QueueType, bool InIsDefaultContext):
	FD3D12ContextCommon(InParent, QueueType, InIsDefaultContext),
	FD3D12CommandContextBase(InParent->GetParentAdapter()),
	FD3D12DeviceChild(InParent)
{

}
