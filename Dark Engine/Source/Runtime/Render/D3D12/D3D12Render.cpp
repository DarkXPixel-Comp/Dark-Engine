#include "D3D12Render.h"
#include <DEngine.h>
#include "D3D12Utils.h"
#include "D3D12Scene.h"
#include "D3D12PSO.h"
#include <World.h>
#include <Timer/GameTimer.h>
#include <ResourceUploadBatch.h>
#include <BufferHelpers.h>


#include <d3d12.h>


#define SizeOfInUint32(obj) ((sizeof(obj) - 1) / sizeof(UINT32) + 1)


#undef max



void TransitionResource(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> commandList,
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
	
	m_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_depthStencilFormat = DXGI_FORMAT_D32_FLOAT; //DXGI_FORMAT_D16_UNORM
}

int32_t D3D12Renderer::Init()
{
	HRESULT Error = S_OK;

#ifdef _DEBUG
	{
		ComPtr<ID3D12Debug1> debug;
		D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
		debug->EnableDebugLayer();
		debug->SetEnableGPUBasedValidation(true);
	}
#endif // _DEBUG
	m_renderWindow = GEngine.GetWindowManager()->GetWindow(0);
	m_renderWindow->onResizeWindow.Bind(this, &D3D12Renderer::OnResize);

	DXCall(Error = CreateDXGIFactory( IID_PPV_ARGS(&m_factory)));
	DXCall(Error = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_device)));
	DXCall(Error = m_factory->EnumAdapterByLuid(m_device->GetAdapterLuid(), IID_PPV_ARGS(&m_adapter)));

	{
		D3D12_FEATURE_DATA_D3D12_OPTIONS5 RayTracingSupports = {};
		DXCall(m_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &RayTracingSupports,
			sizeof(RayTracingSupports)));
		if (RayTracingSupports.RaytracingTier == D3D12_RAYTRACING_TIER_NOT_SUPPORTED)
		{
			Log("PISDEC");
			return -1;
		}
	}




	D3D12_COMMAND_QUEUE_DESC CommandQueueDesc = {};

	CommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	CommandQueueDesc.NodeMask = 0;
	CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	DXCall(Error = m_device->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&m_commandQueue)));

	DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};
	DXGI_SWAP_CHAIN_DESC pDesc = {};

	pDesc.BufferDesc.Width = m_renderWindow->GetWitdh();
	pDesc.BufferDesc.Height = m_renderWindow->GetHeight();
	pDesc.BufferDesc.RefreshRate.Numerator = m_renderWindow->GetRefreshRate();
	pDesc.BufferDesc.RefreshRate.Denominator = 1;
	pDesc.BufferDesc.Format = m_backBufferFormat;
	pDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	pDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	pDesc.SampleDesc.Count = 1;
	pDesc.SampleDesc.Quality = 0;
	pDesc.BufferCount = BACK_BUFFER_COUNT;
	pDesc.OutputWindow = m_renderWindow->GetHandle();
	pDesc.Windowed = true;
	pDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	pDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	DXCall(Error = m_factory->CreateSwapChain(m_commandQueue.Get(), &pDesc, &m_swapchainT));
	m_swapchainT.As(&m_swapchain);


	RTDescriptorHeap = std::make_unique<DescriptorHeap>(m_device.Get(),
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		10);
	DSDescriptorHeap = make_unique<DescriptorHeap>(m_device.Get(),
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		1);
	SRDescriptorHeap = make_unique<DescriptorHeap>(m_device.Get(),
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_1);
	
	RTHandleSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	DSHandleSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	CBSRUAHandleSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (size_t i = 0; i < BACK_BUFFER_COUNT; i++)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = RTDescriptorHeap->GetCpuHandle(i);
		DXCall(Error = m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i])));
		m_device->CreateRenderTargetView(m_backBuffers[i].Get(), nullptr, cpuHandle);
		m_device->CreateShaderResourceView(m_backBuffers[i].Get(), nullptr, SRDescriptorHeap->GetCpuHandle(222 + i));
	}

	{
		D3D12_HEAP_PROPERTIES prop { CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT) };
		D3D12_RESOURCE_DESC desc = m_backBuffers[0]->GetDesc();
		//desc.Flags = D3D12_RESOURCE_FLAG_NONE;





		DXCall(m_device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_RENDER_TARGET, nullptr, IID_PPV_ARGS(&m_RenderTarget)));

		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = RTDescriptorHeap->GetCpuHandle(5);
		m_device->CreateRenderTargetView(m_RenderTarget.Get(), nullptr, cpuHandle);
		m_device->CreateShaderResourceView(m_RenderTarget.Get(), nullptr, SRDescriptorHeap->GetCpuHandle(247));
	}





	DXCall(Error = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_commandAllocator)));
	DXCall(m_commandAllocator->Reset());

	DXCall(m_device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE,
		IID_PPV_ARGS(&m_commandList)));
		
	DXCall(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
	m_fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);


	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
	depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
	depthOptimizedClearValue.DepthStencil.Stencil = 0;

	auto Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto TexScreen = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, m_renderWindow->GetWitdh(),
		m_renderWindow->GetHeight(),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	m_device->CreateCommittedResource(&Properties,
		D3D12_HEAP_FLAG_NONE,
		&TexScreen,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthOptimizedClearValue,
		IID_PPV_ARGS(&m_depthBuffer)
	);
	m_device->CreateDepthStencilView(m_depthBuffer.Get(), &depthStencilDesc,
		DSDescriptorHeap->GetFirstCpuHandle());

	ScissorRect = { 0, 0, static_cast<long>(m_renderWindow->GetWitdh()), 
		static_cast<long>(m_renderWindow->GetHeight()) };
	Viewport = { 0.f, 0.f,  static_cast<float>(m_renderWindow->GetWitdh()),
		static_cast<float>(m_renderWindow->GetHeight()), 0.f, 1.f };

	ScissorRect = { 0, 0, 1920, 1080 };
	Viewport = { 0, 0, 1920, 1080, 0.f, 1.f };


	RenderTargetState RTState(m_backBufferFormat, m_depthStencilFormat);

	PostProcess = std::make_unique<BasicPostProcess>(m_device.Get(), RTState, BasicPostProcess::BloomBlur);
	m_passBuffer = std::make_unique<D3D12UploadBufferResource<D3D12PassConstants>>(1, true);
	GraphMemory = std::make_unique<GraphicsMemory>(m_device.Get());
	D3DUtil::Init();
	D3DUtil::InitStaticSamples();
	D3DUtil::InitPipelines();


		
	GEngine.GetInput()->ActionDelegate.Bind(this, &D3D12Renderer::OnInput);
	m_renderWindow->onResizeWindow.Bind(this, &D3D12Renderer::OnResize);




	{
		struct Vert
		{
			XMFLOAT3 vertices;
			XMFLOAT2 uv;
		} vert;
		ResourceUploadBatch upload(m_device.Get());

		upload.Begin();
		
		TArray<Vert> vertices = { {{-1, -1, 0}, {0, 1}}, {{-1, 1, 0}, {0, 0}}, {{1, -1, 0}, {1, 1}}, {{1, 1, 0}, {1, 0}} };
		TArray<uint32> indices = { 0, 1, 2, 1, 3, 2 };
		TArray<Vert> verticesCube = {{{-1, -1, -1}, {} },
									{{-1,  1, -1}, {} },
									{{ 1,  1, -1}, {} }, 
									{{ 1, -1, -1}, {} },

									{{-1, -1,  1}, {} },
									{{-1,  1,  1}, {} },
									{{ 1,  1,  1}, {} },
									{{ 1, -1,  1}, {} } };
		TArray<uint32> IndicesCube{ 0, 1, 3, 1, 2, 3,
									4, 5, 7, 5, 6, 7,
									4, 5, 0, 5, 0, 1,
									7, 6, 3, 6, 3, 2,
									1, 5, 2, 2, 5, 6,
									0, 4, 3, 4, 7, 3};
					

		CreateStaticBuffer(m_device.Get(), upload, vertices.data(), vertices.size(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_backVertexBuffer);
		CreateStaticBuffer(m_device.Get(), upload, indices.data(), indices.size(), D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_backIndicesBuffer);
		
		CreateStaticBuffer(m_device.Get(), upload, verticesCube.data(), verticesCube.size(),
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_vertexSkyBoxBuffer);
		CreateStaticBuffer(m_device.Get(), upload, IndicesCube.data(), IndicesCube.size(),
			D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_indicesSkyBoxBuffer);



		upload.End(D3DUtil::GetCommandQueue()).wait();

		m_backVertexView.BufferLocation = m_backVertexBuffer->GetGPUVirtualAddress();
		m_backVertexView.SizeInBytes = sizeof(Vert) * vertices.size();
		m_backVertexView.StrideInBytes = sizeof(Vert);

		m_backIndexView.BufferLocation = m_backIndicesBuffer->GetGPUVirtualAddress();
		m_backIndexView.Format = DXGI_FORMAT_R32_UINT;
		m_backIndexView.SizeInBytes = sizeof(uint32) * indices.size();

		m_SkyBoxVertexView.BufferLocation = m_vertexSkyBoxBuffer->GetGPUVirtualAddress();
		m_SkyBoxVertexView.SizeInBytes = sizeof(Vert) * verticesCube.size();
		m_SkyBoxVertexView.StrideInBytes = sizeof(Vert);

		m_SkyBoxIndexView.BufferLocation = m_indicesSkyBoxBuffer->GetGPUVirtualAddress();
		m_SkyBoxIndexView.Format = DXGI_FORMAT_R32_UINT;
		m_SkyBoxIndexView.SizeInBytes = sizeof(uint32) * IndicesCube.size();

		{
			TArray<D3D12_ROOT_PARAMETER1> params(3);
			TArray<D3D12_INPUT_ELEMENT_DESC> elements(2);
			D3D12_INPUT_LAYOUT_DESC layout;

			elements[0] = { "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			elements[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			layout.NumElements = 2;
			layout.pInputElementDescs = elements.data();

			CD3DX12_DESCRIPTOR_RANGE1 range(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);
			CD3DX12_ROOT_PARAMETER1::InitAsDescriptorTable(params[2], 1, &range);
			CD3DX12_ROOT_PARAMETER1::InitAsConstantBufferView(params[0], 0, 0, 
				D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE, D3D12_SHADER_VISIBILITY_VERTEX);
			CD3DX12_ROOT_PARAMETER1::InitAsConstantBufferView(params[1], 1, 0,
				D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE, D3D12_SHADER_VISIBILITY_VERTEX);


			m_SkyBoxPSO = std::make_unique<D3D12PipelineShaderRootSignature>(m_device.Get(),
				FPaths::CombineDir(FPaths::EngineShaderDir(), "Skybox.hlsl"), 
				FPaths::CombineDir(FPaths::EngineShaderDir(), "SkyboxPixel.hlsl"),
				params, layout, true, D3D12_CULL_MODE_NONE, D3D12_COMPARISON_FUNC_LESS_EQUAL);

		}

		{

			TArray<D3D12_ROOT_PARAMETER1> params(1);
			TArray<D3D12_INPUT_ELEMENT_DESC> elements(2);
			D3D12_INPUT_LAYOUT_DESC layout;

			elements[0] = { "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			elements[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			layout.NumElements = 2;
			layout.pInputElementDescs = elements.data();


			CD3DX12_DESCRIPTOR_RANGE1 range(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
			CD3DX12_ROOT_PARAMETER1::InitAsDescriptorTable(params[0], 1, &range);
			m_PostProcessPSO = std::make_unique<D3D12PipelineShaderRootSignature>(m_device.Get(),
				FPaths::CombineDir(FPaths::EngineShaderDir(), "PostProcess.hlsl"),
				FPaths::CombineDir(FPaths::EngineShaderDir(), "IncreaseBright.hlsl"), params, layout, false);
			m_Blur = std::make_unique<D3D12PipelineShaderRootSignature>(m_device.Get(), 
				FPaths::CombineDir(FPaths::EngineShaderDir(), "PostProcess.hlsl"),
				FPaths::CombineDir(FPaths::EngineShaderDir(), "Blur.hlsl"),
				params, layout, false);
		}

	}

	m_skyboxTexture = D3DUtil::LoadTexture(FPaths::CombineDir(FPaths::EngineContentDir(), "Textures/cubemap.dds"), true);

	m_cbvObjectSkyBox = std::make_unique<D3D12UploadBufferResource<XMMATRIX>>(1, true);


	ImGui::CreateContext();

	ImGui_ImplWin32_Init(m_renderWindow->GetHandle());


	ImGui_ImplDX12_Init(m_device.Get(), BACK_BUFFER_COUNT, m_backBufferFormat, SRDescriptorHeap->Heap(), 
		SRDescriptorHeap->GetCpuHandle(778), SRDescriptorHeap->GetGpuHandle(778));


	GEngine.OnRenderInterface.Bind(this, &D3D12Renderer::OnInterface);





	if (IsRaster)
		return Error;



	// RayTracing

	{
		CD3DX12_DESCRIPTOR_RANGE UAVDescriptor;
		UAVDescriptor.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);

		CD3DX12_ROOT_PARAMETER rootParameters[2];
		rootParameters[0].InitAsDescriptorTable(1, &UAVDescriptor);
		rootParameters[1].InitAsShaderResourceView(0);
		CD3DX12_ROOT_SIGNATURE_DESC globalRootSignatureDesc(ARRAYSIZE(rootParameters), rootParameters);

		ID3DBlob* blob;
		ID3DBlob* error;

		D3D12SerializeRootSignature(&globalRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error);
		m_device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&m_RayTracingGlobalRootSignature));

	}

	{
		CD3DX12_DESCRIPTOR_RANGE UAVDescriptor;
		UAVDescriptor.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);

		CD3DX12_ROOT_PARAMETER rootParameters[1];
		rootParameters[0].InitAsConstants(SizeOfInUint32(m_RayGenCB), 0, 0);
		CD3DX12_ROOT_SIGNATURE_DESC localRootSignatureDesc(ARRAYSIZE(rootParameters), rootParameters);
		localRootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;

		ID3DBlob* blob;
		ID3DBlob* error;

		D3D12SerializeRootSignature(&localRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error);
		m_device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&m_RayTracingLocalRootSignature));

	}

	return Error;

}


