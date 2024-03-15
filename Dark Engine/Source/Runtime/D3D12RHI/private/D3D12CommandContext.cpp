#include "D3D12CommandContext.h"
#include "imgui_impl_dx12.h"
#include "D3D12Descriptors.h"
#include "D3D12Viewport.h"
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
	Queue.ObjectPool.Allocators.Add(CommandAllocator);
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

void FD3D12ContextCommon::TransitionResource(FD3D12Resource* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After, uint32 SubResource)
{
	PIXScopedEvent(GetCommandList().GetGraphicsCommandList(), PIX_COLOR_DEFAULT, "TEST");
	if (Before == After)
	{
		Resource->SetState(After);
		return;
	}
	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(Resource->GetResource(), Before, After);
	GetCommandList().GetGraphicsCommandList()->ResourceBarrier(1, &Barrier);
	Resource->SetState(After);
}

void FD3D12ContextCommon::FlushCommands(ED3D12FlushFlags Flags)
{
	check(bIsDefualtContext);

	if (Flags == ED3D12FlushFlags::WaitForSubmission)
	{
		FD3D12Queue& Queue = Device->GetQueue(ED3D12QueueType::Direct);
		Queue.WaitFrame();
		return;
	}

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
	//auto RenderTargetsInfo = InInfo.ConvertToRenderTargetInfo();
	//float ClearColor[4] = { 0, 0, 0, 1 };
	//TArray<D3D12_RENDER_PASS_RENDER_TARGET_DESC> RenderPassRenderTargetDesc(RenderTargetsInfo.NumColorRenderTargets);
	//TArray<D3D12_RENDER_PASS_DEPTH_STENCIL_DESC> DepthStencilPassDesc(RenderTargetsInfo.NumColorRenderTargets);
	//for (int32 i = 0; i < RenderTargetsInfo.NumColorRenderTargets; ++i)
	//{
	//	FD3D12Texture* RenderTarget = static_cast<FD3D12Texture*>(RenderTargetsInfo.ColorRenderTarget[i].Texture);
	//	D3D12_RENDER_PASS_BEGINNING_ACCESS BeginningAccess;
	//	BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
	//	BeginningAccess.Clear.ClearValue.Format = FD3D12Viewport::GetRenderTargetFormat(RenderTarget->GetPixelFormat());
	//	{
	//		BeginningAccess.Clear.ClearValue.Color[0] = 0;
	//		BeginningAccess.Clear.ClearValue.Color[1] = 0;
	//		BeginningAccess.Clear.ClearValue.Color[2] = 0;
	//		BeginningAccess.Clear.ClearValue.Color[3] = 1;
	//	}


	//	RenderPassRenderTargetDesc[i].cpuDescriptor = RenderTarget->RenderTargetView->GetCpuHandle();
	//	RenderPassRenderTargetDesc[i].BeginningAccess = BeginningAccess;
	//	RenderPassRenderTargetDesc[i].EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;

	//	//DepthStencilPassDesc[i].
	//}




	//GetCommandList().GetGraphicsCommandList4()->BeginRenderPass(RenderTargetsInfo.NumColorRenderTargets, RenderPassRenderTargetDesc.GetData(), );



	//Logger::log("[D3D12RHI] Begin render pass");

	FRHISetRenderTargetInfo RTInfo = InInfo.ConvertToRenderTargetInfo();
	SetRenderTargetsAndClear(RTInfo);
}

void FD3D12CommandContext::SetRenderTargets(int32 NumRenderTargets, const FRHIRenderTargetView *RenderTargetsRHI, const FRHIDepthRenderTargetView *DepthStencilViewRHI, bool bClear)
{
	//FD3D12Texture* NewDepthTexture = DepthStencilViewRHI ? (FD3D12Texture*)DepthStencilViewRHI->Texture : nullptr;


	FD3D12RenderTargetView* NewRenderTargets[MAX_RENDER_TARGETS];
	TArray<FD3D12CpuDescriptor>	RTVDescriptors(NumRenderTargets);
	for (uint32 i = 0; i < MAX_RENDER_TARGETS; ++i)
	{
		FD3D12RenderTargetView* RenderTarget = nullptr;
		if (i < MAX_RENDER_TARGETS && RenderTargetsRHI[i].Texture != nullptr)
		{
			FD3D12Texture* NewRenderTarget = static_cast<FD3D12Texture*>(RenderTargetsRHI[i].Texture);
			RenderTarget = NewRenderTarget->RenderTargetView.get();
			TransitionResource(NewRenderTarget->GetResource(),
				NewRenderTarget->GetResource()->GetCurrentState(), D3D12_RESOURCE_STATE_RENDER_TARGET, 0);
			RTVDescriptors[i] = RenderTarget->GetCpuHandle();
			if (bClear)
			{
				const FLOAT ClearColor[4] = { 0, 0, 0, 1 };
				GetCommandList().GetGraphicsCommandList()->ClearRenderTargetView(RenderTarget->GetCpuHandle(), ClearColor, 0, nullptr);;
			}
		}	

		NewRenderTargets[i] = RenderTarget;
	}


	GetCommandList().GetGraphicsCommandList()->OMSetRenderTargets(NumRenderTargets, RTVDescriptors.GetData(), FALSE, nullptr);

}

void FD3D12CommandContext::SetRenderTargetsAndClear(const FRHISetRenderTargetInfo& RenderTargetsInfo)
{
	//GetCommandList().GetGraphicsCommandList()->ClearRenderTargetView();
	SetRenderTargets(RenderTargetsInfo.NumColorRenderTargets, RenderTargetsInfo.ColorRenderTarget, &RenderTargetsInfo.DepthStencilRenderTarget, true);
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
