#include "Engine/SceneViewport.h"
#include "DynamicRHI.h"

#include "Misc/Paths.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



				
struct FColor
{
	FColor(uint8 InX, uint8 InY, uint8 InZ, uint8 InW) : x(InX), y(InY), z(InZ), w(InW)
	{}
	FColor()
	{}
	uint8 x = 0;
	uint8 y = 0;
	uint8 z = 0;
	uint8 w = 0xFF;
};

struct FColor3
{
	FColor3(uint8 InX, uint8 InY, uint8 InZ, uint8 InW) : x(InX), y(InY), z(InZ)
	{}
	FColor3()
	{}
	uint8 x = 0;
	uint8 y = 0;
	uint8 z = 0;
};




void FSceneViewport::Resize(int32 InSizeX, int32 InSizeY)
{
	SetSize(InSizeX, InSizeY);
	InitRHI();
}

void FSceneViewport::SetTexturePath(FString Path)
{
}


void FSceneViewport::InitRHI()
{
	RTTSize = FIntPoint(0, 0);

	BufferedShaderResourceTextureRHI.Reset();
	BufferedRenderTargetRHI.Reset();
	RenderTargetTextureRHI.Reset();


	/*int32 W, H, Channels;

	uint8* img = stbi_load(-FString::PrintF(TEXT("%sTextures/mem.jpg"), *FPaths::EngineContentDir()), &W, &H, &Channels, 0);


	SetSize(W, H);*/




	//TRefCountPtr<FRHITexture> RTRHI, SRVRHI;
	//
	//const FRHITextureCreateDesc Desc =
	//	FRHITextureCreateDesc::Create2D(TEXT("BufferRT"))
	//	.SetExtent(GetSize())
	//	.SetFormat(EPixelFormat::PF_R8G8B8A8_UNORM)
	//	.SetFlags(ETextureCreateFlags::RenderTargetable | ETextureCreateFlags::ShaderResource | ETextureCreateFlags::DepthStencilTargetable)
	//	.SetInitialState(ERHIAccess::SRVGraphics);

	//SRVRHI = RTRHI = RHICreateTexture(Desc);
	//check(SRVRHI && RTRHI);

	//BufferedRenderTargetRHI = RTRHI;
	//BufferedShaderResourceTextureRHI = SRVRHI;
	//RenderTargetTextureRHI = BufferedShaderResourceTextureRHI;


}
