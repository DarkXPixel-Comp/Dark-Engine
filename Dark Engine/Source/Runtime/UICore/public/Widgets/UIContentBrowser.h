#pragma once

#include "CoreMinimal.h"
#include "UIWidget.h"





class UIContentBrowser : public UIWidget
{
public:
	UIContentBrowser();
	virtual void Update(float DeltaTime);

	virtual void DrawImGui();

private:
	void ImportAsset();

private:
	TRefCountPtr<class FRHITexture>	GreenPlusTexture;

};