void D3D12Renderer::OnInput(eInputActionState action)
{
	if (action == F3)
		IsRaster = false;
	if (action == F4)
	{
		IsRaster = true;
	}
}

void D3D12Renderer::Shutdown()
{
	D3DUtil::Shutdown();
}



static DirectX::XMFLOAT3 SphericalToCartesian(float radius, float theta, float phi)
{
	return XMFLOAT3(-(radius * sinf(phi) * cosf(theta)),
		-(radius * cosf(phi)),
		-(radius * sinf(phi) * sinf(theta)));
}




void D3D12Renderer::BeginFrame(D3D12Scene* scene)
{
	auto models = scene->GetModels();
	auto DefaultTex = D3DUtil::LoadTexture(D3D_DEFAULT_TEXTURE);
	auto SkyboxTex = D3DUtil::LoadTexture(D3D_DEFAULT_SKYBOX);

	if (!isDefaultTexLoaded)
	{
		m_device->CreateShaderResourceView(
			DefaultTex->m_textureBuffer,
			&DefaultTex->m_srvDesc,
			SRDescriptorHeap->GetCpuHandle(m_freeSRVDescriptor++));
		isDefaultTexLoaded = !isDefaultTexLoaded;
	}
	if (!isSkyboxTexLoaded)
	{
		m_device->CreateShaderResourceView(
			SkyboxTex->m_textureBuffer,
			&SkyboxTex->m_srvDesc,
			SRDescriptorHeap->GetCpuHandle(333));
		isSkyboxTexLoaded = !isSkyboxTexLoaded;
	}

	for (auto& model : models)
	{
		auto material = model->m_material;

		if (material->m_BaseDescriptorIndex == -1)
		{
			material->m_BaseDescriptorIndex = m_freeSRVDescriptor;
			m_freeSRVDescriptor += 2;
		}
		else
		{
			continue;
		}

		{
			if (material->t_Albedo)
			{
				m_device->CreateShaderResourceView(
					material->t_Albedo->m_textureBuffer,
					&material->t_Albedo->m_srvDesc,
					SRDescriptorHeap->GetCpuHandle(material->m_BaseDescriptorIndex + 0)
				);
			}
			else
			{
				m_device->CreateShaderResourceView(
					DefaultTex->m_textureBuffer,
					&DefaultTex->m_srvDesc,
					SRDescriptorHeap->GetCpuHandle(material->m_BaseDescriptorIndex + 0)
				);
			}
		}

		{
			if (material->t_Normal)
			{
				m_device->CreateShaderResourceView(
					material->t_Normal->m_textureBuffer,
					&material->t_Normal->m_srvDesc,
					SRDescriptorHeap->GetCpuHandle(material->m_BaseDescriptorIndex + 1)
				);
			}
			else
			{
				m_device->CreateShaderResourceView(
					DefaultTex->m_textureBuffer,
					&DefaultTex->m_srvDesc,
					SRDescriptorHeap->GetCpuHandle(material->m_BaseDescriptorIndex + 1)
				);
			}
		}


	}
}


