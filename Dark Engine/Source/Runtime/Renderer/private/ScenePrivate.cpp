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
	struct FAddPrimitiveCommand
	{
		FAddPrimitiveCommand(FPrimitiveSceneProxy* InProxy, 
			FPrimitiveSceneInfo* InInfo, 
			const FMatrix& InRenderMatrix, 
			const FVector& InPosition):
			PrimitiveSceneInfo(InInfo),
			PrimitiveSceneProxy(InProxy),
			RenderMatrix(InRenderMatrix),
			RootPosition(InPosition)
		{}



		FPrimitiveSceneProxy* PrimitiveSceneProxy;
		FPrimitiveSceneInfo* PrimitiveSceneInfo;
		FMatrix RenderMatrix;
		FVector RootPosition;
	};

	TArray<FAddPrimitiveCommand> AddCommands;
	AddCommands.Reserve(Primitives.Num());


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
		PrimitiveSceneProxy->PrimitiveSceneInfo = PrimitiveSceneInfo;

		FMatrix RenderMatrix = Primitive->GetRenderMatrix();

		AddCommands.Emplace(PrimitiveSceneProxy, PrimitiveSceneInfo, RenderMatrix, FVector());
	}


	for (auto& Command : AddCommands)
	{
		Command.PrimitiveSceneProxy->SetTransform(Command.RenderMatrix, Command.RootPosition);

		//AddPrimitiveSceneInfo
	}


}
