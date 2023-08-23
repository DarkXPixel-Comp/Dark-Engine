#pragma once
#include <Windows.h>
#include "D3D12.h"
#include <vector>
#include <string>
#include <Core/CoreDefines.h>
#include "D3D12Mesh.h"
#include "D3D12Utils.h"





struct D3D12ShaderDesc
{

	

};



class D3D12PipelineShaderRootSignature
{
	friend class D3D12Renderer;
	friend class D3DUtil;
	static UINT _idCounter;

	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12RootSignature> m_rootSignature;
	D3D12_INPUT_LAYOUT_DESC m_inputLayout;
	UINT id;




public:
	D3D12PipelineShaderRootSignature(ID3D12Device8* device,
		std::string vertexShaderPath,
		std::string pixelShaderPath,
		std::vector<D3D12_ROOT_PARAMETER1> rootParametrs)
	{
		//CD3DX12_ROOT_PARAMETER1 parametrs[1];
		//CD3DX12_DESCRIPTOR_RANGE1 cbvTable;
		auto staticSamplers = D3DUtil::GetStaticSamples();

		//cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

		//parametrs[0].InitAsDescriptorTable(1, &cbvTable);


		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSigDescVers(
			rootParametrs.size(),
			rootParametrs.data(),
			staticSamplers.size(),
			staticSamplers.data(),
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


		ComPtr<ID3DBlob> rootSignatureBlob;
		ComPtr<ID3DBlob> errorBlob;

		//D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);
		D3D12SerializeVersionedRootSignature(&rootSigDescVers, &rootSignatureBlob, &errorBlob);


		if (errorBlob)
		{
			std::string err = (const char*)errorBlob->GetBufferPointer();
		}

		DXCall(device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
			rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));



		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;

		//D3DCompileFromFile(strw(vertexShaderPath).c_str(), nullptr,
		//	nullptr, "main", "vs_5_1", 0, 0, &vertexShader, &errorBlob);


		//D3DCompileFromFile(strw(pixelShaderPath).c_str(), nullptr,
		//	nullptr, "main", "ps_5_1", 0, 0, &pixelShader, &errorBlob);


		D3DReadFileToBlob(L"shaders/VertexShader.cso", &vertexShader);
		D3DReadFileToBlob(L"shaders/PixelShader.cso", &pixelShader);


		D3D12_RASTERIZER_DESC rasterDesc = {};

		rasterDesc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		//rasterDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;


		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = {};



		pipelineStateDesc.InputLayout = Vertex::InputLayout;
		pipelineStateDesc.pRootSignature = m_rootSignature.Get();
		pipelineStateDesc.VS = { vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() };
		pipelineStateDesc.PS = { pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() };
		pipelineStateDesc.RasterizerState = rasterDesc;
		pipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT());
		pipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(CD3DX12_DEFAULT());
		pipelineStateDesc.SampleMask = UINT_MAX;
		pipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineStateDesc.NumRenderTargets = 1;
		pipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		pipelineStateDesc.SampleDesc = { 1, 0 };
		pipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		auto er = (device->CreateGraphicsPipelineState(&pipelineStateDesc, IID_PPV_ARGS(&m_pipelineState)));

		

		id = _idCounter++;

	}

	UINT GetID() const { return id; }




};