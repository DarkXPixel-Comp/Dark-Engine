#include "Widgets/UIConsole.h"
#include "Console/GlobalInputOutConsole.h"
#include "imgui.h"


UIConsole::UIConsole():
	UIWidget(TEXT("UIConsole"))
{
	GGlobalConsole.OnAddConsoleMessage.Bind(this, &UIConsole::OnLogAdd);
}

int32 InputCallback(ImGuiInputTextCallbackData* data)
{
	UIConsole* CurrentConsole = (UIConsole*)data->UserData;

	if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit)
	{
		CurrentConsole->CompletionState = 0;
		CurrentConsole->CompletionDirty = true;
		CurrentConsole->CompletionBuf.Empty();
	}
	
	if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion)
	{
		if (CurrentConsole->CompletionDirty)
		{
			CurrentConsole->CompletionBuf = GGlobalConsole.InputText(data->Buf, true);
			CurrentConsole->CompletionDirty = false;
		}

		if (CurrentConsole->CompletionState < CurrentConsole->CompletionBuf.Num())
		{
			FString& Str = CurrentConsole->CompletionBuf[CurrentConsole->CompletionState++];
			strcpy(data->Buf, !Str);
			data->BufTextLen = Str.Len();
			data->BufDirty = true;
			data->CursorPos = data->BufTextLen;
		}
		if (CurrentConsole->CompletionState >= CurrentConsole->CompletionBuf.Num())
		{
			CurrentConsole->CompletionState = 0;
		}

	}
	if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory)
	{
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
				strcpy(data->Buf, CurrentConsole->History[CurrentConsole->CurrentIndex].GetUTF8());
				//data->Buf = CurrentConsole->History[CurrentConsole->CurrentIndex].ToString().data();
				data->BufTextLen = lstrlenW(*CurrentConsole->History[CurrentConsole->CurrentIndex]);
				data->BufDirty = true;
				data->CursorPos = data->BufTextLen;
				data->BufSize = CurrentConsole->History[CurrentConsole->CurrentIndex].Len();
			}
		}
		else if (data->EventKey == ImGuiKey_DownArrow)
		{
			data->BufTextLen = 0;
			data->CursorPos = 0;
			data->BufDirty = true;
			strcpy(data->Buf, "");
		}
	}

	return 0;
}

void UIConsole::DrawImGui()
{
	if (ImGui::Begin(!Name, IsWidgetClose(), ImGuiWindowFlags_NoScrollbar))
	{
		const TArray<FConsoleLog>& Logs = GGlobalConsole.GetLogs();

		ImGui::BeginChild("ScrollRegion", ImVec2(0, -ImGui::GetTextLineHeightWithSpacing()),
			ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
		for (const auto& Log : Logs)
		{
			ImGui::TextColored(ImVec4(Log.Color.X, Log.Color.Y, Log.Color.Z, 1), Log.Log.GetUTF8());
			//ImGui::LogText(Log.Log.GetUTF8());
			//ImGui::TextWrapped(Log.Log.GetUTF8());

		}

		if (ScrollToBottom)
		{
			ImGui::SetScrollHereY(1.f);
			--ScrollToBottom;
		}

		ImGui::EndChild();

		if (ImGui::InputText("Input", InputText, 256, ImGuiInputTextFlags_EnterReturnsTrue |
			ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackEdit | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackEdit
			, InputCallback, this))
		{
			ImGui::SetKeyboardFocusHere(-1);
			GGlobalConsole.InputText(InputText);
			++ScrollToBottom;
			if (strcmp(InputText, "") != 0)
			{
				History.Add(InputText);
				CurrentIndex = History.Num();
			}
			FMemory::Memzero(InputText, 256);
		}

		ImGui::End();
	}


}

void UIConsole::OnLogAdd(FConsoleLog InLog)
{
	++ScrollToBottom;
}
