#pragma once
#include <d3d12.h>
#include "Memory/TUniquePtr.h"

#include "D3D12RHICommon.h"
#include "Templates/DarkTemplate.h"




class FD3D12BaseShaderResource;




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





class FD3D12Resource
{
public:
	ID3D12Resource* GetResource() const { return Resource.Get(); }



private:


	TRefCountPtr<ID3D12Resource> Resource;


	TRefCountPtr<FD3D12Heap> Heap;





};



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