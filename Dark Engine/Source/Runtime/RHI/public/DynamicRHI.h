#pragma once
#include "RHIDefines.h"
#include <RHIResources.h>
#include "RHIContext.h"
#include "Memory/TUniquePtr.h"
//#include "Shader.h"




class IDynamicRHIModule
{
public:
	virtual class FDynamicRHI* CreateRHI() = 0;
	virtual void Shutdown() = 0;
};



/**
*	The base class of the Render Hardware Interface
*	@see FD3D12DynamicRHI
*/
class FDynamicRHI
{
public:
	virtual void Init() = 0;
	virtual void PostInit() {}
	virtual void Shutdown() = 0;
	virtual void GetAvailableDynamicSuperResolutions(TArray<FRHIDynamicSuperResolution>& OutArray) {}
	virtual TSharedPtr<FRHIViewport> RHICreateViewport(void* WindowHandle, uint32 SizeX, uint32 SizeY, bool bIsFullscreen) = 0;
	virtual FRHITexture* RHIGetViewportBackBuffer(FRHIViewport* Viewport) = 0;
	virtual IRHIComputeContext* RHIGetCommandContext() = 0;
	virtual IRHIComputeContext* RHIGetDefaultContext() = 0;
	virtual TRefCountPtr<FRHIBuffer> RHICreateBuffer(const FRHIBufferDesc& CreateDesc, const TCHAR* DebugName, ERHIAccess InitialState) = 0;
	virtual TRefCountPtr<FRHITexture> RHICreateTexture(const FRHITextureCreateDesc& CreateDesc) = 0;
	virtual void* RHILockTexture2D(FRHITexture* TextureRHI, uint32 MipIndex, EResourceLockMode LockMode, uint32& DestStride, bool bLockWithinMiptail, uint64* OutLockedByteCount) = 0;
	virtual TRefCountPtr<FRHIUniformBuffer>	RHICreateUniformBuffer(const void* Contents, uint32 Size, EUniformBufferUsage Usage) = 0;
	virtual void RHIUpdateUniformBuffer(FRHIUniformBuffer* BufferRHI, const void* Contents, uint32 Size) = 0;
	virtual void* RHILockBuffer(FRHIBuffer* BufferRHI, uint32 Offset, uint32 Size, EResourceLockMode LockMode) = 0;
	virtual void  RHIUnlockBuffer(FRHIBuffer* BufferRHI) = 0;
	virtual TRefCountPtr<FRHIGraphicsPipelineState> RHICreateGraphicsPipelineState(const class FGraphicsPipelineStateInitializer& Initializer) = 0;
	virtual TRefCountPtr<FRHIVertexDeclaration>	RHICreateVertexDeclaration(const FVertexDeclarationElementList& Elements) = 0;
	virtual TRefCountPtr<FRHIVertexShader> RHICreateVertexShader(const TArray<uint8>& Code, const struct FShaderBounds& Bounds) = 0;
	virtual TRefCountPtr<FRHIPixelShader> RHICreatePixelShader(const TArray<uint8>& Code, const struct FShaderBounds& Bounds) = 0;
	virtual TRefCountPtr<FRHIComputeShader>	RHICreateComputeShader(const TArray<uint8>& Code, const struct FShaderBounds& Bounds) = 0;
	virtual TRefCountPtr<FRHIRasterizerState> RHICreateRasterizerState(const FRasterizerStateInitializer& Initializer) = 0;
	virtual TRefCountPtr<FRHIComputePipelineState>	RHICreateComputePipelineState(class FRHIComputeShader* InComputeShader) = 0;
	virtual void GetStaticsMemoryToFString(FString& OutString) = 0;

	virtual void RHIUnlockTexture2D(FRHITexture* TextureRHI, uint32 MipIndex, bool bLockWithinMiptail) = 0;
	virtual ERHIInterfaceType GetInterfaceType() const { return ERHIInterfaceType::Hidden; }

};

extern FDynamicRHI* GDynamicRHI;


class IDynamicRHI
{
public:
	virtual FDynamicRHI* CreateRHI() = 0;
};


FDynamicRHI* PlatformCreateDynamicRHI();

extern void DynamicRHIShutdown();



/**
*	Create FRHIViewport
*	@param[in] WindowHandle OS handle on window
*	@param[in] SizeX Width Viewport
*	@param[in] SizeY Heigth Viewport
*	@param[in] Fullscreen On
*/
TSharedPtr<FRHIViewport> CreateRHIViewport(void* WindowHandle, uint32 SizeX, uint32 SizeY, bool bIsFullscreen);


/**
*	Get BackBuffer Texture
*	@param[in] Viewport Viewport
*	@return FRHITexture back buffer
*/
FORCEINLINE FRHITexture* RHIGetViewportBackBuffer(FRHIViewport* Viewport)
{
	return GDynamicRHI->RHIGetViewportBackBuffer(Viewport);
}


