#pragma once

#include "Widgets/UIWidget.h"
#include "Math/IntPoint.h"


class UIEditorViewport;


class UIEditorSettings : public UIWidget
{
public:
	UIEditorSettings() : UIWidget(TEXT("UIEditorSettings")) {}

public:
	void SetEditorViewport(TSharedPtr<UIEditorViewport> InEditorViewport)
	{
		EditorViewport = InEditorViewport;
	}
	void DrawImGui();
	void Update(float DeltaTime);

protected:
	struct
	{
		FIntPoint ResolutionViewport = FIntPoint(1280, 720);
		float Color[3] = {0, 0, 0};
		bool bVsync = true;
	} CurrentSettings, TempSettings;


	bool bChangeSettings = false;
private:
	TSharedPtr<UIEditorViewport> EditorViewport;



};
