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



class FDynamicRHI
{
public:
	virtual void Init() = 0;
	virtual void PostInit() {}
	virtual void Shutdown() = 0;
	virtual TSharedPtr<FRHIViewport> RHICreateViewport(void* WindowHandle, uint32 SizeX, uint32 SizeY, bool bIsFullscreen) = 0;
	virtual FRHITexture* RHIGetViewportBackBuffer(FRHIViewport* Viewport) = 0;
	virtual IRHIComputeContext* RHIGetCommandContext() = 0;
	virtual IRHIComputeContext* RHIGetDefaultContext() = 0;
	virtual TRefCountPtr<FRHIBuffer> CreateBuffer(const FRHIBufferDesc& CreateDesc, const TCHAR* DebugName, ERHIAcces InitialState) = 0;
	virtual TRefCountPtr<FRHITexture> RHICreateTexture(const FRHITextureCreateDesc& CreateDesc) = 0;
	virtual void* RHILockTexture2D(FRHITexture* TextureRHI, uint32 MipIndex, EResourceLockMode LockMode, uint32& DestStride, bool bLockWithinMiptail, uint64* OutLockedByteCount) = 0;
	virtual TRefCountPtr<FRHIGraphicsPipelineState> RHICreateGraphicsPipelineState(const class FGraphicsPipelineStateInitializer& Initializer) = 0;
	virtual TRefCountPtr<FRHIVertexDeclaration>	RHICreateVertexDeclaration(const FVertexDeclarationElementList& Elements) = 0;
	virtual TRefCountPtr<FRHIVertexShader> RHICreateVertexShader(const TArray<uint8>& Code, const struct FShaderBounds& Bounds) = 0;
	virtual TRefCountPtr<FRHIPixelShader> RHICreatePixelShader(const TArray<uint8>& Code, const struct FShaderBounds& Bounds) = 0;

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


TSharedPtr<FRHIViewport> CreateRHIViewport(void* WindowHandle, uint32 SizeX, uint32 SizeY, bool bIsFullscreen);
FORCEINLINE FRHITexture* RHIGetViewportBackBuffer(FRHIViewport* Viewport)
{
	return GDynamicRHI->RHIGetViewportBackBuffer(Viewport);
}

FORCEINLINE TRefCountPtr<FRHITexture> RHICreateTexture(const FRHITextureCreateDesc& CreateDesc)
{
	//FRHICommandListImmediate& RHICmdList = FRHICommandListExecutor::GetImmediateCommandList();

	return GDynamicRHI->RHICreateTexture(CreateDesc);
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

FORCEINLINE TRefCountPtr<FRHIGraphicsPipelineState> RHICreateGraphicsPipelineState(const class FGraphicsPipelineStateInitializer& Initializer)
{
	return GDynamicRHI->RHICreateGraphicsPipelineState(Initializer);
}

FORCEINLINE TRefCountPtr<FRHIVertexShader> RHICreateVertexShader(const TArray<uint8>& Code, const struct FShaderBounds& Bounds)
{
	return GDynamicRHI->RHICreateVertexShader(Code, Bounds);
}

FORCEINLINE TRefCountPtr<FRHIVertexDeclaration>RHICreateVertexDeclaration(const FVertexDeclarationElementList& Elements)
{
	return GDynamicRHI->RHICreateVertexDeclaration(Elements);
}



FORCEINLINE TRefCountPtr<FRHIPixelShader> RHICreatePixelShader(const TArray<uint8>& Code, const struct FShaderBounds& Bounds)
{
	return GDynamicRHI->RHICreatePixelShader(Code, Bounds);
}


