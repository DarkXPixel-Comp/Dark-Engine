#pragma once

#include "CoreMinimal.h"
#include "UIWidget.h"






class UIEntityAdder : public UIWidget
{
public:
	UIEntityAdder();
	virtual void Update(float DeltaTime);

	virtual void DrawImGui();

private:
	class GClass* CreateClass = nullptr;


};