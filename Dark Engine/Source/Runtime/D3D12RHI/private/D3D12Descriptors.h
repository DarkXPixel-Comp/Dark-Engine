#pragma once

#include "D3D12RHI.h"
#include "D3D12RHICommon.h"
#include <d3d12.h>
#include "RHIDefines.h"


class FD3D12DescriptorHeap;



FORCEINLINE D3D12_DESCRIPTOR_HEAP_TYPE GetD3D12DescriptorHeapType(ERHIDescriptorHeapType HeapType)
{
	switch (HeapType)
	{
	case Standart:
		return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	case Sampler:
		return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	case RenderTarget:
		return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	case DepthStencil:
		return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	default:
		return D3D12_DESCRIPTOR_HEAP_TYPE();
	}
}


FORCEINLINE uint32 GetCpuDescriptorHeapSize(ERHIDescriptorHeapType HeapType)
{
	switch (HeapType)
	{
	case Standart:
		return 2048;
	case Sampler:
		return 128;
	case RenderTarget:
		return 256;
	case DepthStencil:
		return 256;
	}

}



struct FD3D12CpuDescriptor : public D3D12_CPU_DESCRIPTOR_HANDLE
{
public:
	operator bool()	const { return ptr != 0; }
	FD3D12CpuDescriptor() { ptr = 0; }

	void Increment() { ++Version; }
	uint32 GetVersion() const { return Version; }


private:
	friend class FD3D12CpuDescriptorManager;
	uint32 HeapIndex = 0;
	uint32 Version;
};



struct FD3D12CpuFreeRange
{
	uint64 Start;
	uint64 End;
};

struct FD3D12CpuEntry
{
	FD3D12CpuEntry(FD3D12DescriptorHeap* InHeap, const D3D12_CPU_DESCRIPTOR_HANDLE& InHeapBase, uint64 InSize) :
		Heap(InHeap)
	{
		FreeList.Add({ InHeapBase.ptr, InHeapBase.ptr + InSize });
	}

	TSharedPtr<FD3D12DescriptorHeap> Heap;
	TArray<FD3D12CpuFreeRange> FreeList;
};


class FD3D12DescriptorHeap : public FD3D12DeviceChild
{
public:
	FD3D12DescriptorHeap(FD3D12Device* InDevice, ID3D12DescriptorHeap* InHeap, uint32 InNumDescriptors,
		ERHIDescriptorHeapType InType, D3D12_DESCRIPTOR_HEAP_FLAGS InFlags);

	~FD3D12DescriptorHeap() {}

	ID3D12DescriptorHeap* GetHeap() const { return Heap.Get(); }
	ERHIDescriptorHeapType GetType() const { return Type; }
	D3D12_DESCRIPTOR_HEAP_FLAGS GetFlags() const { return Flags; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSlotHandle(uint32 Slot) { return CD3DX12_CPU_DESCRIPTOR_HANDLE(CpuBase, Slot, DescriptorSize); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSlotHandle(uint32 Slot) { return CD3DX12_GPU_DESCRIPTOR_HANDLE(GpuBase, Slot, DescriptorSize); }
								
private:
	TRefCountPtr<ID3D12DescriptorHeap> Heap;
	const CD3DX12_CPU_DESCRIPTOR_HANDLE CpuBase;
	const CD3DX12_GPU_DESCRIPTOR_HANDLE GpuBase;
	const uint32 NumDescriptors;
	const uint32 DescriptorSize;
	const ERHIDescriptorHeapType Type; 
	const D3D12_DESCRIPTOR_HEAP_FLAGS Flags;



};


class FD3D12DescriptorManager : public FD3D12DeviceChild
{
public:
	FD3D12DescriptorManager(FD3D12Device* Device, FD3D12DescriptorHeap* InHeap);
	~FD3D12DescriptorManager() {}

	FD3D12DescriptorHeap* GetHeap() { return Heap.get(); }

	bool HandlesAllocation(ERHIDescriptorHeapType InType, D3D12_DESCRIPTOR_HEAP_FLAGS InFlags) const
	{
		return Heap->GetType() == InType && Heap->GetFlags() == InFlags;
	}

private:
	TSharedPtr<FD3D12DescriptorHeap> Heap;

};




class FD3D12DescriptorHeapManager : public FD3D12DeviceChild
{
public:
	FD3D12DescriptorHeapManager(FD3D12Device* InDevice) :
		FD3D12DeviceChild(InDevice)
	{}
	~FD3D12DescriptorHeapManager() {}

	void Init(uint32 InNumGlobalResourceDescriptors, uint32 InNumGlobalSamplerDescriptors);
	void Destroy() {}

	FD3D12DescriptorHeap* AllocateIndependentHeap(const TCHAR* InDebugName, ERHIDescriptorHeapType InHeapType, uint32 InNumDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS InHeapFlags);
	FD3D12DescriptorHeap* AllocateHeap(const TCHAR* InDebugName, ERHIDescriptorHeapType InHeapType, uint32 InNumDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS InHeapFlags);
	void DeferredFreeHeap(FD3D12DescriptorHeap* InHeap);
	void ImmediateFreeHeap(FD3D12DescriptorHeap* InHeap);

	//inline FD3D12DescriptorHeap* GetGlobalHeap() const { return GlobalHeap; }

private:
	TArray<FD3D12DescriptorManager> GlobalHeaps;
};



class FD3D12BindlessDescriptorManager : public FD3D12DeviceChild
{
public:
	FD3D12BindlessDescriptorManager(FD3D12Device* InDevice);
	~FD3D12BindlessDescriptorManager();

	void Init();


};


class FD3D12CpuDescriptorManager : public FD3D12DeviceChild
{
public:
	FD3D12CpuDescriptorManager(FD3D12Device* InDevice, ERHIDescriptorHeapType InHeapType);
	~FD3D12CpuDescriptorManager() {}


	FD3D12CpuDescriptor AllocateHeapSlot();
	void FreeHeapSlot(FD3D12CpuDescriptor& Descriptor);

private:
	void AllocateHeap();
	const ERHIDescriptorHeapType HeapType;
	uint32 NumDescriptorsPerHeap;
	uint32 DescriptorSize;

	TArray<FD3D12CpuEntry> Heaps;
	TArray<uint32> FreeHeapIndex;


};






FD3D12DescriptorHeap* CreateDescriptorHeap(FD3D12Device* Device, const TCHAR* DebugName, ERHIDescriptorHeapType HeapType,
	uint32 NumDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS Flags);