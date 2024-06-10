#pragma once
#include "UIWidget.h"
#include "Containers/DarkString.h"
#include "Containers/Array.h"



class UIGPUMemoryStats : public UIWidget
{
public:
	UIGPUMemoryStats();

	virtual void DrawImGui() override;
	virtual void Update(float DeltaTime) override;
private:
	FString Stats;

	uint64 Counter = 0;
};