#include "ScenePrivate.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "PrimitiveSceneProxy.h"
#include "PrimitiveSceneInfo.h"
#include "Math/LookToMatrix.h"
#include "Math/Vector.h"

#include "DirectXMath.h"


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

void FScene::UpdateCamera(GCameraComponent* InCamera)
{
	check(InCamera == Camera);
	FCameraMatrices& Matrices = InCamera->CameraMatrices;

	FVector Loc = InCamera->GetLocation();
	FVector FrontVector = InCamera->GetRotation().GetForwardVector();

	Matrices.ViewMatrix = FLookToMatrix(InCamera->GetLocation(), InCamera->GetRotation().GetForwardVector(), FVector(0, 1, 0));

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
