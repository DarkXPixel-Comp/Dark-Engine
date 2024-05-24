#include "D3D12Device.h"
#include "D3D12RHIPrivate.h"
#include "D3D12Util.h"
#include "D3D12Descriptors.h"
#include <Logger.h>
#include "D3D12PipelineState.h"



FD3D12Device::FD3D12Device(FD3D12Adapter* InAdapter):
	FD3D12AdapterChild(InAdapter),
	DescriptorHeapManager(this),
	BindlessDescriptorManager(this),
	PipelineStateManager(new FD3D12PipelineStateManager(GetParentAdapter()))
{
	for (uint32 i = 0; i < (uint32)ED3D12QueueType::Count; ++i)
	{
		Queues.Emplace(this, (ED3D12QueueType)i);
	}

	DE_LOG(D3D12RHI, Log, TEXT("Queues init"));

	for (uint32 HeapType = 0; HeapType < (int32)ERHIDescriptorHeapType::Count; ++HeapType)
	{
		CpuDescriptorManagers.Emplace(this, (ERHIDescriptorHeapType)HeapType);
	}
	DE_LOG(D3D12RHI, Log, TEXT("Cpu Descriptor Managers init"));

	BindlessDescriptorManager.Init();
	DE_LOG(D3D12RHI, Log, TEXT("Bindless descriptor manager init"));

	DescriptorHeapManager.Init(0, 0);

	DE_LOG(D3D12RHI, Log, TEXT("Descriptor Heap Manager init"));





	ImmediateCommandContext = FD3D12DynamicRHI::GetD3D12RHI()->CreateCommandContext(this, ED3D12QueueType::Direct, true);

	DE_LOG(D3D12RHI, Log, TEXT("Immediate context init"));
}

FD3D12Device::~FD3D12Device()
{
}

void FD3D12Device::Destroy()
{
	if (PipelineStateManager)
	{
		PipelineStateManager->CachePSO();
	}
}

FD3D12CommandAllocator* FD3D12Device::GetCommandAllocator(ED3D12QueueType QueueType)
{
	//Queues[(uint32)QueueType];
	FD3D12CommandAllocator* Allocator = Queues[(uint32)QueueType].ObjectPool.Allocators.PopBack();


	if (!Allocator)
	{
		Allocator = new FD3D12CommandAllocator(this, QueueType);
	}

	Allocator->Reset();
	check(Allocator);

	return Allocator;
}

FD3D12CommandList* FD3D12Device::GetCommandList(FD3D12CommandAllocator* CommandAllocator)
{
	check(CommandAllocator->Device == this);

	FD3D12CommandList* List = Queues[(uint32)CommandAllocator->QueueType].ObjectPool.Lists.PopBack();
	if (!List)
	{
		List = new FD3D12CommandList(CommandAllocator);
	}
	else
	{
		List->Reset(CommandAllocator);
	}

	return List;

}

FD3D12Queue::FD3D12Queue(FD3D12Device* InDevice, ED3D12QueueType InQueueType):
		Device(InDevice),
		QueueType(InQueueType)
{
	D3D12_COMMAND_QUEUE_DESC CommandQueueDesc = {};
	CommandQueueDesc.Type = GetD3DCommandListType(QueueType);
	CommandQueueDesc.Priority = 0;
	CommandQueueDesc.NodeMask = 0;
	CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;


	DXCall(Device->GetDevice()->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&CommandQueue)));
	UpgradeInterface(&CommandQueue);
	CommandQueue->SetName(*(FString(GetD3D12CommandQueueTypeName(InQueueType)) + FString(TEXT(" queue"))));

	DXCall(Device->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));

	Fence->SetName(*(FString(GetD3D12CommandQueueTypeName(InQueueType)) + FString(TEXT(" queue fence"))));
	FenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

}

void FD3D12Queue::WaitFrame()
{
	if (Fence->GetCompletedValue() < FenceValue)
	{
		Fence->SetEventOnCompletion(FenceValue, FenceEvent);
		WaitForSingleObject(FenceEvent, 100000);
	}

}

void FD3D12Queue::Signal()
{
	CommandQueue->Signal(Fence.Get(), ++FenceValue);
}

void FD3D12Queue::AddCommandList(FD3D12CommandList* InCommandList)
{
	if (!InCommandList->IsClosed())
	{
		InCommandList->Close();
	}
	ListsToExecute.Push(InCommandList);
}

void FD3D12Queue::CheckCommandAllocators()
{
	uint64 WaitValue = Fence->GetCompletedValue();

	for (int32 i = AllocatorsToRelease.Num() - 1; i >= 0; --i)
	{
		if (AllocatorsToRelease[i]->Index >= WaitValue)
		{
			ObjectPool.Allocators.Add(AllocatorsToRelease[i]);
			IndexesToDelete.Push(AllocatorsToRelease[i]);
		}
	}

	for (uint64 i = 0; i < IndexesToDelete.Num(); i++)
	{
		AllocatorsToRelease.Remove(IndexesToDelete[i]);
	}

	IndexesToDelete.Empty();
}

void FD3D12Queue::ExecuteCommandLists()
{
	ID3D12CommandList* Lists[32];

	for (int32 i = 0; i < ListsToExecute.Num(); ++i)
	{
		FD3D12CommandAllocator* Allocator = ListsToExecute[i]->GetCommandAllocator();
		ListsToExecute[i]->Reset();
		Lists[i] = ListsToExecute[i]->GetCommandList();
		ObjectPool.Lists.Add(ListsToExecute[i]);
		Allocator->Index = FenceValue + 1;
		AllocatorsToRelease.Add(Allocator);
		
	}

	if(ListsToExecute.Num())
	{
		CommandQueue->ExecuteCommandLists(ListsToExecute.Num(), Lists);
		CommandQueue->Signal(Fence.Get(), ++FenceValue);
		ListsToExecute.Empty();
	}


	//CheckCommandAllocators();
}
