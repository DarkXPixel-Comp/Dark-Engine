#include "D3D12Device.h"
#include "D3D12RHIPrivate.h"
#include "D3D12Util.h"
#include "D3D12Descriptors.h"



FD3D12Device::FD3D12Device(FD3D12Adapter* InAdapter):
	FD3D12AdapterChild(InAdapter),
	DescriptorHeapManager(this)
{
	for (uint32 i = 0; i < (uint32)ED3D12QueueType::Count; ++i)
	{
		Queues.Emplace(this, (ED3D12QueueType)i);
	}

	for (uint32 HeapType = 0; HeapType < (int32)ERHIDescriptorHeapType::Count; ++HeapType)
	{
		CpuDescriptorManagers.Emplace(this, (ERHIDescriptorHeapType)HeapType);
	}

}

FD3D12CommandAllocator* FD3D12Device::GetCommandAllocator(ED3D12QueueType QueueType)
{
	FD3D12CommandAllocator* Allocator = Queues[(uint32)QueueType].ObjectPool.Allocators.PopBack();


	if (!Allocator)
	{
		Allocator = new FD3D12CommandAllocator(this, QueueType);
	}
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

}
