#pragma once
#include "D3D12.h"
#include "../Renderer.h"
#include <VertexTypes.h>
#include <Core/CommandLine/CommandLine.h>
#include <vector>
#include <string>
#include <DescriptorHeap.h>
#include <memory>
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
	void Init() override;
	void Shutdown() override;

	void BeginFrame() override;
	void Render(D3D12Scene*) override;
	void RenderObj();
	void RenderScene();
	void EndFrame() override;
	std::vector<D3D12Mesh1*> LoadMesh(std::string path = "");

	void Update();

private:
	uint64_t Signal(uint64_t& fenceValue);
	void WaitForFenceValue(uint64_t fenceValue);
	void WaitFrame();




	float rot = 0.0f;

public:
	friend class D3D12Mesh;

	ComPtr<IDXGIFactory7> m_factory;
	ComPtr<ID3D12Device8> m_device;
	ComPtr<IDXGIAdapter1> m_adapter;

	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	ComPtr<ID3D12GraphicsCommandList6> m_commandList;
	ComPtr<ID3D12CommandQueue> m_commandQueue;

	unique_ptr <DescriptorHeap> RTDescriptorHeap;
	unique_ptr <DescriptorHeap> DSDescriptorHeap;
	unique_ptr<DescriptorHeap> SRDescriptorHeap;

	ComPtr<IDXGISwapChain4> m_swapChain;

	Resource m_backBuffers[BACK_BUFFER_COUNT];
	Resource m_depthBuffer;

	DXGI_FORMAT m_backBufferFormat;
	DXGI_FORMAT m_depthStencilFormat;


	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	uint64_t m_fenceValue;

	uint64_t CurrentBackBufferIndex = 0;
	SIZE_T RTHandleSize = 0;

	D3D12_VIEWPORT Viewport;
	D3D12_RECT ScissorRect;
	float Fov;

	
	unique_ptr<D3D12UploadBufferResource<D3D12PassConstants>> m_passBuffer;
};