/**
* Create FRHIBuffer
* @param[in] CreateDesc BufferDesc
* @param[in] DebugName Debug name for debugger
* @param[in] InitialState Initial RHI state
* @return RefCount object FRHIBuffer
*/

FORCEINLINE TRefCountPtr<FRHIBuffer> RHICreateBuffer(const FRHIBufferDesc& CreateDesc, const TCHAR* DebugName, ERHIAccess InitialState)
{
	return GDynamicRHI->RHICreateBuffer(CreateDesc, DebugName, InitialState);
}

/**
* Create FRHITexture
* @param[in] CreateDesc Texture create description
* @return RefCount object FRHITexture
*/
FORCEINLINE TRefCountPtr<FRHITexture> RHICreateTexture(const FRHITextureCreateDesc& CreateDesc)
{
	//FRHICommandListImmediate& RHICmdList = FRHICommandListExecutor::GetImmediateCommandList();

	return GDynamicRHI->RHICreateTexture(CreateDesc);
}

FORCEINLINE TRefCountPtr<FRHIRasterizerState> RHICreateRasterizerState(const FRasterizerStateInitializer& Initializer)
{
	return GDynamicRHI->RHICreateRasterizerState(Initializer);
}

FORCEINLINE void RHIGetStaticsMemoryToFString(FString& Out)
{
	GDynamicRHI->GetStaticsMemoryToFString(Out);
}



FORCEINLINE TRefCountPtr<FRHIUniformBuffer>	RHICreateUniformBuffer(const void* Contents, uint32 Size, EUniformBufferUsage Usage)
{
	return GDynamicRHI->RHICreateUniformBuffer(Contents, Size, Usage);
}

FORCEINLINE void RHIUpdateUniformBuffer(FRHIUniformBuffer* BufferRHI, const void* Contents, uint32 Size)
{
	GDynamicRHI->RHIUpdateUniformBuffer(BufferRHI, Contents, Size);
}




FORCEINLINE void* RHILockTexture2D(FRHITexture* TextureRHI, uint32 MipIndex, EResourceLockMode LockMode,
	uint32& DestStride, uint64* OutLockedByteCount, bool bLockWithinMiptail = false)
{
	return GDynamicRHI->RHILockTexture2D(TextureRHI, MipIndex, LockMode, DestStride, bLockWithinMiptail, OutLockedByteCount);
}

FORCEINLINE void RHIUnlockTexture2D(FRHITexture* TextureRHI, uint32 MipIndex, bool bLockWithinMiptail = false)
{
	GDynamicRHI->RHIUnlockTexture2D(TextureRHI, MipIndex, bLockWithinMiptail);
}
FORCEINLINE void* RHILockBuffer(FRHIBuffer* BufferRHI, uint32 Offset, uint32 Size, EResourceLockMode LockMode)
{
	return GDynamicRHI->RHILockBuffer(BufferRHI, Offset, Size, LockMode);
}
FORCEINLINE void  RHIUnlockBuffer(FRHIBuffer* BufferRHI)
{
	GDynamicRHI->RHIUnlockBuffer(BufferRHI);
}


FORCEINLINE TRefCountPtr<FRHIGraphicsPipelineState> RHICreateGraphicsPipelineState(const class FGraphicsPipelineStateInitializer& Initializer)
{
	return GDynamicRHI->RHICreateGraphicsPipelineState(Initializer);
}

FORCEINLINE TRefCountPtr<FRHIComputePipelineState>	RHICreateComputePipelineState(class FRHIComputeShader* InComputeShader)
{
	return GDynamicRHI->RHICreateComputePipelineState(InComputeShader);
}

FORCEINLINE TRefCountPtr<FRHIVertexShader> RHICreateVertexShader(const TArray<uint8>& Code, const struct FShaderBounds& Bounds)
{
	return GDynamicRHI->RHICreateVertexShader(Code, Bounds);
}

FORCEINLINE TRefCountPtr<FRHIComputeShader> RHICreateComputeShader(const TArray<uint8>& Code, const struct FShaderBounds& Bounds)
{
	return GDynamicRHI->RHICreateComputeShader(Code, Bounds);
}


FORCEINLINE TRefCountPtr<FRHIVertexDeclaration>RHICreateVertexDeclaration(const FVertexDeclarationElementList& Elements)
{
	return GDynamicRHI->RHICreateVertexDeclaration(Elements);
}



FORCEINLINE TRefCountPtr<FRHIPixelShader> RHICreatePixelShader(const TArray<uint8>& Code, const struct FShaderBounds& Bounds)
{
	return GDynamicRHI->RHICreatePixelShader(Code, Bounds);
}


