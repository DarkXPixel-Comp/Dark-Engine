#pragma once

#include "CoreMinimal.h"
#include "Rendering/RenderingCommon.h"
#include "UIWidget.h"
#include "Engine/World.h"





class UIWorldOutliner : public UIWidget
{
public:
	UIWorldOutliner();
	virtual void Update(float DeltaTime);

	virtual void DrawImGui();


private:
	int32 Selected = -1;

};