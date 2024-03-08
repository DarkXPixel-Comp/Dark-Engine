#pragma once
#include "D3D12RHICommon.h"
#include "D3D12Descriptors.h"
#include "D3D12Device.h"



class FD3D12View : public FD3D12DeviceChild
{
public:
	FD3D12View(FD3D12Device* Parent, ERHIDescriptorHeapType InHeapType) :
		FD3D12DeviceChild(Parent),
		HeapType(InHeapType),
		CpuHandle(Parent->GetCpuDescriptorManger(InHeapType).AllocateHeapSlot())
	{}
	FD3D12CpuDescriptor GetCpuHandle() const { return CpuHandle; }

protected:
	FD3D12CpuDescriptor CpuHandle;
	ERHIDescriptorHeapType HeapType;

};

class FD3D12RenderTargetView : public FD3D12View
{
public:
	FD3D12RenderTargetView(FD3D12Device* Parent) : FD3D12View(Parent, ERHIDescriptorHeapType::RenderTarget)
	{}

	void CreteView(const D3D12_RENDER_TARGET_VIEW_DESC& InDesc, class FD3D12Texture* InResource);

private:
	D3D12_RENDER_TARGET_VIEW_DESC RTVDesc;
	class FD3D12Texture* Resource;



};