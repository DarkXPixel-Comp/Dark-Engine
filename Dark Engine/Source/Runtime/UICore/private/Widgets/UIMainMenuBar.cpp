#include "Widgets/UIMainMenuBar.h"
#include "Widgets/UIWindow.h"
#include "Math/MathFwd.h"
#include "imgui.h"
#include "Application/UIApplication.h"
#include "imgui_internal.h"
#include "CoreGlobals.h"
#include "imdark_extension.h"
#include "Widgets/UIMenu.h"
#include "RHIResources.h"
#include "DynamicRHI.h"


#include "stb_image.h"
#include "Misc/Paths.h"










const uint8 g_WindowCloseIcon[] =
{
0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x0e, 0x08, 0x06, 0x00, 0x00, 0x00, 0x1f, 0x48, 0x2d,
0xd1, 0x00, 0x00, 0x00, 0x77, 0x49, 0x44, 0x41, 0x54, 0x28, 0x91, 0xad, 0xd2, 0xdd, 0x0a, 0x80,
0x20, 0x0c, 0x86, 0xe1, 0xd7, 0xb0, 0xbb, 0x0e, 0x9a, 0xd0, 0x65, 0x1b, 0x0b, 0x49, 0x43, 0xfc,
0x2b, 0xaa, 0x81, 0x07, 0x7e, 0xdb, 0x33, 0x3c, 0x90, 0xb7, 0x65, 0x82, 0x53, 0xd5, 0x0d, 0xf0,
0x80, 0xdc, 0xec, 0x09, 0x7d, 0x6b, 0x8c, 0x59, 0xa6, 0x18, 0xec, 0xc0, 0x7a, 0x03, 0x25, 0xce,
0xf8, 0x2b, 0x51, 0xd5, 0x70, 0x9c, 0x9e, 0x25, 0xf1, 0x9e, 0x1f, 0xc9, 0x7b, 0x25, 0xec, 0x61,
0x29, 0xb3, 0x16, 0x2c, 0x07, 0x2b, 0x34, 0x82, 0x39, 0xae, 0x50, 0x82, 0x13, 0x5f, 0xea, 0x8f,
0xa7, 0xb6, 0x06, 0x53, 0xb6, 0xf6, 0x60, 0x73, 0x7b, 0x0b, 0xe7, 0x70, 0x84, 0x2a, 0x1c, 0x8c,
0x8d, 0x76, 0x06, 0xdc, 0x83, 0x9f, 0x93, 0x66, 0x5f, 0x16, 0x70, 0x00, 0xf8, 0x25, 0x49, 0x6a,
0x1b, 0xd2, 0x84, 0x22, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

const uint8 g_WindowMinimizeIcon[] =
{
0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x02, 0x08, 0x06, 0x00, 0x00, 0x00, 0x68, 0x8a, 0xed,
0xaa, 0x00, 0x00, 0x00, 0x17, 0x49, 0x44, 0x41, 0x54, 0x08, 0x99, 0x63, 0xfc, 0xff, 0xff, 0xff,
0x7f, 0x06, 0x32, 0x00, 0x13, 0x39, 0x9a, 0x18, 0x18, 0x18, 0x18, 0x00, 0xbd, 0xa7, 0x04, 0x00,
0x48, 0xa5, 0x4e, 0xd9, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

const uint8 g_WindowMaximizeIcon[] =
{
0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x0e, 0x08, 0x06, 0x00, 0x00, 0x00, 0x1f, 0x48, 0x2d,
0xd1, 0x00, 0x00, 0x00, 0x2b, 0x49, 0x44, 0x41, 0x54, 0x28, 0x91, 0x63, 0xfc, 0xff, 0xff, 0xff,
0x7f, 0x06, 0x32, 0x00, 0x0b, 0x54, 0x0b, 0x23, 0x89, 0x5a, 0xff, 0x33, 0x91, 0x63, 0x1b, 0x08,
0x8c, 0x6a, 0x1c, 0xd5, 0x48, 0x1e, 0x80, 0xa5, 0x55, 0xd2, 0x12, 0x3a, 0x03, 0x03, 0x03, 0x00,
0x75, 0x70, 0x06, 0x1b, 0xf7, 0xa3, 0xe2, 0x95, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44,
0xae, 0x42, 0x60, 0x82
};


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


UIMainMenuBar::UIMainMenuBar() : UIWidget(TEXT("UIMainMenuBar"))  
{
	FastLoadIcon(IconTextureRHI, FString::PrintF(TEXT("%sImages/Logo.png"),
		*FPaths::EngineContentDir()));
	FastLoadIcon(MaximizeTextureRHI, (uint8*)g_WindowMaximizeIcon,
		sizeof(g_WindowMaximizeIcon));
	FastLoadIcon(MinimizeTextureRHI, FString::PrintF(TEXT("%sImages/Logo.png"),
		*FPaths::EngineContentDir()));
	FastLoadIcon(CloseTextureRHI, (uint8*)g_WindowCloseIcon,
		sizeof(g_WindowCloseIcon));

	/*int32 W, H, Channels;

	uint8* img = stbi_load(-FString::PrintF(TEXT("%sImages/Logo.png"), *FPaths::EngineContentDir()),
		&W, &H, &Channels, 0);

	if (!img)
	{
		return;
	}


	const FRHITextureCreateDesc Desc =
		FRHITextureCreateDesc::Create2D(TEXT("IconTexture"))
		.SetExtent(FIntPoint(W, H))
		.SetFormat(EPixelFormat::PF_R8G8B8A8_UNORM)
		.SetFlags(ETextureCreateFlags::ShaderResource)
		.SetInitialState(ERHIAcces::SRVGraphics);

	IconTextureRHI = RHICreateTexture(Desc);
	check(IconTextureRHI);

	uint32 Stride;
	uint64 OutSize;
	uint8* Color = (uint8*)RHILockTexture2D(IconTextureRHI.Get(), 0, RLM_WriteOnly,
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

	RHIUnlockTexture2D(IconTextureRHI.Get(), 0);
*/


}

void UIMainMenuBar::DrawImGui()
{
	bool bIsMaximized = Window->GetNativeWindow()->IsMaximized();

	const float TitleBarHeight = 58.f;
	float TitleBarVerticalOffset = bIsMaximized ? -6.f : 0.f;
	const ImVec2 WindowPadding = ImGui::GetCurrentWindow()->WindowPadding;

	ImGui::SetCursorPos(ImVec2(WindowPadding.x, WindowPadding.y + TitleBarVerticalOffset));
	const ImVec2 TitleBarMin = ImGui::GetCursorScreenPos();
	const ImVec2 TitleBarMax = { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - WindowPadding.y * 2.0f,
								ImGui::GetCursorScreenPos().y + TitleBarHeight };
	auto* BgDrawList = ImGui::GetBackgroundDrawList();
	auto* FgDrawList = ImGui::GetForegroundDrawList();
	BgDrawList->AddRectFilled(TitleBarMin, TitleBarMax, ImGui::GetColorU32(ImVec4(0, 0, 0, 1)));

	if (IconTextureRHI)
	{
		const int32 LogoWidth = 60;
		const int32 LogoHeight = 60;
		const ImVec2 LogoOffset(0.0f + WindowPadding.x, 0.f + WindowPadding.y + TitleBarVerticalOffset);
		const ImVec2 LogoRectStart = { ImGui::GetItemRectMin().x + LogoOffset.x, ImGui::GetItemRectMin().y + LogoOffset.y };
		const ImVec2 LogoRectMax = { LogoRectStart.x + LogoWidth, LogoRectStart.y + LogoHeight };
		FgDrawList->AddImage(IconTextureRHI->GetNativeShaderResourceView(), LogoRectStart, LogoRectMax);

	}



	ImGui::BeginHorizontal("TitleBar", { ImGui::GetWindowWidth() - WindowPadding.y * 2.f, ImGui::GetFrameHeightWithSpacing() });

	const float W = ImGui::GetContentRegionAvail().x;
	const float ButtonAreaWidth = 94;

	ImGui::SetCursorPos(ImVec2(WindowPadding.x, WindowPadding.y + TitleBarVerticalOffset));
	ImGui::SetNextItemAllowOverlap();
	//ImGui::Button("##TitleBarDragZone", ImVec2(W - ButtonAreaWidth, TitleBarHeight));
	ImGui::InvisibleButton("##TitleBarDragZone", ImVec2(W - ButtonAreaWidth, TitleBarHeight));

	Window->GetNativeWindow()->bTitleBarHovarered = ImGui::IsItemHovered();


	ImGui::SuspendLayout();



	const float LogoHorizontalOffset = 16.f * 2.f + 48.f + WindowPadding.x;
	ImGui::SetCursorPos(ImVec2(LogoHorizontalOffset, 6.f + TitleBarVerticalOffset));

	const ImRect MenuBarRect = { ImGui::GetCursorPos(), {ImGui::GetContentRegionAvail().x +
	ImGui::GetCursorScreenPos().x, ImGui::GetFrameHeightWithSpacing()} };

	/*if (ImGui::Button("tt"))
	{
		GIsRequestingExit = true;
	}*/

	ImGui::BeginGroup();
	if (ImDark::BeginMenubar(MenuBarRect))
	{
		for (auto& Menu : MenuItems)
		{
			Menu->DrawImGui();
		}
	}
	ImDark::EndMenubar();
	ImGui::EndGroup();

	if (ImGui::IsItemHovered())
	{
		Window->GetNativeWindow()->bTitleBarHovarered = false;
	}

	ImGui::ResumeLayout();



	ImVec2 CurrentCursorPos = ImGui::GetCursorPos();
	ImVec2 TextSize = ImGui::CalcTextSize(!Title);
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - TextSize.x * 0.5f, 2.f + WindowPadding.y + 6.f));
	ImGui::Text(!Title);
	ImGui::SetCursorPos(CurrentCursorPos);


	const ImU32 ButtonCol = ImGui::GetColorU32(ImVec4(0, 0, 0, 0.5));
	const float buttonWidth = 14.0f;
	const float buttonHeight = 14.0f;
	
	/*ImGui::Spring();
	ImDark::ShiftCursorY(8.f);
	{
		const int iconWidth = MinimizeTextureRHI->GetSize().X;
		const int iconHeight = MinimizeTextureRHI->GetSize().X;
		const float padY = (buttonHeight - (float)iconHeight) / 2.0f;
		if (ImGui::InvisibleButton("Minimize", ImVec2(buttonWidth, buttonHeight)))
		{

		}

		ImDark::DrawButtonImage(MinimizeTextureRHI->GetNativeShaderResourceView(), ButtonCol, ButtonCol, ButtonCol, ImDark::RectExpanded(ImDark::GetItemRect(), 0.0f, -padY));
	}*/




	ImGui::EndHorizontal();


	ImGui::SetCursorPosY(TitleBarHeight);


}


