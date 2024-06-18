#pragma once

#include "CoreMinimal.h"
#include "Rendering/RenderingCommon.h"
#include "UIWidget.h"
#include "Engine/World.h"





class UIWorldSettings : public UIWidget
{
public:
	UIWorldSettings();
	virtual void Update(float DeltaTime);

	virtual void DrawImGui();


private:
};