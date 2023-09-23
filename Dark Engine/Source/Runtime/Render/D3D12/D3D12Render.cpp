#include "D3D12Render.h"
#include <Engine/public/DEngine.h>
#include "D3D12Utils.h"
#include "D3D12Scene.h"
#include "D3D12PSO.h"
#include <World/World.h>
#include <Core/Timer/GameTimer.h>

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
		ComPtr<ID3D12Debug> debug;
		D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
		debug->EnableDebugLayer();
	}
#endif // _DEBUG
	m_renderWindow = GEngine.GetWindowManager()->GetWindow(0);
	m_renderWindow->onResizeWindow.Bind(this, &D3D12Renderer::OnResize);

	DXCall(Error = CreateDXGIFactory2(0, IID_PPV_ARGS(&m_factory)));
	DXCall(Error = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device)));
	DXCall(Error = m_factory->EnumAdapterByLuid(m_device->GetAdapterLuid(), IID_PPV_ARGS(&m_adapter)));

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
		BACK_BUFFER_COUNT);
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


	m_passBuffer = std::make_unique<D3D12UploadBufferResource<D3D12PassConstants>>(1, true);
	D3DUtil::InitStaticSamples();
	D3DUtil::InitPipelines();
		

	return Error;

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
		m_device->CreateShaderResourceView(
			DefaultTex->m_textureBuffer,
			&DefaultTex->m_srvDesc,
			SRDescriptorHeap->GetCpuHandle(m_freeSRVDescriptor++));

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

void D3D12Renderer::Render(D3D12Scene* scene)
{
	BeginFrame(scene);

	if (scene == nullptr) return;
	auto& backBuffer = m_backBuffers[CurrentBackBufferIndex];
	auto PSO = D3DUtil::GetPipeline(eShaderType::Default);
	auto models = scene->GetModels();
	auto& camera = scene->GetCamera();
	auto window = GEngine.GetWindowManager()->GetWindow(0);
	CD3DX12_CPU_DESCRIPTOR_HANDLE backBufferHandle(RTDescriptorHeap->GetCpuHandle(CurrentBackBufferIndex));
	CD3DX12_CPU_DESCRIPTOR_HANDLE depthBufferHandle(DSDescriptorHeap->GetFirstCpuHandle());
	FLOAT clearColor[] = { 0.0f, 0.0f, 0.0f, 1.f };
	ID3D12DescriptorHeap* ppHeaps[] = { SRDescriptorHeap->Heap() };
	m_commandAllocator->Reset();
	m_commandList->Reset(m_commandAllocator.Get(), PSO->m_pipelineState.Get());

	TransitionResource(m_commandList, backBuffer.Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	m_commandList->OMSetRenderTargets(1, &backBufferHandle, FALSE, &depthBufferHandle);
	m_commandList->ClearRenderTargetView(backBufferHandle, clearColor, 0, nullptr);
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


		m_commandList->IASetVertexBuffers(0, 1, &mesh->m_vertexBufferView);
		m_commandList->IASetIndexBuffer(&mesh->m_indexBufferView);
		m_commandList->SetGraphicsRootConstantBufferView(0, model->m_cbvObject.GetResource()->GetGPUVirtualAddress());
		m_commandList->SetGraphicsRootConstantBufferView(2, model->m_cbvMaterial.GetResource()->GetGPUVirtualAddress());
		m_commandList->DrawIndexedInstanced(mesh->m_indexBufferView.SizeInBytes / sizeof(WORD), 1, 0, 0, 0);
	}

	TransitionResource(m_commandList, backBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	m_commandList->Close();
	ID3D12CommandList* lists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(1, lists);
	WaitFrame();

	m_swapchain->Present(bVsync, 0);


	CurrentBackBufferIndex = m_swapchain->GetCurrentBackBufferIndex();

	m_freeSRVDescriptor = 0;
	return;
}

void D3D12Renderer::RenderObj()
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

uint64_t D3D12Renderer::Signal(uint64_t &fenceValue)
{
	m_commandQueue->Signal(m_fence.Get(), ++fenceValue);
	return fenceValue;
}

void D3D12Renderer::WaitForFenceValue(uint64_t fenceValue)
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

