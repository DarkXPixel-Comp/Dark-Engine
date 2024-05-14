#pragma once
#include "Templates/DarkTemplate.h"
#include "CoreTypes.h"
#include "RHIResources.h"
#include "RHIContext.h"
#include "DynamicRHI.h"

class FGraphEventRef;

class FRHICommandListBase : public FNoncopyble
{
public:
	enum class ERecordingThread
	{
		Render,
		Any
	};

protected:
	FRHICommandListBase() {}

public:
	FRHICommandListBase(FRHICommandListBase&& Other) {}
	~FRHICommandListBase() {}

	bool IsImmediate();
	class FRHICommandListImmediate& GetAsImmediate();
	const int32 GetUsedMemory() const;


	void SwitchPipeline()
	{
		IRHIComputeContext* Context = GDynamicRHI->RHIGetDefaultContext();
		GraphicsContext = static_cast<IRHICommandContext*>(Context);
	}

	void AddDispatchPrerequisite(const FGraphEventRef& InPrereq);

	void FinishRecording();

	void* Alloc(int64 InAlocSize, int64 InAllignment);

	IRHICommandContext& GetContext()
	{
		return *GraphicsContext;
	}



private:
	IRHICommandContext* GraphicsContext = nullptr;


};

class FRHIComputeCommandList : public FRHICommandListBase
{
public:
	FRHIComputeCommandList()
	{

	}

protected:

};


class FRHICommandList : public FRHIComputeCommandList
{
public:
	FRHICommandList()
	{

	}



	void RHISetViewport(float MinX, float MinY, float MinZ, float MaxX, float MaxY, float MaxZ)
	{
		GetContext().RHISetViewport(MinX, MinY, MinZ, MaxX, MaxY, MaxZ);
	}


	void SetGraphicsPipelineState(class FRHIGraphicsPipelineState* PipelineState, class FBoundShaderStateInput& ShaderInput)
	{
		GetContext().RHISetGraphicsPipelineState(PipelineState, ShaderInput);
	}

	void SetComputePipelineState(FRHIComputePipelineState* ComputeState)
	{
		GetContext().RHISetComputePipelineState(ComputeState);
	}

	void SetStreamSource(uint32 StreamIndex, FRHIBuffer* VertexBuffer, uint32 Offset, uint32 Stride)
	{
		GetContext().RHISetStreamSource(StreamIndex, VertexBuffer, Offset, Stride);
	}

	void DrawIndexedPrimitive(FRHIBuffer* IndexBufferRHI, int32 BaseVertexIndex, uint32 FirstInstance, uint32 NumVertices, uint32 StartIndex, uint32 NumPrimitives, uint32 NumInstances)
	{
		GetContext().RHIDrawIndexedPrimitive(IndexBufferRHI, BaseVertexIndex, FirstInstance, NumVertices, StartIndex, NumPrimitives, NumInstances);
	}

	void DispatchComputeShader(uint32 ThreadGroupCountX, uint32 ThreadGroupCountY, uint32 ThreadGroupCountZ)
	{
		GetContext().RHIDispatchComputeShader(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
	}

};





class FRHICommandListImmediate : public FRHICommandList
{
	friend class FRHICommandListExecutor;

	FRHICommandListImmediate()
	{

	}
	

	void ExecuteAndReset();
	void Reset();


public:
	void BeginScene();
	void EndScene();
	void BeginRenderPass(struct FRHIRenderPassInfo& InInfo)
	{
		GetContext().RHIBeginRenderPass(InInfo);
	}
	void EndRenderPass(struct FRHIRenderPassInfo& InInfo)
	{
		GetContext().RHIEndRenderPass(InInfo);
	}
	void BeginDrawingViewport(FRHIViewport* Viewport, FRHITexture* RenderTargetRHI)
	{
		GetContext().RHIBeginDrawingViewport(Viewport, RenderTargetRHI);
	}
	void EndDrawingViewport(FRHIViewport* Viewport, bool bPresent, int32 bVsync)
	{
		GetContext().RHIEndDrawingViewport(Viewport, bPresent, bVsync);
	}
	void BeginFrame();
	void EndFrame()
	{
		GetContext().RHIEndFrame();
	}
	void BeginImGui()
	{
		GetContext().RHIBeginImGui();
	}
	void EndImGui()
	{
		GetContext().RHIEndImGui();
	}
	void InitializeContexts();

	void ClearTextureColor(FRHITexture* InTexture, FVector InColor)
	{
		GetContext().RHIClearTextureColor(InTexture, InColor);
	}

	void SetShaderParameters(
		FRHIGraphicsShader* Shader,
		TArray<uint8>& InParameters,
		TArray<FRHIShaderParameterResource>& InBindlessParameters,
		TArray<FRHIShaderParameterResource>& InResourceParameters)
	{
		GetContext().RHISetShaderParameters(Shader, InParameters, InBindlessParameters, InResourceParameters);
	}

	void SetShaderParameters(
		FRHIComputeShader* Shader,
		TArray<uint8> InParameters,
		TArray<FRHIShaderParameterResource>& InBindlessParameters,
		TArray<FRHIShaderParameterResource>& InResourceParameters)
	{
		GetContext().RHISetShaderParameters(Shader, InParameters, InBindlessParameters, InResourceParameters);
	}


	void SetRenderTargets(int32 NumRenderTargets,
		const FRHIRenderTargetView* RenderTargetsRHI,
		const FRHIDepthRenderTargetView* DepthStencilViewRHI,
		bool bClear = false)
	{
		GetContext().SetRenderTargets(NumRenderTargets, RenderTargetsRHI, DepthStencilViewRHI, bClear);
	}
};




class FRHICommandListExecutor
{
public:
	FRHICommandListExecutor()
		: bLatchedByPass(false)
	{

	}

	static FRHICommandListImmediate& GetImmediateCommandList();
	void LatchByPass() {}




private:
	FRHICommandListImmediate CommandListImmediate;
	bool bLatchedByPass;


};

extern FRHICommandListExecutor GRHICommandList;