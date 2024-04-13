#pragma once
#include "D3D12RHICommon.h"	
#include "D3D12RootSignature.h"
#include "Containers/UnordoredMap.h"

class FD3D12PipelineState;

class FD3D12PipelineStateManager : public FD3D12AdapterChild
{
public:
	FD3D12PipelineStateManager(FD3D12Adapter* InParent) : FD3D12AdapterChild(InParent) {}

	FD3D12PipelineState* GetPipelineState(const FGraphicsPipelineStateInitializer& Initializer, const FD3D12RootSignature* InRootSignature);



private:
	TUnordoredMap<FGraphicsPipelineStateInitializer, struct FD3D12PipelineState*> PSOMap;


};



struct FD3D12PipelineState : public  FD3D12AdapterChild
{
	FD3D12PipelineState(FD3D12Adapter* InParent) : FD3D12AdapterChild(InParent) {}

	void Create(const FGraphicsPipelineStateInitializer& Initalizer, const FD3D12RootSignature* InRootSignature);

	TRefCountPtr<ID3D12PipelineState> PSO;
};



struct FD3D12GraphicsPipelineState : public FRHIGraphicsPipelineState
{
	FD3D12GraphicsPipelineState(const FGraphicsPipelineStateInitializer& Initializer, const FD3D12RootSignature* InRootSignature, FD3D12PipelineState* InPipelineState);


	FGraphicsPipelineStateInitializer PipelineStateInitializer;



	FD3D12PipelineState* PipelineState;
	const FD3D12RootSignature* const RootSignature;
};