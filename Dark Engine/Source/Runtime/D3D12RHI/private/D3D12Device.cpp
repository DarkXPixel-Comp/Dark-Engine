#include "D3D12Device.h"
#include "D3D12RHIPrivate.h"
#include "D3D12Util.h"
#include "D3D12Descriptors.h"
#include <Logger.h>



FD3D12Device::FD3D12Device(FD3D12Adapter* InAdapter):
	FD3D12AdapterChild(InAdapter),
	DescriptorHeapManager(this),
	BindlessDescriptorManager(this)
{
	for (uint32 i = 0; i < (uint32)ED3D12QueueType::Count; ++i)
	{
		Queues.Emplace(this, (ED3D12QueueType)i);
	}

	DE_LOG(D3D12RHI, Display, TEXT("Queues init"));

	for (uint32 HeapType = 0; HeapType < (int32)ERHIDescriptorHeapType::Count; ++HeapType)
	{
		CpuDescriptorManagers.Emplace(this, (ERHIDescriptorHeapType)HeapType);
	}
	DE_LOG(D3D12RHI, Display, TEXT("Cpu Descriptor Managers init"));

	BindlessDescriptorManager.Init();
	DE_LOG(D3D12RHI, Display, TEXT("Bindless descriptor manager init"));

	DescriptorHeapManager.Init(0, 0);

	DE_LOG(D3D12RHI, Display, TEXT("Descriptor Heap Manager init"));





	ImmediateCommandContext = FD3D12DynamicRHI::GetD3D12RHI()->CreateCommandContext(this, ED3D12QueueType::Direct, true);

	DE_LOG(D3D12RHI, Display, TEXT("Immediate context init"));
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
	CommandQueue->SetName(*(FString(GetD3D12CommandQueueTypeName(InQueueType)) + FString(TEXT(" queue"))));

	DXCall(Device->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));

	Fence->SetName(*(FString(GetD3D12CommandQueueTypeName(InQueueType)) + FString(TEXT(" queue fence"))));
	FenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

}

void FD3D12Queue::WaitFrame()
{
	CommandQueue->Signal(Fence.Get(), ++FenceValue);
	if (Fence->GetCompletedValue() < FenceValue)
	{
		Fence->SetEventOnCompletion(FenceValue, FenceEvent);
		WaitForSingleObject(FenceEvent, 100000);
	}

}
