#include "Engine/World.h"
#include "RenderGlobals.h"
#include "Engine/Level.h"
#include "Framework/MeshObject.h"
#include "Math/Transform.h"
#include "extensions/PxSimpleFactory.h"
#include "PxRigidStatic.h"


FWorld* GWorld = nullptr;


void FWorld::InitWorld()
{
	Scene = GetRenderer()->AllocateScene(this);

	physx::PxSceneDesc SceneDesc(GPhysicsCore->GetScaleScene());
	SceneDesc.userData = this;
	SceneDesc.gravity = physx::PxVec3(0, -9.8f, 0);

	PhysicScene = GPhysicsCore->CreateScene(SceneDesc);
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

	physx::PxMaterial* Material = GPhysicsCore->GetPhysics()->createMaterial(0.5f, 0.5f, 0.6f);
	physx::PxRigidStatic* Plane = physx::PxCreatePlane(*GPhysicsCore->GetPhysics(), physx::PxPlane(0, 1, 0, 0), *Material);
	bool Test = PhysicScene->addActor(*Plane);

	LevelToSpawn->Entities.Add(Entity);


	return Entity;
}