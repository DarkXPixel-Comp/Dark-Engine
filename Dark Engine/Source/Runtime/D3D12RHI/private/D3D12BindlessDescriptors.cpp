#include "D3D12BindlessDescriptors.h"
#include "D3D12Device.h"
#include "D3D12Descriptors.h"




FD3D12DescriptorHeap* FD3D12BindlessDescriptorHeapManager::CreateCpuHeapImpl(uint32 InNumDescriptorsPerHeap)
{
	const TCHAR* const HeapName = Type == ERHIDescriptorHeapType::Standart ? TEXT("BindlessResourcesCPU") :
		TEXT("BindlessSamplersCPU");

	return GetParentDevice()->GetDescriptorHeapManager().AllocateHeap(HeapName, Type, InNumDescriptorsPerHeap, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

}

FD3D12DescriptorHeap* FD3D12BindlessDescriptorHeapManager::CreateGpuHeapImpl(uint32 InNumDescriptorsPerHeap)
{
	const TCHAR* const HeapName = Type == ERHIDescriptorHeapType::Standart ? TEXT("BindlessResources") :
		TEXT("BindlessSamplers");

	return GetParentDevice()->GetDescriptorHeapManager().AllocateHeap(HeapName, Type, InNumDescriptorsPerHeap, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

}



FD3D12BindlessDescriptorHeapManager::FD3D12BindlessDescriptorHeapManager(FD3D12Device* InDevice, ERHIDescriptorHeapType InType, 
	ERHIBindlessConfiguration InConfiguration, uint32 InNumDescriptorsPerHeap):
	FD3D12DeviceChild(InDevice),
	Type(InType),
	Configuration(InConfiguration),
	NumDescriptorsPerHeap(InNumDescriptorsPerHeap)
{
	SetupInitialState(InNumDescriptorsPerHeap);
}

FRHIDescriptorHandle FD3D12BindlessDescriptorHeapManager::Allocate()
{
	int32 AllocatedIndex = -1;

	while ((AllocatedIndex = Allocations.FindAndSetFirstZeroBit()) == -1)
	{
		const int32 NewNumDescriptorsPerHeap = NumDescriptorsPerHeap * 1.5;
		ResizeHeaps(NewNumDescriptorsPerHeap);
	}

	++NumAllocations;								



	return FRHIDescriptorHandle(Type, AllocatedIndex);
}

void FD3D12BindlessDescriptorHeapManager::Free(FRHIDescriptorHandle InHandle)
{
	if (InHandle.IsValid())
	{
		const int32 Index = InHandle.GetIndex();
		
		check(Allocations[Index] == true);
		Allocations.SetBit(Index, false);

		--NumAllocations;

	}

}

FD3D12DescriptorHeap* FD3D12BindlessDescriptorHeapManager::GetHeap()
{
	return GpuHeap.Get();
}

const FD3D12DescriptorHeap* FD3D12BindlessDescriptorHeapManager::GetHeap() const
{
	return GpuHeap.Get();
}

void FD3D12BindlessDescriptorHeapManager::ResizeHeaps(uint32 InNumDescriptorsPerHeap)
{
	if (InNumDescriptorsPerHeap > NumDescriptorsPerHeap)
	{
		TRefCountPtr<FD3D12DescriptorHeap> OldCpuHeap = CpuHeap;
		TRefCountPtr<FD3D12DescriptorHeap> OldGpuHeap = GpuHeap;
		const uint32 OldNumDescriptorsPerHeap = NumDescriptorsPerHeap;

		Allocations.Resize(InNumDescriptorsPerHeap);

		NumDescriptorsPerHeap = InNumDescriptorsPerHeap;

		CpuHeap = CreateCpuHeapImpl(InNumDescriptorsPerHeap);
		GpuHeap = CreateCpuHeapImpl(InNumDescriptorsPerHeap);

		CopyDescriptors(GetParentDevice(), CpuHeap.Get(), OldCpuHeap.Get(), OldNumDescriptorsPerHeap);
		CopyDescriptors(GetParentDevice(), GpuHeap.Get(), OldCpuHeap.Get(), OldNumDescriptorsPerHeap);

		FD3D12DescriptorHeapManager& HeapManager = GetParentDevice()->GetDescriptorHeapManager();

		HeapManager.ImmediateFreeHeap(OldCpuHeap.Get());
		HeapManager.ImmediateFreeHeap(OldGpuHeap.Get());


	}


}

void FD3D12BindlessDescriptorHeapManager::UpdateImmediate(FRHIDescriptorHandle InHandle, D3D12_CPU_DESCRIPTOR_HANDLE InSourceCpuHandle)
{
	CopyDescriptor(GetParentDevice(), CpuHeap.Get(), InHandle, InSourceCpuHandle);
	CopyDescriptor(GetParentDevice(), GpuHeap.Get(), InHandle, InSourceCpuHandle);
}

void FD3D12BindlessDescriptorHeapManager::SetupInitialState(uint32 InNumDescriptorsPerHeap)
{
	const TCHAR* const HeapNameCpu = Type == ERHIDescriptorHeapType::Standart ? TEXT("BindlessResourcesCPU") : TEXT("BindlessSamplersCPU");
	const TCHAR* const HeapNameGpu = Type == ERHIDescriptorHeapType::Standart ? TEXT("BindlessResourcesCPU") : TEXT("BindlessSamplersCPU");
	CpuHeap = GetParentDevice()->GetDescriptorHeapManager().AllocateHeap(HeapNameCpu, Type, InNumDescriptorsPerHeap, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	GpuHeap = GetParentDevice()->GetDescriptorHeapManager().AllocateHeap(HeapNameGpu, Type, InNumDescriptorsPerHeap, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);


	Allocations = TBitArray(false, InNumDescriptorsPerHeap);
	NumAllocations = 0;

	NumDescriptorsPerHeap = InNumDescriptorsPerHeap;
}

FD3D12BindlessDescriptorManager::FD3D12BindlessDescriptorManager(FD3D12Device* InDevice):
	FD3D12DeviceChild(InDevice)
{

}



/*FD3D12BindlessDescriptorHeapManager(FD3D12Device* InDevice, ERHIDescriptorHeapType InType,
		ERHIBindlessConfiguration InConfiguration, uint32 InNumDescriptorsPerHeap);*/
void FD3D12BindlessDescriptorManager::Init()
{
	ResourcesConfiguration = ERHIBindlessConfiguration::AllShaders;
	ResourcesConfiguration = ERHIBindlessConfiguration::AllShaders;


	const uint32 NumResourceDescriptors = 1000 * 1000;
	const uint32 NumSamplersDescriptors = 2048;

	if (NumResourceDescriptors > 0)
	{
		Managers.Emplace(GetParentDevice(), ERHIDescriptorHeapType::Standart, ResourcesConfiguration, NumResourceDescriptors);
	}

	if (NumSamplersDescriptors > 0)
	{
		Managers.Emplace(GetParentDevice(), ERHIDescriptorHeapType::Sampler, SamplersConfiguration, NumSamplersDescriptors);
	}


}

FRHIDescriptorHandle FD3D12BindlessDescriptorManager::Allocate(ERHIDescriptorHeapType InType)
{
	for (FD3D12BindlessDescriptorHeapManager& Manager : Managers)
	{
		if (Manager.GetType() == InType)
		{
			return Manager.Allocate();
		}
	}

	return FRHIDescriptorHandle();
}

void FD3D12BindlessDescriptorManager::ImmediateFree(FRHIDescriptorHandle InHandle)
{
	for (FD3D12BindlessDescriptorHeapManager& Manager : Managers)
	{
		if (Manager.GetType() == InHandle.GetHeapType())
		{
			return Manager.Free(InHandle);
		}
	}
}

void FD3D12BindlessDescriptorManager::UpdateImmediate(FRHIDescriptorHandle InHandle, D3D12_CPU_DESCRIPTOR_HANDLE InSourceCpuHandle)
{
	for (FD3D12BindlessDescriptorHeapManager& Manager : Managers)
	{
		if (Manager.GetType() == InHandle.GetHeapType())
		{
			return Manager.UpdateImmediate(InHandle, InSourceCpuHandle);
		}
	}

}

FD3D12DescriptorHeap* FD3D12BindlessDescriptorManager::GetHeap(ERHIDescriptorHeapType InType)
{
	for (FD3D12BindlessDescriptorHeapManager& Manager : Managers)
	{
		if (Manager.GetType() == InType)
		{
			return Manager.GetHeap();
		}

	}

	return nullptr;
}

D3D12_GPU_DESCRIPTOR_HANDLE FD3D12BindlessDescriptorManager::GetGpuHandle(FRHIDescriptorHandle InHandle) const
{
	for (const FD3D12BindlessDescriptorHeapManager& Manager : Managers)
	{
		if (Manager.GetType() == InHandle.GetHeapType())
		{
			return Manager.GetHeap()->GetGPUSlotHandle(InHandle.GetIndex());
		}

	}



	return D3D12_GPU_DESCRIPTOR_HANDLE();
}
