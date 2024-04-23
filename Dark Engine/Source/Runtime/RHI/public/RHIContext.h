#pragma once
#include "RHIResources.h"
#include "Containers/Array.h"


class IRHIComputeContext
{
public:
	virtual void RHISetComputePipelineState(FRHIComputePipelineState* ComputePipelineState) {}
	
	virtual void RHIDispatchComputeShader(uint32 ThreadGroupCountX, uint32 ThreadGroupCountY, uint32 ThreadGroupCountZ) {}

	virtual void RHIDispatchIndirectComputeShader(FRHIBuffer* InBuffer, uint32 ArgumentOffset) {}

	//virtual void RHIClearUAVFloat(FRHIUn)







};





class IRHICommandContext : public IRHIComputeContext
{
public:
	virtual void RHIBeginDrawingViewport(FRHIViewport* Viewport, FRHITexture* RenderTarget) = 0;
	virtual void RHIEndDrawingViewport(FRHIViewport* Viewport, bool bPresent, int32 Vsync) = 0;
	virtual void RHIBeginFrame() = 0;
	virtual void RHIBeginRenderPass(struct FRHIRenderPassInfo& InInfo) = 0;
	virtual void RHIEndRenderPass(struct FRHIRenderPassInfo& InInfo) = 0;
	virtual void RHIEndFrame() = 0;
	virtual void BeginScene() {}
	virtual void EndScene() {}
	//virtual void RHISetStreamSource(uint32 StreamIndex, FRHIBuffer* VertexBuffer, uint32 Offset) = 0;
	virtual void RHISetViewport(float MinX, float MinY, float MinZ, float MaxX, float MaxY, float MaxZ) = 0;
	virtual void RHISetScissorRect(bool bEnable, uint32 MinX, uint32 MinY, uint32 MaxX, uint32 MaxY) {}
	virtual void RHISetGraphicsPipelineState(class FRHIGraphicsPipelineState* GraphicsPSO, const FBoundShaderStateInput& ShaderInput) = 0;
	virtual void RHISetShaderTexture(FRHIGraphicsShader* Shader, uint32 TextureIndex, FRHITexture* Texture) {}
	virtual void RHISetShaderSampler(FRHIGraphicsShader* Shader, uint32 SamplerIndex, FRHISamplerState* State) {}
	virtual void RHIBeginImGui() = 0;
	virtual void RHIEndImGui() = 0;
	virtual void RHIClearTextureColor(FRHITexture* InTexture, FVector InColor) = 0;
	virtual void RHISetStreamSource(uint32 StreamIndex, FRHIBuffer* VertexBuffer, uint32 Offset, uint32 Stride) = 0;
	virtual void RHIDrawIndexedPrimitive(FRHIBuffer* IndexBufferRHI, int32 BaseVertexIndex, uint32 FirstInstance, uint32 NumVertices, uint32 StartIndex, uint32 NumPrimitives, uint32 NumInstances) = 0;

};