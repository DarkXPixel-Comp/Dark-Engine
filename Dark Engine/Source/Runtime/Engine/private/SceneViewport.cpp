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

	TRefCountPtr<FRHITexture> RTRHI;
	
	const FRHITextureCreateDesc Desc =
		FRHITextureCreateDesc::Create2D(TEXT("BufferRT"))
		.SetExtent(GetSize())
		.SetFormat(EPixelFormat::PF_R8G8B8A8)
		.SetFlags(ETextureCreateFlags::RenderTargetable | ETextureCreateFlags::ShaderResource)
		.SetInitialState(ERHIAcces::SRVGraphics);

	RTRHI = RHICreateTexture(Desc);
	




}
