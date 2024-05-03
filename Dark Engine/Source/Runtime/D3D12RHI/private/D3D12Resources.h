#pragma once
#include <d3d12.h>
#include "Memory/TUniquePtr.h"

#include "D3D12RHICommon.h"
#include "Templates/DarkTemplate.h"
#include "Templates/RefCounting.h"



class FD3D12BaseShaderResource;
class FD3D12Resource;
class FD3D12Heap;
struct FD3D12LockedResource;


struct FD3D12ResourceDesc : public D3D12_RESOURCE_DESC
{
	FD3D12ResourceDesc() = default;
	FD3D12ResourceDesc(const CD3DX12_RESOURCE_DESC& Other) :
		D3D12_RESOURCE_DESC(Other)
	{}
	FD3D12ResourceDesc(const D3D12_RESOURCE_DESC& Other) :
		D3D12_RESOURCE_DESC(Other)
	{}
};



class FD3D12Resource : public FD3D12DeviceChild, public FRefCountedObject
{
private:
	TRefCountPtr<ID3D12Resource> Resource;
	TSharedPtr<FD3D12Heap> Heap;

	D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress;
	void* ResourceBaseAddress;

	const class FD3D12ResourceDesc Desc;

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

	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return GPUVirtualAddress; }

	D3D12_RESOURCE_STATES GetCurrentState() const { return DefaultResourceState; }
	D3D12_HEAP_TYPE GetHeapType() const { return HeapType; }
	void SetState(D3D12_RESOURCE_STATES State) { DefaultResourceState = State; }
	void SetIsBackBuffer(bool bInBackBuffer) { bBackBuffer = bInBackBuffer; }
	ID3D12Resource* GetResource() const { return Resource.Get(); }
	const FD3D12ResourceDesc& GetDesc() const { return Desc; }


};



class FD3D12ResourceLocation : public FD3D12DeviceChild
{
public:
	FD3D12ResourceLocation(FD3D12Device* Parent):
		FD3D12DeviceChild(Parent)
	{}


	void SetResource(FD3D12Resource* InResource) { Resource = InResource; }
	FD3D12Resource* GetResource() const { return Resource.Get(); }
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return Resource->GetGPUVirtualAddress(); }
	void SetOwner(FD3D12BaseShaderResource* InOwner) { Owner = InOwner; }

	void* GetMappedBaseAddress() const { return MappedAddress; }

	uint64 GetSize() const { return Resource->GetDesc().Width; }

	bool IsValid() const
	{
		return Resource != nullptr;
	}

	void AsStandAlone(FD3D12Resource* InResource, uint64 InSize, const D3D12_HEAP_PROPERTIES* HeapProperties)
	{
		SetResource(InResource);
		SetSize(InSize);

		if (Resource->GetHeapType() == D3D12_HEAP_TYPE_UPLOAD || Resource->GetHeapType() == D3D12_HEAP_TYPE_READBACK)
		{
			D3D12_RANGE Range = { 0, 0 };
			Resource->GetResource()->Map(0, &Range, &MappedAddress);
		}

		SetGPUVirtualAddress(Resource->GetGPUVirtualAddress());
	}

	void SetSize(uint64 InValue) { Size = InValue; }

	void Clear()
	{
		if (Resource)
		{
			GPUVirtualAddress = D3D12_GPU_VIRTUAL_ADDRESS();
			if (MappedAddress)
			{
				D3D12_RANGE Range = { 0, 0 };
				Resource->GetResource()->Unmap(0, &Range);
				MappedAddress = nullptr;
			}
			Owner = nullptr;
			Size = 0;
			Resource.Reset();
		}
	}

	void SetGPUVirtualAddress(D3D12_GPU_VIRTUAL_ADDRESS InAddress) { GPUVirtualAddress = InAddress; }
																  
private:
	D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress;
	TRefCountPtr<FD3D12Resource> Resource;
	FD3D12BaseShaderResource* Owner; 
	void* MappedAddress = nullptr;
	uint64 Size = 0;
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



public:
	FD3D12ResourceLocation ResourceLocation;
};



struct FD3D12LockedResource : public FD3D12DeviceChild
{
	FD3D12LockedResource(FD3D12Device* InDevice) :
		FD3D12DeviceChild(InDevice),
		LockedOffset(0),
		LockedPitch(0),
		bLocked(false),
		bLockedForReadOnly(false),
		bHasNeverBeenLocked(true),
		MappedAddress(nullptr)
	{}

	FORCEINLINE void Reset()
	{
		Resource.Reset();
		bLocked = false;
		bLockedForReadOnly = false;
		LockedOffset = 0;
		LockedPitch = 0;
	}

	TRefCountPtr<ID3D12Resource> Resource;
	//FD3D12ResourceLocation ResourceLocation;
	uint32 LockedOffset;
	uint32 LockedPitch;
	uint32 bLocked : 1;
	uint32 bLockedForReadOnly : 1;
	uint32 bHasNeverBeenLocked : 1;
	void* MappedAddress;

};


class FD3D12Buffer : public FRHIBuffer, public FD3D12BaseShaderResource
{
public:
	FD3D12Buffer(FD3D12Device* InParent, const FRHIBufferDesc& Desc):
		FRHIBuffer(Desc),
		FD3D12BaseShaderResource(InParent),
		LockedData(InParent)
	{}

	virtual ~FD3D12Buffer()
	{
	}
	void UploadResourceData(class FRHICommandListBase& InRHICmdList, const void* Buffer, uint32 SizeBuffer,
		D3D12_RESOURCE_STATES InDestinationState, const TCHAR* Name = nullptr);



	struct FD3D12LockedResource LockedData;


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


class FD3D12UniformBuffer : public FRHIUniformBuffer, public FD3D12DeviceChild
{
public:
	FD3D12UniformBuffer(class FD3D12Device* InParent, EUniformBufferUsage InUniformBufferUsage):
		FD3D12DeviceChild(InParent),
		ResourceLocation(InParent),
		UniformBufferUsage(InUniformBufferUsage)
	{}

	FD3D12ResourceLocation ResourceLocation;

	EUniformBufferUsage UniformBufferUsage;

};
