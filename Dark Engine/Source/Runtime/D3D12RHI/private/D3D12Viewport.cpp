#pragma once
#include "D3D12Viewport.h"
#include "D3D12Adapter.h"
#include "D3D12Device.h"
#include "D3D12Util.h"
#include "D3D12CommandContext.h"


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


	const FD3D12ResourceDesc BackBufferDesc = BackBufferResource->GetDesc();

	FString Name = "BackBuffer" + std::to_string(BackBufferIndex);

	FRHITextureCreateDesc CreateDesc = FRHITextureCreateDesc::Create2D(*Name);

	CreateDesc
		.SetExtent(FIntPoint((uint32)BackBufferDesc.Width, (uint32)BackBufferDesc.Height))
		.SetFormat(PixelFormat);

	FD3D12DynamicRHI* DynamicRHI = FD3D12DynamicRHI::GetD3D12RHI();
	
	FD3D12Texture* SwapChainTexture = DynamicRHI->CreateD3D12Texture(CreateDesc, Adapter->GetDevice());

	const D3D12_RESOURCE_STATES InitialState = D3D12_RESOURCE_STATE_COMMON;

	FD3D12Resource* NewResource = new FD3D12Resource(Parent->GetDevice(), BackBufferResource.Get(), InitialState, BackBufferDesc);
	NewResource->SetIsBackBuffer(true);
	SwapChainTexture->SetResource(NewResource);


	D3D12_RENDER_TARGET_VIEW_DESC RTVDesc{};
	RTVDesc.Format = BackBufferDesc.Format;
	RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	RTVDesc.Texture2D.MipSlice = 0;

	SwapChainTexture->AddRTV(RTVDesc, 0);

	DX12::SetName(SwapChainTexture->GetResource(), *Name);

	SwapChainTexture->GetResource()->SetIsBackBuffer(true);


	return SwapChainTexture;

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


	Resize(BufferDesc.Width, BufferDesc.Height, bIsFullscreen, PixelFormat);




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
		BackBuffers[i] = MakeShareble(GetSwapChainSurface(PixelFormat, SizeX, SizeY, SwapChain1.Get(),
			i, nullptr));
	}
	CurrentBackBufferIndex = 0;

	
}

bool FD3D12Viewport::Present(bool bVsync)
{
	FD3D12Texture* BackBuffer = GetCurrentBackBuffer();
	FD3D12CommandContext& DefaultContext = Parent->GetDevice()->GetDefaultCommandContext();

	DefaultContext.TransitionResource(BackBuffer->GetResource(),
		BackBuffer->GetResource()->GetCurrentState(), D3D12_RESOURCE_STATE_PRESENT, 0);


	DefaultContext.FlushCommands();

	UINT Flags = 0;
	if (!bVsync && !bIsFullscreen)
	{
		Flags |= DXGI_PRESENT_ALLOW_TEARING;
	}

	if (SwapChain4)
	{
		SwapChain4->Present(bVsync, Flags);
		CurrentBackBufferIndex = SwapChain4->GetCurrentBackBufferIndex();
	}



	return true;
}

void FD3D12Viewport::WaitForFrameEventCompletion()
{
	
}
