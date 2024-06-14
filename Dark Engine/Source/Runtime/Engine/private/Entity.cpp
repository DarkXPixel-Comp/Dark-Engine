#include "Framework/Entity.h"
#include "Class.h"
#include "Engine/Level.h"
#include "Components/BaseComponent.h"
#include "Components/SceneComponent.h"
#include "Math/Transform.h"
#include "Engine/World.h"



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

void EEntity::SetRootComponent(class GSceneComponent* NewRootComponent)
{
	if (NewRootComponent == nullptr || NewRootComponent->GetOwner() == this)
	{
		if (RootComponent != NewRootComponent)
		{
			GSceneComponent* OldRootComponent = RootComponent;
			RootComponent = NewRootComponent;
		}
	}
}

void EEntity::SetOwner(EEntity* InOwner)
{
	if (Owner != InOwner)
	{
		if (Owner != nullptr)
		{
			Owner->Children.Remove(this);
		}
		Owner = InOwner;
		

		if (Owner != nullptr)
		{
			check(std::find(Owner->Children.begin(), Owner->Children.end(), this) == Owner->Children.end());
			Owner->Children.Add(this);
		}

	}
}

FWorld* EEntity::GetWorld() const
{
	if (GLevel* Level = GetLevel())
	{
		return Level->OwningWorld;
	}
	return nullptr;
}

GLevel* EEntity::GetLevel() const
{
	return GetTypedOuter<GLevel>();
}


static GSceneComponent* FixupNativeActorComponents(EEntity* Entity)
{
	GSceneComponent* SceneRootComponent = Entity->GetRootComponent();
	if (SceneRootComponent == nullptr)
	{
		TArray<GSceneComponent*> SceneComponents;
		Entity->GetComponents(SceneComponents);


		for (GSceneComponent* Component : SceneComponents)
		{
			if (Component == nullptr)
			{
				continue;
			}

			SceneRootComponent = Component;
			Entity->SetRootComponent(SceneRootComponent);
			break;
		}
	}

	return SceneRootComponent;
}

void EEntity::PostSpawnInitialize(const FTransform& SpawnTransform, EEntity* InOwner)
{
	SetOwner(InOwner);

	GSceneComponent* const SceneComponent = FixupNativeActorComponents(this);

	if (SceneComponent != nullptr)
	{
		const FTransform RootTransform(SceneComponent->GetLocation(), SceneComponent->GetRotation(), SceneComponent->GetScale());


		//SceneComponent->SetWorldTransform();
	}


	RegisterAllComponents();


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

void EEntity::AddOwnedComponent(GBaseComponent* InComponent)
{
	if (OwnedComponents.find(InComponent) == OwnedComponents.end())
	{
		OwnedComponents.insert(InComponent);
	}
}

void EEntity::PreRegisterAllComponents()
{


}

void EEntity::RegisterAllComponents()
{
	PreRegisterAllComponents();

	FWorld* const World = GetWorld();
	check(World);

	if (RootComponent != nullptr && !RootComponent->IsRegistered() && RootComponent->bAutoRegister)
	{
		RootComponent->RegisterComponentWithWorld(World);
	}
}
