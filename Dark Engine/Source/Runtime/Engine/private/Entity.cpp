#include "Framework/Entity.h"
#include "Class.h"



IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(EEntity);


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
	





}