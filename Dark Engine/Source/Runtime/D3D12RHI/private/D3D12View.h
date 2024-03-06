#pragma once
#include "D3D12RHICommon.h"



class FD3D12View : public FD3D12DeviceChild
{
public:
	FD3D12View(FD3D12Device* Parent) : FD3D12DeviceChild(Parent)
	{}

};

class FD3D12RenderTargetView : public FD3D12View
{
public:
	FD3D12RenderTargetView(FD3D12Device* Parent) : FD3D12View(Parent)
	{}

	void CreteView(const D3D12_RENDER_TARGET_VIEW_DESC& InDesc, class FD3D12Texture* InResource);

private:
	D3D12_RENDER_TARGET_VIEW_DESC RTVDesc;
	D3D12_CPU_DESCRIPTOR_HANDLE	CpuHandle;
	class FD3D12Texture* Resource;



};