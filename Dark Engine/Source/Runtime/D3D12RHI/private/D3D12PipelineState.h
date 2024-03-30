#pragma once
#include "D3D12RHICommon.h"	
#include "D3D12RootSignature.h"



struct FD3D12PipelineState : public  FD3D12AdapterChild
{
	TRefCountPtr<ID3D12PipelineState> PSO;
};



struct FD3D12GraphicsPipelineState : public FRHIGraphicsPipelineState
{	


	FGraphicsPipelineStateInitializer PipelineStateInitializer;



	FD3D12PipelineState* PipelineState;
	const FD3D12RootSignature* const RootSignature;
};