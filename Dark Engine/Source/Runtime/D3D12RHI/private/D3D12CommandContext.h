#pragma once
#include "D3D12Queue.h"
#include "D3D12CommandList.h"
#include "D3D12Resources.h"
#include "D3D12Submission.h"
#include "RHIContext.h"
#include "RHIResources.h"
#include "D3D12ConstantBuffer.h"
#include "D3D12StateCache.h"
#include "RHIShaderParameters.h"
#include "D3D12BarrierBatcher.h"



class FD3D12Device;


struct FD3D12ResourceBarrier
{
	D3D12_RESOURCE_STATES After;
	FD3D12Resource* Resource = nullptr;
	uint64 Subresource = 0;
	ID3D12Resource* NativeResource = nullptr;
	D3D12_RESOURCE_STATES Before;
};


enum class ED3D12FlushFlags
{
	None = 0,
	WaitForSubmission,
	WaitForCompletion
};

class FD3D12CommandContext;


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
	void TransitionResource(FD3D12Resource* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After, uint32 SubResource);
	void TransitionResource(ID3D12Resource* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After, uint32 SubResource);
	void TransitionResource(FD3D12Resource* Resource, D3D12_RESOURCE_STATES After, uint32 SubResource);
	void TransitionResource(FD3D12Buffer* Buffer, CD3DX12_BUFFER_BARRIER BufferBarrier);
	void TransitionResource(FD3D12Texture* Texture, CD3DX12_TEXTURE_BARRIER TextureBarrier);
	void TransitionResource(const FD3D12ResourceLocation& Resource, CD3DX12_BUFFER_BARRIER BufferBarrier);

	void TransitionResource(FD3D12Buffer* Buffer, D3D12_BARRIER_SYNC AfterSync, D3D12_BARRIER_ACCESS AfterAccess,
		uint64 Size = UINT64_MAX, uint64 Offset = 0);

	void TransitionResource(FD3D12Texture* InBuffer, D3D12_BARRIER_SYNC AfterSync,
		D3D12_BARRIER_ACCESS AfterAccess, D3D12_BARRIER_LAYOUT AfterLayout, CD3DX12_BARRIER_SUBRESOURCE_RANGE Subresource = CD3DX12_BARRIER_SUBRESOURCE_RANGE(0));

	void TransitionBuffer(FD3D12ResourceLocation& Resource, D3D12_BARRIER_SYNC AfterSync, D3D12_BARRIER_ACCESS AfterAccess,
		uint64 Size = UINT64_MAX, uint64 Offset = 0);

	void TransitionBuffer(FD3D12Resource* Resource, D3D12_BARRIER_SYNC AfterSync, D3D12_BARRIER_ACCESS AfterAccess,
		uint64 Size = UINT64_MAX, uint64 Offset = 0);

	void TransitionTexture(FD3D12Resource* InTexture, D3D12_BARRIER_SYNC AfterSync,
		D3D12_BARRIER_ACCESS AfterAccess, D3D12_BARRIER_LAYOUT AfterLayout, CD3DX12_BARRIER_SUBRESOURCE_RANGE Subresource = CD3DX12_BARRIER_SUBRESOURCE_RANGE(0));

	void TransitionBuffer(ID3D12Resource* Resource, CD3DX12_BUFFER_BARRIER Buffer);

	void FlushBarriers();
	void EnhancedFlushBarriers();

	void AddLockedResource(const FD3D12LockedResource& Resource)
	{
		LockedResources.Add(Resource);
	}

	FD3D12CommandList& GetCommandList()
	{
		if (!CommandList)
		{
			OpenCommandList();
		}
#if USE_OPTICK
		Optick::SetGpuContext(Optick::GPUContext(CommandList->GetGraphicsCommandList(), CommandList->QueueType == ED3D12QueueType::Direct ? Optick::GPUQueueType::GPU_QUEUE_GRAPHICS : Optick::GPU_QUEUE_COMPUTE));
#endif
		return *CommandList;
	}

	ID3D12GraphicsCommandList* GetGraphicsList()
	{
		return GetCommandList().GetGraphicsCommandList();
	}

	ID3D12GraphicsCommandList4* GetGraphicsList4()
	{
		return GetCommandList().GetGraphicsCommandList4();
	}

	ID3D12GraphicsCommandList7* GetGraphicsList7()
	{
		return GetCommandList().GetGraphicsCommandList7();
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
	bool const bIsDefaultContext;
	bool const bSupportEnhancedBarriers;

	class FD3D12CommandAllocator* CommandAllocator = nullptr;
	class FD3D12CommandList* CommandList = nullptr;
	FD3D12BarrierBatcher BarrierBatcher;
	TArray<FD3D12Payload*> Payloads;
	TArray<FD3D12LockedResource> LockedResources;
	TArray<FD3D12ResourceBarrier> Barriers;

	TArray<CD3DX12_BUFFER_BARRIER> BufferBarriers;
	TArray<CD3DX12_TEXTURE_BARRIER>	TextureBarriers;
	TArray<CD3DX12_GLOBAL_BARRIER> GlobalBarriers;

	

public:
	void FlushCommands(ED3D12FlushFlags Flags = ED3D12FlushFlags::WaitForCompletion);



};



class FD3D12CommandContextBase : public IRHICommandContext, public FD3D12AdapterChild
{
public:
	FD3D12CommandContextBase(FD3D12Adapter* InParent);

