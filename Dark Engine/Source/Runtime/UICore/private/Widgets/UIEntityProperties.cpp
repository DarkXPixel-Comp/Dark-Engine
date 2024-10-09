#include "Widgets/UIEntityProperties.h"
#include "imgui.h"
#include "CoreGlobals.h"
#include <cinttypes>
#include "Math/Rotator.h"

void UIEntityProperties::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
}

std::string GetPropertyName(GProperty* Property, GObject* Entity)
{
	return (-Property->GetName()) + std::string("##") + std::string(-Entity->GetName());
}

void DrawDragInt32(GProperty* Property, GObject* Entity)
{
	int32& Value = Property->GetValueTRef<int32>(Entity);
	if (ImGui::DragInt(GetPropertyName(Property, Entity).c_str(), &Value, Property->MoveSpeed, Property->MinValue, Property->MaxValue, "%d"))
	{
		Property->SetValue(Entity, Value);
	}
	//ImGui::InputInt(-Property->GetName(), &Value);
}

void DrawDragUInt32(GProperty* Property, GObject* Entity)
{
	uint32& Value = Property->GetValueTRef<uint32>(Entity);
	/*ImGui::DragInt(-Property->GetName(),
		(int32*)Value, Property->MoveSpeed, Property->MinValue < 0 ? 0 : Property->MinValue, 
		Property->MaxValue == 0 ? INT32_MAX : Property->MaxValue, "%d");*/

	if (ImGui::DragScalar(GetPropertyName(Property, Entity).c_str(),
		ImGuiDataType_U32, &Value, Property->MoveSpeed,
		Property->MinValue ? &Property->MinValue : nullptr,
		Property->MaxValue ? &Property->MaxValue : nullptr, "%u", 0))
	{
		Property->SetValue(Entity, Value);
	}

}

void DrawVector3(GProperty* Property, GObject* Entity)
{
	FVector& Vector = Property->GetValueTRef<FVector>(Entity);
	if(ImGui::DragScalarN(GetPropertyName(Property, Entity).c_str(), ImGuiDataType_Double, (void*)&Vector, 3, Property->MoveSpeed, &Property->MinValue, &Property->MaxValue, "%.2f"))
	{
		Property->SetValue(Entity, Vector);
	}
}

void DrawRotator3(GProperty* Property, GObject* Entity)
{
	FRotator& Rotator = Property->GetValueTRef<FRotator>(Entity);

	if (ImGui::DragScalarN(GetPropertyName(Property, Entity).c_str(), ImGuiDataType_Double, (void*)&Rotator, 3, Property->MoveSpeed, &Property->MinValue, &Property->MaxValue, "%.2f"))
	{
		Property->SetValue(Entity, Rotator);
	}
}

void DrawSubClassOf(GProperty* Property, GObject* Entity)
{
	TSubClassOf<GObject>* Subclass = (TSubClassOf<GObject>*)Property->GetValue(Entity);

	const auto& Classes = GClass::GetAllClasses();


	int32 CurrentId = -1;


	if (Subclass->GetCurrentClass())
	{
		auto It = std::find(Classes.begin(), Classes.end(), Subclass->GetCurrentClass());
		CurrentId = (It - Classes.begin());
	}

	if (ImGui::BeginCombo(GetPropertyName(Property, Entity).c_str(), Subclass->GetCurrentClass() ? GetPropertyName(Property, Entity).c_str() : "None"))
	{	
		if (ImGui::Selectable("None", CurrentId == -1))
		{
			CurrentId = -1;
			Subclass->SetCurrentClass(nullptr);
		}
		if (CurrentId == -1)
		{
			ImGui::SetItemDefaultFocus();
		}
		for (uint32 i = 0; i < Classes.Num(); ++i)
		{
			if (!Classes[i]->IsChildOf(Subclass->StaticClass()))
				continue;
			if (ImGui::Selectable(-Classes[i]->GetName(), CurrentId == i))
			{
				CurrentId = i;
				Subclass->SetCurrentClass(Classes[i]);
			}

			if (CurrentId == i)
			{
				ImGui::SetItemDefaultFocus();
			}

		}
		ImGui::EndCombo();
	}


}

void DrawCheckBox(GProperty* Property, GObject* Entity)
{
	uint8 Value = Property->GetValueTRef<uint8>(Entity);
	bool bValue = Value;
	if (ImGui::Checkbox(GetPropertyName(Property, Entity).c_str(), &bValue))
	{
		Value = bValue ? 0xFF : 0;
		Property->SetValue(Entity, Value);
	}
}

void DrawFloat(GProperty* Property, GObject* Entity)
{
	float Value = *Property->GetValueT<float>(Entity);
	if (ImGui::DragFloat(GetPropertyName(Property, Entity).c_str(), &Value, 1.f, 0.f, 0.f, "%.2f"))
	{
		Property->SetValue(Entity, Value);
	}
	
}

void DrawProperty(GProperty::EType InType, GProperty* Property, GObject* Entity)
{
	switch (InType)
	{
	case GProperty::EType::Puint8:return DrawCheckBox(Property, Entity);
	case GProperty::EType::Pint8: return;
	case GProperty::EType::Puint16:return;
	case GProperty::EType::Pint16:return;
	case GProperty::EType::Puint32:return DrawDragUInt32(Property, Entity);
	case GProperty::EType::Pint32:return DrawDragInt32(Property, Entity);
	case GProperty::EType::Puint64:return;
	case GProperty::EType::Pint64:return;
	case GProperty::EType::Pfloat:return DrawFloat(Property, Entity);
	case GProperty::EType::PFVector: return DrawVector3(Property, Entity);
	case GProperty::EType::PFRotator: return DrawRotator3(Property, Entity);
	case GProperty::EType::PTSubClassOf: return DrawSubClassOf(Property, Entity);
	case GProperty::EType::PTObjectPtr:
	case GProperty::EType::PGObject:
	{
		GObject* Object = *(GObject**)Property->GetValue(Entity);
		if (Object == nullptr)
		{
			return;
		}
		if (Object->GetClass()->GetProperties().Num() > 0)
		{
			ImGui::SeparatorText(GetPropertyName(Property, Entity).c_str());
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
	}

	ImGui::End();

}