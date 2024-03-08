#include "D3D12Descriptors.h"
#include "D3D12Device.h"
#include "D3D12Util.h"

FD3D12DescriptorHeap::FD3D12DescriptorHeap(FD3D12Device* InDevice, ID3D12DescriptorHeap* InHeap, 
	uint32 InNumDescriptors, ERHIDescriptorHeapType InType, D3D12_DESCRIPTOR_HEAP_FLAGS InFlags):
	FD3D12DeviceChild(InDevice),
	Heap(InHeap),
	CpuBase(Heap->GetCPUDescriptorHandleForHeapStart()),
	GpuBase(InFlags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE ? Heap->GetGPUDescriptorHandleForHeapStart() : D3D12_GPU_DESCRIPTOR_HANDLE()),
	NumDescriptors(InNumDescriptors),
	DescriptorSize(Parent->GetDevice()->GetDescriptorHandleIncrementSize(GetD3D12DescriptorHeapType(InType))),
	Type(InType),
	Flags(InFlags)
{
}

FD3D12DescriptorHeap* CreateDescriptorHeap(FD3D12Device* Device, const TCHAR* DebugName, ERHIDescriptorHeapType HeapType, uint32 NumDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS Flags)
{
	D3D12_DESCRIPTOR_HEAP_DESC Desc = {};
	Desc.Type = GetD3D12DescriptorHeapType(HeapType);
	Desc.NumDescriptors = NumDescriptors;
	Desc.Flags = Flags;

	ID3D12DescriptorHeap* Heap;
	DXCall(Device->GetDevice()->CreateDescriptorHeap(&Desc, IID_PPV_ARGS(&Heap)));
	SetName(Heap, DebugName);

	return new FD3D12DescriptorHeap(Device, Heap, NumDescriptors, HeapType, Flags);
}

void FD3D12DescriptorHeapManager::Init(uint32 InNumGlobalResourceDescriptors, uint32 InNumGlobalSamplerDescriptors)
{
	if (InNumGlobalResourceDescriptors > 0)
	{
		FD3D12DescriptorHeap* Heap =
			CreateDescriptorHeap(Parent,
				TEXT("GlobalResourceHeap"),
				ERHIDescriptorHeapType::Standart,
				InNumGlobalResourceDescriptors,
				D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
			);
		GlobalHeaps.Emplace(Parent, Heap);
	}

	if (InNumGlobalSamplerDescriptors > 0)
	{
		FD3D12DescriptorHeap* Heap =
			CreateDescriptorHeap(Parent,
				TEXT("GlobalSamplerHeap"),
				ERHIDescriptorHeapType::Sampler,
				InNumGlobalSamplerDescriptors,
				D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
			);
		GlobalHeaps.Emplace(Parent, Heap);
	}
}

FD3D12DescriptorHeap* FD3D12DescriptorHeapManager::AllocateHeap(const TCHAR* InDebugName, ERHIDescriptorHeapType InHeapType, uint32 InNumDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS InHeapFlags)
{
	return CreateDescriptorHeap(Parent, InDebugName, InHeapType, InNumDescriptors, InHeapFlags);
}


FD3D12CpuDescriptorManager::FD3D12CpuDescriptorManager(FD3D12Device* InDevice, ERHIDescriptorHeapType InHeapType):
	FD3D12DeviceChild(InDevice),
	HeapType(InHeapType)
{
	DescriptorSize = Parent->GetDevice()->GetDescriptorHandleIncrementSize(GetD3D12DescriptorHeapType(InHeapType));
	NumDescriptorsPerHeap = GetCpuDescriptorHeapSize(InHeapType);


}

FD3D12CpuDescriptor FD3D12CpuDescriptorManager::AllocateHeapSlot()
{
	FD3D12CpuDescriptor Result;

	if (FreeHeapIndex.Num() == 0)
	{
		AllocateHeap();
	}

	auto &IndexFreeHeap = FreeHeapIndex.Last();
	Result.HeapIndex = IndexFreeHeap;

	FD3D12CpuEntry& HeapEntry = Heaps[Result.HeapIndex];
	FD3D12CpuFreeRange& Range = HeapEntry.FreeList.Last();

	Result.ptr = Range.Start;
	Range.Start += DescriptorSize;

	if (Range.Start == Range.End)
	{
		HeapEntry.FreeList.RemovePtr(HeapEntry.FreeList.Last());
		if (HeapEntry.FreeList.Num() == 0)
		{
			FreeHeapIndex.Remove(IndexFreeHeap);
		}
	}
	return Result;
}

void FD3D12CpuDescriptorManager::AllocateHeap()
{
	FD3D12DescriptorHeap* Heap = Parent->GetDescriptorHeapManager().AllocateHeap
	(
		TEXT("FD3D12CpuDescriptorManager"),
		HeapType,
		NumDescriptorsPerHeap,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE
	);

	D3D12_CPU_DESCRIPTOR_HANDLE HeapBase = Heap->GetCPUSlotHandle(0);

	check(HeapBase.ptr != 0);

	const uint32 HeapIndex = Heaps.Num();
	Heaps.Emplace(Heap, HeapBase, NumDescriptorsPerHeap * DescriptorSize);
	FreeHeapIndex.Add(HeapIndex);
}

FD3D12DescriptorManager::FD3D12DescriptorManager(FD3D12Device* Device, FD3D12DescriptorHeap* InHeap):
	FD3D12DeviceChild(Device),
	Heap(InHeap)
{

}
