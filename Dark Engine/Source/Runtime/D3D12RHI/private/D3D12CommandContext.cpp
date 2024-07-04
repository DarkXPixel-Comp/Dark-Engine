#include "D3D12CommandContext.h"
#include "imgui_impl_dx12.h"
#include "D3D12Descriptors.h"
#include "D3D12Viewport.h"
#include "D3D12View.h"
#include "RHIResources.h"
#include "D3D12PipelineState.h"
#include <d3dx12/d3dx12_barriers.h>
#include "optick.h"


FD3D12ContextCommon::FD3D12ContextCommon(FD3D12Device* InDevice, ED3D12QueueType InQueueType, bool InbIsDefaultContext):
	Device(InDevice),
	QueueType(InQueueType),
	bIsDefaultContext(InbIsDefaultContext),
	bSupportEnhancedBarriers(true)
{}

void FD3D12ContextCommon::OpenCommandList()
{
	if (CommandAllocator == nullptr)
	{
		CommandAllocator = Device->GetCommandAllocator(QueueType);
	}

	CommandList = Device->GetCommandList(CommandAllocator);

	ID3D12DescriptorHeap* Heaps[] = { Device->GetBindlessDescriptorManager().
	GetHeap(ERHIDescriptorHeapType::Standart)->GetHeap(),
		Device->GetBindlessDescriptorManager().GetHeap(ERHIDescriptorHeapType::Sampler)->GetHeap()};

	CommandList->GetGraphicsCommandList()->SetDescriptorHeaps(DE_ARRAY_COUNT(Heaps), Heaps);
	//OPTICK_GPU_CONTEXT(CommandList->GetGraphicsCommandList());
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
	//PIXScopedEvent(GetCommandList().GetGraphicsCommandList(), PIX_COLOR_DEFAULT, "TEST");
	if (Before == After)
	{
		Resource->SetState(After);
		return;
	}
	Barriers.Add({ After, Resource, SubResource, nullptr, Before });
}

void FD3D12ContextCommon::TransitionResource(ID3D12Resource* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After, uint32 SubResource)
{
	if (Before == After)
	{
		return;
	}

	Barriers.Add({ After, nullptr, SubResource, Resource, Before });
}

void FD3D12ContextCommon::TransitionResource(FD3D12Resource* Resource, D3D12_RESOURCE_STATES After, uint32 SubResource)
{
	/*D3D12_RESOURCE_STATES Before = Resource->GetCurrentState();
	if (Before == After)
	{
		Resource->SetState(After);
		return;
	}

	Barriers.Add({After, Resource, SubResource});*/
}
void FD3D12ContextCommon::TransitionResource(FD3D12Buffer* Buffer, CD3DX12_BUFFER_BARRIER BufferBarrier)
{
	if(Buffer)
		Buffer->GetResouce()->EmptyBarrierFlags();
	BufferBarrier.pResource = Buffer ? Buffer->GetResouce()->GetResource() : BufferBarrier.pResource;
	BufferBarriers.Add(BufferBarrier);
}

void FD3D12ContextCommon::TransitionResource(FD3D12Texture* Texture, CD3DX12_TEXTURE_BARRIER TextureBarrier)
{
	if (Texture->GetResource()->GetBarrierAccess() == TextureBarrier.AccessBefore && Texture->GetResource()->GetBarrierSync() == TextureBarrier.SyncBefore && TextureBarrier.LayoutAfter == Texture->GetBarrierLayout())
	{
		return;
	}
	Texture->SetBarrierLayout(TextureBarrier.LayoutAfter);
	Texture->GetResource()->EmptyBarrierFlags();
	TextureBarrier.pResource = Texture ? Texture->GetResource()->GetResource() : TextureBarrier.pResource;
	TextureBarriers.Add(TextureBarrier);
}


void FD3D12ContextCommon::TransitionResource(const FD3D12ResourceLocation& Resource, CD3DX12_BUFFER_BARRIER BufferBarrier)
{
	if (Resource.GetBarrierAccess() == BufferBarrier.AccessBefore && Resource.GetBarrierSync() == BufferBarrier.SyncBefore)
	{
		return;
	}
	Resource.GetResource()->EmptyBarrierFlags();
	BufferBarrier.pResource = Resource.GetResource()->GetResource();
	BufferBarriers.Add(BufferBarrier);
}

