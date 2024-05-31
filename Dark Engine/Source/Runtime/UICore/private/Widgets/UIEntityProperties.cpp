#include "Widgets/UIEntityProperties.h"
#include "imgui.h"
#include "CoreGlobals.h"

void UIEntityProperties::Update(float DeltaTime)
{


}

void DrawDragInt32(GProperty* Property, GObject* Entity)
{
	int32& Value = Property->GetInt32(Entity);
	ImGui::DragInt(-Property->GetName(), &Value, Property->MoveSpeed, Property->MinValue, Property->MaxValue, "%d");
	//ImGui::InputInt(-Property->GetName(), &Value);
}

void DrawDragUInt32(GProperty* Property, GObject* Entity)
{
	uint32* Value = (uint32*)Property->GetValue(Entity);
	ImGui::DragInt(-Property->GetName(),
		(int32*)Value, Property->MoveSpeed, Property->MinValue < 0 ? 0 : Property->MinValue, 
		Property->MaxValue == 0 ? INT32_MAX : Property->MaxValue, "%d");
	//ImGui::InputInt(-Property->GetName(), &Value);
}


void DrawProperty(GProperty::EType InType, GProperty* Property, GObject* Entity)
{
	switch (InType)
	{
	case GProperty::EType::uint8:return;
	case GProperty::EType::int8: return;
	case GProperty::EType::uint16:return;
	case GProperty::EType::int16:return;
	case GProperty::EType::uint32:return DrawDragUInt32(Property, Entity);
	case GProperty::EType::int32:return DrawDragInt32(Property, Entity);
	case GProperty::EType::uint64:return;
	case GProperty::EType::int64:return;
	case GProperty::EType::GObject:
	{
		GObject* Object = *(GObject**)Property->GetValue(Entity);
		ImGui::SeparatorText(-Object->GetName());
		for (auto& Property : Object->GetClass()->GetProperties())
		{
			DrawProperty(Property->GetType(), Property, Object);
		}
		break;
	}
		break;
	}
}





void UIEntityProperties::DrawImGui()
{
	if (ImGui::Begin(-Name, IsWidgetClose()))
	{
		if (GCurrentEntity != -1)
		{
			const auto& Entity = GWorld->GetEntitiesOnCurrentLevel()[GCurrentEntity];

			ImGui::Separator();

			for (auto& Property : Entity->GetClass()->GetProperties())
			{
				DrawProperty(Property->GetType(), Property, Entity);
			}

		}

		ImGui::End();
	}

}