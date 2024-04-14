#include "D3D12PipelineState.h"
#include "D3D12Adapter.h"
#include "D3D12Shader.h"
#include <D3D12Util.h>
#include <fstream>
#include <filesystem>
#include "Misc/Paths.h"




FD3D12PipelineStateManager::FD3D12PipelineStateManager(FD3D12Adapter* InParent) : FD3D12AdapterChild(InParent)
{
	std::ifstream fin;
	FString File = FString::PrintF(TEXT("%sPSO.cache"), *FPaths::CacheDir());
	std::filesystem::path Path = *File;
	fin.open(*File, std::ios::binary);
	if (fin.is_open())
	{
		uint64 SizeFile = std::filesystem::file_size(Path);
		Cache.Resize(SizeFile);
		fin.read((char*)Cache.GetData(), SizeFile);
		fin.close();
	}
	

	HRESULT hr = S_OK;
	DXCall(hr = GetParentAdapter()->GetD3DDevice2()->CreatePipelineLibrary(Cache.GetData(), Cache.Num(),
		IID_PPV_ARGS(&PipelineLibrary)));
	//PipelineLibrary->Serialize();
}

void FD3D12PipelineStateManager::CachePSO()
{
	std::ofstream fout;
	FString File = FString::PrintF(TEXT("%sPSO.cache"), *FPaths::CacheDir());
	fout.open(*File, ios::binary);

	if (fout.is_open())
	{
		Cache.Resize(PipelineLibrary->GetSerializedSize());
		PipelineLibrary->Serialize(Cache.GetData(), Cache.Num());

		fout.write((char*)Cache.GetData(), Cache.Num());
		fout.close();
	}
}


FD3D12PipelineState* FD3D12PipelineStateManager::GetPipelineState(const FGraphicsPipelineStateInitializer& Initializer, const FD3D12RootSignature* InRootSignature)
{
	FD3D12PipelineState* Result = nullptr;
	FD3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc = {};
	auto ItPSO = PSOMap.find(Initializer);

	if (ItPSO == PSOMap.end())
	{
		Result = new FD3D12PipelineState(GetParentAdapter());
		TRefCountPtr<ID3D12PipelineState> PipelineState;
		FString Hash = FString::NumToString(std::hash<FGraphicsPipelineStateInitializer>{}(Initializer));
		D3D12_PIPELINE_STATE_STREAM_DESC StreamDesc = SetPipelineDesc(Initializer, PipelineStateDesc, InRootSignature);
		HRESULT hr = S_OK;
		DXCall(hr = PipelineLibrary->LoadPipeline(*Hash, &StreamDesc,
			IID_PPV_ARGS(&PipelineState)));

		if (PipelineState)
		{
			Result->PSO = PipelineState;
		}
		else
		{
			Result->Create(StreamDesc, InRootSignature);
			
			if (Result->PSO)
			{
				PipelineLibrary->StorePipeline(*Hash, Result->PSO.Get());
				//CachePSO();
			}
		}
		PSOMap.emplace(Initializer, Result);
	}

	return Result;
}

