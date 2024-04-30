#include "D3D12CommandContext.h"
#include "imgui_impl_dx12.h"
#include "D3D12Descriptors.h"
#include "D3D12Viewport.h"
#include "D3D12View.h"
#include "RHIResources.h"
#include "D3D12PipelineState.h"

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
	//PIXScopedEvent(GetCommandList().GetGraphicsCommandList(), PIX_COLOR_DEFAULT, "TEST");
	if (Before == After)
	{
		Resource->SetState(After);
		return;
	}
	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(Resource->GetResource(), Before, After);
	GetCommandList().GetGraphicsCommandList()->ResourceBarrier(1, &Barrier);
	Resource->SetState(After);
}

void FD3D12ContextCommon::TransitionResource(ID3D12Resource* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After, uint32 SubResource)
{
	if (Before == After)
	{
		return;
	}
	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(Resource, Before, After);
	GetCommandList().GetGraphicsCommandList()->ResourceBarrier(1, &Barrier);
}

void FD3D12ContextCommon::TransitionResource(FD3D12Resource* Resource, D3D12_RESOURCE_STATES After, uint32 SubResource)
{
	D3D12_RESOURCE_STATES Before = Resource->GetCurrentState();
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

void FD3D12CommandContextBase::RHIEndDrawingViewport(FRHIViewport *Viewport, bool bPresent, int32 Vsync)
{
	FD3D12Viewport* ViewportRHI = reinterpret_cast<FD3D12Viewport*>(Viewport);

	Parent->SetDrawingViewport(nullptr);


	ViewportRHI->Present(Vsync);
}

void FD3D12CommandContext::CommitNonComputeShaderConstants()
{
	//const FD3D12GraphicsPipelineState* const GraphicPSO = StateCache.Ge

	for (int32 Index = 0; Index < ST_NumStandartTypes; ++Index)
	{
		StateCache.SetConstantBuffer(static_cast<EShaderType>(Index), StageConstantBuffers[Index], false);
	}

}

void FD3D12CommandContext::RHIBeginFrame()
{	
	FD3D12CommandList& List = GetCommandList();
	FD3D12Viewport* Viewport = FD3D12AdapterChild::Parent->GetDrawingViewport();
	//ID3D12DescriptorHeap* Heaps[] = { FD3D12DynamicRHI::GetD3D12RHI()->ImGuiDescriptorHeap->GetHeap() };
	ID3D12DescriptorHeap* Heaps[] = { FD3D12DeviceChild::Parent->GetBindlessDescriptorManager().
		GetHeap(ERHIDescriptorHeapType::Standart)->GetHeap()};




	List.GetGraphicsCommandList()->SetDescriptorHeaps(1, Heaps);
	Viewport->GetCurrentBackBuffer();
}

void FD3D12CommandContext::RHIEndFrame()
{
	FD3D12CommandList& List = GetCommandList();
	/*FD3D12Viewport* Viewport = FD3D12AdapterChild::Parent->GetDrawingViewport();
	const FLOAT Color[4] = { 0, 0, 0, 1 };

	List.GetGraphicsCommandList()->ClearRenderTargetView(Viewport->GetCurrentBackBuffer()->RenderTargetViews[0]->GetCpuHandle(),
		Color, 0, nullptr);
	Viewport->GetCurrentBackBuffer()->RenderTargetViews[0]->GetCpuHandle();*/
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
	FRHISetRenderTargetInfo RTInfo = InInfo.ConvertToRenderTargetInfo();
	SetRenderTargetsAndClear(RTInfo);
}

void FD3D12CommandContext::RHIEndRenderPass(FRHIRenderPassInfo& InInfo)
{
	FRHISetRenderTargetInfo RTInfo = InInfo.ConvertToRenderTargetInfo();

	for (uint32 i = 0; i < MAX_RENDER_TARGETS; ++i)
	{
		FD3D12RenderTargetView* RenderTarget = nullptr;
		if (i < MAX_RENDER_TARGETS && RTInfo.ColorRenderTarget[i].Texture != nullptr)
		{
			FD3D12Texture* D3DRenderTarget = static_cast<FD3D12Texture*>(RTInfo.ColorRenderTarget[i].Texture);
			TransitionResource(D3DRenderTarget->GetResource(), D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, 0);
		}
	}
	

}

void FD3D12CommandContext::SetRenderTargets(int32 NumRenderTargets, const FRHIRenderTargetView *RenderTargetsRHI, const FRHIDepthRenderTargetView *DepthStencilViewRHI, bool bClear)
{
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
			TransitionResource(NewRenderTarget->GetResource(),
				NewRenderTarget->GetResource()->GetCurrentState(), D3D12_RESOURCE_STATE_RENDER_TARGET, 0);
			RTVDescriptors[i] = RenderTarget->GetCpuHandle();
			if (bClear && NewRenderTarget->IsValid())
			{
				const FLOAT ClearColor[4] = { 0, 0, 0, 1 };
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
	FD3D12Texture* D3DTexture = static_cast<FD3D12Texture*>(InTexture);


	TransitionResource(D3DTexture->GetResource(), D3DTexture->GetResource()->GetCurrentState(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, 0);


	FLOAT Color[4];
	for (int32 i = 0; i < 3; ++i)
	{
		Color[i] = InColor[i];
	}
	Color[3] = 1;


	GetCommandList().GetGraphicsCommandList()->ClearRenderTargetView(D3DTexture->RenderTargetViews[0]->GetCpuHandle(),
		Color, 0, nullptr);
	TransitionResource(D3DTexture->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0);

}

void FD3D12CommandContext::RHISetShaderParameter(FRHIGraphicsShader* ShaderRHI, uint32 BufferIndex, uint32 Offset, uint32 NumBytes, const void* Data)
{
	const EShaderType ShaderType = ShaderRHI->GetType();




}

void FD3D12CommandContext::RHISetViewport(float MinX, float MinY, float MinZ, float MaxX, float MaxY, float MaxZ)
{
	D3D12_VIEWPORT Viewport = { MinX, MinY, (MaxX - MinX), (MaxY - MinY), MinZ, MaxZ };

	StateCache.SetViewport(MinX, MinY, MinZ, MaxX, MaxY, MaxZ);
}

void FD3D12CommandContext::RHIDrawIndexedPrimitive(FRHIBuffer* IndexBufferRHI, int32 BaseVertexIndex, uint32 FirstInstance, uint32 NumVertices, uint32 StartIndex, uint32 NumPrimitives, uint32 NumInstances)
{
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

	GetGraphicsList()->DrawIndexedInstanced(IndexCount, NumInstances, StartIndex, BaseVertexIndex,
		FirstInstance);
}

void FD3D12CommandContext::RHISetShaderParameters(FRHIGraphicsShader* Shader, TArray<uint8>& InParameters, TArray<FRHIShaderParameterResource>& InBindlessParameters, TArray<FRHIShaderParameterResource>& InResourceParameters)
{
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
			case FRHIShaderParameterResource::EType::UAV:
				break;
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

void FD3D12CommandContext::RHISetStreamSource(uint32 StreamIndex, FRHIBuffer* VertexBufferRHI, uint32 Offset, uint32 Stride)
{
	FD3D12Buffer* VertexBuffer = static_cast<FD3D12Buffer*>(VertexBufferRHI);

	StateCache.SetStreamSource(&VertexBuffer->ResourceLocation, StreamIndex, Offset, Stride);
}

void FD3D12CommandContext::RHISetGraphicsPipelineState(FRHIGraphicsPipelineState* GraphicsPSO, const FBoundShaderStateInput& ShaderInput)
{
	FD3D12GraphicsPipelineState* GraphicsPipelineState = static_cast<FD3D12GraphicsPipelineState*>(GraphicsPSO);


	for (uint32 Index = 0; Index < ST_NumStandartTypes; ++Index)
	{
		StageConstantBuffers[Index].Reset();
	}

	StateCache.SetGrapicsPipelineState(GraphicsPipelineState);
}

void FD3D12CommandContext::BindUniformBuffer(FRHIShader* Shader, EShaderType ShaderType, uint32 BufferIndex, FD3D12UniformBuffer* InBuffer)
{
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
