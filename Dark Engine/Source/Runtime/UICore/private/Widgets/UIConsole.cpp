#include "Widgets/UIConsole.h"
#include "Console/GlobalInputOutConsole.h"
#include "imgui.h"


UIConsole::UIConsole():
	UIWidget(TEXT("UIConsole"))
{
	GGlobalConsole.OnAddConsoleMessage.Bind(this, &UIConsole::OnLogAdd);
}

int InputCallback(ImGuiInputTextCallbackData* data)
{
	UIConsole* CurrentConsole = (UIConsole*)data->UserData;
	if (data->EventKey == ImGuiKey_UpArrow && CurrentConsole->CurrentIndex > 0)
	{
		CurrentConsole->CurrentIndex--;
		if (CurrentConsole->History.IsValidIndex(CurrentConsole->CurrentIndex))
		{
			strcpy(data->Buf, !CurrentConsole->History[CurrentConsole->CurrentIndex]);
			//data->Buf = CurrentConsole->History[CurrentConsole->CurrentIndex].ToString().data();
			data->BufTextLen = lstrlenW(*CurrentConsole->History[CurrentConsole->CurrentIndex]);
			data->BufDirty = true;
			//data->BufSize = CurrentConsole->History[CurrentConsole->CurrentIndex].Len();
			data->CursorPos = data->BufTextLen;
		}

	}
	if (data->EventKey == ImGuiKey_DownArrow && CurrentConsole->CurrentIndex < CurrentConsole->History.Num())
	{
		CurrentConsole->CurrentIndex++;
		if (CurrentConsole->History.IsValidIndex(CurrentConsole->CurrentIndex))
		{
			strcpy(data->Buf, !CurrentConsole->History[CurrentConsole->CurrentIndex]);
			//data->Buf = CurrentConsole->History[CurrentConsole->CurrentIndex].ToString().data();
			data->BufTextLen = lstrlenW(*CurrentConsole->History[CurrentConsole->CurrentIndex]);
			data->BufDirty = true;
			data->CursorPos = data->BufTextLen;
			//data->BufSize = CurrentConsole->History[CurrentConsole->CurrentIndex].Len();
		}
	}
	else if (data->EventKey == ImGuiKey_DownArrow)
	{
		data->BufTextLen = 0;
		data->CursorPos = 0;
		data->BufDirty = true;
		strcpy(data->Buf, "");
	}

	return 0;
}

void UIConsole::DrawImGui()
{
	if (ImGui::Begin(!Name, IsWidgetClose(), ImGuiWindowFlags_HorizontalScrollbar))
	{
		const TArray<FConsoleLog>& Logs = GGlobalConsole.GetLogs();

		for (const auto& Log : Logs)
		{
			ImGui::TextColored(ImVec4(Log.Color.X, Log.Color.Y, Log.Color.Z, 1), Log.Log.ToString().c_str());
		}

		if (ImGui::InputText("Input", InputText, 256, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_AlwaysOverwrite, InputCallback, this))
		{
			GGlobalConsole.InputText(InputText);
			++ScrollToBottom;
			History.Add(InputText);
			CurrentIndex = History.Num();
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
