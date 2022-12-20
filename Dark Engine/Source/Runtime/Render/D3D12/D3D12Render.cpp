#include "D3D12Render.h"

#include <Source/Runtime/Engine/public/DEngine.h>
#include <DDSTextureLoader.h>
#include <DirectXHelpers.h>
#include <Model.h>
#include <BinaryReader.h>
#include <thread>

//#include <SimpleMath.h>
#undef max

using namespace D3D12MA;


void TransitionResource(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList,
	Microsoft::WRL::ComPtr<ID3D12Resource> resource,
	D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		resource.Get(),
		beforeState, afterState);

	commandList->ResourceBarrier(1, &barrier);


}


D3D12Renderer::D3D12Renderer()
{
}

void D3D12Renderer::Init()
{
	//D3D12MA::CreateAllocator()


	

	HRESULT Error = S_OK;

	DXCall(Error = CreateDXGIFactory2(0, IID_PPV_ARGS(&Factory)));
	DXCall(Error = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&Device)));

	DXCall(Factory->EnumAdapterByLuid(Device->GetAdapterLuid(), IID_PPV_ARGS(&Adapter)));

	D3D12_COMMAND_QUEUE_DESC CommandQueueDesc = {};

	CommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	CommandQueueDesc.NodeMask = 0;
	CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;


	DXCall(Error = Device->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&CommandQueue)));

	DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};

	SwapChainDesc.Width = GEngine.GetWindow()->GetWitdh();
	SwapChainDesc.Height = GEngine.GetWindow()->GetHeight();
	SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.SampleDesc = { 1, 0 };
	SwapChainDesc.BufferCount = BACK_BUFFER_COUNT;
	SwapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	ComPtr<IDXGISwapChain1> SwapChain1;

	Error = Factory->CreateSwapChainForHwnd(CommandQueue.Get(), GEngine.GetWindow()->GetHandle(), &SwapChainDesc, nullptr, nullptr, &SwapChain1);


	SwapChain1.As(&SwapChain);

	graphicsMemory = make_unique<GraphicsMemory>(Device.Get());



	


	/*D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc = {};
	DescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	DescriptorHeapDesc.NodeMask = 0;
	DescriptorHeapDesc.NumDescriptors = BACK_BUFFER_COUNT;

	DXCall(Device->CreateDescriptorHeap(&DescriptorHeapDesc, IID_PPV_ARGS(&RTDescriptorHeap)));*/

	RTDescriptorHeap = std::make_unique<DescriptorHeap>(Device.Get(),
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		BACK_BUFFER_COUNT);

	DSDescriptorHeap = make_unique<DescriptorHeap>(Device.Get(),
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		1);

	SRDescriptorHeap = make_unique<DescriptorHeap>(Device.Get(),
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_1);

	//D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_1
	

	RTHandleSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	for (size_t i = 0; i < BACK_BUFFER_COUNT; i++)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = RTDescriptorHeap->GetCpuHandle(i);
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&BackBuffers[i]));

		Device->CreateRenderTargetView(BackBuffers[i].Get(), nullptr, cpuHandle);
	}




	
	

	(Error = Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&CommandAllocator)));

	Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator.Get(),
		nullptr, IID_PPV_ARGS(&GraphicsCommandList));
		

	DXCall(Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));


	FenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);


	ResourceUploadBatch ResourceUpload(Device.Get());

	ResourceUpload.Begin();

	CreateStaticBuffer(Device.Get(), ResourceUpload,
		Vertices,
		_countof(Vertices),
		sizeof(Vertex),
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		&VertexBuffer);



	VertexBufferView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
	VertexBufferView.SizeInBytes = sizeof(Vertices);
	VertexBufferView.StrideInBytes = sizeof(Vertex);

	int ssss = _countof(Vertices);


	auto test = sizeof(decltype(Vertices));

	CreateStaticBuffer(Device.Get(), ResourceUpload,
		Indices,
		_countof(Indices),
		sizeof(WORD),
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&IndexBuffer);

	IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
	IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
	IndexBufferView.SizeInBytes = sizeof(Indices);


	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
	depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
	depthOptimizedClearValue.DepthStencil.Stencil = 0;

	auto Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto TexScreen = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, 1920, 1080,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);


	Device->CreateCommittedResource(&Properties,
		D3D12_HEAP_FLAG_NONE,
		&TexScreen,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthOptimizedClearValue,
		IID_PPV_ARGS(&DepthBuffer)
	);

	Device->CreateDepthStencilView(DepthBuffer.Get(), &depthStencilDesc,
		DSDescriptorHeap->GetFirstCpuHandle());





	ResourceUpload.End(CommandQueue.Get()).wait();



	/*D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	  { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};*/


	const D3D12_INPUT_LAYOUT_DESC inputLayout = Vertex::InputLayout;

	D3D12_FEATURE_DATA_ROOT_SIGNATURE FeatureData = { D3D_ROOT_SIGNATURE_VERSION_1_0 };


	ComPtr<ID3DBlob> RootSignatureBlob;
	ComPtr<ID3DBlob> ErrorBlob;


	CD3DX12_ROOT_PARAMETER RootParametrs[7];

	CD3DX12_DESCRIPTOR_RANGE ranges[1];
	ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1,
		1, 0);


	//RootParametrs[0].InitAsConstants((sizeof(XMMATRIX) / 4) * 2, 0, 0, D3D12_SHADER_VISIBILITY_ALL);


	RootParametrs[0].InitAsConstants(sizeof(XMMATRIX) / 4, 0, 0, D3D12_SHADER_VISIBILITY_ALL);
	RootParametrs[1].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
	RootParametrs[2].InitAsConstants(sizeof(XMFLOAT4) / 4, 2, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	RootParametrs[3].InitAsConstants(sizeof(XMFLOAT4) / 4, 3, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	RootParametrs[4].InitAsConstants(sizeof(XMFLOAT4) / 4, 4, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	RootParametrs[5].InitAsConstants(sizeof(XMMATRIX) / 4, 5, 0, D3D12_SHADER_VISIBILITY_ALL);
	RootParametrs[6].InitAsConstants(1, 6, 0, D3D12_SHADER_VISIBILITY_ALL);

	//RootParametrs[2].InitAsShaderResourceView(4, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	D3D12_STATIC_SAMPLER_DESC sampler = {};


	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


	Bone = Model::CreateFromCMO(Device.Get(), L"Resources/test.cmo");


	TestBone = Bone->meshes[0]->opaqueMeshParts[0].get();


	size_t dataSize = 0;
	std::unique_ptr<uint8_t[]> data;

	BinaryReader::ReadEntireFile(L"Resources/Texture.dds", data, &dataSize);

	vector<char> bb(dataSize);

	for (size_t i = 0; i < dataSize; i++)
	{
		bb[i] = *(data.get() + i);
	}



	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;


	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
	RootSignatureDesc.Init(_countof(RootParametrs), RootParametrs, 1,
		&sampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &RootSignatureBlob, &ErrorBlob);


	Device->CreateRootSignature(0, RootSignatureBlob->GetBufferPointer(),
		RootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&RootSignature));

	ComPtr<ID3DBlob> VertexShader;
	ComPtr<ID3DBlob> PixelShader;

	D3DCompileFromFile(L"shaders/VertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0",
		0, 0, &VertexShader, nullptr);
	D3DCompileFromFile(L"shaders/PixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0",
		0, 0, &PixelShader, nullptr);







	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc = {};

	PipelineStateDesc.InputLayout = (inputLayout);
	//PipelineStateDesc.InputLayout.pInputElementDescs = inputLayout;
	PipelineStateDesc.pRootSignature = RootSignature.Get();
	PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	PipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(VertexShader->GetBufferPointer(), VertexShader->GetBufferSize());
	PipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(PixelShader->GetBufferPointer(), PixelShader->GetBufferSize());
	PipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	PipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	PipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	PipelineStateDesc.SampleMask = UINT_MAX;
	PipelineStateDesc.NumRenderTargets = 1;
	PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	PipelineStateDesc.SampleDesc = { 1, 0 };
	//PipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	HRESULT hr;


	DXCall(Error = Device->CreateGraphicsPipelineState(&PipelineStateDesc, IID_PPV_ARGS(&PipelineState)));


	ResourceUpload.Begin();



	CreateDDSTextureFromFile(Device.Get(), ResourceUpload,
		L"C:\\Users\\nahmu\\source\\repos\\Dark Engine\\Dark Engine\\Resources\\Texture.dds", &Texture);



	ResourceUpload.End(CommandQueue.Get());

	CreateShaderResourceView(Device.Get(), Texture.Get(), SRDescriptorHeap->GetFirstCpuHandle());

	

	ScissorRect = { 0, 0, (long)GEngine.GetWindow()->GetWitdh(), (long)GEngine.GetWindow()->GetHeight() };

	Viewport = { 0.f, 0.f,  (float)GEngine.GetWindow()->GetWitdh(),  (float)GEngine.GetWindow()->GetHeight(), 0.f, 1.f };


	

	CameraX = 0;
	CameraY = 0;
	CameraZ = -6;

	pitch = 0;
	yaw = 90;
	roll = 0;

	ScaleX = 1;
	ScaleY = 1;
	ScaleZ = 1;

	x = 0;
	y = 0;
	z = 0;

	Scale = { 1, 1, 1 };
	
	//auto M = Model::Crea


	D3D12Mesh model;

	//std::vector<Vertex> vert(&Vertices[0], &Vertices[_countof(Vertices) - 1]);


	D3D12MeshInitInfo info;

	info.TexturePath = L"C:\\Users\\nahmu\\source\\repos\\Dark Engine\\Dark Engine\\Resources\\Texture.dds";
	info.VertexShader = L"shaders/VertexShader.hlsl";
	info.PixelShader = L"shaders/PixelShader.hlsl";
	info.Vertices.insert(info.Vertices.begin(),	begin(Vertices), end(Vertices));
	info.Indices.insert(begin(info.Indices), begin(Indices), end(Indices));


	D3D12Mesh1 mesh;

	mesh.Init(Device.Get(), CommandQueue.Get(), Vertices,
		_countof(Vertices), Indices, _countof(Indices));


	



	/*for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			int32_t index = i * 10 + j * 1;
			models.emplace_back();
			models[index].Init(this, info);
			models[index].Position = XMFLOAT3(i * 5, j * 5, 0);
		}
	}

*/




	/*models.emplace_back();
	models.emplace_back();

	models[0].Init(this, info);
	models[1].Init(this, info);

	models[1].Position = XMFLOAT3(2, 5, 0);*/


	//model.Init(this, info);


//	auto s = importer.ReadFile("Models/spere.obj", aiProcess_Triangulate);




}

using namespace DirectX::SimpleMath;









void D3D12Renderer::BeginFrame()
{











}

void D3D12Renderer::Render()
{		
	/*CommandConsole::Print(std::to_string(timefps).c_str());
	CommandConsole::Print("\n");*/

	/*CameraX = 0;
	CameraY = 0;
	CameraZ = 0;
	*/

	auto scene = GEngine.GetWorld();

	static bool on = true;

	if (on)
	{
		D3D12Mesh1* mesh = new D3D12Mesh1();
		mesh->Init(Device.Get(), CommandQueue.Get(), Vertices,
			_countof(Vertices), Indices, _countof(Indices));




		/*for (size_t i = 0; i < 1000; i++)
		{
			auto actor = scene->CreateActor();
			actor->SetMesh(mesh);
			actor->SetLocation({ i * 2.f , 0.f, 0.f });
		}*/

		/*thread th([&]()
			{
				while (true)
				{
					Update();
				}
			});*/

		//th.detach();

		auto me = LoadMesh("Models/sphere.obj");

		for (size_t i = 0; i < 1; i++)
		{
			auto actor = scene->CreateActor();

			actor->SetMesh(me[0]);

			actor->AddLocation(i * 3);
		}


		//actor->SetScaling({ 0.1, 0.1, 0.1 });

		on = false;



	}
	
	//this_thread::sleep_for(1ns);

	CommandConsole::Print(to_string(GEngine.GetDeltaTime()).c_str());
	CommandConsole::Print("\n");







	Update();

	RenderScene();

	return;

}

void D3D12Renderer::RenderObj()
{
}



void D3D12Renderer::RenderScene()
{
	vector<ID3D12CommandList*> CommandLists;

	auto scene = GEngine.GetWorld();
	auto& actors = scene->GetActors();

	//scene->GetCamera()->SetLocation({ CameraX, CameraY, CameraZ });
	//scene->GetCamera()->SetRotation({ 0, pitch, yaw });



	auto BackBuffer = BackBuffers[CurrentBackBufferIndex];

	CD3DX12_RESOURCE_BARRIER Barrier;

	CommandAllocator->Reset();

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(RTDescriptorHeap->GetCpuHandle(CurrentBackBufferIndex));
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsv(DSDescriptorHeap->GetFirstCpuHandle());



	GraphicsCommandList->Reset(CommandAllocator.Get(), PipelineState.Get());


	GraphicsCommandList->OMSetRenderTargets(1, &rtv,
		FALSE, &dsv);


	FLOAT clearColor[] = { 0.4f, 0.6f, 0.9f, 1.0f };

	GraphicsCommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
	GraphicsCommandList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH,
		1, 0.f, 0, nullptr);

	GraphicsCommandList->Close();

	CommandLists.push_back(GraphicsCommandList.Get());







	for (auto& i : actors)
	{
		if (!i->GetModel())
			continue;


		auto model = i->GetModel();
		model->Update();

		model->m_Allocator->Reset();
		model->m_CommandList->Reset(model->m_Allocator.Get(), PipelineState.Get());
		
		Barrier = CD3DX12_RESOURCE_BARRIER::Transition(
						BackBuffer.Get(),
						D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			
			
		model->m_CommandList->SetGraphicsRootSignature(RootSignature.Get());
			
		ID3D12DescriptorHeap* ppHeaps[] = { SRDescriptorHeap->Heap() };
		model->m_CommandList->SetDescriptorHeaps(1, ppHeaps);
			
		model->m_CommandList->SetGraphicsRootDescriptorTable(1, SRDescriptorHeap->GetFirstGpuHandle());
			
		model->m_CommandList->RSSetViewports(1, &Viewport);
		model->m_CommandList->RSSetScissorRects(1, &ScissorRect);
			
		model->m_CommandList->ResourceBarrier(1, &Barrier);
			
		model->m_CommandList->OMSetRenderTargets(1, &rtv,
		FALSE, &dsv);



		model->m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				
		model->m_CommandList->IASetVertexBuffers(0, 1, &model->Mesh->VertexBufferView);
		model->m_CommandList->IASetIndexBuffer(&model->Mesh->IndexBufferView);
		
		
		XMMATRIX MVP = XMMatrixMultiply(model->ModelMatrix, scene->GetCamera()->GetMatrixCamera());
		
		model->m_CommandList->SetGraphicsRoot32BitConstants(0, sizeof(XMMATRIX) / 4,
					&MVP, 0);
		
		model->m_CommandList->SetGraphicsRoot32BitConstants(5, sizeof(XMMATRIX) / 4,
					&model->ModelMatrix, 0);
		
		
		XMVECTOR Col = XMVectorSet(1.f, 1.f, 1.f, 1.f),
					CamPos = XMVectorSet(CameraX, CameraY, CameraZ, 0.f),
					LPos = XMVectorSet(LightPosition.x, LightPosition.y, LightPosition.z, 0.f);
		
		
		model->m_CommandList->SetGraphicsRoot32BitConstants(2, 4,
				&Col, 0);
		model->m_CommandList->SetGraphicsRoot32BitConstants(3, 4,
					&LPos, 0);
		model->m_CommandList->SetGraphicsRoot32BitConstants(4, 4,
					&CamPos, 0);
		model->m_CommandList->SetGraphicsRoot32BitConstants(6, 1,
					&iTime, 0);
		
		model->m_CommandList->DrawIndexedInstanced(model->Mesh->Indexes.size(), 1,
					0, 0, 0);
		
		Barrier = CD3DX12_RESOURCE_BARRIER::Transition(
					BackBuffer.Get(),
					D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		model->m_CommandList->ResourceBarrier(1, &Barrier);
	
		
		model->m_CommandList->Close();
		
		CommandLists.push_back(model->m_CommandList.Get());


		

	}


	CommandQueue->ExecuteCommandLists(CommandLists.size(), CommandLists.data());

	WaitFrame();

	SwapChain->Present(0, 0);

	graphicsMemory->Commit(CommandQueue.Get());



	CurrentBackBufferIndex = SwapChain->GetCurrentBackBufferIndex();







	CommandLists.clear();

}



void D3D12Renderer::EndFrame()
{
}

std::vector<D3D12Mesh1*> D3D12Renderer::LoadMesh(std::string path)
{
	std::vector<D3D12Mesh1*> meshes;
	
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);


	if (!scene)
		return meshes;


	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* Mesh = scene->mMeshes[i];

		D3D12Mesh1* mesh = new D3D12Mesh1;

		std::vector<Vertex> vertices(Mesh->mNumVertices);
		std::vector<WORD> indices;


		for (size_t i = 0; i < Mesh->mNumVertices; i++)
		{
			Vertex vertex;

			XMFLOAT3 position;

			position.x = Mesh->mVertices[i].x;
			position.y = Mesh->mVertices[i].y;
			position.z = Mesh->mVertices[i].z;

			vertex.position = position;


			XMFLOAT3 normal;

			normal.x = Mesh->mNormals[i].x;
			normal.y = Mesh->mNormals[i].y;
			normal.z = Mesh->mNormals[i].z;

			vertex.normal = normal;


			if (Mesh->HasTextureCoords(0))
			{
				XMFLOAT2 texCoords;

				texCoords.x = Mesh->mTextureCoords[0][i].x;
				texCoords.y = (Mesh->mTextureCoords[0][i]).y;

				vertex.textureCoordinate = texCoords;
			

			}
			else
			{
				vertex.textureCoordinate = XMFLOAT2(0.f, 0.f);
			}

			vertex.color = XMFLOAT4(0.f, 0.f, 0.f, 0.f);

			vertices[i] = vertex;

		}

		for (unsigned int i = 0; i < Mesh->mNumFaces; i++)
		{
			aiFace face = Mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}


		mesh->Init(Device.Get(), CommandQueue.Get(), vertices.data(), vertices.size(),
			indices.data(), indices.size());

		meshes.push_back(mesh);
	}

	importer.FreeScene();
	return meshes;
}

