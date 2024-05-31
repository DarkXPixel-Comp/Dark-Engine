#pragma once
#include "Framework/Entity.h"
#include "Math/MathFwd.h"


class FWorld
{
public:
	void InitWorld();

	class FSceneInterface* Scene;

	EEntity* SpawnEntity(GClass* Class);
	EEntity* SpawnEntity(GClass* Class, const FVector& Location, const FRotator& Rotation);
	EEntity* SpawnEntity(GClass* Class, const FTransform& Transform);

	template<class T>
	T* SpawnEntity(GClass* Class)
	{
		return reinterpret_cast<T*>(SpawnEntity(Class));
	}

	void InitializeNewWorld();

	const TArray<TObjectPtr<EEntity>>& GetEntitiesOnCurrentLevel() const;

private:
	TObjectPtr<class GLevel>  CurrentLevel;


};

extern FWorld* GWorld;