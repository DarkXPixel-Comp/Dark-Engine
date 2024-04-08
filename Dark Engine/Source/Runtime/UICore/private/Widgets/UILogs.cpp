#include "Widgets/UILogs.h"
#include "imgui.h"
#include <Logger.h>

UILogs::UILogs():
	UIWidget(TEXT("UILogs"))
{
	
}

void UILogs::DrawImGui()
{
	if (ImGui::Begin(-Name, IsWidgetClose()))
	{
		//auto& Logs = Logger::GetLogs();
		std::vector<log_>& Logs = Logger::GetLogs();
		for (uint32 i = 0; i < Logger::GetMaxCountLogs(); ++i)
		{
			ImGui::TextColored(FVectorToImVec4(Logs[i].Color), -Logs[i].Result);
			//ImGui::Text(-Logs[i].Result);
		}
		if (LastLogsSize != Logger::GetCurrentCountLogs())
		{
			ScrollToBottom = true;
		}
		ImGui::InputText("Input", InputText, 256, 0);

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