D3D12_PIPELINE_STATE_STREAM_DESC FD3D12PipelineStateManager::SetPipelineDesc(const FGraphicsPipelineStateInitializer& Initializer,
	FD3D12_GRAPHICS_PIPELINE_STATE_DESC& PipelineStateDesc, const FD3D12RootSignature* InRootSignature)
{

	/*D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc = {};
	PipelineStateDesc.pRootSignature = InRootSignature->GetRootSignature();
	PipelineStateDesc.InputLayout.pInputElementDescs = ((FD3D12VertexDeclaration*)Initalizer.BoundShaderState.VertexDeclaration)->VertexElements.GetData();
	PipelineStateDesc.InputLayout.NumElements = ((FD3D12VertexDeclaration*)Initalizer.BoundShaderState.VertexDeclaration)->VertexElements.Num();
	PipelineStateDesc.VS = ((FD3D12VertexShader*)Initalizer.BoundShaderState.VertexShaderRHI)->GetShaderByteCode();
	PipelineStateDesc.PS = ((FD3D12PixelShader*)Initalizer.BoundShaderState.PixelShaderRHI)->GetShaderByteCode();
	PipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT());
	PipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(CD3DX12_DEFAULT());
	PipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(CD3DX12_DEFAULT());
	PipelineStateDesc.PrimitiveTopologyType = Initalizer.PrimitiveType == PT_TriangleList || Initalizer.PrimitiveType == PT_TriangleStrip ? D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE : D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
	PipelineStateDesc.SampleDesc = { 1, 0 };*/
	PipelineStateDesc = {};
	PipelineStateDesc.RootSignature = InRootSignature->GetRootSignature();
	PipelineStateDesc.InputLayout = Initializer.BoundShaderState.VertexDeclaration ? static_cast<FD3D12VertexDeclaration*>(Initializer.BoundShaderState.VertexDeclaration)->GetLayoutDsec() : D3D12_INPUT_LAYOUT_DESC();
	PipelineStateDesc.VertexShader = Initializer.BoundShaderState.VertexShaderRHI ? static_cast<FD3D12VertexShader*>(Initializer.BoundShaderState.VertexShaderRHI)->GetShaderByteCode() : D3D12_SHADER_BYTECODE();
	PipelineStateDesc.PixelShader = Initializer.BoundShaderState.PixelShaderRHI ? static_cast<FD3D12PixelShader*>(Initializer.BoundShaderState.PixelShaderRHI)->GetShaderByteCode() : D3D12_SHADER_BYTECODE();
	PipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT());
	PipelineStateDesc.RasterState = CD3DX12_RASTERIZER_DESC(CD3DX12_DEFAULT());
	PipelineStateDesc.DepthState = CD3DX12_DEPTH_STENCIL_DESC1(CD3DX12_DEFAULT());
	PipelineStateDesc.PrimitiveTopology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	PipelineStateDesc.SampleDesc = { 1, 0 };




	D3D12_PIPELINE_STATE_STREAM_DESC StreamDesc = { sizeof(PipelineStateDesc), &PipelineStateDesc };
	return StreamDesc;
}



FD3D12GraphicsPipelineState::FD3D12GraphicsPipelineState(const FGraphicsPipelineStateInitializer& Initializer, const FD3D12RootSignature* InRootSignature, FD3D12PipelineState* InPipelineState) :
	RootSignature(InRootSignature),
	PipelineState(InPipelineState)
{

}

void FD3D12PipelineState::Create(const D3D12_PIPELINE_STATE_STREAM_DESC& Initalizer, const FD3D12RootSignature* InRootSignature)
{
	ID3D12Device2* const Device = Parent->GetD3DDevice2();

	/*D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc = {};
	PipelineStateDesc.pRootSignature = InRootSignature->GetRootSignature();
	PipelineStateDesc.InputLayout.pInputElementDescs = ((FD3D12VertexDeclaration*)Initalizer.BoundShaderState.VertexDeclaration)->VertexElements.GetData();
	PipelineStateDesc.InputLayout.NumElements = ((FD3D12VertexDeclaration*)Initalizer.BoundShaderState.VertexDeclaration)->VertexElements.Num();
	PipelineStateDesc.VS = ((FD3D12VertexShader*)Initalizer.BoundShaderState.VertexShaderRHI)->GetShaderByteCode();
	PipelineStateDesc.PS = ((FD3D12PixelShader*)Initalizer.BoundShaderState.PixelShaderRHI)->GetShaderByteCode();
	PipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT());
	PipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(CD3DX12_DEFAULT());
	PipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(CD3DX12_DEFAULT());
	PipelineStateDesc.PrimitiveTopologyType = Initalizer.PrimitiveType == PT_TriangleList || Initalizer.PrimitiveType == PT_TriangleStrip ? D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE : D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
	PipelineStateDesc.SampleDesc = { 1, 0 };*/
				 

	DXCall(Device->CreatePipelineState(&Initalizer, IID_PPV_ARGS(&PSO)));
}