void D3D12Renderer::Update()
{
	auto scene = GEngine.GetWorld();

	eyePosition = XMVectorSet(CameraX, CameraY, CameraZ, 1);
	focusPoint = XMVectorSet(0, 0, 1, 1) + eyePosition;
	upDirection = XMVectorSet(0, 1, 0, 0);



	const auto StateKeyboard = GEngine.GetKeyboard()->GetState();
	//const auto StateMouse = GEngine.GetMouse()->GetState();
	tracker.Update(StateKeyboard);



	auto delta = GEngine.GetDeltaTime();

	/*float mouseX = StateMouse.x * 0.05;
	float mouseY = StateMouse.y * 0.05;*/

	//yaw += mouseX;
	//pitch += mouseY;


	//yaw = 100;

	/*if (pitch > 89.f)
		pitch = 89.f;
	if (pitch < -89.f)
		pitch = -89.f;

	if (yaw > 360)
		yaw -= 360;
	if (yaw < 0)
		yaw += 360;*/

	Vector3 front;
	front.x = cos(XMConvertToRadians(yaw)) * cos(XMConvertToRadians(pitch));
	front.y = sin(XMConvertToRadians(pitch));
	front.z = sin(XMConvertToRadians(yaw)) * cos(XMConvertToRadians(pitch));




	Vector3 right;
	right.x = cos(XMConvertToRadians(yaw + 90)) * cos(XMConvertToRadians(0));
	right.y = sin(XMConvertToRadians(0));
	right.z = sin(XMConvertToRadians(yaw + 90)) * cos(XMConvertToRadians(0));

	Vector3 Up;
	Up.x = cos(XMConvertToRadians(0)) * cos(XMConvertToRadians(90));
	Up.y = sin(XMConvertToRadians(90));
	Up.z = sin(XMConvertToRadians(0)) * cos(XMConvertToRadians(90));



	right.Normalize();

	front.Normalize();



	float aspectRatio = 16 / 9;


	Transl = abs(0.005);




	if (StateKeyboard.W)
	{
		CameraZ += Transl * front.z * delta;
		CameraX += Transl * -front.x * delta;
		CameraY += Transl * -front.y * delta;
	}

	if (StateKeyboard.S)
	{
		//	ModelMatrix = XMMatrixMultiply(ModelMatrix, XMMatrixTranslation(0, -transl, 0));
			//CameraZ -= Transl;

		CameraZ += Transl * -front.z * delta;
		CameraX += Transl * +front.x * delta;
		CameraY += Transl * +front.y * delta;
	}

	if (StateKeyboard.D)
	{
		//ModelMatrix = XMMatrixMultiply(ModelMatrix, XMMatrixTranslation(-transl, 0, 0));

		//CameraX += Transl;

		CameraZ += Transl * right.z * delta;
		CameraX += Transl * -right.x * delta;
		CameraY += Transl * -right.y * delta;


	}

	if (StateKeyboard.A)
	{
		//ModelMatrix = XMMatrixMultiply(ModelMatrix, XMMatrixTranslation(transl, 0, 0));

		CameraZ += Transl * -right.z * delta;
		CameraX += Transl * right.x * delta;
		CameraY += Transl * right.y * delta;
	}

	if (StateKeyboard.Space)
	{
		//CameraY += Transl;
		CameraZ += Transl * +Up.z * delta;
		CameraX += Transl * -Up.x * delta;
		CameraY += Transl * Up.y * delta;
	}

	if (StateKeyboard.NumPad2)
	{
		//CameraY += Transl;
		//pitch += -sensevity;

		Scale.y += -sensevity * 0.01;


	}
	if (StateKeyboard.NumPad4)
	{

		//yaw += -sensevity;

		//Scale -= sensevity;

		Scale.x += -sensevity * 0.01;


	}
	if (StateKeyboard.NumPad6)
	{
		//CameraY += Transl;
		/*CameraZ += Transl * +Up.z;
		CameraX += Transl * -Up.x;
		CameraY += Transl * Up.y;*/

		//yaw += sensevity;

		Scale.x += sensevity * 0.01;
	}
	if (StateKeyboard.NumPad8)
	{
		//CameraY += Transl;
		/*CameraZ += Transl * +Up.z;
		CameraX += Transl * -Up.x;
		CameraY += Transl * Up.y;*/

		Scale.y += sensevity * 0.01;
	}


	if (StateKeyboard.Left)
	{
		x += -Transl;

	}

	if (StateKeyboard.Right)
	{
		x += Transl;
	}

	if (StateKeyboard.PageUp)
	{
		z += Transl;
	}
	if (StateKeyboard.PageDown)
	{
		z += -Transl;
	}
	if (StateKeyboard.Up)
	{
		y += Transl;

	}

	if (StateKeyboard.Down)
	{
		{
			y += -Transl;
		}
	}


	if (StateKeyboard.LeftShift)
	{
		CameraY -= Transl * delta;
	}

	if (StateKeyboard.Escape)
	{
		GEngine.Quit();

	}

	if (StateKeyboard.Home)
	{
		CameraX = 0;
		CameraY = 0;
		CameraZ = 0;
		pitch = 0;
		yaw = 90;
		roll = 0;
	}

	scene->GetCamera()->SetLocation({ CameraX, CameraY, CameraZ });
	scene->GetCamera()->SetRotation({ 0, pitch, yaw });

}

