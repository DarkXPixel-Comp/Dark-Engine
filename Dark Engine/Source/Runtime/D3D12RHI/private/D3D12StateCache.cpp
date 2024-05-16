#include "D3D12StateCache.h"
#include <D3D12PipelineState.h>
#include "D3D12Shader.h"
#include "D3D12Util.h"
#include "D3D12CommandContext.h"
#include <D3D12View.h>
#include "Templates/DarkTemplate.h"

FD3D12StateCache::FD3D12StateCache(FD3D12CommandContext& CmdContext):
	FD3D12DeviceChild(CmdContext.GetParentDevice()),
	Context(CmdContext)
{




}

void FD3D12StateCache::SetConstantBuffer(EShaderType ShaderType, FD3D12ConstantBuffer& Buffer, bool bDiscardConstants)
{
	FD3D12ResourceLocation* Location;

	if (Buffer.Version(Location, bDiscardConstants))
	{
		const uint32 SlotIndex = 0;

		FD3D12ConstantBufferCache& CBVCache = PipelineState.Common.CBVCache;
		D3D12_GPU_VIRTUAL_ADDRESS& CurrentGPUVirtualAddress = CBVCache.CurrentGPUVirtualAddress[ShaderType][SlotIndex];
		CurrentGPUVirtualAddress = Location->GetGPUVirtualAddress();
		FD3D12ConstantBufferCache::DirtySlot(CBVCache.DirtySlotMask[ShaderType], SlotIndex);
	}

}

void FD3D12StateCache::SetConstantsFromUniformBuffer(EShaderType ShaderType, uint32 SlotIndex, FD3D12UniformBuffer* InBuffer)
{
	FD3D12ConstantBufferCache& CBVCache = PipelineState.Common.CBVCache;
	D3D12_GPU_VIRTUAL_ADDRESS& CurrentGPUVirtualAddress = CBVCache.CurrentGPUVirtualAddress[ShaderType][SlotIndex];
	if (InBuffer && InBuffer->ResourceLocation.GetGPUVirtualAddress())
	{
		const FD3D12ResourceLocation ResourceLocation = InBuffer->ResourceLocation;
		if (ResourceLocation.GetGPUVirtualAddress() != CurrentGPUVirtualAddress)
		{
			CurrentGPUVirtualAddress = ResourceLocation.GetGPUVirtualAddress();
			FD3D12ConstantBufferCache::DirtySlot(CBVCache.DirtySlotMask[ShaderType], SlotIndex);
		}
	}
	else if (CurrentGPUVirtualAddress != 0)
	{
		CurrentGPUVirtualAddress = 0;
		FD3D12ConstantBufferCache::DirtySlot(CBVCache.DirtySlotMask[ShaderType], SlotIndex);
	}

}

void FD3D12StateCache::SetGraphicsPipelineState(FD3D12GraphicsPipelineState* GraphicsPipelineState)
{
	check(GraphicsPipelineState);

	TRefCountPtr<FD3D12GraphicsPipelineState> CurrentPipelineState = PipelineState.Graphics.CurrentPipelineStateObject;
	const bool bForceSet = CurrentPipelineState == nullptr;

	if (bForceSet || CurrentPipelineState.Get() != GraphicsPipelineState)
	{
		if (bForceSet || CurrentPipelineState->GetVertexShader() != GraphicsPipelineState->GetVertexShader())
		{
			SetNewShaderData(ST_Vertex, GraphicsPipelineState->GetVertexShader());
		}

		if (bForceSet || CurrentPipelineState->GetPixelShader() != GraphicsPipelineState->GetPixelShader())
		{
			SetNewShaderData(ST_Pixel, GraphicsPipelineState->GetPixelShader());
		}

		if(bForceSet || CurrentPipelineState->RootSignature != GraphicsPipelineState->RootSignature)
		{
			PipelineState.Graphics.bNeedSetRootSignature = true;
		}


		PipelineState.Common.bNeedSetPSO = true;
		PipelineState.Graphics.CurrentPipelineStateObject = GraphicsPipelineState;

		EPrimitiveType PrimitiveType = GraphicsPipelineState->PipelineStateInitializer.PrimitiveType;

		if (PrimitiveType != PipelineState.Graphics.PrimitiveType)
		{
			PipelineState.Graphics.PrimitiveType = PrimitiveType;
			PipelineState.Graphics.PrimitiveTopology = GetD3D12PrimitiveTopology(PrimitiveType);
			PipelineState.Graphics.PrimitiveTypeFactor = (PrimitiveType == PT_TriangleList) ? 3 : (PrimitiveType == PT_LineList) ? 2 : (PrimitiveType == PT_QuadList) ? 4 : 1;
			PipelineState.Graphics.PrimitiveTypeOffset = (PrimitiveType == PT_TriangleStrip) ? 2 : 0;
			bNeedSetPrimitiveTopology = true;


		}

		ID3D12PipelineState* const CurrentPipelineState = PipelineState.Common.CurrentPipelineState.Get();
		ID3D12PipelineState* const NewPipelineState = GraphicsPipelineState->PipelineState->PSO.Get();

		if (PipelineState.Common.bNeedSetPSO || CurrentPipelineState != NewPipelineState)
		{
			PipelineState.Common.CurrentPipelineState = GraphicsPipelineState->PipelineState->PSO;
			Context.GetCommandList().GetGraphicsCommandList()->SetPipelineState(NewPipelineState);
			PipelineState.Common.bNeedSetPSO = false;
		}

	}
}

