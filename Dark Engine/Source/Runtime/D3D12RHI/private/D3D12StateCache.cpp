#include "D3D12StateCache.h"
#include <D3D12PipelineState.h>
#include "D3D12Shader.h"
#include "D3D12Util.h"
#include "D3D12CommandContext.h"

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
