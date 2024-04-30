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



class FD3D12Device;


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
	void TransitionResource(FD3D12Resource* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After, uint32 SubResource);
	void TransitionResource(ID3D12Resource* Resource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After, uint32 SubResource);
	void TransitionResource(FD3D12Resource* Resource, D3D12_RESOURCE_STATES After, uint32 SubResource);

	FD3D12CommandList& GetCommandList()
	{
		if (!CommandList)
		{
			OpenCommandList();
		}
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
	void RHIEndDrawingViewport(FRHIViewport* Viewport, bool bPresent, int32 Vsync) override;
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


	void CommitGraphicsResourceTables();
	void CommitNonComputeShaderConstants();


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

	virtual void RHISetViewport(float MinX, float MinY, float MinZ, float MaxX, float MaxY, float MaxZ);

	virtual void RHIDrawIndexedPrimitive(FRHIBuffer* IndexBuffer, int32 BaseVertexIndex,
		uint32 FirstInstance, uint32 NumVertices, uint32 StartIndex, uint32 NumPrimitives, uint32 NumInstances);

	virtual void RHISetShaderParameters(
		FRHIGraphicsShader* Shader,
		TArray<uint8>& InParameters,
		TArray<FRHIShaderParameterResource>& InBindlessParameters,
		TArray<FRHIShaderParameterResource>& InResourceParameters);

	//virtual void RHISetShaderUniformBuffer(FRHIGraphicsShader* Shader, uint32 BufferIndex, FRHIUn)

	virtual void RHISetStreamSource(uint32 StreamIndex, FRHIBuffer* VertexBuffer, uint32 Offset, uint32 Stride) override;
	virtual void RHISetGraphicsPipelineState(class FRHIGraphicsPipelineState* GraphicsPSO, const FBoundShaderStateInput& ShaderInput) override;

private:
	void SettingRenderPass();
	void BindUniformBuffer(FRHIShader* Shader, EShaderType ShaderType, uint32 BufferIndex, FD3D12UniformBuffer* InBuffer);
};