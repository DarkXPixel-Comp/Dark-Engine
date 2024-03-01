#pragma once
#include "D3D12Viewport.h"
#include "D3D12Adapter.h"
#include "D3D12Device.h"
#include "D3D12Util.h"


static const uint32 WindowsDefaultNumBackBuffers = 3;


FD3D12Viewport::FD3D12Viewport(FD3D12Adapter* InParent, HWND InWindowHandle, uint32 InSizeX, uint32 InSizeY,
	bool bInIsFullscreen, EPixelFormat InPixelFormat):
	FD3D12AdapterChild(InParent),
	WindowHandle(InWindowHandle),
	SizeX(InSizeX),
	SizeY(InSizeY),
	bIsFullscreen(bInIsFullscreen),
	PixelFormat(InPixelFormat)
{
	GetParentAdapter()->GetViewports().Add(this);


}



void FD3D12Viewport::CalculateSwapchainNum(int32 SwapChainNum)
{
	FD3D12Adapter* Adapter = GetParentAdapter();

	NumBackBuffers = SwapChainNum;

	BackBuffers.Empty();
	for (size_t i = 0; i < SwapChainNum; i++)
	{
		BackBuffers.Add(nullptr);
	}

}


void FD3D12Viewport::Init()
{
	FD3D12Adapter* Adapter = GetParentAdapter();
	IDXGIFactory7* Factory = Adapter->GetDXGIFactory();


	CalculateSwapchainNum(WindowsDefaultNumBackBuffers);

	UINT SwapChainFlags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	DXGI_MODE_DESC BufferDesc;

	BufferDesc.Width = SizeX;
	BufferDesc.Height = SizeY;
	BufferDesc.RefreshRate.Numerator = 0;
	BufferDesc.RefreshRate.Denominator = 0;
	BufferDesc.Format = GetRenderTargetFormat(PixelFormat);
	BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	ID3D12CommandQueue* CommandQueue = Adapter->GetDevice()->GetQueue(ED3D12QueueType::Direct).CommandQueue.Get();

	
	if (SwapChain1 == nullptr)
	{
		DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
		SwapChainDesc.BufferDesc = BufferDesc;
		SwapChainDesc.SampleDesc = { 1, 0 };
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
		SwapChainDesc.BufferCount = NumBackBuffers;
		SwapChainDesc.OutputWindow = WindowHandle;
		SwapChainDesc.Windowed = !bIsFullscreen;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.Flags = SwapChainFlags;


		TRefCountPtr<IDXGISwapChain> SwapChain;

		DXCall(Factory->CreateSwapChain(CommandQueue, &SwapChainDesc, &SwapChain));
		DXCall(SwapChain->QueryInterface(IID_PPV_ARGS(&SwapChain1)));
	}


	if (SwapChain1)
	{
		SwapChain1->QueryInterface(IID_PPV_ARGS(&SwapChain2));
		SwapChain1->QueryInterface(IID_PPV_ARGS(&SwapChain3));
		SwapChain1->QueryInterface(IID_PPV_ARGS(&SwapChain4));
	}


	Factory->MakeWindowAssociation(WindowHandle, DXGI_MWA_NO_WINDOW_CHANGES);





}

void FD3D12Viewport::Resize(uint32 InSizeX, uint32 InSizeY, bool bInIsFullscreen, EPixelFormat InPixelFormat)
{
	FD3D12Adapter* Adapter = GetParentAdapter();



}