void FD3D12StateCache::SetComputePipelineState(FD3D12ComputePipelineState* ComputePipelineState)
{
	check(ComputePipelineState);

	FD3D12ComputePipelineState* CurrentComputePipelineState = PipelineState.Compute.CurrentPipelineStateObject.Get();
	const bool bForceSet = CurrentComputePipelineState == nullptr;

	if (bForceSet || CurrentComputePipelineState != ComputePipelineState)
	{
		if (bForceSet || CurrentComputePipelineState->RootSignature != ComputePipelineState->RootSignature)
		{
			PipelineState.Compute.bNeedSetRootSignature = true;
		}

		if (bForceSet || CurrentComputePipelineState->ComputeShader != ComputePipelineState->ComputeShader)
		{
			SetNewShaderData(ST_Compute, ComputePipelineState->ComputeShader.Get());
		}
		if (PipelineState.Compute.CurrentPipelineStateObject.Get() != ComputePipelineState)
		{
			PipelineState.Compute.CurrentPipelineStateObject = ComputePipelineState;
		}

		PipelineState.Common.bNeedSetPSO = true;
		PipelineState.Common.CurrentPipelineState = ComputePipelineState->PipelineState->PSO;

		ID3D12PipelineState* const CurrentPipelineState = PipelineState.Common.CurrentPipelineState.Get();
		ID3D12PipelineState* const NewPipelineState = ComputePipelineState->PipelineState->PSO.Get();

		if (PipelineState.Common.bNeedSetPSO || CurrentPipelineState != NewPipelineState)
		{
			PipelineState.Common.CurrentPipelineState = ComputePipelineState->PipelineState->PSO;
			Context.GetCommandList().GetGraphicsCommandList()->SetPipelineState(NewPipelineState);
			PipelineState.Common.bNeedSetPSO = false;
		}

	}

}

void FD3D12StateCache::SetNewShaderData(EShaderType InType, const FD3D12ShaderData* InShaderData)
{
	PipelineState.Common.CurrentShaderCBVCount[InType] = InShaderData ? InShaderData->ResourceCounts.NumCBVs : 0;
	PipelineState.Common.CurrentShaderSRVCount[InType] = InShaderData ? InShaderData->ResourceCounts.NumSRVs : 0;
	PipelineState.Common.CurrentShaderUAVCount[InType] = InShaderData ? InShaderData->ResourceCounts.NumUAVs : 0;
	PipelineState.Common.CurrentShaderSamplerCount[InType] = InShaderData ? InShaderData->ResourceCounts.NumSamplers : 0;
}

