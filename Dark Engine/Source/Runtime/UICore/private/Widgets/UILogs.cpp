#include "Widgets/UILogs.h"
#include "imgui.h"
#include <Logger.h>

UILogs::UILogs()
{
	
}

void UILogs::DrawImGui()
{
	if (ImGui::Begin(-Name))
	{
		auto Logs = Logger::GetLogs();
		for (uint32 i = 0; i < Logger::GetMaxCountLogs(); ++i)
		{
			ImGui::Text(-Logs[i].Result);
		}
		if (LastLogsSize != Logger::GetCurrentCountLogs())
		{
			ScrollToBottom = true;
		}

		if (ScrollToBottom)
		{
			ImGui::SetScrollHereY(1.f);
		}	
		ScrollToBottom = false;
		LastLogsSize = Logger::GetCurrentCountLogs();
		
		ImGui::End();
	}
}

void UILogs::OnLogAdd()
{
	ScrollToBottom = true;
}
