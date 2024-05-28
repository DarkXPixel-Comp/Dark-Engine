#pragma once
#include "Framework/Entity.h"
#include "Math/MathFwd.h"


class FWorld
{
public:
	void InitWorld();

	class FSceneInterface* Scene;

	EEntity* SpawnActor(GClass* Class, const FVector& Location, const FRotator& Rotation);

private:
	TObjectPtr<class GLevel>  CurrentLevel;


};

extern FWorld* GWorld;