void FD3D12StateCache::SetStreamSource(FD3D12ResourceLocation* VertexBufferLocation, uint32 StreamIndex, uint32 Offset, uint32 Stride)
{
	check(VertexBufferLocation);

	D3D12_VERTEX_BUFFER_VIEW NewView;
	NewView.BufferLocation = VertexBufferLocation ? VertexBufferLocation->GetGPUVirtualAddress() + Offset : 0;
	NewView.SizeInBytes = VertexBufferLocation ? VertexBufferLocation->GetSize() - Offset : 0;
	NewView.StrideInBytes = Stride;

	D3D12_VERTEX_BUFFER_VIEW& CurrentView = PipelineState.Graphics.VBCache.CurrentVertexBufferViews[StreamIndex];

	if (NewView.BufferLocation != CurrentView.BufferLocation ||
		NewView.SizeInBytes != CurrentView.SizeInBytes ||
		NewView.StrideInBytes != CurrentView.StrideInBytes || true)
	{
		/*Context.TransitionResource(*VertexBufferLocation, CD3DX12_BUFFER_BARRIER
		(
			VertexBufferLocation->GetBarrierSync(), D3D12_BARRIER_SYNC_VERTEX_SHADING,
			VertexBufferLocation->GetBarrierAccess(), D3D12_BARRIER_ACCESS_VERTEX_BUFFER
		));



		VertexBufferLocation->SetBarrierSync(D3D12_BARRIER_SYNC_VERTEX_SHADING);
		VertexBufferLocation->SetBarrierAccess(D3D12_BARRIER_ACCESS_VERTEX_BUFFER);*/

		Context.TransitionBuffer(*VertexBufferLocation, D3D12_BARRIER_SYNC_VERTEX_SHADING, D3D12_BARRIER_ACCESS_VERTEX_BUFFER);

		bNeedSetVB = true;
		PipelineState.Graphics.VBCache.CurrentVertexBufferResources[StreamIndex] = VertexBufferLocation;
		PipelineState.Graphics.VBCache.NumViews = FMath::Max<uint32>(PipelineState.Graphics.VBCache.NumViews, StreamIndex + 1);
		
		FMemory::Memcpy(CurrentView, NewView);
	}



}

void FD3D12StateCache::SetIndexBuffer(FD3D12ResourceLocation* IndexBufferLocation, DXGI_FORMAT Format, uint32 Offset)
{
	D3D12_GPU_VIRTUAL_ADDRESS BufferLocation = IndexBufferLocation->GetGPUVirtualAddress() + Offset;
	uint64 SizeInBytes = IndexBufferLocation->GetSize() - Offset;
	
	D3D12_INDEX_BUFFER_VIEW& CurrentView = PipelineState.Graphics.IBCache.CurrentIndexBufferView;

	if (BufferLocation != CurrentView.BufferLocation ||
		SizeInBytes != CurrentView.SizeInBytes ||
		Format != CurrentView.Format || true)
	{
		CurrentView.BufferLocation = BufferLocation;
		CurrentView.SizeInBytes = SizeInBytes;
		CurrentView.Format = Format;
		PipelineState.Graphics.IBCache.CurrentIndexBufferLocation = IndexBufferLocation;

		//Context.TransitionResource(IndexBufferLocation->GetResource(), D3D12_RESOURCE_STATE_INDEX_BUFFER, 0);

		/*Context.TransitionResource(*IndexBufferLocation, CD3DX12_BUFFER_BARRIER(
			IndexBufferLocation->GetBarrierSync(), D3D12_BARRIER_SYNC_INDEX_INPUT,
			IndexBufferLocation->GetBarrierAccess(), D3D12_BARRIER_ACCESS_INDEX_BUFFER, nullptr));

		IndexBufferLocation->SetBarrierAccess(D3D12_BARRIER_ACCESS_INDEX_BUFFER);
		IndexBufferLocation->SetBarrierSync(D3D12_BARRIER_SYNC_INDEX_INPUT);*/

		Context.TransitionBuffer(*IndexBufferLocation, D3D12_BARRIER_SYNC_INDEX_INPUT, D3D12_BARRIER_ACCESS_INDEX_BUFFER);

		Context.FlushBarriers();
		Context.GetCommandList().GetGraphicsCommandList()->IASetIndexBuffer(&PipelineState.Graphics.IBCache.CurrentIndexBufferView);
	}
}