uint64_t D3D12Renderer::Signal(uint64_t &fenceValue)
{
	CommandQueue->Signal(Fence.Get(), ++fenceValue);
	return fenceValue;
}

void D3D12Renderer::WaitForFenceValue(uint64_t fenceValue)
{
	if (Fence->GetCompletedValue() < fenceValue)
	{
		Fence->SetEventOnCompletion(fenceValue, FenceEvent);
		WaitForSingleObject(FenceEvent, static_cast<DWORD>(std::chrono::milliseconds::max().count()));

	}
}

void D3D12Renderer::WaitFrame()
{
	CommandQueue->Signal(Fence.Get(), ++FenceValue);

	if (Fence->GetCompletedValue() < FenceValue)
	{
		Fence->SetEventOnCompletion(FenceValue, FenceEvent);
		WaitForSingleObject(FenceEvent, static_cast<DWORD>(std::chrono::milliseconds::max().count()));

	}


}

void D3D12Mesh::Init(const D3D12Renderer* renderer, D3D12MeshInitInfo info)
{
	m_device = renderer->Device.Get();
	Vertexes = info.Vertices;
	Indexes = info.Indices;


	SRDescriptorHeap = make_unique<DescriptorHeap>(m_device,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		16);



	m_device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE,
		IID_PPV_ARGS(&CommandList));

	m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator));




	

	ResourceUploadBatch ResourceUpload(m_device);



	const D3D12_INPUT_LAYOUT_DESC inputLayout = Vertex::InputLayout;

	D3D12_FEATURE_DATA_ROOT_SIGNATURE FeatureData = { D3D_ROOT_SIGNATURE_VERSION_1_0 };


	ComPtr<ID3DBlob> RootSignatureBlob;
	ComPtr<ID3DBlob> ErrorBlob;



	CD3DX12_ROOT_PARAMETER RootParametrs[7];

	CD3DX12_DESCRIPTOR_RANGE ranges[1];
	ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1,
		1, 0);


	//RootParametrs[0].InitAsConstants((sizeof(XMMATRIX) / 4) * 2, 0, 0, D3D12_SHADER_VISIBILITY_ALL);


	RootParametrs[0].InitAsConstants(sizeof(XMMATRIX) / 4, 0, 0, D3D12_SHADER_VISIBILITY_ALL);
	RootParametrs[1].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
	RootParametrs[2].InitAsConstants(sizeof(XMFLOAT4) / 4, 2, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	RootParametrs[3].InitAsConstants(sizeof(XMFLOAT4) / 4, 3, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	RootParametrs[4].InitAsConstants(sizeof(XMFLOAT4) / 4, 4, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	RootParametrs[5].InitAsConstants(sizeof(XMMATRIX) / 4, 5, 0, D3D12_SHADER_VISIBILITY_ALL);
	RootParametrs[6].InitAsConstants(1, 6, 0, D3D12_SHADER_VISIBILITY_ALL);

	//RootParametrs[2].InitAsShaderResourceView(4, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	D3D12_STATIC_SAMPLER_DESC sampler = {};


	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;







	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;


	CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
	RootSignatureDesc.Init(_countof(RootParametrs), RootParametrs, 1,
		&sampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


	D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &RootSignatureBlob, &ErrorBlob);


	m_device->CreateRootSignature(0, RootSignatureBlob->GetBufferPointer(),
		RootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&RootSignature));


	ComPtr<ID3DBlob> VertexShader;
	ComPtr<ID3DBlob> PixelShader;

	D3DCompileFromFile(info.VertexShader.c_str(), nullptr, nullptr, "main", "vs_5_0",
		0, 0, &VertexShader, nullptr);
	D3DCompileFromFile(info.PixelShader.c_str(), nullptr, nullptr, "main", "ps_5_0",
		0, 0, &PixelShader, nullptr);



	D3D12_GRAPHICS_PIPELINE_STATE_DESC PipelineStateDesc = {};

	PipelineStateDesc.InputLayout = (inputLayout);
	//PipelineStateDesc.InputLayout.pInputElementDescs = inputLayout;
	PipelineStateDesc.pRootSignature = RootSignature.Get();
	PipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	PipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(VertexShader->GetBufferPointer(), VertexShader->GetBufferSize());
	PipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(PixelShader->GetBufferPointer(), PixelShader->GetBufferSize());
	PipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	PipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	PipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	PipelineStateDesc.SampleMask = UINT_MAX;
	PipelineStateDesc.NumRenderTargets = 1;
	PipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	PipelineStateDesc.SampleDesc = { 1, 0 };
	//PipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;



	DXCall(m_device->CreateGraphicsPipelineState(&PipelineStateDesc, IID_PPV_ARGS(&PipelineState)));




	ResourceUpload.Begin();

	CreateStaticBuffer(m_device, ResourceUpload, Vertexes.data(), Vertexes.size(), sizeof(Vertex), 
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &VertexBuffer);

	VertexBufferView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
	VertexBufferView.SizeInBytes = sizeof(Vertex) * Vertexes.size();
	VertexBufferView.StrideInBytes = sizeof(Vertex);


	CreateStaticBuffer(m_device,
		ResourceUpload,
		Indexes.data(),
		Indexes.size(),
		sizeof(WORD),
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&IndexBuffer);

	IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
	IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
	IndexBufferView.SizeInBytes = sizeof(WORD) * Indexes.size();

	CreateDDSTextureFromFile(m_device, ResourceUpload,
		info.TexturePath.c_str(), &Texture);



	ResourceUpload.End(renderer->CommandQueue.Get());



	CreateShaderResourceView(m_device, Texture.Get(), SRDescriptorHeap->GetFirstCpuHandle());




}

void D3D12Mesh::Update()
{
	ModelMatrix = XMMatrixScalingFromVector(XMVectorSet(1, 1,1, 1));

	ModelMatrix = XMMatrixMultiply(ModelMatrix, XMMatrixTranslation(Position.x, Position.y,
		Position.z));






}

void D3D12Mesh::Clear()
{




}

D3D12Model::D3D12Model(D3D12Mesh1* mesh, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 sc)
{
	Position = pos;
	Rotation = rot;
	Scale = sc;

	Mesh = mesh;
	m_device = mesh->m_device;




	m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_Allocator));
	m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_Allocator.Get(),
		nullptr,IID_PPV_ARGS(&m_CommandList));

}

