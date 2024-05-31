#include "Framework/Entity.h"
#include "Class.h"
#include "Engine/Level.h"



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

	ObjectPtr = NewObject<GTest>(this, TEXT("ObjectPtr"));
	
}