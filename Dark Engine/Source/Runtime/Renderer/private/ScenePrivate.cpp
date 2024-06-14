#include "ScenePrivate.h"
#include "Engine/World.h"



FScene::FScene(FWorld* InWorld):
	World(InWorld)
{






}

void FScene::AddPrimitive(GPrimitiveComponent* Primitive)
{
	BatchAddPrimitivesImpl(TArray<GPrimitiveComponent*>({Primitive}));
}

void FScene::BatchAddPrimitivesImpl(TArray<GPrimitiveComponent*> Primitives)
{


}
