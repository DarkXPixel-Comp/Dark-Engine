#pragma once
#include "RHIResources.h"
#include "DynamicRHI.h"
#include "SceneView.h"




struct FSceneTextures
{
	TRefCountPtr<FRHITexture> Color;
	TRefCountPtr<FRHITexture> Depth;
	TRefCountPtr<FRHITexture> GBufferA;
	TRefCountPtr<FRHITexture> GBufferB;
	TRefCountPtr<FRHITexture> GBufferC;
	TRefCountPtr<FRHITexture> GBufferD;
	TRefCountPtr<FRHITexture> GBufferE;
	TRefCountPtr<FRHITexture> GBufferF;

	FORCEINLINE void Initialize(FSceneView* View)
	{
		if (!Color || Color->GetSize() != View->UnscaledRect.RectToSize())
		{
			FRHITextureCreateDesc Desc = FRHITextureCreateDesc::Create2D(TEXT("SceneColor")).
				SetExtent(View->RenderTarget->GetSizeXY()).
				SetFlags(ETextureCreateFlags::RenderTargetable | ETextureCreateFlags::ShaderResource).
				SetFormat(View->RenderTarget->GetRenderTargetTexture()->GetPixelFormat());

			Color = RHICreateTexture(Desc);
		}
		/*if (!Depth || Depth->GetSize() != View->UnscaledRect.RectToSize())
		{
			FRHITextureCreateDesc Desc = FRHITextureCreateDesc::Create2D(TEXT("SceneDepth")).
				SetExtent(View->RenderTarget->GetSizeXY()).
				SetFlags(ETextureCreateFlags::RenderTargetable | ETextureCreateFlags::ShaderResource).
				SetFormat(View->RenderTarget->GetRenderTargetTexture()->GetPixelFormat());

			Depth = RHICreateTexture(Desc);
		}*/
		if (!GBufferA || GBufferA->GetSize() != View->UnscaledRect.RectToSize())
		{
			FRHITextureCreateDesc Desc = FRHITextureCreateDesc::Create2D(TEXT("GBufferA")).
				SetExtent(View->RenderTarget->GetSizeXY()).
				SetFlags( ETextureCreateFlags::ShaderResource | ETextureCreateFlags::UAV).
				SetFormat(View->RenderTarget->GetRenderTargetTexture()->GetPixelFormat());

			GBufferA = RHICreateTexture(Desc);
		}
		if (!GBufferB || Color->GetSize() != View->UnscaledRect.RectToSize())
		{
			FRHITextureCreateDesc Desc = FRHITextureCreateDesc::Create2D(TEXT("GBufferB")).
				SetExtent(View->RenderTarget->GetSizeXY()).
				SetFlags(ETextureCreateFlags::RenderTargetable | ETextureCreateFlags::ShaderResource | ETextureCreateFlags::UAV).
				SetFormat(View->RenderTarget->GetRenderTargetTexture()->GetPixelFormat());

			GBufferB = RHICreateTexture(Desc);
		}
		if (!GBufferC || Color->GetSize() != View->UnscaledRect.RectToSize())
		{
			FRHITextureCreateDesc Desc = FRHITextureCreateDesc::Create2D(TEXT("GBufferC")).
				SetExtent(View->RenderTarget->GetSizeXY()).
				SetFlags(ETextureCreateFlags::RenderTargetable | ETextureCreateFlags::ShaderResource | ETextureCreateFlags::UAV).
				SetFormat(View->RenderTarget->GetRenderTargetTexture()->GetPixelFormat());

			GBufferC = RHICreateTexture(Desc);
		}
		if (!GBufferD || Color->GetSize() != View->UnscaledRect.RectToSize())
		{
			FRHITextureCreateDesc Desc = FRHITextureCreateDesc::Create2D(TEXT("GBufferD")).
				SetExtent(View->RenderTarget->GetSizeXY()).
				SetFlags(ETextureCreateFlags::RenderTargetable | ETextureCreateFlags::ShaderResource | ETextureCreateFlags::UAV).
				SetFormat(View->RenderTarget->GetRenderTargetTexture()->GetPixelFormat());

			GBufferD = RHICreateTexture(Desc);
		}
		if (!GBufferE || Color->GetSize() != View->UnscaledRect.RectToSize())
		{
			FRHITextureCreateDesc Desc = FRHITextureCreateDesc::Create2D(TEXT("GBufferE")).
				SetExtent(View->RenderTarget->GetSizeXY()).
				SetFlags(ETextureCreateFlags::RenderTargetable | ETextureCreateFlags::ShaderResource | ETextureCreateFlags::UAV).
				SetFormat(View->RenderTarget->GetRenderTargetTexture()->GetPixelFormat());

			GBufferE = RHICreateTexture(Desc);
		}
		if (!GBufferD || Color->GetSize() != View->UnscaledRect.RectToSize())
		{
			FRHITextureCreateDesc Desc = FRHITextureCreateDesc::Create2D(TEXT("GBufferD")).
				SetExtent(View->RenderTarget->GetSizeXY()).
				SetFlags(ETextureCreateFlags::RenderTargetable | ETextureCreateFlags::ShaderResource | ETextureCreateFlags::UAV).
				SetFormat(View->RenderTarget->GetRenderTargetTexture()->GetPixelFormat());

			GBufferD = RHICreateTexture(Desc);
		}
		if (!GBufferF || Color->GetSize() != View->UnscaledRect.RectToSize())
		{
			FRHITextureCreateDesc Desc = FRHITextureCreateDesc::Create2D(TEXT("GBufferF")).
				SetExtent(View->RenderTarget->GetSizeXY()).
				SetFlags(ETextureCreateFlags::RenderTargetable | ETextureCreateFlags::ShaderResource | ETextureCreateFlags::UAV).
				SetFormat(View->RenderTarget->GetRenderTargetTexture()->GetPixelFormat());

			GBufferF = RHICreateTexture(Desc);
		}
	}

};