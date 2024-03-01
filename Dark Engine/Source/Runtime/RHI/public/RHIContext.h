#pragma once
#include "RHIResources.h"
#include "Containers/Array.h"


class IRHIComputeContext
{
public:
	virtual void RHISetComputePipelineState(FRHIComputePipelineState* ComputePipelineState) = 0;
	
	virtual void RHIDispatchComputeShader(uint32 ThreadGroupCountX, uint32 ThreadGroupCountY, uint32 ThreadGroupCountZ) = 0;

	virtual void RHIDispatchIndirectComputeShader(FRHIBuffer* InBuffer, uint32 ArgumentOffset);

	//virtual void RHIClearUAVFloat(FRHIUn)







};





class IRHICommandContext : public IRHIComputeContext
{
public:
	virtual void RHIBeginDrawingViewport(FRHIViewport* Viewport, FRHITexture* RenderTarget) = 0;
	virtual void RHIEndDrawingViewport(FRHIViewport* Viewport, bool bPresent, bool Vsync) = 0;
	virtual void RHIBeginFrame() = 0;
	virtual void RHIENdFrame() = 0;
	virtual void BeginScene() = 0;
	virtual void EndScene() = 0;
	virtual void RHISetStreamSource(uint32 StreamIndex, FRHIBuffer* VertexBuffer) = 0;
	virtual void RHISetViewport(float MinX, float MinY, float MinZ, float MaxX, float MaxY, float MaxZ) = 0;
	virtual void RHISetScissorRect(bool bEnable, uint32 MinX, uint32 MinY, uint32 MaxX, uint32 MaxY) = 0;
	virtual void RHISetGraphicsPipelineState(FRHIGraphicsPipelineState* GraphicsPSO, uint32 Stencil) = 0;
	virtual void RHISetShaderTexture(FRHIGraphicsShader* Shader, uint32 TextureIndex, FRHITexture* Texture) = 0;
	virtual void RHISetShaderSampler(FRHIGraphicsShader* Shader, uint32 SamplerIndex, FRHISamplerState* State) = 0;

	







};