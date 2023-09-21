#pragma once
#include "D3D12.h"
#include "../Renderer.h"
#include <VertexTypes.h>
#include <Core/Containers/Array/Array.h>
#include <DescriptorHeap.h>
#include <memory>
#include <Containers/String/DarkString.h>
#include <Core/CommandLine/CommandLine.h>
#include <ApplicationCore/public/Windows/WindowsWindow.h>
#include "D3D12Types.h"
#include "D3D12Resource.h"






using namespace std;
using namespace DirectX;




struct VertexPosColor
{
	VertexPosColor(XMFLOAT3 iposition, XMFLOAT3 icolor) : Position(iposition), Color(icolor) {}

	XMFLOAT3 Position;
	XMFLOAT3 Color;
};

//typedef VertexPositionNormalColorTexture Vertex;



//VertexPositionNormalColorTexture

class D3D12Mesh;
class D3D12Mesh1;
class D3D12Model;
class D3D12Material;
class D3D12Scene;



class D3D12Renderer : public Renderer
{
private:
	static const uint32_t BACK_BUFFER_COUNT = 2;
	void Test() { PrintLine("Test"); }

public:
	D3D12Renderer();

public:
	int32_t Init() override;
	void Shutdown() override;

	void BeginFrame(D3D12Scene* scene);
	void Render(D3D12Scene*) override;
	void RenderObj();
	void RenderScene();
	void EndFrame() override;
	void OnResize(long x, long y);
	TArray<D3D12Mesh1*> LoadMesh(FString path = "");

	void Update();

private:
	uint64_t Signal(uint64_t& fenceValue);
	void WaitForFenceValue(uint64_t fenceValue);
	void WaitFrame();




	float rot = 0.0f;
	uint64_t m_freeSRVDescriptor = 0;

public:
	friend class D3D12Mesh;

	ComPtr<IDXGIFactory7> m_factory;
	ComPtr<ID3D12Device8> m_device;
	ComPtr<IDXGIAdapter1> m_adapter;

	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	ComPtr<ID3D12GraphicsCommandList6> m_commandList;
	ComPtr<ID3D12CommandQueue> m_commandQueue;

	TUniquePtr <DescriptorHeap> RTDescriptorHeap;
	TUniquePtr <DescriptorHeap> DSDescriptorHeap;
	TUniquePtr <DescriptorHeap> SRDescriptorHeap;

	ComPtr<IDXGISwapChain4> m_swapchain;
	ComPtr<IDXGISwapChain> m_swapchainT;

	FWindowsWindow* m_renderWindow;

	Resource m_backBuffers[BACK_BUFFER_COUNT];
	Resource m_depthBuffer;

	DXGI_FORMAT m_backBufferFormat;
	DXGI_FORMAT m_depthStencilFormat;


	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	uint64_t m_fenceValue;

	uint64_t CurrentBackBufferIndex = 0;
	SIZE_T RTHandleSize =		0;
	SIZE_T DSHandleSize =		0;
	SIZE_T CBSRUAHandleSize =	0;

	D3D12_VIEWPORT Viewport;
	D3D12_RECT ScissorRect;
	float Fov;

	
	TUniquePtr<D3D12UploadBufferResource<D3D12PassConstants>> m_passBuffer;
};