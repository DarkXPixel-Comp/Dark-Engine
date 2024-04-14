#pragma once
#include "D3D12RHICommon.h"	
#include "D3D12RootSignature.h"
#include "Containers/UnordoredMap.h"

class FD3D12PipelineState;



struct FD3D12_GRAPHICS_PIPELINE_STATE_DESC
{
	CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE RootSignature;
	CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
	CD3DX12_PIPELINE_STATE_STREAM_VS VertexShader;
	CD3DX12_PIPELINE_STATE_STREAM_PS PixelShader;
	CD3DX12_PIPELINE_STATE_STREAM_GS GeometryShader;
	CD3DX12_PIPELINE_STATE_STREAM_DS DomainShader;
	CD3DX12_PIPELINE_STATE_STREAM_HS HullShader;
	CD3DX12_PIPELINE_STATE_STREAM_MS MeshShader;
	CD3DX12_PIPELINE_STATE_STREAM_AS AmplificationShader;
	CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC BlendState;
	CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER RasterState;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL1 DepthState;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DepthFormat;
	CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopology;
	CD3DX12_PIPELINE_STATE_STREAM_FLAGS Flags;
	CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_DESC SampleDesc;
};



class FD3D12PipelineStateManager : public FD3D12AdapterChild
{
public:
	FD3D12PipelineStateManager(FD3D12Adapter* InParent);

	FD3D12PipelineState* GetPipelineState(const FGraphicsPipelineStateInitializer& Initializer, const FD3D12RootSignature* InRootSignature);

	D3D12_PIPELINE_STATE_STREAM_DESC SetPipelineDesc(const FGraphicsPipelineStateInitializer& Initializer, FD3D12_GRAPHICS_PIPELINE_STATE_DESC& PipelineStateDesc, const FD3D12RootSignature* InRootSignature);

	void CachePSO();

private:
	TUnordoredMap<FGraphicsPipelineStateInitializer, struct FD3D12PipelineState*> PSOMap;
	TRefCountPtr<ID3D12PipelineLibrary1> PipelineLibrary;
	TArray<uint8> Cache;


};



struct FD3D12PipelineState : public  FD3D12AdapterChild
{
	FD3D12PipelineState(FD3D12Adapter* InParent) : FD3D12AdapterChild(InParent) {}

	void Create(const D3D12_PIPELINE_STATE_STREAM_DESC& Initalizer, const FD3D12RootSignature* InRootSignature);

	TRefCountPtr<ID3D12PipelineState> PSO;
};



struct FD3D12GraphicsPipelineState : public FRHIGraphicsPipelineState
{
	FD3D12GraphicsPipelineState(const FGraphicsPipelineStateInitializer& Initializer, const FD3D12RootSignature* InRootSignature, FD3D12PipelineState* InPipelineState);


	FGraphicsPipelineStateInitializer PipelineStateInitializer;



	FD3D12PipelineState* PipelineState;
	const FD3D12RootSignature* const RootSignature;
};