#include "Widgets/UIWorldSettings.h"
#include "imgui.h"
#include "CoreGlobals.h"
#include "Engine/World.h"


UIWorldSettings::UIWorldSettings() : UIWidget(TEXT("World Settings"))
{
	bHaveCloseButton = true;
}


void UIWorldSettings::Update(float DeltaTime)
{
	//Super::Update(DeltaTime);
}

void UIWorldSettings::DrawImGui()
{
	if (ImGui::Begin(-Name, IsWidgetClose()))
	{
		if (GWorld)
		{
			FVector3f Gravity = GWorld->GetGravity();

			if (ImGui::DragFloat3("Gravity", Gravity.XYZ, 0.2f))
			{
				GWorld->SetGravity(Gravity);
			}



		}
	}

	ImGui::End();
}


