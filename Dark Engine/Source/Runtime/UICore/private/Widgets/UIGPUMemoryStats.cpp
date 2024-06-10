#include "Widgets/UIGPUMemoryStats.h"
#include "DynamicRHI.h"
#include "imgui.h"

UIGPUMemoryStats::UIGPUMemoryStats() : UIWidget(TEXT("UIGPUMemoryStats"))
{
	RHIGetStaticsMemoryToFString(Stats);
	Counter = 0;
}

void UIGPUMemoryStats::DrawImGui()
{
	if (ImGui::Begin(-Name, IsWidgetClose()))
	{
		ImGui::Text(!Stats);
		//ImGui::TextUnformatted(!Stats);
		ImGui::End();
	}
}


void UIGPUMemoryStats::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
	if (Counter > 100)
	{
		RHIGetStaticsMemoryToFString(Stats);
		Counter = 0;
	}

	++Counter;
}