#pragma once
#include "D3D12RHICommon.h"
#include "D3D12Device.h"
#include "Templates/DarkTemplate.h"

enum class ED3D12QueueType;

class FD3D12CommandAllocator;
class FD3D12CommandList;
class FD3D12DynamicRHI;
class FD3D12QueryHeap;
class FD3D12Queue;
class FD3D12Timing;
enum class ED3D12QueueType;

enum class ED3D12SyncPointType
{
	GPUOnly,
	GPUAndCPU
};




class FD3D12SyncPoint
{
	friend FD3D12DynamicRHI;
	friend FD3D12Queue;

	FD3D12SyncPoint(ED3D12SyncPointType Type)
	{

	}



};



struct FD3D12PayloadBase
{
	struct FManualFence
	{
		TRefCountPtr<ID3D12Fence> Fence;
		uint64 Value;
		FManualFence(TRefCountPtr<ID3D12Fence>&& InFence, uint64 InValue):
			Fence(MoveTemp(InFence)),
			Value(InValue)
		{}
	};

	FD3D12Queue& Queue;

	//struct : public TArray<>

	TArray<FManualFence> FencesToWait;
	TArray<FManualFence> FencesToSignal;
	TArray<FD3D12CommandList*> CommandListsToExecute;

	uint64 CompletionFenceValue = 0;


	TArray<FD3D12CommandAllocator*> AllocatorsToRelease;



protected:
	FD3D12PayloadBase(FD3D12Device* Device, ED3D12QueueType QueueType):
		Queue(Device->GetQueue(QueueType))
	{}
};

struct FD3D12Payload : public FD3D12PayloadBase
{
	FD3D12Payload(FD3D12Device* Device, ED3D12QueueType QueueType):
		FD3D12PayloadBase(Device, QueueType)
	{}

};