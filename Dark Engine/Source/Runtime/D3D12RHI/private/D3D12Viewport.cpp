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

DXGI_MODE_DESC FD3D12Viewport::SetupDXGI_MODE_DESC() const
{
	DXGI_MODE_DESC Ret;

	Ret.Width = SizeX;
	Ret.Height = SizeY;
	Ret.RefreshRate.Numerator = 0;	// illamas: use 0 to avoid a potential mismatch with hw
	Ret.RefreshRate.Denominator = 0;	// illamas: ditto
	Ret.Format = GetRenderTargetFormat(PixelFormat);
	Ret.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	Ret.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	return Ret;
}

FD3D12Texture* FD3D12Viewport::GetSwapChainSurface(EPixelFormat PixelFormat, uint32 SizeX, uint32 SizeY, IDXGISwapChain* SwapChain, uint32 BackBufferIndex, TRefCountPtr<ID3D12Resource> BackBufferResourceOverride)
{
	FD3D12Adapter* Adapter = GetParentAdapter();

	TRefCountPtr<ID3D12Resource> BackBufferResource;
	if (SwapChain)
	{	
		DXCall(SwapChain->GetBuffer(BackBufferIndex, IID_PPV_ARGS(&BackBufferResource)));
	}
	else if (BackBufferResourceOverride.Get() != nullptr)
	{
		BackBufferResource = BackBufferResourceOverride;
	}
	else
	{
		const D3D12_HEAP_PROPERTIES HeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		D3D12_RESOURCE_DESC TextureDesc = {};
		TextureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		TextureDesc.Alignment = 0;
		TextureDesc.Width = SizeX;
		TextureDesc.Height = SizeY;
		TextureDesc.DepthOrArraySize = 1;
		TextureDesc.MipLevels = 1;
		TextureDesc.Format = GetRenderTargetFormat(PixelFormat);
		TextureDesc.SampleDesc = { 1, 0 };
		TextureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		TextureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

		Adapter->GetDevice()->GetDevice()->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &TextureDesc,
			D3D12_RESOURCE_STATE_PRESENT, nullptr, IID_PPV_ARGS(&BackBufferResource));
	}

	D3D12_RESOURCE_DESC BackBufferDesc = BackBufferResource->GetDesc();

	FString Name = "BackBuffer" + std::to_string(BackBufferIndex);







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



	for (uint32 i = 0; i < NumBackBuffers; ++i)
	{
		BackBuffers[i].reset();
	}


	if (SizeX != InSizeX || SizeY != InSizeY || PixelFormat != InPixelFormat)
	{
		SizeX = InSizeX;
		SizeY = InSizeY;

		PixelFormat = InPixelFormat;


		if (bInIsFullscreen)
		{
			const DXGI_MODE_DESC Desc = SetupDXGI_MODE_DESC();
			if (FAILED(SwapChain1->ResizeTarget(&Desc)))
			{

			}
		}
	}

	if (bIsFullscreen != bInIsFullscreen)
	{
		bIsFullscreen = bInIsFullscreen;
		SwapChain1->SetFullscreenState(bIsFullscreen, nullptr);
	}


	UINT SwapChainFlags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	SwapChain1->ResizeBuffers(NumBackBuffers, SizeX, SizeY, GetRenderTargetFormat(InPixelFormat), SwapChainFlags);

	FD3D12Device* Device = Adapter->GetDevice();
	for (uint32 i = 0; i < NumBackBuffers; i++)
	{
		check(BackBuffers[i] == nullptr);
		BackBuffers[i] = GetSwap

	}

	
}
