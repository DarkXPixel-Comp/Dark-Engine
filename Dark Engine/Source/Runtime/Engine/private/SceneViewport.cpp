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
	int32 W, H, Channels;

	uint8* img = stbi_load(-FString::PrintF(TEXT("%sTextures/%s"), *FPaths::EngineContentDir(), *Path),
		&W, &H, &Channels, 0);

	if (!img)
	{
		uint32 Stride;
		uint64 OutSize;
		FColor* Color = (FColor*)RHILockTexture2D(GetRenderTargetTexture().Get(), 0, RLM_WriteOnly,
			Stride, &OutSize);

		for (size_t i = 0; i < OutSize / sizeof(FColor); i++)
		{
			*(Color + i) = FColor(FMath::Rand() % 0xFF, FMath::Rand() % 0xFF, FMath::Rand() % 0xFF, 0xFF);
		}


		RHIUnlockTexture2D(GetRenderTargetTexture().Get(), 0);
		return;
	}

	Resize(W, H);


	uint32 Stride;
	uint64 OutSize;
	uint8* Color = (uint8*)RHILockTexture2D(GetRenderTargetTexture().Get(), 0, RLM_WriteOnly,
		Stride, &OutSize);

	TArray<FColor> Colors(H * W);


	uint32 Index = 0;
	for (uint8* p = img; p != img + (H * W * Channels); p += Channels)
	{
		if (Channels == 3)
		{
			Colors[Index] = { *p, *(p + 1), *(p + 2), 0xFF };
		}
		else if (Channels == 4)
		{
			Colors[Index] = { *p, *(p + 1), *(p + 2), *(p + 3)};
		}
		++Index;
	}



	for (int32 Y = 0; Y < H; ++Y)
	{
		FMemory::Memcpy(Color + Stride * Y, ((uint8*)Colors.GetData()) + ((W * 4) * Y), W * 4);
	}


	//for (size_t i = 0; i < OutSize / sizeof(FColor); i++)
	//{
	//	//*(Color + i) = FColor(FMath::Rand() % 0xFF, FMath::Rand() % 0xFF, FMath::Rand() % 0xFF, 0xFF);
	//	uint8* CurCol = (img + i * Channels);
	//	if (i >= W * H)
	//	{
	//		break;
	//	}

	//	*(Color + i) = FColor(*(CurCol + 0), *(CurCol + 1), *(CurCol + 2), 0xFF);

	//}


	RHIUnlockTexture2D(GetRenderTargetTexture().Get(), 0);

	stbi_image_free(img);

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