void FD3D12ContextCommon::TransitionResource(FD3D12Buffer* Buffer, D3D12_BARRIER_SYNC AfterSync, D3D12_BARRIER_ACCESS AfterAccess, uint64 Size, uint64 Offset)
{
	check(Buffer);
	BarrierBatcher.TransitionResource(Buffer, AfterSync, AfterAccess, Size, Offset);
}

void FD3D12ContextCommon::TransitionResource(FD3D12Texture* Texture, D3D12_BARRIER_SYNC AfterSync, D3D12_BARRIER_ACCESS AfterAccess, D3D12_BARRIER_LAYOUT AfterLayout, CD3DX12_BARRIER_SUBRESOURCE_RANGE Subresource)
{
	check(Texture);
	BarrierBatcher.TransitionResource(Texture, AfterSync, AfterAccess, AfterLayout, Subresource);
}

void FD3D12ContextCommon::TransitionBuffer(FD3D12ResourceLocation& Resource, D3D12_BARRIER_SYNC AfterSync, D3D12_BARRIER_ACCESS AfterAccess, uint64 Size, uint64 Offset)
{
	check(Resource.IsValid());

	BarrierBatcher.TransitionBuffer(Resource, AfterSync, AfterAccess, Size, Offset);
}

void FD3D12ContextCommon::TransitionBuffer(FD3D12Resource* Resource, D3D12_BARRIER_SYNC AfterSync, D3D12_BARRIER_ACCESS AfterAccess, uint64 Size, uint64 Offset)
{
	check(Resource);

	//BarrierBatcher.TransitionBuffer(Resource, AfterSync, AfterAccess, Size, Offset);
}

void FD3D12ContextCommon::TransitionTexture(FD3D12Resource* InTexture, D3D12_BARRIER_SYNC AfterSync, D3D12_BARRIER_ACCESS AfterAccess, D3D12_BARRIER_LAYOUT AfterLayout, CD3DX12_BARRIER_SUBRESOURCE_RANGE Subresource)
{
	check(InTexture);

	BarrierBatcher.TransitionTexture(InTexture, AfterSync, AfterAccess, AfterLayout, Subresource);

}

void FD3D12ContextCommon::TransitionBuffer(ID3D12Resource* Resource, CD3DX12_BUFFER_BARRIER BufferBarrier)
{
	//check(Resource);
	//BarrierBatcher.TransitionBuffer()
	BufferBarrier.pResource = Resource;
	BufferBarriers.Add(BufferBarrier);
}





void FD3D12ContextCommon::FlushBarriers()
{
	OPTICK_GPU_EVENT("FlushBarriers");
	if (bSupportEnhancedBarriers)
	{
		BarrierBatcher.FlushBarrierGroups(GetCommandList());
		EnhancedFlushBarriers();
		//return;
	}
	/*if(Barriers.Num())
	{
		TArray<CD3DX12_RESOURCE_BARRIER> ResourceBarriers(ArrayReserve(Barriers.Num()));

		for (int32 i = 0; i < Barriers.Num(); ++i)
		{
			FD3D12ResourceBarrier& Current = Barriers[i];
			
			D3D12_RESOURCE_STATES Before;
			if (Current.Resource)
				Before = Current.Resource->GetCurrentState();
			else
				Before = Current.Before;

			if (Before == Current.After)
			{
				continue;
			}
			ResourceBarriers.Add(CD3DX12_RESOURCE_BARRIER::Transition(
				Current.Resource ? Current.Resource->GetResource() : Current.NativeResource, Before,
				Current.After, Current.Subresource));
			if(Current.Resource)
				Current.Resource->SetState(Current.After);
		}
		GetCommandList().GetGraphicsCommandList()->ResourceBarrier(ResourceBarriers.Num(), ResourceBarriers.GetData());
		Barriers.Empty();
	}*/
}

