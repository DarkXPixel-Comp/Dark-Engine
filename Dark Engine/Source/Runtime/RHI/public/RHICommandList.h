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

	void SetStreamSource(uint32 StreamIndex, FRHIBuffer* VertexBuffer, uint32 Offset, uint32 Stride)
	{
		GetContext().RHISetStreamSource(StreamIndex, VertexBuffer, Offset, Stride);
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