#include "D3D12StateCache.h"
#include <D3D12PipelineState.h>
#include "D3D12Shader.h"
#include "D3D12Util.h"
#include "D3D12CommandContext.h"
#include <D3D12View.h>

FD3D12StateCache::FD3D12StateCache(FD3D12CommandContext& CmdContext):
	FD3D12DeviceChild(CmdContext.GetParentDevice()),
	Context(CmdContext)
{




}

void FD3D12StateCache::SetGrapicsPipelineState(FD3D12GraphicsPipelineState* GraphicsPipelineState)
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
		NewView.StrideInBytes != CurrentView.StrideInBytes)
	{
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
		Format != CurrentView.Format)
	{
		CurrentView.BufferLocation = BufferLocation;
		CurrentView.SizeInBytes = SizeInBytes;
		CurrentView.Format = Format;
		PipelineState.Graphics.IBCache.CurrentIndexBufferLocation = IndexBufferLocation;

		Context.TransitionResource(IndexBufferLocation->GetResource(), D3D12_RESOURCE_STATE_INDEX_BUFFER, 0);
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
				Context.TransitionResource(Resource, D3D12_RESOURCE_STATE_RENDER_TARGET, RTV->GetDesc().Texture2D.MipSlice);
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

}

void FD3D12StateCache::ApplyState(bool bIsCompute)
{
	if (!bIsCompute)
	{
		if (PipelineState.Graphics.bNeedSetRootSignature)
		{
			Context.GetCommandList().GetGraphicsCommandList()->SetGraphicsRootSignature(PipelineState.Graphics.CurrentPipelineStateObject->RootSignature->GetRootSignature());
			PipelineState.Graphics.bNeedSetRootSignature = false;

		//	PipelineState.Common.S
		}
	}





	ID3D12PipelineState* const CurrentPSO = PipelineState.Common.CurrentPipelineState.Get();
	ID3D12PipelineState* const NewPSO = bIsCompute ? nullptr : 
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


	}




}
