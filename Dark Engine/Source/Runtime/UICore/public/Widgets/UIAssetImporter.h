#pragma once

#include "CoreMinimal.h"
#include "UIWidget.h"
#include "Containers/Array.h"
#include "Containers/DarkString.h"
#include "EngineResourceImporter.h"





class UIAssetImporter : public UIWidget
{
public:
	UIAssetImporter();
	virtual void Update(float DeltaTime);

	virtual void DrawImGui();


	void ImportAsset(const FString& Path);
	

private:
	FEngineResourceImporter AssetImporter;
	FString CurrentPath;


private:


};