void FD3D12ContextCommon::EnhancedFlushBarriers()
{
	TArray<CD3DX12_BARRIER_GROUP> BarrierGroups(ArrayReserve(3));

	if (BufferBarriers.Num())
	{
		BarrierGroups.Add(CD3DX12_BARRIER_GROUP(BufferBarriers.Num(), BufferBarriers.GetData()));
		BarrierGroups.Last().Type = D3D12_BARRIER_TYPE_BUFFER;
	}
	if (TextureBarriers.Num())
	{
		BarrierGroups.Add(CD3DX12_BARRIER_GROUP(TextureBarriers.Num(), TextureBarriers.GetData()));
		BarrierGroups.Last().Type = D3D12_BARRIER_TYPE_TEXTURE;
	}
	if (GlobalBarriers.Num())
	{
		BarrierGroups.Add(CD3DX12_BARRIER_GROUP(GlobalBarriers.Num(), GlobalBarriers.GetData()));
		BarrierGroups.Last().Type = D3D12_BARRIER_TYPE_GLOBAL;
	}

	BufferBarriers.Empty();
	TextureBarriers.Empty();
	GlobalBarriers.Empty();

	if(BarrierGroups.Num())
		GetGraphicsList7()->Barrier(BarrierGroups.Num(), BarrierGroups.GetData());
}


void FD3D12ContextCommon::FlushCommands(ED3D12FlushFlags Flags)
{
	OPTICK_GPU_EVENT("FlushCommands");
	check(bIsDefaultContext);

	FlushBarriers();

	if (Flags == ED3D12FlushFlags::WaitForSubmission)
	{
		FD3D12Queue& Queue = Device->GetQueue(ED3D12QueueType::Direct);
		Queue.Signal();
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
		Queue.Signal();
		Queue.WaitFrame();

		LockedResources.Empty();
	}
}

FD3D12CommandContextBase::FD3D12CommandContextBase(FD3D12Adapter* InParent):
	FD3D12AdapterChild(InParent)
{
}

void FD3D12CommandContextBase::RHIBeginDrawingViewport(FRHIViewport* RHIViewport, FRHITexture* RenderTargetRHI)
{
	OPTICK_GPU_EVENT("RHIBeginDrawingViewport");
	FD3D12Viewport* Viewport = reinterpret_cast<FD3D12Viewport*>(RHIViewport);
	Parent->SetDrawingViewport(Viewport);
	


	if (RenderTargetRHI == nullptr)
	{
		RenderTargetRHI = Viewport->GetCurrentBackBuffer();
	}
}

void FD3D12CommandContextBase::RHIEndDrawingViewport(FRHIViewport *Viewport, bool bPresent, int32 Vsync)
{
	FD3D12Viewport* ViewportRHI = reinterpret_cast<FD3D12Viewport*>(Viewport);
	DrawCallsNum = 0;

	Parent->SetDrawingViewport(nullptr);


	ViewportRHI->Present(Vsync);
}

void FD3D12CommandContext::CommitNonComputeShaderConstants()
{
	OPTICK_GPU_EVENT("CommitNonComputeShaderConstants");
	//const FD3D12GraphicsPipelineState* const GraphicPSO = StateCache.Ge

	for (int32 Index = 0; Index < ST_NumStandartTypes; ++Index)
	{
		StateCache.SetConstantBuffer(static_cast<EShaderType>(Index), StageConstantBuffers[Index], bDiscardSharedGraphicsConstants);
	}

	bDiscardSharedGraphicsConstants = false;
}

void FD3D12CommandContext::CommitComputeShaderConstants()
{
	OPTICK_EVENT();
	const FD3D12ComputePipelineState* const ComputePSO = StateCache.GetComputePipelineState();
	check(ComputePSO);

	//if(ComputePSO->bS)

	StateCache.SetConstantBuffer(ST_Compute, StageConstantBuffers[ST_Compute], bDiscardSharedComputeConstants);

	bDiscardSharedGraphicsConstants = false;
}

void FD3D12CommandContext::RHIBeginFrame()
{	
	OPTICK_EVENT();
	FD3D12CommandList& List = GetCommandList();
	FD3D12Viewport* Viewport = FD3D12AdapterChild::Parent->GetDrawingViewport();
	//ID3D12DescriptorHeap* Heaps[] = { FD3D12DynamicRHI::GetD3D12RHI()->ImGuiDescriptorHeap->GetHeap() };

	//Viewport->GetCurrentBackBuffer();
}

