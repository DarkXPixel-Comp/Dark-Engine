#include "D3D12PipelineState.h"
#include "D3D12Adapter.h"
#include "D3D12Shader.h"
#include <D3D12Util.h>


FD3D12GraphicsPipelineState::FD3D12GraphicsPipelineState(const FGraphicsPipelineStateInitializer& Initializer, const FD3D12RootSignature* InRootSignature, FD3D12PipelineState* InPipelineState):
	RootSignature(InRootSignature)
{

}

FD3D12PipelineState* FD3D12PipelineStateManager::GetPipelineState(const FGraphicsPipelineStateInitializer& Initializer, const FD3D12RootSignature* InRootSignature)
{
	FD3D12PipelineState* Result = nullptr;
	auto ItPSO = PSOMap.find(Initializer);

	if (ItPSO == PSOMap.end())
	{
		Result = new FD3D12PipelineState(GetParentAdapter());

		Result->Create(Initializer, InRootSignature);
	}

	return Result;
}

void FD3D12PipelineState::Create(const FGraphicsPipelineStateInitializer& Initalizer, const FD3D12RootSignature* InRootSignature)
{
	ID3D12Device2* const Device = Parent->GetD3DDevice2();


	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc = {};
	PipelineStateDesc.pRootSignature = InRootSignature->GetRootSignature();
	PipelineStateDesc.InputLayout.pInputElementDescs = ((FD3D12VertexDeclaration*)Initalizer.BoundShaderState.VertexDeclaration)->VertexElements.GetData();
	PipelineStateDesc.InputLayout.NumElements = ((FD3D12VertexDeclaration*)Initalizer.BoundShaderState.VertexDeclaration)->VertexElements.Num();
	PipelineStateDesc.VS = ((FD3D12VertexShader*)Initalizer.BoundShaderState.VertexShaderRHI)->GetShaderByteCode();
	PipelineStateDesc.PS = ((FD3D12PixelShader*)Initalizer.BoundShaderState.PixelShaderRHI)->GetShaderByteCode();
	PipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT());
	PipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(CD3DX12_DEFAULT());
	PipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(CD3DX12_DEFAULT());
	PipelineStateDesc.PrimitiveTopologyType = Initalizer.PrimitiveType == PT_TriangleList || Initalizer.PrimitiveType == PT_TriangleStrip ? D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE : D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
	PipelineStateDesc.SampleDesc = { 1, 0 };

	D3D12_PIPELINE_STATE_STREAM_DESC StreamDesc = { sizeof(PipelineStateDesc), &PipelineStateDesc };

	DXCall(Device->CreatePipelineState(&StreamDesc, IID_PPV_ARGS(&PSO)));
}
