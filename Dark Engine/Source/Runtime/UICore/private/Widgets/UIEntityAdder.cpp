#include "Widgets/UIEntityAdder.h"
#include "Engine/World.h"
#include "Class.h"
#include "Framework/Entity.h"
#include "imgui.h"

UIEntityAdder::UIEntityAdder() : UIWidget(TEXT("UIEntityAdder"))
{
	bHaveCloseButton = true;
}

void UIEntityAdder::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
	if (CreateClass)
	{
		GWorld->SpawnEntity(CreateClass);
		CreateClass = nullptr;
	}
}

void UIEntityAdder::DrawImGui()
{
	const auto& Classes = GClass::GetAllClasses();


	if (ImGui::Begin(-GetName(), IsWidgetClose())) 
	{
		for (auto& i : Classes)
		{
			if (i->IsChildOf(EEntity::StaticClass()))
			{
				if (ImGui::Button(-i->GetName()))
				{
					CreateClass = i;
					//GWorld->SpawnEntity(i);
				}
			}
		}
	}  

	ImGui::End();



}