	void RHIBeginDrawingViewport(FRHIViewport* Viewport, FRHITexture* RenderTargetRHI) override;
	void RHIEndDrawingViewport(FRHIViewport* Viewport, bool bPresent, int32 Vsync) override;

protected:
	uint64 DrawCallsNum = 0;
};

class FD3D12CommandContext : public FD3D12ContextCommon, public FD3D12CommandContextBase, public FD3D12DeviceChild
{
public:
	FD3D12CommandContext(FD3D12Device* InParent, ED3D12QueueType QueueType, bool InIsDefaultContext);

	FD3D12ConstantBuffer StageConstantBuffers[ST_NumStandartTypes];

	virtual void CloseCommandList() override;

	FD3D12StateCache StateCache;
	FD3D12UniformBuffer* BoundUniformBuffers[ST_NumStandartTypes][16] = {};
	uint16 DirtyUniformBuffers[ST_NumStandartTypes] = {};

	bool bDiscardSharedComputeConstants = false;
	bool bDiscardSharedGraphicsConstants = false;


	void CommitGraphicsResourceTables();
	void CommitComputeResourceTables();
	void CommitNonComputeShaderConstants();
	void CommitComputeShaderConstants();



	void RHIBeginFrame() override;
	void RHIEndFrame() override;
	void RHIBeginImGui() override;
	void RHIEndImGui() override;
	void RHIBeginRenderPass(struct FRHIRenderPassInfo& InInfo);
	void RHIEndRenderPass(struct FRHIRenderPassInfo& InInfo);
	void SetRenderTargets(int32 NumRenderTargets, const FRHIRenderTargetView* RenderTargetsRHI, const FRHIDepthRenderTargetView* DepthStencilViewRHI, bool bClear = false);
	void SetRenderTargetsAndClear(const FRHISetRenderTargetInfo& RenderTargetsInfo);
	/*void RHISetShaderParameter(FRHIGraphicsShader* ShaderRHI, uint32 BufferIndex, uint32 Offset,
		uint32 Size, const void* Data);*/
	void RHIClearTextureColor(FRHITexture* InTexture, FVector InColor);
	void RHISetShaderParameter(FRHIGraphicsShader* ShaderRHI, uint32 BufferIndex,
		uint32 Offset, uint32 NumBytes, const void* Data);

	void RHICopyTexture(FRHITexture* SourceTextureRHI, FRHITexture* DestTextureRHI);

	virtual void RHISetViewport(float MinX, float MinY, float MinZ, float MaxX, float MaxY, float MaxZ);

	virtual void RHIDrawIndexedPrimitive(FRHIBuffer* IndexBuffer, int32 BaseVertexIndex,
		uint32 FirstInstance, uint32 NumVertices, uint32 StartIndex, uint32 NumPrimitives, uint32 NumInstances);

	virtual void RHIDispatchComputeShader(uint32 ThreadGroupCountX, uint32 ThreadGroupCountY, uint32 ThreadGroupCountZ);

	virtual void RHISetShaderParameters(
		FRHIGraphicsShader* Shader,
		TArray<uint8>& InParameters,
		TArray<FRHIShaderParameterResource>& InBindlessParameters,
		TArray<FRHIShaderParameterResource>& InResourceParameters);

	virtual void RHISetShaderParameters(
		FRHIComputeShader* Shader,
		TArray<uint8>& InParameters,
		TArray<FRHIShaderParameterResource>& InBindlessParameters,
		TArray<FRHIShaderParameterResource>& InResourceParameters);



	//virtual void RHISetShaderUniformBuffer(FRHIGraphicsShader* Shader, uint32 BufferIndex, FRHIUn)

	virtual void RHISetStreamSource(uint32 StreamIndex, FRHIBuffer* VertexBuffer, uint32 Offset, uint32 Stride) override;
	virtual void RHISetStreamSource(uint32 StreamIndex, FRHIUniformBuffer* VertexBuffer, uint32 Offset, uint32 Stride) override;
	virtual void RHISetGraphicsPipelineState(class FRHIGraphicsPipelineState* GraphicsPSO, const FBoundShaderStateInput& ShaderInput) override;

	virtual void RHISetComputePipelineState(FRHIComputePipelineState* ComputeState);
	virtual void RHIPreparePixelShaderResources();

private:
	void SettingRenderPass();
	void BindUniformBuffer(FRHIShader* Shader, EShaderType ShaderType, uint32 BufferIndex, FD3D12UniformBuffer* InBuffer);


private:
	//uint64 DrawCallsNum = 0;
};

//class FScopedTransitionResource
//{
//public:
//	FScopedTransitionResource(FD3D12CommandContext& InContext, FD3D12Resource* InResource, D3D12_RESOURCE_STATES After, uint64 InSubresource) :
//		Context(InContext),
//		Resource(InResource),
//		Before(InResource->GetCurrentState()),
//		Subresource(InSubresource)
//	{
//		Context.TransitionResource(Resource, After, Subresource);
//	}
//	~FScopedTransitionResource()
//	{
//		Context.TransitionResource(Resource, Before, Subresource);
//	}
//
//
//private:
//	FD3D12CommandContext& Context;
//	D3D12_RESOURCE_STATES Before;
//	FD3D12Resource* Resource;
//	uint64 Subresource;
//};
