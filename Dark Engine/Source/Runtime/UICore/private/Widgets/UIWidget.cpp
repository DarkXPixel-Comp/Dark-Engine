#include "Widgets/UIWidget.h"
#include "imgui.h"
#include "Widgets/UIWindow.h"
#include "stb_image.h"
#include <DynamicRHI.h>

TUnordoredMap<FString, UIWidget*> UIWidget::AllWidgets;
uint64 UIWidget::CounterID = 0;

ImVec4 FVectorToImVec4(const FVector& InVector)
{
	return ImVec4(InVector.X, InVector.Y, InVector.Z, 1);
}

const ImVec4 FVector3fToImVec4(const FVector3f& InVector)
{
	return ImVec4(InVector.X, InVector.Y, InVector.Z, 1);
}


void UIWidget::CloseWidget()
{
	AllWidgets.erase(Name);

	if(Owner)
		Owner->DefferedRemoveChild(this);
	//Window->RemoveWidget(this);
	Window = nullptr;
	Owner = nullptr;
}

struct FColor	//<-- Temp
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

void UIWidget::FastLoadIcon(TRefCountPtr<FRHITexture>& InTexture, FString Path)
{
	int32 W, H, Channels;

	uint8* img = stbi_load(-Path,
		&W, &H, &Channels, 0);

	if (!img)
	{
		return;
	}


	const FRHITextureCreateDesc Desc =
		FRHITextureCreateDesc::Create2D(TEXT("FastIconUI"))
		.SetExtent(FIntPoint(W, H))
		.SetFormat(EPixelFormat::PF_R8G8B8A8_UNORM)
		.SetFlags(ETextureCreateFlags::ShaderResource)
		.SetInitialState(ERHIAccess::SRVGraphics);

	InTexture = RHICreateTexture(Desc);
	check(InTexture);

	uint32 Stride;
	uint64 OutSize;
	uint8* Color = (uint8*)RHILockTexture2D(InTexture.Get(), 0, RLM_WriteOnly,
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
			Colors[Index] = { *p, *(p + 1), *(p + 2), *(p + 3) };
		}
		++Index;
	}



	for (int32 Y = 0; Y < H; ++Y)
	{
		FMemory::Memcpy(Color + Stride * Y, ((uint8*)Colors.GetData()) + ((W * 4) * Y), W * 4);
	}

	RHIUnlockTexture2D(InTexture.Get(), 0);
}


void UIWidget::Update(float DeltaTime)
{
	for (auto& i : DefferedRemoved)
	{
		ChildWidgets.RemovePtr(i);
	}
	DefferedRemoved.Empty();

	ForEachChild([DeltaTime](UIWidget* Widget)
		{
			Widget->Update(DeltaTime);
		});

	if (bHaveCloseButton && !bOpen)
	{
		CloseWidget();
	}
}

void UIWidget::ForEachChild(std::function<void(UIWidget*)> Func)
{
	for (const auto& Widget : ChildWidgets)
	{
		Func(Widget.get());
	}
}
