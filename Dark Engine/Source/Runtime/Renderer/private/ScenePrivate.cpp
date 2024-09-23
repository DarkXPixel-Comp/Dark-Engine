#include "ScenePrivate.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "PrimitiveSceneProxy.h"
#include "PrimitiveSceneInfo.h"



FScene::FScene(FWorld* InWorld):
	World(InWorld)
{






}

void FScene::Update()
{
	OPTICK_EVENT();

	for (auto& NewPrimitive : AddedPrimitives)
	{
		Primitives.Add(NewPrimitive);
	}

	for (auto& Primitive : UpdatedPrimitives)
	{
		Primitive->RenderMatrix = Primitive->GetRenderMatrix();
	}

	UpdatedPrimitives.Empty();
	AddedPrimitives.Empty();
}

void FScene::AddPrimitive(GPrimitiveComponent* Primitive)
{
	BatchAddPrimitivesImpl(TArray<GPrimitiveComponent*>({Primitive}));
}

void FScene::UpdatePrimitiveTransform(GPrimitiveComponent* Primitive)
{
	if (Primitive->GetSceneProxy())
	{
		UpdatedPrimitives.Add(Primitive);
	}
}

void FScene::BatchAddPrimitivesImpl(TArray<GPrimitiveComponent*> InPrimitives)
{
	for (GPrimitiveComponent* Primitive : InPrimitives)
	{
		check(Primitive);
		Primitive->RenderMatrix = Primitive->GetRenderMatrix();
		AddedPrimitives.Add(Primitive);
	}
}
