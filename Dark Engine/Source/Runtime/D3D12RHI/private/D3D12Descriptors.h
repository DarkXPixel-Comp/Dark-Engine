#pragma once

#include "D3D12RHI.h"
#include "D3D12RHICommon.h"
#include <d3d12.h>



struct FD3D12DescriptorHeap : public FD3D12DeviceChild
{
public:
	FD3D12DescriptorHeap(FD3D12Device* InDevice, ID3D12DescriptorHeap* InHeap);




private:
	TRefCountPtr<ID3D12DescriptorHeap> Heap;

	const uint32 NumDescriptors;

	const uint32 DescriptorSize;

	const ERHIDescriptorHeapType Type;

	const bool bIsGlobal;


};






struct FD3D12Descriptor	: public D3D12_CPU_DESCRIPTOR_HANDLE
{
	operator bool() const { return ptr != 0; }
	FD3D12Descriptor() { ptr = 0; }




private:
	uint32 HeapIndex;

	friend class FD3D12DescriptorManager;
};



class FD3D12DescriptorManager : public FD3D12DeviceChild
{
public:
	FD3D12DescriptorManager(FD3D12Device* Device, FD3D12DescriptorHeap* InHeap);



private:
	TSharedPtr<FD3D12DescriptorHeap> Heap;
};








class FD3D12DescriptorHeapManager : public FD3D12DeviceChild
{
public:
	FD3D12DescriptorHeapManager(FD3D12Device* InDevice);

	

	void Init(uint32 InNumGlobalResourceDescriptors, uint32 InNumGlobalSamplerDescriptors);
	void Destroy();

	FD3D12DescriptorHeap* AllocateHeap(const TCHAR* InDebugName, ERHIDescriptorHeapType InHeapType, uint32 InNumDescriptors,
		D3D12_DESCRIPTOR_HEAP_FLAGS InHeapFlags);



private:
	TArray<FD3D12DescriptorHeapManager>	GlobalHeaps;






};