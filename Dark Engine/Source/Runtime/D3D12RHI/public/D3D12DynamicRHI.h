#pragma once
#include "DynamicRHI.h"


struct ID3D12DynamicRHI : public FDynamicRHI
{
	virtual class ID3D12CommandQueue* RHIGetCommandQueue() = 0;
	virtual class ID3D12Device* RHIGetDevice() = 0;
	virtual class ID3D12GraphicsCommandList* RHIGetGraphicsCommandList(uint32 InDeviceIndex) = 0;
	//virtual DXGI_FORMAT RHIGetSwapChainFormat() const = 0;

	



};




using ID3D12PlatformDynamicRHI = ID3D12DynamicRHI;