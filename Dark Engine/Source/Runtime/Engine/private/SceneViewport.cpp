#include "Engine/SceneViewport.h"
#include "DynamicRHI.h"



void FSceneViewport::Resize(int32 InSizeX, int32 InSizeY)
{
	SetSize(InSizeX, InSizeY);
	InitRHI();
}

void FSceneViewport::InitRHI()
{
	RTTSize = FIntPoint(0, 0);

	BufferedShaderResourceTextureRHI.Reset();
	BufferedRenderTargetRHI.Reset();
	RenderTargetTextureRHI.Reset();





	TRefCountPtr<FRHITexture> RTRHI, SRVRHI;
	
	const FRHITextureCreateDesc Desc =
		FRHITextureCreateDesc::Create2D(TEXT("BufferRT"))
		.SetExtent(GetSize())
		.SetFormat(EPixelFormat::PF_R8G8B8A8_UNORM)
		.SetFlags(ETextureCreateFlags::RenderTargetable | ETextureCreateFlags::ShaderResource)
		.SetInitialState(ERHIAcces::SRVGraphics);

	SRVRHI = RTRHI = RHICreateTexture(Desc);
	check(SRVRHI && RTRHI);

	BufferedRenderTargetRHI = RTRHI;
	BufferedShaderResourceTextureRHI = SRVRHI;
	RenderTargetTextureRHI = BufferedShaderResourceTextureRHI;
}
