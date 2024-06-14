#include "Widgets/UIEntityProperties.h"
#include "imgui.h"
#include "CoreGlobals.h"
#include <cinttypes>

void UIEntityProperties::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
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
	/*ImGui::DragInt(-Property->GetName(),
		(int32*)Value, Property->MoveSpeed, Property->MinValue < 0 ? 0 : Property->MinValue, 
		Property->MaxValue == 0 ? INT32_MAX : Property->MaxValue, "%d");*/
	ImGui::DragScalar(-Property->GetName(),
		ImGuiDataType_U32, Value, Property->MoveSpeed,
		Property->MinValue ? &Property->MinValue : nullptr,
		Property->MaxValue ? &Property->MaxValue : nullptr, "%u",
		0);

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

void DrawSubClassOf(GProperty* Property, GObject* Entity)
{
	TSubClassOf<GObjectBase>* Subclass = (TSubClassOf<GObjectBase>*)Property->GetValue(Entity);

	const auto& Classes = GClass::GetAllClasses();


	int32 CurrentId = -1;


	if (Subclass->GetCurrentClass())
	{
		auto It = std::find(Classes.begin(), Classes.end(), Subclass->GetCurrentClass());
		CurrentId = (It - Classes.begin());
	}

	if (ImGui::BeginCombo(-Property->GetName(), Subclass->GetCurrentClass() ? -Subclass->GetCurrentClass()->GetName() : "None"))
	{	
		if (ImGui::Selectable("None", CurrentId == -1))
		{
			CurrentId = -1;
			Subclass->SetCurrentClass(nullptr);
		}
		if (CurrentId == -1)
		{
			//ImGui::SetItemDefaultFocus();
		}
		for (uint32 i = 0; i < Classes.Num(); ++i)
		{
			if (ImGui::Selectable(-Classes[i]->GetName(), CurrentId == i))
			{
				CurrentId = i;
				Subclass->SetCurrentClass(Classes[i]);
			}

			if (CurrentId == i)
			{
				//ImGui::SetItemDefaultFocus();
			}

		}
		ImGui::EndCombo();
	}


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
	case GProperty::EType::TSubClassOf: return DrawSubClassOf(Property, Entity);
	case GProperty::EType::GObject:
	{
		GObject* Object = *(GObject**)Property->GetValue(Entity);
		if (Object == nullptr)
		{
			return;
		}
		if (Object->GetClass()->GetProperties().Num() > 0)
		{
			ImGui::SeparatorText(-Object->GetName());
			for (auto& Property : Object->GetClass()->GetProperties())
			{
				DrawProperty(Property->GetType(), Property, Object);
			}
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
			GClass* CurrentClass = Entity->GetClass();

			while (CurrentClass)
			{
				ImGui::Separator();
				for (auto& Property : CurrentClass->GetProperties())
				{
					DrawProperty(Property->GetType(), Property, Entity);
				}
				CurrentClass = CurrentClass->GetSuperClass();
			}
		}

		ImGui::End();
	}

}