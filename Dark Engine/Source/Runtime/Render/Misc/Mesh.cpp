#include "Mesh.h"

FMesh::FMesh(VertexesArray, IndexesArray, XMVECTOR, XMVECTOR, XMVECTOR, XMVECTOR, string)
{
	Renderer = static_cast<D3D12Renderer*>(GEngine.GetRenderer());

	if (Vertices.empty())
		return;


	ResourceUploadBatch Upload(Renderer->Device.Get());

	CreateStaticBuffer(Renderer->Device.Get(), Upload,
		Vertices.data(),
		Vertices.size(),
		sizeof(VertexPositionNormalColorTexture),
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		&VertexBuffer);

	CreateStaticBuffer(Renderer->Device.Get(), Upload,
		Indices.data(),
		Indices.size(),
		sizeof(WORD),
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&IndexBuffer);

	Upload.End(Renderer->CommandQueue.Get()).wait();



	ComPtr<ID3DBlob> RootSignatureBlob;
	ComPtr<ID3DBlob> ErrorBlob;


	CD3DX12_ROOT_PARAMETER RootParametrs[1];


	RootParametrs[0].InitAsConstants(sizeof(XMMATRIX) / 4, 0, 0, D3D12_SHADER_VISIBILITY_ALL);

	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;


	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
	RootSignatureDesc.Init(1, RootParametrs, 0,
		nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &RootSignatureBlob, &ErrorBlob);


	Renderer->Device->CreateRootSignature(0, RootSignatureBlob->GetBufferPointer(),
		RootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&RootSignature));

	ComPtr<ID3DBlob> VertexShader;
	ComPtr<ID3DBlob> PixelShader;

	D3DCompileFromFile(L"shaders/VertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0",
		0, 0, &VertexShader, nullptr);
	D3DCompileFromFile(L"shaders/PixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0",
		0, 0, &PixelShader, nullptr);






	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc = {};

	PipelineStateDesc.InputLayout = VertexPositionNormalColorTexture::InputLayout;
	PipelineStateDesc.pRootSignature = RootSignature.Get();
	PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	PipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(VertexShader->GetBufferPointer(), VertexShader->GetBufferSize());
	PipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(PixelShader->GetBufferPointer(), PixelShader->GetBufferSize());
	PipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	PipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	PipelineStateDesc.DepthStencilState.DepthEnable = FALSE;
	PipelineStateDesc.DepthStencilState.StencilEnable = FALSE;
	PipelineStateDesc.SampleMask = UINT_MAX;
	PipelineStateDesc.NumRenderTargets = 1;
	PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	PipelineStateDesc.SampleDesc = { 1, 0 };
	//PipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;



	DXCall(Renderer->Device->CreateGraphicsPipelineState(&PipelineStateDesc, IID_PPV_ARGS(&PipelineState)));





}
