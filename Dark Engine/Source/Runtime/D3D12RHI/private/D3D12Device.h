#pragma once
#include "D3D12RHICommon.h"
#include "D3D12CommandList.h"
#include "D3D12Queue.h"
#include "D3D12Adapter.h"


class FD3D12ContextCommon;
class FD3D12CommandAllocator;
class FD3D12CommandList;




class FD3D12Queue
{
public:
	FD3D12Queue(FD3D12Device* Device, ED3D12QueueType);


	FD3D12Device* const Device;
	ED3D12QueueType const QueueType;

	TRefCountPtr<ID3D12CommandQueue> CommandQueue;
	TRefCountPtr<ID3D12Fence> Fence;



	struct
	{
		TArray<FD3D12ContextCommon*>	Contexts;
		TArray<FD3D12CommandAllocator*> Allocators;
		TArray<FD3D12CommandList*> Lists;
	} ObjectPool;



};


class FD3D12Device : public FD3D12AdapterChild
{
public:
	FD3D12Device(FD3D12Adapter* InAdapter);

	FORCEINLINE ID3D12Device* GetDevice() const { return GetParentAdapter()->GetD3DDevice(); }


	FD3D12CommandAllocator* GetCommandAllocator(ED3D12QueueType QueueType);
	FD3D12CommandList* GetCommandList(FD3D12CommandAllocator* CommandAllocator);



	FD3D12Queue& GetQueue(ED3D12QueueType QueueType) { return Queues[(uint32)QueueType]; }

private:
	TArray<FD3D12Queue> Queues;


};