void D3D12Renderer::OnInterface()
{
	/*bool p_open = false;

	ImGuiViewport* MainViewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowSize({500, 500});



	if (!ImGui::Begin("HALLO", &p_open, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar))
	{
		return;

	}


	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ImGui::MenuItem("Test", nullptr);

		}

	
	}

	ImGui::End();
	*/

}

void D3D12Renderer::SetResolution(int32 Width, int32 Height, bool IsFullScreen)
{
	m_swapchain->SetFullscreenState(IsFullScreen, nullptr);

	for (uint32 i = 0; i < BACK_BUFFER_COUNT; i++)
	{
		m_backBuffers[i].Reset();
	}

	m_RenderTarget.Reset();



	DXGI_SWAP_CHAIN_DESC Desc = {};

	m_swapchain->GetDesc(&Desc);

	m_swapchain->ResizeBuffers(BACK_BUFFER_COUNT, Width, Height, Desc.BufferDesc.Format, Desc.Flags);


	for (size_t i = 0; i < BACK_BUFFER_COUNT; i++)
	{
		HRESULT Error;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = RTDescriptorHeap->GetCpuHandle(i);
		DXCall(Error = m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i])));
		m_device->CreateRenderTargetView(m_backBuffers[i].Get(), nullptr, cpuHandle);
		m_device->CreateShaderResourceView(m_backBuffers[i].Get(), nullptr, SRDescriptorHeap->GetCpuHandle(222 + i));
	}


	{
		D3D12_HEAP_PROPERTIES prop{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT) };
		D3D12_RESOURCE_DESC desc = m_backBuffers[0]->GetDesc();
		//desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		desc.Width = 1920;
		desc.Height = 1080;




		DXCall(m_device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_RENDER_TARGET, nullptr, IID_PPV_ARGS(&m_RenderTarget)));

		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = RTDescriptorHeap->GetCpuHandle(5);
		m_device->CreateRenderTargetView(m_RenderTarget.Get(), nullptr, cpuHandle);
		m_device->CreateShaderResourceView(m_RenderTarget.Get(), nullptr, SRDescriptorHeap->GetCpuHandle(247));
	}

	CurrentBackBufferIndex = m_swapchain->GetCurrentBackBufferIndex();

}

