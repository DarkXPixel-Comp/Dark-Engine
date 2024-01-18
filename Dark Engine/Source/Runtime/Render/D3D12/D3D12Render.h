#pragma once
#include "D3D12Main.h"
#include "../Renderer.h"
#include <VertexTypes.h>
#include "VertexTypes.h"
#include <Containers/Array.h>
#include <DescriptorHeap.h>
#include <memory>
#include <Containers/DarkString.h>
#include <CommandLine/CommandLine.h>
#include <CoreTypes.h>
#include <dxcapi.h>
#include <d3d12shader.h>
#include <Windows/WindowsWindow.h>
#include "D3D12Types.h"
#include "D3D12Resource.h"
#include <InputCore.h>
#include <PostProcess.h>
#include <GraphicsMemory.h>
#include "D3D12PSO.h"
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include "Delegate/Delegate.h"





using namespace std;
using namespace DirectX;



struct Viewport
{
	float left;
	float top;
	float right;
	float bottom;
};

struct RayGenConstantBuffer
{
	Viewport viewport;
	Viewport stencil;
};


struct VertexPosColor
{
	VertexPosColor(XMFLOAT3 iposition, XMFLOAT3 icolor) : Position(iposition), Color(icolor) {}

	XMFLOAT3 Position;
	XMFLOAT3 Color;
};

//typedef VertexPositionNormalColorTexture Vertex;



//VertexPositionNormalColorTexture

class FD3D12Mesh;
class D3D12Mesh1;
class D3D12Model;
class D3D12Material;
class D3D12Scene;





class D3D12Renderer : public Renderer
{
private:
	static const uint32 BACK_BUFFER_COUNT = 2;
	void Test() { PrintLine("Test"); }

public:
	D3D12Renderer();

public:
	int32_t Init() override;
	void Shutdown() override;

	void BeginFrame(D3D12Scene* scene);
	void Render(D3D12Scene*) override;
	void SetResolution(int32 Width, int32 Height, bool IsFullScreen) override;
	void RenderObj();
	void RenderScene();
	void EndFrame() override;
	void OnResize(long x, long y);
	void OnInput(eInputActionState action);
	void OnInterface();

	void PostProcessing(D3D12_GPU_DESCRIPTOR_HANDLE renderHandle, ID3D12Resource* outputHandle);
	TArray<D3D12Mesh1*> LoadMesh(FString path = "");

	void Update();

private:
	uint64 Signal(uint64& fenceValue);
	void WaitForFenceValue(uint64 fenceValue);
	void WaitFrame();




	float rot = 0.0f;
	uint64 m_freeSRVDescriptor = 0;

public:
	friend class FD3D12Mesh;

	bool IsRaster = true;
	bool isDefaultTexLoaded = false;
	bool isSkyboxTexLoaded = false;
	int msaa = 1;

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




	TUniquePtr<BasicPostProcess> PostProcess;
	TUniquePtr<GraphicsMemory> GraphMemory;



	FWindowsWindow* m_renderWindow;

	Resource m_backBuffers[BACK_BUFFER_COUNT];
	Resource m_depthBuffer;
	Resource m_backVertexBuffer;
	Resource m_backIndicesBuffer;

	Resource m_vertexSkyBoxBuffer;
	Resource m_indicesSkyBoxBuffer;

	D3D12Texture* m_skyboxTexture;



	D3D12_VERTEX_BUFFER_VIEW m_backVertexView;
	D3D12_INDEX_BUFFER_VIEW m_backIndexView;

	D3D12_VERTEX_BUFFER_VIEW m_SkyBoxVertexView;
	D3D12_INDEX_BUFFER_VIEW m_SkyBoxIndexView;

	TUniquePtr <D3D12UploadBufferResource<XMMATRIX>> m_cbvObjectSkyBox;


	TUniquePtr<D3D12PipelineShaderRootSignature> m_PostProcessPSO;
	TUniquePtr<D3D12PipelineShaderRootSignature> m_Blur;
	TUniquePtr<D3D12PipelineShaderRootSignature> m_SkyBoxPSO;


	DXGI_FORMAT m_backBufferFormat;
	DXGI_FORMAT m_depthStencilFormat;


	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	uint64 m_fenceValue;

	uint64 CurrentBackBufferIndex = 0;
	SIZE_T RTHandleSize =		0;
	SIZE_T DSHandleSize =		0;
	SIZE_T CBSRUAHandleSize =	0;

	D3D12_VIEWPORT Viewport;
	D3D12_RECT ScissorRect;
	float Fov;
	TUniquePtr<D3D12UploadBufferResource<D3D12PassConstants>> m_passBuffer;



	// RayTracing


	ComPtr<ID3D12RootSignature> m_RayTracingGlobalRootSignature;
	ComPtr<ID3D12RootSignature> m_RayTracingLocalRootSignature;
	RayGenConstantBuffer m_RayGenCB;


	struct FAccelerationBuffers
	{
		Resource ScratchBuffer;
		Resource ResultBuffer;
		Resource InstanceDescBuffer;
	};

	
};