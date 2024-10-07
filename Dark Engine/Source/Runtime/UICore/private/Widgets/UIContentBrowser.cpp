#include "Widgets/UIContentBrowser.h"
#include "imgui.h"
#include "Misc/Paths.h"
#include "RHIResources.h"
#include "nfd.h"
#include "Widgets/UIWindow.h"
#include "Widgets/UIAssetImporter.h"
UIContentBrowser::UIContentBrowser() : UIWidget(TEXT("UIContentBrowser"))
{
	FastLoadIcon(GreenPlusTexture, FString::PrintF(TEXT("%sImages/green_plus.png"),
		*FPaths::EngineContentDir()));

}

void UIContentBrowser::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
}

void UIContentBrowser::DrawImGui()
{
	if (ImGui::Begin(!Name, IsWidgetClose()))
	{
		if (ImGui::ImageButton(GreenPlusTexture->GetNativeShaderResourceView(), ImVec2(32, 32)))
		{
			ImportAsset();
		}


	}

	ImGui::End();
}

void UIContentBrowser::ImportAsset()
{
	FString Path = GetFile();	

	TSharedPtr<UIAssetImporter>	 AssetImporter = MakeShareble(new UIAssetImporter());
	Window->AddDefferedWidgetWithParent(AssetImporter, Window->GetMainWidget().get());
	AssetImporter->ImportAsset(Path);
}

FString UIContentBrowser::GetFile()
{
	nfdchar_t* OutPath = NULL;
	nfdresult_t Result = NFD_OpenDialog(NULL, NULL, &OutPath);
	if (Result != NFD_OKAY)
	{
		return TEXT("");
	}

	FString Path = OutPath;
	free(OutPath);
	return Path;
}
