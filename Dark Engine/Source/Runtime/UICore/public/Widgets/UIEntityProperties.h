#pragma once

#include "CoreMinimal.h"
#include "Rendering/RenderingCommon.h"
#include "UIWidget.h"
#include "Engine/World.h"





class UIEntityProperties : public UIWidget
{
public:
	UIEntityProperties() : UIWidget(TEXT("UIEntityProperties")) {}
	virtual void Update(float DeltaTime);

	virtual void DrawImGui();


private:
};