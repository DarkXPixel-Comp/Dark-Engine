#pragma once
#include "Framework/Entity.h"
#include "Math/MathFwd.h"
#include "PhysicsCore.h"
#include "PxPtr.h"


class FWorld
{
public:
	void BeginPlay();
	void InitWorld();
	void Tick(float DeltaTime, bool bFetchPhysic);
	void FetchPhysic();

	class FSceneInterface* Scene;

	EEntity* SpawnEntity(GClass* Class);
	EEntity* SpawnEntity(GClass* Class, const FVector& Location, const FRotator& Rotation);
	EEntity* SpawnEntity(GClass* Class, const FTransform& Transform);

	template<class T>
	T* SpawnEntity(GClass* Class)
	{
		return reinterpret_cast<T*>(SpawnEntity(Class));
	}
	bool IsSimulate() const { return bInSimulate; }

	const FVector3f& GetGravity() const;
	void SetGravity(const FVector3f& NewGravity);

	void InitializeNewWorld();

	bool AddPhysicComponent(physx::PxActor* InActor);

	const TArray<TObjectPtr<EEntity>>& GetEntitiesOnCurrentLevel() const;

private:
	TObjectPtr<class GLevel>  CurrentLevel;
	TPxPtr<physx::PxScene> PhysicScene;
	bool bInSimulate = false;
	FVector3f Gravity;
	float Counter = 0.f;
	float StepSize = 0.01f;

};

extern FWorld* GWorld;