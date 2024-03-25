#pragma once
#include <d3d12.h>
#include "Memory/TUniquePtr.h"

#include "D3D12RHICommon.h"
#include "Templates/DarkTemplate.h"
#include "Templates/RefCounting.h"




class FD3D12BaseShaderResource;
class FD3D12Resource;




class FD3D12ResourceLocation : public FD3D12DeviceChild
{
public:
	FD3D12ResourceLocation(FD3D12Device* Parent);
	~FD3D12ResourceLocation();

	FD3D12Resource* GetResource() const { return Resource; }
	void SetOwner(FD3D12BaseShaderResource* InOwner) { Owner = InOwner; }



	void Clear();

private:
	FD3D12Resource* Resource;
	FD3D12BaseShaderResource* Owner; 
	void* MappedAddress;



};



struct FD3D12ResourceDesc : public D3D12_RESOURCE_DESC
{
	FD3D12ResourceDesc() = default;
	FD3D12ResourceDesc(const CD3DX12_RESOURCE_DESC& Other):
		D3D12_RESOURCE_DESC(Other)
	{}
	FD3D12ResourceDesc(const D3D12_RESOURCE_DESC& Other): 
		D3D12_RESOURCE_DESC(Other)
	{}
};





class FD3D12Heap : public FD3D12DeviceChild
{
public:
	FD3D12Heap(FD3D12Device* InDevice);

	ID3D12Heap* GetHeap() const;

	void SetHeap(ID3D12Heap* InHeap, const TCHAR* InName, bool bForceGetGPUAddress);

	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return GPUVirtualAddress; }



private:
	TRefCountPtr<ID3D12Heap> Heap;
	TCHAR* HeapName;
	D3D12_HEAP_DESC HeapDesc;
	D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress = 0;
};





class FD3D12Resource : public FD3D12DeviceChild, public FRefCountedObject
{
private:
	TRefCountPtr<ID3D12Resource> Resource;
	TSharedPtr<FD3D12Heap> Heap;

	D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress;
	void* ResourceBaseAddress;

	const FD3D12ResourceDesc Desc;

	D3D12_RESOURCE_STATES DefaultResourceState;
	D3D12_RESOURCE_STATES ReadableState;
	D3D12_RESOURCE_STATES WritableState;

	D3D12_HEAP_TYPE HeapType;
	FString DebugName;


	bool bBackBuffer = true;




public:
	FD3D12Resource() = delete;
	FD3D12Resource(
		FD3D12Device* ParentDevice,
		ID3D12Resource* InResource,
		D3D12_RESOURCE_STATES InInitialResourceState,
		const FD3D12ResourceDesc& InDesc,
		FD3D12Heap* InHeap = nullptr,
		D3D12_HEAP_TYPE InHeapType = D3D12_HEAP_TYPE_DEFAULT
	);

	~FD3D12Resource()
	{

	}


	D3D12_RESOURCE_STATES GetCurrentState() const { return DefaultResourceState; }
	void SetState(D3D12_RESOURCE_STATES State) { DefaultResourceState = State; }
	void SetIsBackBuffer(bool bInBackBuffer) { bBackBuffer = bInBackBuffer; }
	ID3D12Resource* GetResource() const { return Resource.Get(); }
	const FD3D12ResourceDesc& GetDesc() const { return Desc; }


};









class FD3D12BaseShaderResource : public FD3D12DeviceChild
{
public:
	FD3D12BaseShaderResource(FD3D12Device* InParent) :
		FD3D12DeviceChild(InParent),
		ResourceLocation(InParent)
	{
		ResourceLocation.SetOwner(this);
	}
	~FD3D12BaseShaderResource()
	{

	}
	


public:
	FD3D12Resource* GetResouce() const { return ResourceLocation.GetResource(); }



private:
	FD3D12ResourceLocation ResourceLocation;



};


class FD3D12ResourceBarrierBatcher
{
	struct FD3D12ResourceBarrier : public D3D12_RESOURCE_BARRIER
	{
		FD3D12ResourceBarrier() = default;
		FD3D12ResourceBarrier(D3D12_RESOURCE_BARRIER&& Barrier) : D3D12_RESOURCE_BARRIER(MoveTemp(Barrier)) {}
	};


public:
	bool AddTransition(FD3D12Resource* pResource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After, uint32 SubResource);


private:
	TArray<FD3D12ResourceBarrier> Barriers;


};
