#pragma once

#include "Widgets/UIWidget.h"
#include "Math/IntPoint.h"


class UIEditorViewport;


class UIEditorSettings : public UIWidget
{
public:
	UIEditorSettings() : UIWidget(TEXT("UIEditorSettings"))
	{
		FMemory::Memzero(TempSettings.PathTexture, 256);
	}

public:
	void SetEditorViewport(TSharedPtr<UIEditorViewport> InEditorViewport)
	{
		EditorViewport = InEditorViewport;
	}
	void SetTexturePath(FString Path);

	void DrawImGui();
	void Update(float DeltaTime);

protected:
	struct
	{
		FIntPoint ResolutionViewport = FIntPoint(1280, 720);
		float Color[3] = {0, 0, 0};
		bool bVsync = true;
		char PathTexture[256];
	} CurrentSettings, TempSettings;


	bool bChangeSettings = false;
private:
	TSharedPtr<UIEditorViewport> EditorViewport;



};
