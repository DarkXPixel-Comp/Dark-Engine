#include "Widgets/UIWorldOutliner.h"
#include "Class.h"
#include "imgui.h"
#include "CoreGlobals.h"


UIWorldOutliner::UIWorldOutliner() : UIWidget(TEXT("World Outliner"))
{


}


void UIWorldOutliner::Update(float DeltaTime)
{
	GCurrentEntity = Selected;
}

void UIWorldOutliner::DrawImGui()
{
	if (ImGui::Begin(-Name, IsWidgetClose()))
	{
		const auto& Entities = GWorld->GetEntitiesOnCurrentLevel();
		
		static bool IsOpen = false;

		if (ImGui::TreeNode("BasicLevel"))
		{
			for (int32 i = 0; i < Entities.Num(); i++)
			{
				FString Name = FString::PrintF(TEXT("%s\t\t\t\t\t\t\t\t\t%s"), *Entities[i]->GetName(), *Entities[i]->GetClass()->GetName());
				if (ImGui::Selectable(Name.GetUTF8(), i == Selected))
				{
					Selected = i;
				}
			}

			ImGui::TreePop();
		}

		ImGui::End();
	}
}