void FD3D12CommandContext::RHIEndFrame()
{
	OPTICK_EVENT();
	//Optick::SetGpuContext(Optick::GPUContext());
	FD3D12CommandList& List = GetCommandList();

	/*FD3D12Viewport* Viewport = FD3D12AdapterChild::Parent->GetDrawingViewport();
	const FLOAT Color[4] = { 0, 0, 0, 1 };

	List.GetGraphicsCommandList()->ClearRenderTargetView(Viewport->GetCurrentBackBuffer()->RenderTargetViews[0]->GetCpuHandle(),
		Color, 0, nullptr);
	Viewport->GetCurrentBackBuffer()->RenderTargetViews[0]->GetCpuHandle();*/
	//FlushCommands();
}

void FD3D12CommandContext::RHIBeginImGui()
{
	OPTICK_EVENT();
#ifdef IMGUI
	ImGui_ImplDX12_NewFrame();
#endif
}


void FD3D12CommandContext::RHIEndImGui()
{
	OPTICK_EVENT();
	FD3D12CommandList& List = GetCommandList();

#ifdef IMGUI
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), List.GetGraphicsCommandList());
#endif
}

void FD3D12CommandContext::RHIBeginRenderPass(FRHIRenderPassInfo& InInfo)
{
	OPTICK_GPU_EVENT("RHIBeginRenderPass");
	FRHISetRenderTargetInfo RTInfo = InInfo.ConvertToRenderTargetInfo();
	//SetRenderTargetsAndClear(RTInfo);


	FD3D12RenderTargetView* NewRenderTargets[MAX_RENDER_TARGETS];
	TArray<FD3D12CpuDescriptor>	RTVDescriptors(RTInfo.NumColorRenderTargets);
	TArray<D3D12_RENDER_PASS_RENDER_TARGET_DESC> RenderTargetPassDescs(RTInfo.NumColorRenderTargets);
	for (uint32 i = 0; i < MAX_RENDER_TARGETS; ++i)
	{
		FD3D12RenderTargetView* RenderTarget = nullptr;
		if (RTInfo.NumColorRenderTargets > i && i < MAX_RENDER_TARGETS && RTInfo.ColorRenderTarget[i].Texture != nullptr)
		{
			FD3D12Texture* NewRenderTarget = static_cast<FD3D12Texture*>(RTInfo.ColorRenderTarget[i].Texture);
			RenderTarget = NewRenderTarget->RenderTargetViews[0].get();
			//TransitionResource(NewRenderTarget->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, 0);
			TransitionResource(NewRenderTarget, D3D12_BARRIER_SYNC_RENDER_TARGET, D3D12_BARRIER_ACCESS_RENDER_TARGET, D3D12_BARRIER_LAYOUT_RENDER_TARGET);
			RTVDescriptors[i] = RenderTarget->GetCpuHandle();

			RenderTargetPassDescs[i].cpuDescriptor = RTVDescriptors[i];

			D3D12_CLEAR_VALUE ClearValue = {};
			ClearValue.Format = GetDXGIFormat(NewRenderTarget->GetPixelFormat());
			ClearValue.DepthStencil = { 0, 0 };
			FMemory::Memcpy(ClearValue.Color, &FVector4f::ZeroVectorOneW, sizeof(FVector4f));
			RenderTargetPassDescs[i].BeginningAccess.Type = GetRenderPassBeginningAccess(RTInfo.BeginRenderPassMode[i]);
			RenderTargetPassDescs[i].BeginningAccess.Clear.ClearValue = ClearValue;

			RenderTargetPassDescs[i].EndingAccess.Type = GetRenderPassEndingAccess(RTInfo.EndRenderPassMode[i]);
		}
		NewRenderTargets[i] = RenderTarget;
	}

	FlushBarriers();
	GetGraphicsList4()->BeginRenderPass(RenderTargetPassDescs.Num(), RenderTargetPassDescs.GetData(), nullptr, D3D12_RENDER_PASS_FLAG_NONE);
	StateCache.SetRenderTargets(RTInfo.NumColorRenderTargets, NewRenderTargets, nullptr);

}

