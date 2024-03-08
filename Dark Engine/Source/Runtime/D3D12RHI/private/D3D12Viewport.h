#pragma once

#include "CoreMinimal.h"
#include "D3D12RHICommon.h"
#include "D3D12Resources.h"
#include "RHIResources.h"
#include "D3D12Texture.h"

class FD3D12Viewport : public FRHIViewport , public FD3D12AdapterChild
{
public:

	FD3D12Viewport(FD3D12Adapter* InParent, HWND InWindowHandle, uint32 InSizeX, uint32 InSizeY,
		bool bInIsFullscreen, EPixelFormat InPixelFormat);
	void Init();

	void Resize(uint32 InSizeX, uint32 InSizeY, bool bInIsFullscreen, EPixelFormat InPixelFormat);

	bool Present(bool bVsync);

	uint32 GetCountBackBuffers() const { return NumBackBuffers; }

	void WaitForFrameEventCompletion() override;


	FORCEINLINE static DXGI_FORMAT GetRenderTargetFormat(EPixelFormat PixelFormat)
	{
		switch (PixelFormat)
		{
		case PF_Unknown:
			break;
		case PF_A32B32G32R32F:
			break;
		case PF_R32_FLOAT:
			break;
		case PF_R8G8B8A8:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		default:
			break;
		}
	}

	FD3D12Texture* GetCurrentBackBuffer() const { return BackBuffers[CurrentBackBufferIndex].get(); }

private:
	void CalculateSwapchainNum(int32 SwapchainNum);
	DXGI_MODE_DESC SetupDXGI_MODE_DESC() const;
	FD3D12Texture* GetSwapChainSurface( EPixelFormat PixelFormat, uint32 SizeX, uint32 SizeY,
		IDXGISwapChain* SwapChain, uint32 BackBufferIndex, TRefCountPtr<ID3D12Resource> BackBufferResourceOverride);




private:
	HWND WindowHandle;
	uint32 SizeX;
	uint32 SizeY;
	bool bIsFullscreen;
	EPixelFormat PixelFormat;
	bool bIsValid;
	uint32 CurrentBackBufferIndex;
	
	TArray<TSharedPtr<FD3D12Texture>> BackBuffers;
	uint32 NumBackBuffers;

	TRefCountPtr<IDXGISwapChain1> SwapChain1;
	TRefCountPtr<IDXGISwapChain2> SwapChain2;
	TRefCountPtr<IDXGISwapChain3> SwapChain3;
	TRefCountPtr<IDXGISwapChain4> SwapChain4;




};