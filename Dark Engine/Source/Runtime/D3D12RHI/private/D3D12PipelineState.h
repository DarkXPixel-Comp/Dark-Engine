#pragma once
#include "D3D12RHICommon.h"	
#include "D3D12RootSignature.h"
#include "Containers/UnordoredMap.h"
#include "d3d12shader.h"
#include "dxcapi.h"
#include <d3dx12/d3dx12_pipeline_state_stream.h>


class FD3D12PipelineState;
class FD3D12VertexShader;
class FD3D12PixelShader;




struct FD3D12_GRAPHICS_PIPELINE_STATE_DESC
{
	CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE RootSignature;
	CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
	CD3DX12_PIPELINE_STATE_STREAM_VS VertexShader;
	CD3DX12_PIPELINE_STATE_STREAM_PS PixelShader;
//	CD3DX12_PIPELINE_STATE_STREAM_GS GeometryShader;
//	CD3DX12_PIPELINE_STATE_STREAM_DS DomainShader;
//	CD3DX12_PIPELINE_STATE_STREAM_HS HullShader;
//	CD3DX12_PIPELINE_STATE_STREAM_MS MeshShader;
//	CD3DX12_PIPELINE_STATE_STREAM_AS AmplificationShader;
	CD3DX12_PIPELINE_STATE_STREAM_BLEND_DESC BlendState;
	CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER RasterState;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL DepthState;
	CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DepthFormat;
	CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopology;
	CD3DX12_PIPELINE_STATE_STREAM_FLAGS Flags;
	CD3DX12_PIPELINE_STATE_STREAM_SAMPLE_DESC SampleDesc;
	CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RenderTargetFormats;
	CD3DX12_PIPELINE_STATE_STREAM_NODE_MASK NodeMask;
};

struct FD3D12_COMPUTE_PIPELINE_STATE_DESC
{
	CD3DX12_PIPELINE_STATE_STREAM_NODE_MASK NodeMask;
	CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE RootSignature;
	CD3DX12_PIPELINE_STATE_STREAM_CS ComputeShader;

};



class FD3D12PipelineStateManager : public FD3D12AdapterChild
{
public:
	FD3D12PipelineStateManager(FD3D12Adapter* InParent);

	FD3D12PipelineState* GetPipelineState(const FGraphicsPipelineStateInitializer& Initializer, const FD3D12RootSignature* InRootSignature);
	FD3D12PipelineState* GetPipelineState(FD3D12ComputeShader* ComputeShader, uint64 Hash);

	D3D12_PIPELINE_STATE_STREAM_DESC SetPipelineDesc(const FGraphicsPipelineStateInitializer& Initializer, FD3D12_GRAPHICS_PIPELINE_STATE_DESC& PipelineStateDesc, const FD3D12RootSignature* InRootSignature);
	D3D12_PIPELINE_STATE_STREAM_DESC SetPipelineDesc(FD3D12ComputeShader* ComputeShader, FD3D12_COMPUTE_PIPELINE_STATE_DESC& PipelineStateDesc);

	void CachePSO();

private:
	TUnordoredMap<FGraphicsPipelineStateInitializer, struct FD3D12PipelineState*> PSOMap;
	TMap<uint64, struct FD3D12PipelineState*> ComputePSOMap;
	TRefCountPtr<ID3D12PipelineLibrary1> PipelineLibrary;
	TArray<uint8> Cache;


};



struct FD3D12PipelineState : public  FD3D12AdapterChild
{
	FD3D12PipelineState(FD3D12Adapter* InParent) : FD3D12AdapterChild(InParent) {}

	void Create(const D3D12_PIPELINE_STATE_STREAM_DESC& Initalizer, const FD3D12RootSignature* InRootSignature);

	TRefCountPtr<ID3D12PipelineState> PSO;
};


struct FD3D12ComputePipelineState : public FRHIComputePipelineState
{
	FD3D12ComputePipelineState(FD3D12ComputeShader* InComputeShader, const FD3D12RootSignature* InRootSignature, FD3D12PipelineState* InPipelineState);


	TRefCountPtr<FD3D12ComputeShader> ComputeShader;


	FD3D12PipelineState* PipelineState;
	const FD3D12RootSignature* const RootSignature;
};


struct FD3D12GraphicsPipelineState : public FRHIGraphicsPipelineState
{
	FD3D12GraphicsPipelineState(const FGraphicsPipelineStateInitializer& Initializer, const FD3D12RootSignature* InRootSignature, FD3D12PipelineState* InPipelineState);

	FD3D12VertexShader* GetVertexShader() const { return (FD3D12VertexShader*)PipelineStateInitializer.BoundShaderState.VertexShaderRHI; }
	FD3D12PixelShader* GetPixelShader() const { return (FD3D12PixelShader*)PipelineStateInitializer.BoundShaderState.PixelShaderRHI; }
	//FD3D12VertexShader* GetVertexShader() const { return (FD3D12VertexShader*)PipelineStateInitializer.BoundShaderState.VertexShaderRHI; }


	FGraphicsPipelineStateInitializer PipelineStateInitializer;
	FD3D12PipelineState* PipelineState;
	const FD3D12RootSignature* const RootSignature;
};



class FD3D12RasterizerState : public FRHIRasterizerState
{
public:
	D3D12_RASTERIZER_DESC Desc;
};