void FD3D12StateCache::SetRenderTargets(uint32 NumRenderTargets, FD3D12RenderTargetView** RTArray, FD3D12DepthStencilView* DSTarget)
{
	if (DSTarget)
	{
	}

	if (PipelineState.Graphics.CurrentDepthStencilTarget != DSTarget)
	{
		PipelineState.Graphics.CurrentDepthStencilTarget = DSTarget;
		bNeedSetRTs = true;
	}

	PipelineState.Graphics.CurrentNumberRenderTargets = 0;

	for (uint32 Index = 0; Index < _countof(PipelineState.Graphics.RenderTargetArray); ++Index)
	{
		FD3D12RenderTargetView* RTV = Index < NumRenderTargets ? RTArray[Index] : nullptr;

		if (RTV)
		{
			FD3D12Resource* Resource = RTV->GetResource();

			switch (RTV->GetDesc().ViewDimension)
			{
			case D3D12_RTV_DIMENSION_TEXTURE2D:
			case D3D12_RTV_DIMENSION_TEXTURE2DMS:
			case D3D12_RTV_DIMENSION_TEXTURE3D:
				//Context.TransitionResource(Resource, D3D12_RESOURCE_STATE_RENDER_TARGET, RTV->GetDesc().Texture2D.MipSlice);
				//Context.TransitionTexture(Resource, D3D12_BARRIER_SYNC_RENDER_TARGET, D3D12_BARRIER_ACCESS_RENDER_TARGET, D3D12_BARRIER_LAYOUT_RENDER_TARGET);
				break;
			case D3D12_RTV_DIMENSION_TEXTURE2DARRAY:
				break;
			case D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY:
				break;
			default:
				break;
			}

			++PipelineState.Graphics.CurrentNumberRenderTargets;
		}
		
		if (PipelineState.Graphics.RenderTargetArray[Index] != RTV)
		{
			PipelineState.Graphics.RenderTargetArray[Index] = RTV;
			bNeedSetRTs = true;
		}

	}

	bNeedSetRTs = false;
}

void FD3D12StateCache::SetViewport(float MinX, float MinY, float MinZ, float MaxX, float MaxY, float MaxZ)
{
	D3D12_VIEWPORT Viewport = {MinX, MinY, (MaxX - MinX), (MaxY - MinY), MinZ, MaxZ};
	
	if (PipelineState.Graphics.CurrentViewport[0] != Viewport)
	{
		bNeedSetViewports = true;
		PipelineState.Graphics.CurrentViewport[0] = Viewport;
	}
}

void FD3D12StateCache::ApplyState(bool bIsCompute)
{
	if (!bIsCompute)
	{
		if (PipelineState.Graphics.bNeedSetRootSignature)
		{
			Context.GetCommandList().GetGraphicsCommandList()->SetGraphicsRootSignature(PipelineState.Graphics.CurrentPipelineStateObject->RootSignature->GetRootSignature());
			PipelineState.Graphics.bNeedSetRootSignature = false;
		}
	}
	if (bIsCompute)
	{
		if (PipelineState.Compute.bNeedSetRootSignature)
		{
			Context.GetCommandList().GetGraphicsCommandList()->SetComputeRootSignature(PipelineState.Compute.CurrentPipelineStateObject->RootSignature->GetRootSignature());
			PipelineState.Compute.bNeedSetRootSignature = false;
		}
	}

	

	ID3D12PipelineState* const CurrentPSO = PipelineState.Common.CurrentPipelineState.Get();
	ID3D12PipelineState* const NewPSO = bIsCompute ? PipelineState.Compute.CurrentPipelineStateObject->PipelineState->PSO.Get() :
		PipelineState.Graphics.CurrentPipelineStateObject->PipelineState->PSO.Get();

	if (PipelineState.Common.bNeedSetPSO || CurrentPSO == nullptr || NewPSO != CurrentPSO)
	{
		PipelineState.Common.CurrentPipelineState = NewPSO;
		Context.GetCommandList().GetGraphicsCommandList()->SetPipelineState(NewPSO);
		PipelineState.Common.bNeedSetPSO = false;
	}

	if (!bIsCompute)
	{
		if (bNeedSetVB)
		{
			bNeedSetVB = false;
			Context.GetGraphicsList()->IASetVertexBuffers(0, PipelineState.Graphics.VBCache.NumViews,
				PipelineState.Graphics.VBCache.CurrentVertexBufferViews);
		}
		if (bNeedSetPrimitiveTopology)
		{
			bNeedSetPrimitiveTopology = false;
			Context.GetGraphicsList()->IASetPrimitiveTopology(PipelineState.Graphics.PrimitiveTopology);
		}

		if (bNeedSetViewports)
		{
			bNeedSetViewports = false;
			D3D12_RECT Rect = {(LONG)0, (LONG)0, (LONG)PipelineState.Graphics.CurrentViewport[0].Width, (LONG)PipelineState.Graphics.CurrentViewport[0].Height };
			
			Context.GetGraphicsList()->RSSetViewports(1, &PipelineState.Graphics.CurrentViewport[0]);
			Context.GetGraphicsList()->RSSetScissorRects(1, &Rect);
		}

		if (bNeedSetRTs)
		{
			bNeedSetRTs = false;
		}

		ApplyConstants(0, ST_Compute,
			PipelineState.Graphics.CurrentPipelineStateObject->RootSignature);
	} 
	else
	{
		ApplyConstants(ST_Compute, ST_Compute + 1, PipelineState.Compute.CurrentPipelineStateObject->RootSignature);
	}
	

}

