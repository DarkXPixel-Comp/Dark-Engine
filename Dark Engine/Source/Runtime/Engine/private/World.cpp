#include "Engine/World.h"
#include "RenderGlobals.h"
#include "Engine/Level.h"
#include "Components/PrimitiveComponent.h"
#include "Framework/MeshObject.h"
#include "Framework/DefaultCamera.h"
#include "Components/CameraComponent.h"
#include "Framework/Pawn.h"
#include "SceneInterface.h"
#include "Math/Transform.h"
#include "extensions/PxSimpleFactory.h"
#include "PxRigidStatic.h"
#include "PxMaterial.h"
#include "PxSimulationEventCallback.h"
#include "optick.h"
#include "ScriptManager.h"


FWorld* GWorld = nullptr;


class FWorldPhysicsCallback : public physx::PxSimulationEventCallback
{
public:
	FWorldPhysicsCallback(FWorld* InWorld) : World(InWorld)
	{}

	virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) {}

	virtual void onWake(physx::PxActor** actors, physx::PxU32 count) {}


	virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) {}

	virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) {}

	virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {}

	virtual void onAdvance(const physx::PxRigidBody* const* BodyBuffer, const physx::PxTransform* PoseBuffer, const physx::PxU32 Count)
	{
		for (uint32 i = 0; i < Count; ++i)
		{
			GPrimitiveComponent* Component = (GPrimitiveComponent*)BodyBuffer[i]->userData;
			physx::PxVec3 NewLocation = PoseBuffer->p;
			physx::PxQuat NewRotation = PoseBuffer->q;
			Component->SetWorldLocation(FVector(NewLocation.x, NewLocation.y, NewLocation.z));
			//Component->SetWorldRotation(FRotator(NewRotation))
		}
	}

	virtual ~FWorldPhysicsCallback() noexcept {}

private:
	FWorld* World = nullptr;

};

TUniquePtr<FWorldPhysicsCallback> WorldCallbacks;

void FWorld::InitWorld()
{
	Scene = GetRenderer()->AllocateScene(this);

	physx::PxSceneDesc SceneDesc(GPhysicsCore->GetScaleScene());
	SceneDesc.userData = this;
	SceneDesc.gravity = physx::PxVec3(0, 0.f, 0);
	Gravity = *(FVector3f*)&SceneDesc.gravity;

	PhysicScene = GPhysicsCore->CreateScene(SceneDesc);
	WorldCallbacks = MakeUnique(new FWorldPhysicsCallback(this));

	PhysicScene->setSimulationEventCallback(WorldCallbacks.get());
}

void FWorld::EndPlay()
{
	for (auto& i : CurrentLevel->Entities)
	{
		if (i->IsBeginPlay())
		{
			i->EndPlay();
		}
	}
	bBeginPlay = false;



}

const FVector3f& FWorld::GetGravity() const
{
	if (PhysicScene)
	{
		return Gravity;
	}
}

void FWorld::SetGravity(const FVector3f& NewGravity)
{
	Gravity = NewGravity;
	if (PhysicScene)
	{
		PhysicScene->setGravity(*(physx::PxVec3*)&Gravity);
	}

}


void FWorld::BeginPlay()
{	
	TSubClassOf<EPawn> PawnClass;
	if (ClassForCreatePawn.GetCurrentClass() == EPawn::StaticClass())
	{
		PawnClass = EDefaultCamera::StaticClass();
	}
	

	CurrentPawn = SpawnEntity<EPawn>(PawnClass);


	TArray<GCameraComponent*> CameraComponents;
	CurrentPawn->GetComponents<GCameraComponent>(CameraComponents);

	if (CameraComponents.size())
	{
		Scene->SetCamera(CameraComponents[0]);
	}


	for (auto &i : CurrentLevel->Entities)
	{			   
		if(!i->IsBeginPlay())
		{
			i->BeginPlay();
		}
	}
	bBeginPlay = true;
}


void FWorld::Tick(float DeltaTime, bool InFetchPhysic)
{
	OPTICK_EVENT("World Tick");
	OPTICK_TAG("InFetchPhysic", InFetchPhysic);

	if (Scene)
	{
		Scene->Update();
	}

	//InFetchPhysic = true;
	if (bBeginPlay && PhysicScene && !bInSimulate)
	{
		Counter += DeltaTime;
		if (Counter < StepSize)
		{

		}
		else
		{
			OPTICK_CATEGORY("Update physics", Optick::Category::Physics);
			Counter -= StepSize;
			bInSimulate = true;
			PhysicScene->simulate(StepSize);
			if (InFetchPhysic)
			{
				FetchPhysic();
			}
		}
	}
}

void FWorld::FetchPhysic()
{
	if (bBeginPlay && PhysicScene && bInSimulate)
	{
		PhysicScene->fetchResults(true);
		bInSimulate = false;
	}
}

bool FWorld::AddPhysicComponent(physx::PxActor* InActor)
{
	PhysicScene->addActor(*InActor);
	return false;
}

void FWorld::SetCurrentPawn(TObjectPtr<EPawn> InPawn)
{
	CurrentPawn = InPawn.Get();
}


void FWorld::InitializeNewWorld()
{
	OPTICK_EVENT("InitializeNewWorld");
	CurrentLevel = NewObject<GLevel>(nullptr, TEXT("PersistentLevel"));
	CurrentLevel->OwningWorld = this;



	InitWorld();

	//EEntity* Test = SpawnEntity(EEntity::StaticClass(), FVector(), FRotator());
	//SpawnEntity(EEntity::StaticClass(), FVector(), FRotator());
	//SpawnEntity(EEntity::StaticClass(), FVector(), FRotator());
	//SpawnEntity(EEntity::StaticClass(), FVector(), FRotator());
	//SpawnEntity(EEntity::StaticClass(), FVector(), FRotator());
	//SpawnEntity(EEntity::StaticClass(), FVector(), FRotator());
	EMeshObject* MeshObject = (EMeshObject*)SpawnEntity(EMeshObject::StaticClass(), FVector(), FRotator());

	//EMeshObject& MeshRef = *MeshObject;

	//auto& Lua = FScriptManager::Get()->VMLua;
	//Lua["MeshObject"] = MeshObject;

	//auto Result = Lua.script("MeshObject:TestPrint('Hello')");
}

const TArray<TObjectPtr<EEntity>>& FWorld::GetEntitiesOnCurrentLevel() const
{
	return CurrentLevel->Entities;
}

EEntity* FWorld::SpawnEntity(GClass* Class)
{
	return SpawnEntity(Class, FVector(), FRotator());
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

	/*physx::PxMaterial* Material = GPhysicsCore->GetPhysics()->createMaterial(0.5f, 0.5f, 0.6f);
	physx::PxRigidStatic* Plane = physx::PxCreatePlane(*GPhysicsCore->GetPhysics(), physx::PxPlane(0, 1, 0, 0), *Material);
	PhysicScene->addActor(*Plane);*/

	LevelToSpawn->Entities.Add(Entity);

	if (bBeginPlay)
	{
		Entity->BeginPlay();
	}


	return Entity;
}