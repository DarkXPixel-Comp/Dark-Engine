#include "Framework/Entity.h"
#include "Class.h"
#include "Engine/Level.h"
#include "Components/BaseComponent.h"
#include "Components/SceneComponent.h"



//IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(EEntity);
IMPLEMENT_INTRINSIC_CLASS(EEntity, GObject);
IMPLEMENT_INTRINSIC_CLASS(GTest, GObject);


EEntity::EEntity()
{
	Init();
}

EEntity::EEntity(const FObjectInitializer& ObjectInitializer)
{
	Init();
}

void EEntity::Init()
{
	PrimaryTick.bCanEverTick = false;
	PrimaryTick.TickGroup = TG_PrePhysics;
	PrimaryTick.Target = this;	

	SubClass = NewObject<GTest>(this);
	/*BaseComponent = AddComponentByClass(GBaseComponent::StaticClass());
	SceneComponent = (GSceneComponent*)AddComponentByClass(GSceneComponent::StaticClass());*/	\
	BaseComponent = CreateDefaultSubobject<GBaseComponent>("Base Component");
	SceneComponent = CreateDefaultSubobject<GSceneComponent>("Scene Component123");
}

GBaseComponent* EEntity::AddComponentByClass(GClass* Class)
{
	if (Class == nullptr)
	{
		return nullptr;
	}				   

	GBaseComponent* NewComponent = NewObject<GBaseComponent>(this, Class);


	FinishAddComponent(NewComponent);


	return NewComponent;
}


void EEntity::FinishAddComponent(GBaseComponent* NewComponent)
{
	if (NewComponent)
	{
		if (NewComponent->bAutoRegister)
		{
			NewComponent->RegisterComponent();
		}
	}
}

void EEntity::AddOwnedComponent()
{

}