void D3D12Mesh1::Init(ID3D12Device* device, ID3D12CommandQueue* queue, void* vertexes, size_t vertSize, void* indexes, size_t indSize)
{
	Position = { 0.0, 0.f, 0.f };
	Rotation = { 0.f, 0.f, 0.f };
	Scale = { 1, 1, 1 };
	m_device = device;

	Vertexes.resize(vertSize);

	for (size_t i = 0; i < vertSize; i++)
	{
		Vertexes[i] = *(static_cast<Vertex*>(vertexes) + i);
	}


	Indexes.resize(indSize);

	for (size_t i = 0; i < indSize; i++)
	{
		Indexes[i] = *(static_cast<WORD*>(indexes) + i);
	}




	ResourceUploadBatch ResourceUpload(device);

	Vertexes.resize(vertSize);
	Indexes.resize(indSize);


	ResourceUpload.Begin();

	CreateStaticBuffer(device, ResourceUpload, vertexes, vertSize, sizeof(Vertex),
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &VertexBuffer);

	VertexBufferView.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
	VertexBufferView.SizeInBytes = sizeof(Vertex) * vertSize;
	VertexBufferView.StrideInBytes = sizeof(Vertex);


	CreateStaticBuffer(device,
		ResourceUpload,
		indexes,
		indSize,
		sizeof(WORD),
		D3D12_RESOURCE_STATE_INDEX_BUFFER,
		&IndexBuffer);

	IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
	IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
	IndexBufferView.SizeInBytes = sizeof(WORD) * indSize;



	ResourceUpload.End(queue);





}

void D3D12Model::Update()
{
	ModelMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&Scale));
	ModelMatrix = XMMatrixMultiply(ModelMatrix, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&Rotation)));
	ModelMatrix = XMMatrixMultiply(ModelMatrix, XMMatrixTranslationFromVector(XMLoadFloat3(&Position)));


}