void FD3D12CommandContext::RHIEndRenderPass(FRHIRenderPassInfo& InInfo)
{
	OPTICK_GPU_EVENT("RHIEndRenderPass");
	FRHISetRenderTargetInfo RTInfo = InInfo.ConvertToRenderTargetInfo();
	GetGraphicsList4()->EndRenderPass();

	for (uint32 i = 0; i < MAX_RENDER_TARGETS; ++i)
	{
		FD3D12RenderTargetView* RenderTarget = nullptr;
		if (i < MAX_RENDER_TARGETS && RTInfo.ColorRenderTarget[i].Texture != nullptr)
		{
			FD3D12Texture* D3DRenderTarget = static_cast<FD3D12Texture*>(RTInfo.ColorRenderTarget[i].Texture);
			//TransitionResource(D3DRenderTarget->GetResource(), D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, 0);
		}
	}

	//FlushBarriers();
}

void FD3D12CommandContext::SetRenderTargets(int32 NumRenderTargets, const FRHIRenderTargetView *RenderTargetsRHI, const FRHIDepthRenderTargetView *DepthStencilViewRHI, bool bClear)
{
	OPTICK_GPU_EVENT("SetRenderTargets");
	//FD3D12Texture* NewDepthTexture = DepthStencilViewRHI ? (FD3D12Texture*)DepthStencilViewRHI->Texture : nullptr;


	FD3D12RenderTargetView* NewRenderTargets[MAX_RENDER_TARGETS];
	TArray<FD3D12CpuDescriptor>	RTVDescriptors(NumRenderTargets);
	for (uint32 i = 0; i < MAX_RENDER_TARGETS; ++i)
	{
		FD3D12RenderTargetView* RenderTarget = nullptr;
		if (NumRenderTargets > i && i < MAX_RENDER_TARGETS && RenderTargetsRHI[i].Texture != nullptr)
		{
			FD3D12Texture* NewRenderTarget = static_cast<FD3D12Texture*>(RenderTargetsRHI[i].Texture);
			RenderTarget = NewRenderTarget->RenderTargetViews[0].get();
			//TransitionResource(NewRenderTarget->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, 0);
			TransitionResource(NewRenderTarget, D3D12_BARRIER_SYNC_RENDER_TARGET, D3D12_BARRIER_ACCESS_RENDER_TARGET, D3D12_BARRIER_LAYOUT_RENDER_TARGET);
			RTVDescriptors[i] = RenderTarget->GetCpuHandle();
			if (bClear && NewRenderTarget->IsValid())
			{
				static const FLOAT ClearColor[4] = { 0, 0, 0, 1 };
				GetCommandList().GetGraphicsCommandList()->ClearRenderTargetView(RenderTarget->GetCpuHandle(), ClearColor, 0, nullptr);
			}
		}	

		NewRenderTargets[i] = RenderTarget;
	}


	StateCache.SetRenderTargets(NumRenderTargets, NewRenderTargets, nullptr);
	GetCommandList().GetGraphicsCommandList()->OMSetRenderTargets(NumRenderTargets, RTVDescriptors.GetData(), FALSE, nullptr);

}

void FD3D12CommandContext::SetRenderTargetsAndClear(const FRHISetRenderTargetInfo& RenderTargetsInfo)
{
	//GetCommandList().GetGraphicsCommandList()->ClearRenderTargetView();
	SetRenderTargets(RenderTargetsInfo.NumColorRenderTargets, RenderTargetsInfo.ColorRenderTarget, &RenderTargetsInfo.DepthStencilRenderTarget, true);
}

