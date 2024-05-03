#include "Widgets/UIEditorSettings.h"
#include "Widgets/UIEditorViewport.h"
#include "imspinner.h"
#include "Application/UIApplication.h"
#include "RHIResources.h"
#include "DynamicRHI.h"
#include "stb_image.h"
#include "Misc/Paths.h"
#include "Console/GlobalInputOutConsole.h"


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



UIEditorSettings::UIEditorSettings() : UIWidget(TEXT("UIEditorSettings"))
{
	FMemory::Memzero(TempSettings.PathTexture, 256);

	GGlobalConsole.RegisterConsoleCommand(TEXT("r.D3D12.Debug.Background"), [this](const TArray<FString>& Args)
		{
			if (Args.Num() > 0)
			{
				strcpy(TempSettings.PathTexture, Args[0].ToString().c_str());
				bChangeSettings = true;
			}
		});
}

void UIEditorSettings::DrawImGui()
{
	if (ImGui::Begin(-Name, IsWidgetClose(), ImGuiWindowFlags_Modal))
	{
		ImGui::Text("Viewport");

		ImGui::PushItemWidth(100);

		if (ImGui::DragInt2("Resolution", TempSettings.ResolutionViewport.XY, 5.f, 512, 16384, "%i"))
		{
			
		}
		ImGui::PopItemWidth();

		ImGui::Separator();



		ImGui::ColorEdit3("Background color", TempSettings.Color);
		//ImGui::ColorPicker3("Background color", TempSettings.Color);

		ImGui::Checkbox("Vsync", &TempSettings.bVsync);

		if (ImGui::InputText("Path", TempSettings.PathTexture, 256, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			CurrentSettings = TempSettings;
			bChangeSettings = true;
		}

		ImSpinner::SpinnerAtom("Test", 5, 3);
		
		ImVec2 CachePos = ImGui::GetCursorPos();

		if (ImGui::Button("Save", ImVec2(50, 20)))
		{
			CurrentSettings = TempSettings;
			bChangeSettings = true;
		}
		CachePos.x += 60;

		ImGui::SetCursorPos(CachePos);
		if (ImGui::Button("Discard", ImVec2(50, 20)))
		{
			TempSettings = CurrentSettings;
		}


		ImGui::End();
	}


}

void UIEditorSettings::Update(float DeltaTime)
{
	if (bChangeSettings)
	{
		bChangeSettings = false;
		if (EditorViewport)
		{
			EditorViewport->SetResolution(CurrentSettings.ResolutionViewport.X, CurrentSettings.ResolutionViewport.Y);
			EditorViewport->SetColorBackground(FVector(CurrentSettings.Color[0], CurrentSettings.Color[1],
				CurrentSettings.Color[2]));
			//EditorViewport->SetTexturePath(FString(CurrentSettings.PathTexture));
			SetTextureBackground(CurrentSettings.PathTexture);
			UIApplication::Get()->GetRenderer()->SetVsync(CurrentSettings.bVsync);
		}
	}
	UIWidget::Update(DeltaTime);
}

void UIEditorSettings::SetTextureBackground(FString Path)
{
	int32 W, H, Channels;

	uint8* img = stbi_load(-FString::PrintF(TEXT("%sTextures/%s"), *FPaths::EngineContentDir(), *Path),
		&W, &H, &Channels, 0);

	if (!img)
	{
		uint32 Stride;
		uint64 OutSize;

		FColor* Color = (FColor*)RHILockTexture2D(EditorViewport->GetSceneViewport()->GetRenderTargetTexture().Get(),
			0, RLM_WriteOnly,Stride, &OutSize);

		for (size_t i = 0; i < OutSize / sizeof(FColor); i++)
		{
			*(Color + i) = FColor(FMath::Rand() % 0xFF, FMath::Rand() % 0xFF, FMath::Rand() % 0xFF, 0xFF);
		}


		RHIUnlockTexture2D(EditorViewport->GetSceneViewport()->GetRenderTargetTexture().Get(), 0);
		return;
	}

	EditorViewport->SetResolution(W, H);


	uint32 Stride;
	uint64 OutSize;
	uint8* Color = (uint8*)RHILockTexture2D(EditorViewport->GetSceneViewport()->GetRenderTargetTexture().Get()
		, 0, RLM_WriteOnly,
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


	RHIUnlockTexture2D(EditorViewport->GetSceneViewport()->GetRenderTargetTexture().Get(), 0);

	stbi_image_free(img);


}
