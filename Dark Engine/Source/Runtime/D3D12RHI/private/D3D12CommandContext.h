#pragma once
#include "D3D12Device.h"
#include "D3D12Queue.h"
#include "D3D12CommandList.h"
#include "D3D12Resources.h"
#include "D3D12Submission.h"
#include "RHIContext.h"
#include "RHIResources.h"




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

	class FD3D12CommandAllocator* CommandAllocator = nullptr;
	class FD3D12CommandList* CommandList = nullptr;
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
	void RHIEndDrawingViewport(FRHIViewport* Viewport, bool bPresent, bool Vsync) override;
	void RHISetViewport(float MinX, float MinY, float MinZ, float MaxX, float MaxY, float MaxZ) override;
};

class FD3D12CommandContext : public FD3D12ContextCommon, public FD3D12CommandContextBase, public FD3D12DeviceChild
{
public:
	FD3D12CommandContext(FD3D12Device* InParent, ED3D12QueueType QueueType, bool InIsDefaultContext);
	void RHIBeginFrame() override;
	void RHIEndFrame() override;
	void RHIBeginImGui() override;
	void RHIEndImGui() override;
	void RHIBeginRenderPass(struct FRHIRenderPassInfo& InInfo);

	void SetRenderTargets(const FRHIRenderTargetView* RenderTargetsRHI, const FRHIDepthRenderTargetView* DepthStencilViewRHI);

};