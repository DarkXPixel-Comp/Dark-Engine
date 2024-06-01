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

void DrawVector3(GProperty* Property, GObject* Entity)
{
	FVector* Vector = (FVector*)Property->GetValue(Entity);
	//ImGui::InputScalarN(-Property->GetName(), ImGuiDataType_Double, (void*)Vector, 3, 0, 0, "%.6f");
	ImGui::DragScalarN(-Property->GetName(), ImGuiDataType_Double, (void*)Vector, 3, 1.f, &Property->MinValue, &Property->MaxValue, "%.2f");
}

void DrawRotator3(GProperty* Property, GObject* Entity)
{
	FRotator* Rotator = (FRotator*)Property->GetValue(Entity);

	ImGui::DragScalarN(-Property->GetName(), ImGuiDataType_Double, (void*)Rotator, 3, 1.f, &Property->MinValue, &Property->MaxValue, "%.2f");
}

void DrawCheckBox(GProperty* Property, GObject* Entity)
{
	uint8* Value = (uint8*)Property->GetValue(Entity);
	ImGui::Checkbox(-Property->GetName(), (bool*)Value);
}

void DrawProperty(GProperty::EType InType, GProperty* Property, GObject* Entity)
{
	switch (InType)
	{
	case GProperty::EType::uint8:return DrawCheckBox(Property, Entity);
	case GProperty::EType::int8: return;
	case GProperty::EType::uint16:return;
	case GProperty::EType::int16:return;
	case GProperty::EType::uint32:return DrawDragUInt32(Property, Entity);
	case GProperty::EType::int32:return DrawDragInt32(Property, Entity);
	case GProperty::EType::uint64:return;
	case GProperty::EType::int64:return;
	case GProperty::EType::FVector: return DrawVector3(Property, Entity);
	case GProperty::EType::FRotator: return DrawRotator3(Property, Entity);
	case GProperty::EType::GObject:
	{
		GObject* Object = *(GObject**)Property->GetValue(Entity);
		if (Object == nullptr)
		{
			return;
		}
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