void FD3D12CommandContext::RHIClearTextureColor(FRHITexture* InTexture, FVector InColor)
{
	OPTICK_EVENT();
	OPTICK_GPU_EVENT("RHIClearTextureColor");
	FD3D12Texture* D3DTexture = static_cast<FD3D12Texture*>(InTexture);


	/*TransitionResource(D3DTexture->GetResource(), D3DTexture->GetResource()->GetCurrentState(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, 0);*/

	FlushBarriers();

	FLOAT Color[4];
	for (int32 i = 0; i < 3; ++i)
	{
		Color[i] = InColor[i];
	}
	Color[3] = 1;


	GetCommandList().GetGraphicsCommandList()->ClearRenderTargetView(D3DTexture->RenderTargetViews[0]->GetCpuHandle(),
		Color, 0, nullptr);
	TransitionResource(D3DTexture->GetResource(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0);

}

void FD3D12CommandContext::RHISetShaderParameter(FRHIGraphicsShader* ShaderRHI, uint32 BufferIndex, uint32 Offset, uint32 NumBytes, const void* Data)
{
	OPTICK_EVENT();
	const EShaderType ShaderType = ShaderRHI->GetType();

}

void FD3D12CommandContext::RHICopyTexture(FRHITexture* SourceTextureRHI, FRHITexture* DestTextureRHI)
{
	OPTICK_EVENT();
	OPTICK_GPU_EVENT("RHICopyTexture");
	FD3D12Texture* SourceTexture = static_cast<FD3D12Texture*>(SourceTextureRHI);
	FD3D12Texture* DestTexture = static_cast<FD3D12Texture*>(DestTextureRHI);
	/*FScopedTransitionResource SourceScopedBarrier(*this, SourceTexture->GetResource(), D3D12_RESOURCE_STATE_COPY_SOURCE, 0);
	FScopedTransitionResource DestScopedBarrier(*this, DestTexture->GetResource(), D3D12_RESOURCE_STATE_COPY_DEST, 0);*/

	//TransitionResource(SourceTexture->GetResource(), D3D12_RESOURCE_STATE_COPY_SOURCE, 0);
	//TransitionResource(DestTexture->GetResource(), D3D12_RESOURCE_STATE_COPY_DEST, 0);

	TransitionResource(SourceTexture, D3D12_BARRIER_SYNC_COPY, D3D12_BARRIER_ACCESS_COPY_SOURCE, D3D12_BARRIER_LAYOUT_COPY_SOURCE);
	TransitionResource(DestTexture, D3D12_BARRIER_SYNC_COPY, D3D12_BARRIER_ACCESS_COPY_DEST, D3D12_BARRIER_LAYOUT_COPY_DEST);


	FlushBarriers();

	const FRHITextureDesc& SourceDesc = SourceTexture->GetDesc();
	const FRHITextureDesc& DestDesc = DestTexture->GetDesc();

	const uint16 SourceArraySize = SourceDesc.ArraySize * (SourceDesc.IsTextureCube() ? 6 : 1);
	const uint16 DestArraySize = DestDesc.ArraySize * (DestDesc.IsTextureCube() ? 6 : 1);


	const bool bAllTexture = SourceTexture->GetSize() == DestTexture->GetSize();
	if (bAllTexture)
	{
		GetGraphicsList()->CopyResource(DestTexture->GetResource()->GetResource(), SourceTexture->GetResource()->GetResource());
	}



}

void FD3D12CommandContext::RHISetViewport(float MinX, float MinY, float MinZ, float MaxX, float MaxY, float MaxZ)
{
	OPTICK_EVENT();
	D3D12_VIEWPORT Viewport = { MinX, MinY, (MaxX - MinX), (MaxY - MinY), MinZ, MaxZ };

	StateCache.SetViewport(MinX, MinY, MinZ, MaxX, MaxY, MaxZ);
}

void FD3D12CommandContext::RHIDrawIndexedPrimitive(FRHIBuffer* IndexBufferRHI, int32 BaseVertexIndex, uint32 FirstInstance, uint32 NumVertices, uint32 StartIndex, uint32 NumPrimitives, uint32 NumInstances)
{
	OPTICK_GPU_EVENT("RHIDrawIndexedPrimitive");

	FD3D12Buffer* IndexBuffer = static_cast<FD3D12Buffer*>(IndexBufferRHI);
	check(NumPrimitives > 0);
	check(IndexBuffer->GetSize() > 0);
	check(IndexBuffer->ResourceLocation.GetResource());

	CommitNonComputeShaderConstants();

	NumInstances = FMath::Max<uint32>(1, NumInstances);

	uint32 IndexCount = StateCache.GetVertexCount(NumPrimitives);
	const DXGI_FORMAT Format = IndexBuffer->GetStride() == sizeof(uint16) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	StateCache.SetIndexBuffer(&IndexBuffer->ResourceLocation, Format, 0);
	StateCache.ApplyState(false);

	FlushBarriers();

	GetGraphicsList()->DrawIndexedInstanced(IndexCount, NumInstances, StartIndex, BaseVertexIndex,
		FirstInstance);

	++DrawCallsNum;
}

void FD3D12CommandContext::RHIDispatchComputeShader(uint32 ThreadGroupCountX, uint32 ThreadGroupCountY, uint32 ThreadGroupCountZ)
{
	OPTICK_EVENT();
	OPTICK_GPU_EVENT("RHIDispatchComputeShader");
	CommitComputeShaderConstants();
	//CommitComputeResourceTables();
	FlushBarriers();

	StateCache.ApplyState(true);


	GetGraphicsList()->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
}

void FD3D12CommandContext::RHISetShaderParameters(FRHIGraphicsShader* Shader, TArray<uint8>& InParameters, TArray<FRHIShaderParameterResource>& InBindlessParameters, TArray<FRHIShaderParameterResource>& InResourceParameters)
{
	OPTICK_EVENT();
	OPTICK_GPU_EVENT("RHISetShaderParameters");
	EShaderType ShaderType = Shader->GetType();
	FD3D12ConstantBuffer& ConstantBuffer = StageConstantBuffers[ShaderType];


	for (FRHIShaderParameterResource& Parameter : InBindlessParameters)
	{
		if (FRHIResource* Resource = Parameter.Resource)
		{
			FRHIDescriptorHandle Handle;

			switch (Parameter.Type)
			{
			case FRHIShaderParameterResource::EType::Texture:
			{
				FD3D12Texture* Texture = static_cast<FD3D12Texture*>(Resource);
				StateCache.QueueBindlessSRV(ShaderType, Texture->GetShaderResourceView());
				Handle = Texture->GetBindlessHandle();
				break;
			}
			case FRHIShaderParameterResource::EType::ResourceView:
			{
				FD3D12ShaderResourceView* ResourceView = reinterpret_cast<FD3D12ShaderResourceView*>(Resource);
				StateCache.QueueBindlessSRV(ShaderType, ResourceView);
				Handle = ResourceView->GetBindlessHandle();
				break;
			}
			case FRHIShaderParameterResource::EType::Sampler:
				break;
			}

			if (Handle.IsValid())
			{
				const uint32 BindlessIndex = Handle.GetIndex();
				ConstantBuffer.UpdateConstant(reinterpret_cast<const uint8*>(&BindlessIndex), Parameter.Index * 4, 4);
			}
		}

	}


	for (FRHIShaderParameterResource& Parameter : InResourceParameters)
	{
		if (FRHIResource* Resource = Parameter.Resource)
		{
			switch (Parameter.Type)
			{
				case FRHIShaderParameterResource::EType::UniformBuffer:
					BindUniformBuffer(Shader, ShaderType, Parameter.Index, static_cast<FD3D12UniformBuffer*>(Parameter.Resource));
					break;
			}
		}
	}
}

void FD3D12CommandContext::RHISetShaderParameters(FRHIComputeShader* Shader, TArray<uint8>& InParameters, TArray<FRHIShaderParameterResource>& InBindlessParameters, TArray<FRHIShaderParameterResource>& InResourceParameters)
{
	OPTICK_EVENT();
	FD3D12ConstantBuffer& ConstantBuffer = StageConstantBuffers[ST_Compute];

	for (FRHIShaderParameterResource& Parameter : InBindlessParameters)
	{
		if (FRHIResource* Resource = Parameter.Resource)
		{
			FRHIDescriptorHandle Handle;

			switch (Parameter.Type)
			{
			case FRHIShaderParameterResource::EType::Texture:
			{
				FD3D12Texture* Texture = static_cast<FD3D12Texture*>(Resource);
				StateCache.QueueBindlessSRV(ST_Compute, Texture->GetShaderResourceView());
				Handle = Texture->GetBindlessHandle();
				break;
			}
			case FRHIShaderParameterResource::EType::ResourceView:
			{
				FD3D12ShaderResourceView* ResourceView = reinterpret_cast<FD3D12ShaderResourceView*>(Resource);
				StateCache.QueueBindlessSRV(ST_Compute, ResourceView);
				Handle = ResourceView->GetBindlessHandle();
				break;
			}
			case FRHIShaderParameterResource::EType::UAV:
			{
				FD3D12UnorderedAccessView* UAV = reinterpret_cast<FD3D12UnorderedAccessView*>(Resource);
				Handle = UAV->GetBindlessHandle();
				//TransitionResource(UAV->Resource, D3D12_RESOURCE_STATE_COMMON, 0);
				break;
			}
			}

			if (Handle.IsValid())
			{
				const uint32 BindlessIndex = Handle.GetIndex();
				ConstantBuffer.UpdateConstant(reinterpret_cast<const uint8*>(&BindlessIndex), Parameter.Index * 4, 4);
			}
		}

	}


	for (FRHIShaderParameterResource& Parameter : InResourceParameters)
	{
		if (FRHIResource* Resource = Parameter.Resource)
		{
			switch (Parameter.Type)
			{
			case FRHIShaderParameterResource::EType::UniformBuffer:
				BindUniformBuffer(Shader, ST_Compute, Parameter.Index, static_cast<FD3D12UniformBuffer*>(Parameter.Resource));
				break;
			}
		}
	}

}

void FD3D12CommandContext::RHISetStreamSource(uint32 StreamIndex, FRHIBuffer* VertexBufferRHI, uint32 Offset, uint32 Stride)
{
	OPTICK_EVENT();
	FD3D12Buffer* VertexBuffer = static_cast<FD3D12Buffer*>(VertexBufferRHI);

	StateCache.SetStreamSource(&VertexBuffer->ResourceLocation, StreamIndex, Offset, Stride);
}

void FD3D12CommandContext::RHISetStreamSource(uint32 StreamIndex, FRHIUniformBuffer* VertexBufferRHI, uint32 Offset, uint32 Stride)
{
	OPTICK_EVENT();
	FD3D12UniformBuffer* VertexBuffer = static_cast<FD3D12UniformBuffer*>(VertexBufferRHI);

	StateCache.SetStreamSource(&VertexBuffer->ResourceLocation, StreamIndex, Offset, Stride);
}

void FD3D12CommandContext::RHISetGraphicsPipelineState(FRHIGraphicsPipelineState* GraphicsPSO, const FBoundShaderStateInput& ShaderInput)
{
	OPTICK_EVENT();
	FD3D12GraphicsPipelineState* GraphicsPipelineState = static_cast<FD3D12GraphicsPipelineState*>(GraphicsPSO);


	for (uint32 Index = 0; Index < ST_NumGraphicTypes; ++Index)
	{
		StageConstantBuffers[Index].Reset();
	}
	bDiscardSharedGraphicsConstants = true;

	StateCache.SetGraphicsPipelineState(GraphicsPipelineState);
}

void FD3D12CommandContext::RHISetComputePipelineState(FRHIComputePipelineState* ComputeState)
{
	OPTICK_EVENT();
	FD3D12ComputePipelineState* ComputePipelineState = static_cast<FD3D12ComputePipelineState*>(ComputeState);

	StageConstantBuffers[ST_Compute].Reset();

	StateCache.SetComputePipelineState(ComputePipelineState);

}

void FD3D12CommandContext::RHIPreparePixelShaderResources()
{
	OPTICK_EVENT();
	FlushBarriers();
}

void FD3D12CommandContext::BindUniformBuffer(FRHIShader* Shader, EShaderType ShaderType, uint32 BufferIndex, FD3D12UniformBuffer* InBuffer)
{
	OPTICK_EVENT();
	StateCache.SetConstantsFromUniformBuffer(ShaderType, BufferIndex, InBuffer);
	BoundUniformBuffers[ShaderType][BufferIndex] = InBuffer;
	DirtyUniformBuffers[ShaderType] |= (1 << BufferIndex);
}


FD3D12CommandContext::FD3D12CommandContext(FD3D12Device* InParent, ED3D12QueueType QueueType, bool InIsDefaultContext):
	FD3D12ContextCommon(InParent, QueueType, InIsDefaultContext),
	FD3D12CommandContextBase(InParent->GetParentAdapter()),
	FD3D12DeviceChild(InParent),
	StageConstantBuffers
	{
		FD3D12ConstantBuffer(InParent),
		FD3D12ConstantBuffer(InParent),
		FD3D12ConstantBuffer(InParent),
		FD3D12ConstantBuffer(InParent),
		FD3D12ConstantBuffer(InParent)
	},
	StateCache(*this)
{

}

void FD3D12CommandContext::CloseCommandList()
{
	FD3D12ContextCommon::CloseCommandList();
	StateCache.DirtyStateForNewCommandList();
}
