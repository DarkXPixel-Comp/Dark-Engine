#include "Widgets/UIAssetImporter.h"
#include "imgui.h"

UIAssetImporter::UIAssetImporter() :UIWidget(TEXT("UIAssetImporter"))
{
	bHaveCloseButton = true;

}

void UIAssetImporter::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
}

void UIAssetImporter::DrawImGui()
{
	if (!CurrentPath.Len())
	{
		return;
	}
	if (ImGui::Begin(!Name, IsWidgetClose(), ImGuiWindowFlags_NoDocking))
	{


		/*auto Task = AssetImporter.ImportObjects(Path);
	if (!Task.valid())
	{
		DE_LOG(UICoreLog, Log, TEXT("Extension is not supported"));
		return;
	}

	Task.wait();*/


	}

	ImGui::End();

}

void UIAssetImporter::ImportAsset(const FString& Path)
{
	if (Path == TEXT(""))
	{
		return;
	}
	DE_LOG(UICoreLog, Log, TEXT("%s"), *Path);

	CurrentPath = Path;
}



