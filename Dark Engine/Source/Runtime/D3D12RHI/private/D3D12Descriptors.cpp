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

	TRefCountPtr<ID3D12DescriptorHeap> Heap;
	DXCall(Device->GetDevice()->CreateDescriptorHeap(&Desc, IID_PPV_ARGS(&Heap)));
	SetName(Heap.Get(), DebugName);

	return new FD3D12DescriptorHeap(Device, Heap.Get(), NumDescriptors, HeapType, Flags);
}

void CopyDescriptors(FD3D12Device* Device, FD3D12DescriptorHeap* TargetHeap, FD3D12DescriptorHeap* SourceHeap, uint32 NumDescriptors)
{
	const D3D12_CPU_DESCRIPTOR_HANDLE TargetStart = TargetHeap->GetCPUSlotHandle(0);
	const D3D12_CPU_DESCRIPTOR_HANDLE SourceStart = SourceHeap->GetCPUSlotHandle(0);
	const D3D12_DESCRIPTOR_HEAP_TYPE D3DHeapType = GetD3D12DescriptorHeapType(TargetHeap->GetType());

	Device->GetDevice()->CopyDescriptorsSimple(NumDescriptors, TargetStart,
		SourceStart, D3DHeapType);
}

void CopyDescriptor(FD3D12Device* Device, FD3D12DescriptorHeap* TargetHeap, FRHIDescriptorHandle Handle,
	D3D12_CPU_DESCRIPTOR_HANDLE SourceCpuHandle)
{
	if (Handle.IsValid())
	{
		const D3D12_CPU_DESCRIPTOR_HANDLE DestCpuHandle = TargetHeap->GetCPUSlotHandle(Handle.GetIndex());
		const D3D12_DESCRIPTOR_HEAP_TYPE D3DHeapType = GetD3D12DescriptorHeapType(Handle.GetHeapType());

		Device->GetDevice()->CopyDescriptorsSimple(1, DestCpuHandle,
			SourceCpuHandle, D3DHeapType);
	}
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

void FD3D12DescriptorHeapManager::ImmediateFreeHeap(FD3D12DescriptorHeap* InHeap)
{
	if (InHeap)
	{
		InHeap->AddRef();
		InHeap->Release();
	}
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

	auto &IndexFreeHeap = FreeHeapIndex.First();
	Result.HeapIndex = IndexFreeHeap;

	FD3D12CpuEntry& HeapEntry = Heaps[Result.HeapIndex];
	FD3D12CpuFreeRange& Range = HeapEntry.FreeList.First();

	Result.ptr = Range.Start;
	Range.Start += DescriptorSize;

	if (Range.Start == Range.End)
	{
		HeapEntry.FreeList.Remove(HeapEntry.FreeList.First());
		if (HeapEntry.FreeList.Num() == 0)
		{
			FreeHeapIndex.Remove(IndexFreeHeap);
		}
	}


	//DE_LOG(D3D12RHI, Log, TEXT("Alloc slot (%i)"), Result.ptr);

	return Result;
}

void FD3D12CpuDescriptorManager::FreeHeapSlot(FD3D12CpuDescriptor& Descriptor)
{
	FD3D12CpuEntry& HeapEntry = Heaps[Descriptor.HeapIndex];

	const FD3D12CpuFreeRange NewRange{ Descriptor.ptr, Descriptor.ptr + DescriptorSize };

	bool bFound = false;
	for (auto& i : HeapEntry.FreeList)
	{
		FD3D12CpuFreeRange& Range = i;
		if (Range.Start == Descriptor.ptr + DescriptorSize)
		{
			Range.Start = Descriptor.ptr;

			bFound = true;
		}
		else if (Range.End == Descriptor.ptr)
		{
			Range.End += DescriptorSize;
			bFound = true;
		}
		else
		{
			if (Range.Start > Descriptor.ptr)
			{
				HeapEntry.FreeList.Add(NewRange);
				bFound = true;
			}
		}

		if (bFound)
		{
			break;
		}
	}
	if (!bFound)
	{
		if (HeapEntry.FreeList.Num() == 0)
		{
			FreeHeapIndex.Add(Descriptor.HeapIndex);
		}
		HeapEntry.FreeList.Add(NewRange);
	}

	//DE_LOG(D3D12RHI, Log, TEXT("Free slot (%i)"), Descriptor.ptr);
	Descriptor = {};
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