void D3D12Renderer::Render(D3D12Scene* scene)
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	GEngine.OnRenderInterface.BroadCast();

	BeginFrame(scene);



	//ImGui::ShowDemoWindow();


	if (scene == nullptr) return;
	auto& backBuffer = m_backBuffers[CurrentBackBufferIndex];
	auto PSO = D3DUtil::GetPipeline(eShaderType::Default);
	auto models = scene->GetModels();
	auto& camera = scene->GetCamera();
	std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> RtObjects;
	auto window = GEngine.GetWindowManager()->GetWindow(0);
	CD3DX12_CPU_DESCRIPTOR_HANDLE backBufferHandle(RTDescriptorHeap->GetCpuHandle(CurrentBackBufferIndex));
	CD3DX12_CPU_DESCRIPTOR_HANDLE RenderTargetHandle(RTDescriptorHeap->GetCpuHandle(5));
	CD3DX12_CPU_DESCRIPTOR_HANDLE depthBufferHandle(DSDescriptorHeap->GetFirstCpuHandle());
	FLOAT clearColor[] = { 0.0f, 0.0f, 0.0f, 1.f };
	if (IsRaster)
	{
		ID3D12DescriptorHeap* ppHeaps[] = { SRDescriptorHeap->Heap() };
		m_commandAllocator->Reset();
		m_commandList->Reset(m_commandAllocator.Get(), PSO->m_pipelineState.Get());

		TransitionResource(m_commandList, backBuffer.Get(),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		TransitionResource(m_commandList, m_RenderTarget.Get(), D3D12_RESOURCE_STATE_COPY_SOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
		m_commandList->OMSetRenderTargets(1, &RenderTargetHandle, FALSE, &depthBufferHandle);
		m_commandList->ClearRenderTargetView(backBufferHandle, clearColor, 0, nullptr);
		m_commandList->ClearRenderTargetView(RenderTargetHandle, clearColor, 0, nullptr);
		m_commandList->ClearDepthStencilView(depthBufferHandle, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
		m_commandList->RSSetScissorRects(1, &ScissorRect);
		m_commandList->RSSetViewports(1, &Viewport);
		m_commandList->SetGraphicsRootSignature(PSO->m_rootSignature.Get());
		m_commandList->SetDescriptorHeaps(1, ppHeaps);
		m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		{
			D3D12PassConstants passConst;

			passConst.ViewProjectionMatrix = D3DUtil::GetViewProjMatrix(&camera);
			passConst.RenderTargetSize = XMFLOAT2(window->GetWitdh(), window->GetHeight());
			passConst.TotalTime = FGameTimer::TotalTime();
			passConst.DeltaTime = FGameTimer::DeltaTime();
			passConst.EyePos = camera.m_position;
			passConst.AmbientLight = { 0.25f, 0.25f, 0.25f, 1.0f };

			passConst.Lights[0].Strenght = { 1.f, 1.f, 1.f };
			passConst.Lights[0].Direction = { 1, -1, 0 };

			passConst.Lights[1].Strenght = { 1.0f, 1.f, 1.0f };
			passConst.Lights[1].Direction = { 1, -1, 0 };
			passConst.Lights[1].Position = { 2, 2, 3 };
			passConst.Lights[1].FalloffStart = 0;
			passConst.Lights[1].FalloffEnd = 200.f;

			m_passBuffer->CopyData(0, passConst);



			m_commandList->SetGraphicsRootConstantBufferView(3, m_passBuffer->GetResource()->GetGPUVirtualAddress());


		}

		for (auto& model : models)
		{
			auto mesh = model->m_mesh;
			auto material = model->m_material;
			if (!mesh) continue;
			model->FillConstantBuffer();
			m_commandList->SetGraphicsRootDescriptorTable(1,
				SRDescriptorHeap->GetGpuHandle(material->m_BaseDescriptorIndex));
			//......


			m_commandList->IASetVertexBuffers(0, 1, &mesh->m_vertexBufferView);
			m_commandList->IASetIndexBuffer(&mesh->m_indexBufferView);
			m_commandList->SetGraphicsRootConstantBufferView(0, model->m_cbvObject.GetResource()->GetGPUVirtualAddress());
			m_commandList->SetGraphicsRootConstantBufferView(2, model->m_cbvMaterial.GetResource()->GetGPUVirtualAddress());
			m_commandList->DrawIndexedInstanced(mesh->m_indexBufferView.SizeInBytes / sizeof(WORD), 1, 0, 0, 0);
		}


		{	//Skybox

			m_commandList->SetPipelineState(m_SkyBoxPSO->m_pipelineState.Get());
			m_commandList->SetGraphicsRootSignature(m_SkyBoxPSO->m_rootSignature.Get());
			//m_commandList->SetGraphicsRootConstantBufferView(1, m_passBuffer->GetResource()->GetGPUVirtualAddress());
			{
				XMMATRIX ModelMatrix = XMMatrixIdentity();
				XMMATRIX Translation = XMMatrixTranslationFromVector(XMLoadFloat3(&camera.m_position));
				XMMATRIX Rotation = XMMatrixIdentity();
				//Rotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&camera.m_rotation));
				XMMATRIX Scaling = XMMatrixIdentity();


				ModelMatrix = XMMatrixMultiply(Scaling, Rotation);
				ModelMatrix = XMMatrixMultiply(ModelMatrix, Translation);

				m_cbvObjectSkyBox->CopyData(0, ModelMatrix);
			}
			m_commandList->SetGraphicsRootConstantBufferView(1,
			m_cbvObjectSkyBox->GetResource()->GetGPUVirtualAddress());
			m_commandList->SetGraphicsRootConstantBufferView(0, m_passBuffer->GetResource()->GetGPUVirtualAddress());
			m_commandList->SetGraphicsRootDescriptorTable(2, SRDescriptorHeap->GetGpuHandle(333));



			m_commandList->IASetVertexBuffers(0, 1, &m_SkyBoxVertexView);
			m_commandList->IASetIndexBuffer(&m_SkyBoxIndexView);

			  
			m_commandList->DrawIndexedInstanced(36, 1, 0, 0, 0);



		}


		{
			TransitionResource(m_commandList, m_RenderTarget.Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE);

			m_commandList->SetPipelineState(m_PostProcessPSO->m_pipelineState.Get());
			m_commandList->SetGraphicsRootSignature(m_PostProcessPSO->m_rootSignature.Get());
			//m_commandList->ClearDepthStencilView(depthBufferHandle, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
			m_commandList->OMSetRenderTargets(1, &RenderTargetHandle, FALSE, nullptr);

			m_commandList->SetGraphicsRootDescriptorTable(0, SRDescriptorHeap->GetGpuHandle(247));

			m_commandList->IASetVertexBuffers(0, 1, &m_backVertexView);
			m_commandList->IASetIndexBuffer(&m_backIndexView);
			//m_commandList->DrawInstanced(3, 1, 0, 0);
			//m_commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);


			TransitionResource(m_commandList, m_RenderTarget.Get(),
				D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_COPY_SOURCE);
			TransitionResource(m_commandList, backBuffer.Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_DEST);



			D3D12_TEXTURE_COPY_LOCATION textureDst = {};
			D3D12_TEXTURE_COPY_LOCATION textureSrc = {};
			textureDst.pResource = backBuffer.Get();
			textureDst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			//textureDst.PlacedFootprint.Footprint.
			textureDst.SubresourceIndex = 0;
			textureSrc = textureDst;
			textureSrc.pResource = m_RenderTarget.Get();


			//m_commandList->CopyTextureRegion(&textureDst, 0, 0, 0, &textureSrc, nullptr);
			//m_commandList->CopyResource(tempBuffer.Get(), backBuffer.Get());



			m_RenderTarget->GetDesc().Width;
			m_RenderTarget->GetDesc().Height;



			ImGui::Begin("Window");

			ImVec2 WndSize = ImGui::GetWindowSize();
			ImVec2 WndPos = ImGui::GetWindowPos();

			Viewport = { 0, 0, WndSize.x, WndSize.y, 0, 1 };

			ScissorRect = { (long)0, (long)0, (long)WndSize.x, (long)WndSize.y};

			ImGui::Image((void*)SRDescriptorHeap->GetGpuHandle(247).ptr,
				{ (float)m_RenderTarget->GetDesc().Width, (float)m_RenderTarget->GetDesc().Height });

	/*		ImGui::Image((void*)SRDescriptorHeap->GetGpuHandle(247).ptr,
				{1920, 1080 });*/



			ImGui::End();


			

			TransitionResource(m_commandList, backBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_RENDER_TARGET);

			m_commandList->OMSetRenderTargets(1, &backBufferHandle, false, nullptr);

		}

		TransitionResource(m_commandList, backBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_commandList.Get());

		m_commandList->Close();
		ID3D12CommandList* lists[] = { m_commandList.Get() };
		m_commandQueue->ExecuteCommandLists(1, lists);
	}
	else
	{
		for (auto& model : models)
		{
			D3D12_RAYTRACING_GEOMETRY_DESC GeometryDesc = {};
			D3D12_GPU_VIRTUAL_ADDRESS_AND_STRIDE VertexAddress;
			VertexAddress.StartAddress = model->m_mesh->m_vertexBufferView.BufferLocation;
			VertexAddress.StrideInBytes = model->m_mesh->m_vertexBufferView.StrideInBytes;


			GeometryDesc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
			GeometryDesc.Triangles.VertexBuffer = VertexAddress;
			GeometryDesc.Triangles.VertexCount = model->m_mesh->m_vertexBufferView.SizeInBytes / VertexAddress.StrideInBytes;
			GeometryDesc.Triangles.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			GeometryDesc.Triangles.IndexBuffer = model->m_mesh->m_indexBufferView.BufferLocation;
			GeometryDesc.Triangles.IndexFormat = DXGI_FORMAT_R16_UINT;
			GeometryDesc.Triangles.IndexCount = model->m_mesh->m_indexBufferView.SizeInBytes / sizeof(WORD);

			RtObjects.push_back(GeometryDesc);
		}

		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS bottomLevelInputs;
		bottomLevelInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
		bottomLevelInputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_NONE;
		bottomLevelInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
		bottomLevelInputs.pGeometryDescs = RtObjects.data();
		bottomLevelInputs.NumDescs = RtObjects.size();

		FAccelerationBuffers TempRTBuffers;
		D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO RTInfo;

		m_device->GetRaytracingAccelerationStructurePrebuildInfo(&bottomLevelInputs, &RTInfo);



		D3D12_HEAP_PROPERTIES prop{ CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT) };
		D3D12_RESOURCE_DESC desc{ CD3DX12_RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_BUFFER, 0,
			RTInfo.ScratchDataSizeInBytes, 1, 1, 1, DXGI_FORMAT_UNKNOWN, 1, 0,
			D3D12_TEXTURE_LAYOUT_ROW_MAJOR, D3D12_RESOURCE_FLAG_RAYTRACING_ACCELERATION_STRUCTURE | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS) };
		//desc.Flags = D3D12_RESOURCE_FLAG_NONE;



		DXCall(m_device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&TempRTBuffers.ScratchBuffer)));

		desc.Width = RTInfo.ResultDataMaxSizeInBytes;
		desc.Flags = D3D12_RESOURCE_FLAG_RAYTRACING_ACCELERATION_STRUCTURE | D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
		
		DXCall(m_device->CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE, nullptr, IID_PPV_ARGS(&TempRTBuffers.ResultBuffer)));


		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC bottomLevelBuildDesc = {};
		bottomLevelBuildDesc.Inputs = bottomLevelInputs;
		bottomLevelBuildDesc.ScratchAccelerationStructureData = TempRTBuffers.ScratchBuffer->GetGPUVirtualAddress();
		bottomLevelBuildDesc.DestAccelerationStructureData = TempRTBuffers.ResultBuffer->GetGPUVirtualAddress();

		m_commandList->BuildRaytracingAccelerationStructure(&bottomLevelBuildDesc, 0, nullptr);







	}
	WaitFrame();


	m_swapchain->Present(bVsync, 0);

	CurrentBackBufferIndex = !CurrentBackBufferIndex;

	//CurrentBackBufferIndex = m_swapchain->GetCurrentBackBufferIndex();

	m_freeSRVDescriptor = 0;
	return;
}

void D3D12Renderer::RenderObj()
{
}


void D3D12Renderer::PostProcessing(D3D12_GPU_DESCRIPTOR_HANDLE renderHandle, ID3D12Resource* outputHandle)
{







}


void D3D12Renderer::RenderScene()
{
	


}



void D3D12Renderer::EndFrame()
{
}

void D3D12Renderer::OnResize(long x, long y)
{
	
}


void D3D12Renderer::Update()
{
	

}

uint64 D3D12Renderer::Signal(uint64 &fenceValue)
{
	m_commandQueue->Signal(m_fence.Get(), ++fenceValue);
	return fenceValue;
}

void D3D12Renderer::WaitForFenceValue(uint64 fenceValue)
{
	if (m_fence->GetCompletedValue() < fenceValue)
	{
		m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, static_cast<DWORD>(std::chrono::milliseconds::max().count()));

	}
}

void D3D12Renderer::WaitFrame()
{
	m_commandQueue->Signal(m_fence.Get(), ++m_fenceValue);

	if (m_fence->GetCompletedValue() < m_fenceValue)
	{
		m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, static_cast<DWORD>(std::chrono::milliseconds::max().count()));

	}


}

