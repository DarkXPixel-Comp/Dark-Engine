#include "ScenePrivate.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "PrimitiveSceneProxy.h"
#include "PrimitiveSceneInfo.h"

#include <DirectXMath.h>


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
	FCameraMatrices& Matrices = InCamera->CameraMatrices;


	//TODO: Create View Proj Matrices  Example: (https://github.com/DarkXPixel-Comp/Dark-Engine/blob/a21760284bb3f01ad63c2360d63dc6cd46ab207d/Dark%20Engine/Source/Runtime/Render/D3D12/D3D12Utils.h)
	//DirectX::XMMatrixLookAtLH()




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
