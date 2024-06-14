#include "Engine/World.h"
#include "RenderGlobals.h"
#include "Engine/Level.h"
#include "Framework/MeshObject.h"
#include "Math/Transform.h"


FWorld* GWorld = nullptr;


void FWorld::InitWorld()
{
	Scene = GetRenderer()->AllocateScene(this);
}


void FWorld::InitializeNewWorld()
{
	CurrentLevel = NewObject<GLevel>(nullptr, TEXT("PersistentLevel"));
	CurrentLevel->OwningWorld = this;



	InitWorld();

	EEntity* Test = SpawnEntity(EEntity::StaticClass(), FVector(), FRotator());
	SpawnEntity(EEntity::StaticClass(), FVector(), FRotator());
	SpawnEntity(EEntity::StaticClass(), FVector(), FRotator());
	SpawnEntity(EEntity::StaticClass(), FVector(), FRotator());
	SpawnEntity(EEntity::StaticClass(), FVector(), FRotator());
	SpawnEntity(EEntity::StaticClass(), FVector(), FRotator());
	SpawnEntity(EMeshObject::StaticClass(), FVector(), FRotator());
}

const TArray<TObjectPtr<EEntity>>& FWorld::GetEntitiesOnCurrentLevel() const
{
	return CurrentLevel->Entities;
}

EEntity* FWorld::SpawnEntity(GClass* Class, const FVector& Location, const FRotator& Rotation)
{
	GLevel* LevelToSpawn = nullptr;

	if (!LevelToSpawn)
	{
		LevelToSpawn = CurrentLevel.Get();
	}


	EEntity* Entity = NewObject<EEntity>(LevelToSpawn, Class);
	Entity->PostSpawnInitialize(FTransform(), nullptr);

	LevelToSpawn->Entities.Add(Entity);


	return Entity;
}