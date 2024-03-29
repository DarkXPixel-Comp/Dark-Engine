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
	int32 W, H, Channels;

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
}

void UIMainMenuBar::DrawImGui()
{
	bool bIsMaximized = Owner->GetNativeWindow()->IsMaximized();

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

	Owner->GetNativeWindow()->bTitleBarHovarered = ImGui::IsItemHovered();


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
		Owner->GetNativeWindow()->bTitleBarHovarered = false;
	}

	ImGui::ResumeLayout();



	ImVec2 CurrentCursorPos = ImGui::GetCursorPos();
	ImVec2 TextSize = ImGui::CalcTextSize(-Title);
	ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() * 0.5f - TextSize.x * 0.5f, 2.f + WindowPadding.y + 6.f));
	ImGui::Text(-Title);
	ImGui::SetCursorPos(CurrentCursorPos);

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
	InMenu->Owner = Owner;
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

