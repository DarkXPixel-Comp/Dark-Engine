#include "D3D12CommandList.h"
#include "D3D12Util.h"


uint64 FD3D12CommandList::FState::NextCommandListID = 0;





FD3D12CommandAllocator::FD3D12CommandAllocator(FD3D12Device* InDevice, ED3D12QueueType InQueueType):
		Device(InDevice),
	QueueType(InQueueType)
{
	DXCall(Device->GetDevice()->CreateCommandAllocator(GetD3DCommandListType(QueueType), IID_PPV_ARGS(&CommandAllocator)));


}

FD3D12CommandAllocator::~FD3D12CommandAllocator()
{
	CommandAllocator.Reset();
}

void FD3D12CommandAllocator::Reset()
{
	CommandAllocator->Reset();
}

FD3D12CommandList::FD3D12CommandList(FD3D12CommandAllocator* CommandAllocator):
	Device(CommandAllocator->Device),
	QueueType(CommandAllocator->QueueType),
	State(CommandAllocator)
{
	switch (QueueType)
	{
	case ED3D12QueueType::Direct:
	case ED3D12QueueType::Async:
		DXCall(Device->GetDevice()->CreateCommandList(0, GetD3DCommandListType(QueueType), *CommandAllocator,
			nullptr, IID_PPV_ARGS(&GraphicsCommandList)));
		CommandList = GraphicsCommandList;
		CommandList->QueryInterface(IID_PPV_ARGS(&CopyCommandList));
		CommandList->QueryInterface(IID_PPV_ARGS(&DebugCommandList));
		CommandList->QueryInterface(IID_PPV_ARGS(&GraphicsCommandList4));
		CommandList->QueryInterface(IID_PPV_ARGS(&GraphicsCommandList7));
		break;
	case ED3D12QueueType::Copy:
		DXCall(Device->GetDevice()->CreateCommandList(0, GetD3DCommandListType(QueueType), *CommandAllocator,
			nullptr, IID_PPV_ARGS(&CopyCommandList)));
		CommandList = CopyCommandList;
		break;
	}

	State.CommandAllocator = CommandAllocator;
	State.CommandListID = State.NextCommandListID++;
	FString Name = FString::PrintF(TEXT("FD3D12CommandList%i"), State.CommandListID);

	SetName(CommandList.Get(), *Name);
}

FD3D12CommandList::~FD3D12CommandList()
{

}

void FD3D12CommandList::Reset(FD3D12CommandAllocator* NewCommandAllocator)
{
	check(IsClosed());
	check(NewCommandAllocator->Device == Device && NewCommandAllocator->QueueType == QueueType);

	DXCall(GraphicsCommandList->Reset(*NewCommandAllocator, nullptr));

	State.CommandAllocator = NewCommandAllocator;
	State.bIsClosed = false;
}

void FD3D12CommandList::Reset()
{
	State.CommandAllocator = nullptr;
}

void FD3D12CommandList::Close()
{
	if(!State.bIsClosed)
	{
		State.bIsClosed = true;
		DXCall(GraphicsCommandList->Close());
	}
}

FD3D12CommandList::FState::FState(FD3D12CommandAllocator* InCommandAllocator):
	CommandAllocator(InCommandAllocator),
	CommandListID(NextCommandListID++)
{

}