void FD3D12StateCache::ApplyConstants(uint32 StartStage, uint32 EndStage, const FD3D12RootSignature* RootSignature)
{
	uint16 CurrentShaderDirtyCBVSlots[ST_NumStandartTypes] = {};

	for (uint32 Stage = StartStage; Stage < EndStage; ++Stage)
	{
		const uint16 CurrentShaderCBVRegisterMask = BitMask<uint16>(PipelineState.Common.CurrentShaderCBVCount[Stage]);
		CurrentShaderDirtyCBVSlots[Stage] = CurrentShaderCBVRegisterMask & PipelineState.Common.CBVCache.DirtySlotMask[Stage];
	}


	FD3D12ConstantBufferCache& CBVCache = PipelineState.Common.CBVCache;

	for (uint32 Stage = StartStage; Stage < EndStage; ++Stage)
	{
		if (CurrentShaderDirtyCBVSlots[Stage])
		{
			uint16& CurrentDirtyMask = CBVCache.DirtySlotMask[Stage];
			const uint32 CBVDirty = FMath::FloorLog2(CurrentShaderDirtyCBVSlots[Stage]) + 1;

			const uint32 BaseIndex = RootSignature->GetCBVDBindSlot((EShaderType)Stage);

			for (uint32 SlotIndex = 0; SlotIndex < CBVDirty; ++SlotIndex)
			{
				if(FD3D12ConstantBufferCache::IsSlotDirty(CurrentShaderDirtyCBVSlots[Stage], SlotIndex))
				{
					const D3D12_GPU_VIRTUAL_ADDRESS CurrentGPUVirtualAddress = CBVCache.CurrentGPUVirtualAddress[Stage][SlotIndex];

					if (Stage != ST_Compute)
					{
						Context.GetGraphicsList()->SetGraphicsRootConstantBufferView(BaseIndex + SlotIndex, CurrentGPUVirtualAddress);
					}
					else
					{
						Context.GetGraphicsList()->SetComputeRootConstantBufferView(BaseIndex + SlotIndex, CurrentGPUVirtualAddress);
					}

					CurrentDirtyMask &= ~(1 << SlotIndex);
				}
			}

		}
	}
}

void FD3D12StateCache::DirtyStateForNewCommandList()
{
	PipelineState.Common.bNeedSetPSO = true;
	PipelineState.Compute.bNeedSetRootSignature = true;
	PipelineState.Common.CBVCache.Clear();
	PipelineState.Graphics.bNeedSetRootSignature = true;
	bNeedSetPrimitiveTopology = true;
	bNeedSetVB = true;
	PipelineState.Graphics.IBCache.Clear();
	bNeedSetViewports = true;
	bNeedSetRTs = true;
	bNeedSetBlendFactor = true;

}

void FD3D12StateCache::QueueBindlessSRV(EShaderType Type, FD3D12ShaderResourceView* SRV)
{
	PipelineState.Common.QueuedBindlessSRVs[Type].Emplace(SRV);
}