void UIMainMenuBar::Update(float DeltaTime)
{	
	for (auto& Menu : MenuItems)
	{
		Menu->Update(DeltaTime);

	}
	UIWidget::Update(DeltaTime);
}

void UIMainMenuBar::AddMenu(const TSharedPtr<class UIMenu>& InMenu)
{
	MenuItems.Add(InMenu);
	InMenu->Window = Window;
}

void UIMainMenuBar::DrawMenuBar()
{
	const ImRect MenuBarRect = { ImGui::GetCursorPos(), {ImGui::GetContentRegionAvail().x +
		ImGui::GetCursorScreenPos().x, ImGui::GetFrameHeightWithSpacing()} };

	ImGui::BeginGroup();
	if (ImDark::BeginMenubar(MenuBarRect))
	{
		if (ImGui::BeginMenu("Test"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				GIsRequestingExit = true;
			}
			ImGui::EndMenu();
		}

		//for (auto& Menu : MenuItems)
		//{
		//	Menu->DrawImGui();
		//}
	}



	ImDark::EndMenubar();
	ImGui::EndGroup();

}

void UIMainMenuBar::FastLoadIcon(TRefCountPtr<FRHITexture>& InTexture, FString Path)
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
		.SetInitialState(ERHIAcces::SRVGraphics);

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

void UIMainMenuBar::FastLoadIcon(TRefCountPtr<FRHITexture>& InTexture, uint8* Bytes, uint64 SizeBytes)
{
	int32 W, H, Channels;

	/*uint8* img = stbi_load(-FString::PrintF(TEXT("%sImages/Logo.png"), *FPaths::EngineContentDir()),
		&W, &H, &Channels, 0);*/
	
	uint8* img = stbi_load_from_memory(Bytes, SizeBytes, &W, &H, &Channels, 0);

	if (!img)
	{
		return;
	}


	const FRHITextureCreateDesc Desc =
		FRHITextureCreateDesc::Create2D(TEXT("IconTexture"))
		.SetExtent(FIntPoint(W, H))
		.SetFormat(EPixelFormat::PF_R8G8B8A8_UNORM)
		.SetFlags(ETextureCreateFlags::ShaderResource)
		.SetInitialState(ERHIAcces::SRVGraphics);

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





