#pragma once
#include "D3D12Device.h"
#include "D3D12Queue.h"
#include "D3D12CommandList.h"
#include "D3D12Resources.h"
#include "D3D12Submission.h"
#include "RHIContext.h"




enum class ED3D12FlushFlags
{
	None = 0,
	WaitForSubmission,
	WaitForCompletion
};



class FD3D12ContextCommon
{
protected:
	FD3D12ContextCommon(FD3D12Device* Device, ED3D12QueueType QueueType, bool bIsDefaultContext);

public:
	virtual void OpenCommandList();
	virtual void CloseCommandList();

	bool IsOpen() const { return CommandList != nullptr; }

	void SignalManualFence(ID3D12Fence* Fence, uint64 Value);
	void WaitManualFence(ID3D12Fence* Fence, uint64 Value);
	void Finalize(TArray<FD3D12Payload*>& OutPayloads);
	bool isAsyncComputeContext() const { return QueueType == ED3D12QueueType::Async; }
	uint64 GetCommandListID() { return GetCommandList().State.CommandListID; }

	FD3D12CommandList& GetCommandList()
	{
		if (!CommandList)
		{
			OpenCommandList();
		}
		return *CommandList;
	}

protected:
	enum class EPhase
	{
		Wait,
		Execute,
		Signal
	} CurrentPhase = EPhase::Wait;
	FD3D12Payload* GetPayload(EPhase Phase)
	{
		if (Payloads.Num() == 0 || Phase < CurrentPhase)
		{
			Payloads.Add(new FD3D12Payload(Device, QueueType));
		}
		CurrentPhase = Phase;
		return Payloads.Last();
	}





private:
	FD3D12Device* const Device;
	ED3D12QueueType const QueueType;
	bool const bIsDefualtContext;

	class FD3D12CommandAllocator* CommandAllocator;
	class FD3D12CommandList* CommandList;
	class FD3D12ResourceBarrierBatcher ResourceBarrierBatcher;
	TArray<FD3D12Payload*> Payloads;

	

public:
	void FlushCommands(ED3D12FlushFlags Flags = ED3D12FlushFlags::None);



};



class FD3D12CommandContextBase : public IRHICommandContext, public FD3D12AdapterChild
{
public:
	FD3D12CommandContextBase(FD3D12Adapter* InParent);

	void RHIBeginDrawingViewport(FRHIViewport* Viewport, FRHITexture* RenderTargetRHI) override;





};