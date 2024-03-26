#include "Widgets/UIEditorSettings.h"
#include "Widgets/UIEditorViewport.h"
#include "imspinner.h"
#include "Application/UIApplication.h"

void UIEditorSettings::DrawImGui()
{
	if (ImGui::Begin(-Name, nullptr, ImGuiWindowFlags_Modal))
	{
		ImGui::Text("Viewport");

		ImGui::PushItemWidth(100);

		if (ImGui::DragInt2("Resolution", TempSettings.ResolutionViewport.XY, 5.f, 512, 16384, "%i"))
		{
		}
		ImGui::PopItemWidth();

		ImGui::Separator();

		

		ImGui::ColorEdit3("Background color", TempSettings.Color);
		//ImGui::ColorPicker3("Background color", TempSettings.Color);
	   
		ImGui::Checkbox("Vsync", &TempSettings.bVsync);

		ImSpinner::SpinnerAtom("Test", 5, 3);
		
		ImVec2 CachePos = ImGui::GetCursorPos();

		if (ImGui::Button("Save", ImVec2(50, 20)))
		{
			CurrentSettings = TempSettings;
			bChangeSettings = true;
		}
		CachePos.x += 60;

		ImGui::SetCursorPos(CachePos);
		if (ImGui::Button("Discard", ImVec2(50, 20)))
		{
			TempSettings = CurrentSettings;
		}


		ImGui::End();
	}


}

void UIEditorSettings::Update(float DeltaTime)
{
	if (bChangeSettings)
	{
		bChangeSettings = false;
		if (EditorViewport)
		{
			EditorViewport->SetResolution(CurrentSettings.ResolutionViewport.X, CurrentSettings.ResolutionViewport.Y);
			EditorViewport->SetColorBackground(FVector(CurrentSettings.Color[0], CurrentSettings.Color[1],
				CurrentSettings.Color[2]));
			UIApplication::Get()->GetRenderer()->SetVsync(CurrentSettings.bVsync);
		}
	}
}
