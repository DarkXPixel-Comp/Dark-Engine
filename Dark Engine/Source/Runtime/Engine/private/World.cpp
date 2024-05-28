#include "Engine/World.h"
#include "RenderGlobals.h"
#include "Engine/Level.h"


FWorld* GWorld = nullptr;


void FWorld::InitWorld()
{
	GetRenderer()->AllocateScene(this);
}


EEntity* FWorld::SpawnActor(GClass* Class, const FVector& Location, const FRotator& Rotation)
{
	GLevel* LevelToSpawn = nullptr;

	if (!LevelToSpawn)
	{
		LevelToSpawn = CurrentLevel.Get();
	}


	EEntity* const Entity = NewObject<EEntity>(LevelToSpawn, TEXT("test"));


	return Entity;
}