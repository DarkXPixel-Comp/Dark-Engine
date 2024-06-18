#include "ScenePrivate.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "PrimitiveSceneProxy.h"
#include "PrimitiveSceneInfo.h"



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
	for (GPrimitiveComponent* Primitive : Primitives)
	{
		FPrimitiveSceneInfoData& SceneData = Primitive->GetSceneData();

		FPrimitiveSceneProxy* PrimitiveSceneProxy = nullptr;
		
		if (!Primitive->GetSceneProxy())
		{
			PrimitiveSceneProxy = Primitive->CreateSceneProxy();
		}
		else
		{
			PrimitiveSceneProxy = Primitive->GetSceneProxy();
		}

		if (!PrimitiveSceneProxy)
		{
			continue;
		}

		FPrimitiveSceneInfo* PrimitiveSceneInfo = new FPrimitiveSceneInfo(Primitive, this);



	}




}
