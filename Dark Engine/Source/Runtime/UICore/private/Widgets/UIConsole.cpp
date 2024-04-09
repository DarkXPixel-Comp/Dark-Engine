#include "Widgets/UIConsole.h"
#include "Console/GlobalInputOutConsole.h"
#include "imgui.h"


UIConsole::UIConsole():
	UIWidget(TEXT("UIConsole"))
{
	GGlobalConsole.OnAddConsoleMessage.Bind(this, &UIConsole::OnLogAdd);
}

void UIConsole::DrawImGui()
{
	if (ImGui::Begin(!Name, IsWidgetClose()))
	{
		const TArray<FConsoleLog>& Logs = GGlobalConsole.GetLogs();

		for (const auto& Log : Logs)
		{
			ImGui::TextColored(ImVec4(Log.Color.X, Log.Color.Y, Log.Color.Y, 1), Log.Log.ToString().c_str());
		}

		if (ImGui::InputText("Input", InputText, 256, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			GGlobalConsole.InputText(InputText);
			++ScrollToBottom;
			FMemory::Memzero(InputText, 256);
		}

		if (ScrollToBottom)
		{
			ImGui::SetScrollHereY(1.f);
			--ScrollToBottom;
		}

		ImGui::End();
	}


}

void UIConsole::OnLogAdd(FConsoleLog InLog)
{
	++ScrollToBottom;
}
