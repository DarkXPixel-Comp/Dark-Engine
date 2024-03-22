#pragma once

#include "D3D12RHICommon.h"
#include "RHIDefines.h"
#include "Containers/BitArray.h"

class FD3D12DescriptorHeap;

class FD3D12BindlessDescriptorHeapManager : public FD3D12DeviceChild
{
public:
	FD3D12BindlessDescriptorHeapManager(FD3D12Device* InDevice, ERHIDescriptorHeapType InType,
		ERHIBindlessConfiguration InConfiguration, uint32 InNumDescriptorsPerHeap);

	FRHIDescriptorHandle Allocate();
	void Free(FRHIDescriptorHandle InHandle);



	FD3D12DescriptorHeap* GetHeap();
	const FD3D12DescriptorHeap* GetHeap() const;

	ERHIDescriptorHeapType GetType() const { return Type; }


	void UpdateImmediate(FRHIDescriptorHandle InHandle, D3D12_CPU_DESCRIPTOR_HANDLE InSourceCpuHandle);

private:
	void SetupInitialState(uint32 InNumDescriptorsPerHeap);
	void ResizeHeaps(uint32 InNumDescriptorsPerHeap);


	FD3D12DescriptorHeap* CreateCpuHeapImpl(uint32 InNumDescriptorsPerHeap);
	FD3D12DescriptorHeap* CreateGpuHeapImpl(uint32 InNumDescriptorsPerHeap);

private:
	const ERHIDescriptorHeapType Type;
	const ERHIBindlessConfiguration Configuration;
	uint32 NumDescriptorsPerHeap;

	int32 NumAllocations = 0;
	TBitArray Allocations;


	TRefCountPtr<FD3D12DescriptorHeap> CpuHeap = nullptr;
	TRefCountPtr<FD3D12DescriptorHeap> GpuHeap = nullptr;
	

};




class FD3D12BindlessDescriptorManager : public FD3D12DeviceChild
{
public:
	FD3D12BindlessDescriptorManager(FD3D12Device* InDevice);

	void Init();


	FRHIDescriptorHandle Allocate(ERHIDescriptorHeapType InType);
	void ImmediateFree(FRHIDescriptorHandle InHandle);

	void UpdateImmediate(FRHIDescriptorHandle InHandle, D3D12_CPU_DESCRIPTOR_HANDLE InSourceCpuHandle);

	FD3D12DescriptorHeap* GetHeap(ERHIDescriptorHeapType InType) const;

	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(FRHIDescriptorHandle InHandle) const;

private:
	ERHIBindlessConfiguration ResourcesConfiguration;
	ERHIBindlessConfiguration SamplersConfiguration;
	
	TArray<FD3D12BindlessDescriptorHeapManager>	Managers;


};