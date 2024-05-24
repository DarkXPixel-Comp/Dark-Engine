#pragma once
#include "D3D12Device.h"
#include "D3D12Queue.h"
#include "Memory/TUniquePtr.h"




class FD3D12CommandAllocator
{
private:
	friend FD3D12Device;

	FD3D12CommandAllocator(FD3D12Device* InDevice, ED3D12QueueType InQueueType);

public:
	~FD3D12CommandAllocator();

	FD3D12Device* const Device;
	ED3D12QueueType QueueType;

	void Reset();

	operator ID3D12CommandAllocator* () { return CommandAllocator.Get(); }

	uint64 Index = 0;


private:
	TRefCountPtr<ID3D12CommandAllocator> CommandAllocator;
};


class FD3D12CommandList
{
private:
	friend FD3D12Device;
	friend class FD3D12ContextCommon;

	FD3D12CommandList(FD3D12CommandAllocator* CommandAllocator);

public:
	~FD3D12CommandList();

	FD3D12Device* const Device;
	ED3D12QueueType const QueueType;

	void Reset(FD3D12CommandAllocator* NewCommandAllocator);
	void Reset();
	void Close();

	bool IsClosed() const { return State.bIsClosed; }


	ID3D12CommandList* GetCommandList() const { return CommandList.Get(); }
	ID3D12GraphicsCommandList* GetGraphicsCommandList() const { return GraphicsCommandList.Get(); }
	ID3D12GraphicsCommandList4* GetGraphicsCommandList4() const { return GraphicsCommandList4.Get(); }
	ID3D12GraphicsCommandList7* GetGraphicsCommandList7() const { return GraphicsCommandList7.Get(); }

	FD3D12CommandAllocator* GetCommandAllocator() const { return State.CommandAllocator; }



private:
	struct
	{
		TRefCountPtr<ID3D12CommandList>	CommandList;
		TRefCountPtr<ID3D12GraphicsCommandList>	CopyCommandList;
		TRefCountPtr<ID3D12GraphicsCommandList>	GraphicsCommandList;
		TRefCountPtr<ID3D12GraphicsCommandList4>	GraphicsCommandList4;
		TRefCountPtr<ID3D12GraphicsCommandList7>	GraphicsCommandList7;
		TRefCountPtr<ID3D12DebugCommandList>	DebugCommandList;
	};

	struct FState
	{
		static uint64 NextCommandListID;
		FState(FD3D12CommandAllocator* CommandAllocator);

		FD3D12CommandAllocator* CommandAllocator;

		uint64 CommandListID;

		uint32 NumCommands;
		bool bIsClosed = false;
	} State;
};

