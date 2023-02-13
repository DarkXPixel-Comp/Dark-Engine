#include "D3D12Render.h"
#include <Engine/public/DEngine.h>
#include "D3D12Utils.h"
#include "D3D12Scene.h"
#include "D3D12PSO.h"
#include <World/World.h>

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

void D3D12Renderer::Init()
{
	{
		ComPtr<ID3D12Debug> debug;
		D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
		debug->EnableDebugLayer();
	}


	HRESULT Error = S_OK;

	DXCall(Error = CreateDXGIFactory2(0, IID_PPV_ARGS(&m_factory)));
	DXCall(Error = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device)));

	DXCall(m_factory->EnumAdapterByLuid(m_device->GetAdapterLuid(), IID_PPV_ARGS(&m_adapter)));

	D3D12_COMMAND_QUEUE_DESC CommandQueueDesc = {};

	CommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	CommandQueueDesc.NodeMask = 0;
	CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;


	DXCall(Error = m_device->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&m_commandQueue)));

	DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};

	auto window = GEngine.GetWindowManager()->GetWindow(0);


	{
		auto wnd = GEngine.GetWindowManager()->GetWindow(0);
		ComPtr<IDXGISwapChain> swapchain;

		DXGI_SWAP_CHAIN_DESC pDesc = {};
		pDesc.BufferDesc.Width = wnd->GetWitdh();
		pDesc.BufferDesc.Height = wnd->GetHeight();
		pDesc.BufferDesc.RefreshRate.Numerator = wnd->GetRefreshRate();
		pDesc.BufferDesc.RefreshRate.Denominator = 1;
		pDesc.BufferDesc.Format = m_backBufferFormat;
		pDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		pDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		pDesc.SampleDesc.Count = 1;
		pDesc.SampleDesc.Quality = 0;
		pDesc.BufferCount = BACK_BUFFER_COUNT;
		pDesc.OutputWindow = wnd->GetHandle();
		pDesc.Windowed = true;
		pDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		pDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		DXCall(Error = m_factory->CreateSwapChain(m_commandQueue.Get(), &pDesc, &swapchain));


		swapchain.As(&m_swapChain);


	}


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

	for (size_t i = 0; i < BACK_BUFFER_COUNT; i++)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = RTDescriptorHeap->GetCpuHandle(i);
		m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i]));

		m_device->CreateRenderTargetView(m_backBuffers[i].Get(), nullptr, cpuHandle);
	}




	
	

	(Error = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));

	m_commandAllocator->Reset();

	m_device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE,
		IID_PPV_ARGS(&m_commandList));
		

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
	auto TexScreen = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, 1920, 1080,
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

	ScissorRect = { 0, 0, (long)window->GetWitdh(), (long)window->GetHeight() };

	Viewport = { 0.f, 0.f,  (float)window->GetWitdh(),  (float)window->GetHeight(), 0.f, 1.f };


	m_passBuffer = std::make_unique<D3D12UploadBufferResource<D3D12PassConstants>>(1, true);


	D3DUtil::InitPipelines();
	
	

}

void D3D12Renderer::Shutdown()
{
}







void D3D12Renderer::BeginFrame()
{

}

void D3D12Renderer::Render(D3D12Scene* scene)
{
	if (scene == nullptr) return;
	auto& backBuffer = m_backBuffers[CurrentBackBufferIndex];
	auto PSO = D3DUtil::GetPipeline(eShaderType::Default);
	auto models = scene->GetModels();
	auto camera = scene->GetCamera();
	CD3DX12_CPU_DESCRIPTOR_HANDLE backBufferHandle(RTDescriptorHeap->GetCpuHandle(CurrentBackBufferIndex));
	CD3DX12_CPU_DESCRIPTOR_HANDLE depthBufferHandle(DSDescriptorHeap->GetFirstCpuHandle());
	FLOAT clearColor[] = { 0.4f, 0.6f, 0.9f, 1.f };
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

		m_passBuffer->CopyData(0, passConst);

		D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
		desc.BufferLocation = m_passBuffer->GetResource()->GetGPUVirtualAddress();
		desc.SizeInBytes = m_passBuffer->GetElementByteSize();
		m_device->CreateConstantBufferView(&desc, SRDescriptorHeap->GetFirstCpuHandle());

		m_commandList->SetGraphicsRootDescriptorTable(1, SRDescriptorHeap->GetFirstGpuHandle());
	}
	
	for (auto& model : models)
	{	
		auto mesh = model->m_mesh;
		model->FillConstantBuffer();
		m_commandList->IASetVertexBuffers(0, 1, &mesh->m_vertexBufferView);
		m_commandList->IASetIndexBuffer(&mesh->m_indexBufferView);
		m_commandList->SetGraphicsRootConstantBufferView(0, model->m_cbvObject.GetResource()->GetGPUVirtualAddress());
		m_commandList->DrawIndexedInstanced(mesh->m_indexBufferView.SizeInBytes / sizeof(WORD), 1, 0, 0, 0);
	}

	TransitionResource(m_commandList, backBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	m_commandList->Close();
	ID3D12CommandList* lists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(1, lists);
	WaitFrame();

	m_swapChain->Present(0, 0);


	CurrentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
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

