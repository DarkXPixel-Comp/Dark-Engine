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
		//Primitives.Add(std::move(NewPrimitive));
	}

	for (auto& Primitive : UpdatedPrimitives)
	{
		Primitive->Proxy->SetTransform(std::move(Primitive->Primitive->GetRenderMatrix()));
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
		UpdatedPrimitives.Add(Primitive->GetSceneProxy()->PrimitiveSceneInfo);
	}
}

void FScene::BatchAddPrimitivesImpl(TArray<GPrimitiveComponent*> InPrimitives)
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
	AddCommands.Reserve(InPrimitives.Num());


	for (GPrimitiveComponent* Primitive : InPrimitives)
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
		PrimitiveSceneProxy->Scene = this;

		FMatrix ModelMatrix = Primitive->GetRenderMatrix();

		AddCommands.Emplace(PrimitiveSceneProxy, PrimitiveSceneInfo, ModelMatrix, FVector());
	}


	for (auto& Command : AddCommands)
	{
		Command.PrimitiveSceneInfo->Proxy->SetTransform(Command.RenderMatrix);
		AddedPrimitives.Add(Command.PrimitiveSceneInfo);
	}
}
