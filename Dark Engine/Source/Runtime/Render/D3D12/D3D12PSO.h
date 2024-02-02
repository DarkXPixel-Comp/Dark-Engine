#pragma once
#include <Windows.h>
#include "D3D12Main.h"
#include <Containers/Array.h>
#include <Containers/DarkString.h>
#include <CoreDefines.h>
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
		FString vertexShaderPath,
		FString pixelShaderPath,
		TArray<D3D12_ROOT_PARAMETER1> rootParametrs, D3D12_INPUT_LAYOUT_DESC layout = Vertex::InputLayout,
		bool depth = true,
		D3D12_CULL_MODE CullMode = D3D12_CULL_MODE_BACK, D3D12_COMPARISON_FUNC depthFunc = D3D12_COMPARISON_FUNC_LESS)
	{
		//CD3DX12_ROOT_PARAMETER1 parametrs[1];
		//CD3DX12_DESCRIPTOR_RANGE1 cbvTable;
		auto staticSamplers = D3DUtil::GetStaticSamples();

		//cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

		//parametrs[0].InitAsDescriptorTable(1, &cbvTable);


		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSigDescVers(
			rootParametrs.GetSize(),
			rootParametrs.GetData(),
			staticSamplers.GetSize(),
			staticSamplers.GetData(),
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


		ComPtr<ID3DBlob> rootSignatureBlob;
		ComPtr<ID3DBlob> errorBlob;

		//D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob);
		D3D12SerializeVersionedRootSignature(&rootSigDescVers, &rootSignatureBlob, &errorBlob);


		if (errorBlob)
		{
			FString err = (const char*)errorBlob->GetBufferPointer();
		}

		DXCall(device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(),
			rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));


		auto pUtils = D3DUtil::m_Utils.Get();
		auto pCompiler = D3DUtil::m_ShaderCompiler.Get();
		ComPtr<IDxcBlob> pVertexShader;
		ComPtr<IDxcBlob> pPixelShader;

		{
			ComPtr<IDxcIncludeHandler> pIncludeHandler;
			D3DUtil::m_Utils->CreateDefaultIncludeHandler(&pIncludeHandler);



			int len = MultiByteToWideChar(CP_ACP, 0, vertexShaderPath.c_str(), -1, NULL, 0);
			wstring wPathShader(L" ", len);
			MultiByteToWideChar(CP_ACP, 0, vertexShaderPath.c_str(), -1, wPathShader.data(), len);


			LPCWSTR pszArgs[] =
			{
				L"-E main",
				L"-T vs_6_6",
				L"Zi",
				wPathShader.c_str()
				//L"-E", L"main",              // Entry point.
				//L"-T", L"vs_6_6",            // Target.
			};

			ComPtr<IDxcBlobEncoding> pSource = nullptr;
			pUtils->LoadFile(wPathShader.c_str(), nullptr, &pSource);
			DxcBuffer Source;
			Source.Ptr = pSource->GetBufferPointer();
			Source.Size = pSource->GetBufferSize();
			Source.Encoding = DXC_CP_ACP;

			wstring ws = (wchar_t*)pSource->GetBufferPointer();



			ComPtr<IDxcResult> 	pResult;
			D3DUtil::m_ShaderCompiler->Compile(&Source, pszArgs, _countof(pszArgs), pIncludeHandler.Get(), IID_PPV_ARGS(&pResult));


			pResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&pVertexShader), nullptr);

		}
		{
			ComPtr<IDxcIncludeHandler> pIncludeHandler;
			D3DUtil::m_Utils->CreateDefaultIncludeHandler(&pIncludeHandler);



			int len = MultiByteToWideChar(CP_ACP, 0, pixelShaderPath.c_str(), -1, NULL, 0);
			wstring wPathShader(L" ", len);
			MultiByteToWideChar(CP_ACP, 0, pixelShaderPath.c_str(), -1, wPathShader.data(), len);


			LPCWSTR pszArgs[] =
			{
				L"-E main",
				L"-T ps_6_6",
				L"Zi",
				wPathShader.c_str()
				//L"-E", L"main",              // Entry point.
				//L"-T", L"vs_6_6",            // Target.
			};

			ComPtr<IDxcBlobEncoding> pSource = nullptr;
			pUtils->LoadFile(wPathShader.c_str(), nullptr, &pSource);
			DxcBuffer Source;
			Source.Ptr = pSource->GetBufferPointer();
			Source.Size = pSource->GetBufferSize();
			Source.Encoding = DXC_CP_ACP;

			wstring ws = (wchar_t*)pSource->GetBufferPointer();



			ComPtr<IDxcResult> 	pResult;
			D3DUtil::m_ShaderCompiler->Compile(&Source, pszArgs, _countof(pszArgs), pIncludeHandler.Get(), IID_PPV_ARGS(&pResult));


			pResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&pPixelShader), nullptr);

		}



		//ComPtr<ID3DBlob> vertexShader;
		//ComPtr<ID3DBlob> pixelShader;

		////D3DCompileFromFile(strw(vertexShaderPath).c_str(), nullptr,
		////	nullptr, "main", "vs_5_1", 0, 0, &vertexShader, &errorBlob);


		////D3DCompileFromFile(strw(pixelShaderPath).c_str(), nullptr,
		////	nullptr, "main", "ps_5_1", 0, 0, &pixelShader, &errorBlob);


		//D3DReadFileToBlob(strw(vertexShaderPath).c_str(), &vertexShader);
		//D3DReadFileToBlob(strw(pixelShaderPath).c_str(), &pixelShader);


		D3D12_RASTERIZER_DESC rasterDesc = {};

		rasterDesc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		//rasterDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
		rasterDesc.CullMode = CullMode;


		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc = {};



		pipelineStateDesc.InputLayout = layout;
		pipelineStateDesc.pRootSignature = m_rootSignature.Get();
		pipelineStateDesc.VS = { pVertexShader->GetBufferPointer(), pVertexShader->GetBufferSize() };
		pipelineStateDesc.PS = { pPixelShader->GetBufferPointer(), pPixelShader->GetBufferSize() };
		pipelineStateDesc.RasterizerState = rasterDesc;
		pipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT());
		if(depth)
		{
			pipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(CD3DX12_DEFAULT());
			pipelineStateDesc.DepthStencilState.DepthFunc = depthFunc;
		}
		else
		{
			pipelineStateDesc.DepthStencilState.DepthEnable = false;
		}
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