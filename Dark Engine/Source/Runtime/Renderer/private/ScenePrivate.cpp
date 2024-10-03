#include "ScenePrivate.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "PrimitiveSceneProxy.h"
#include "PrimitiveSceneInfo.h"
#include "Math/LookToMatrix.h"
#include "Math/PersperctiveMatrix.h"
#include "Math/Vector.h"
#include "DynamicRHI.h"
#include "BS_thread_pool.hpp"

#include "DirectXMath.h"


FScene::FScene(FWorld* InWorld):
	World(InWorld),
	UpdateThreadPool(std::thread::hardware_concurrency() / 2)
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
		UpdateThreadPool.submit_task([&Primitive, this]()
			{
				UpdateMatrixPrimitive(Primitive);
				Primitive->PrimitiveUpdate();
			});
	}

	UpdateThreadPool.wait();

	UpdatedPrimitives.Empty();
	AddedPrimitives.Empty();
}

void FScene::AddPrimitive(GPrimitiveComponent* Primitive)
{
	BatchAddPrimitivesImpl(TArray<GPrimitiveComponent*>({Primitive}));
}

void FScene::UpdatePrimitiveTransform(GPrimitiveComponent* Primitive)
{
	UpdatedPrimitives.Add(Primitive);
}

void FScene::UpdateMatrixPrimitive(GPrimitiveComponent* Primitive)
{
	check(Primitive);
	Primitive->RenderMatrix = Primitive->GetRenderMatrix();
	if (!Primitive->RHIRenderMatrix)
	{
		Primitive->RHIRenderMatrix = RHICreateUniformBuffer(&Primitive->RenderMatrix, sizeof(FMatrix), UniformBuffer_MultiFrame);
	}

	RHIUpdateUniformBuffer(Primitive->RHIRenderMatrix.Get(), &Primitive->RenderMatrix, sizeof(FMatrix));
}

void FScene::UpdateCamera(GCameraComponent* InCamera)
{
	check(InCamera == Camera);
	FCameraMatrices& Matrices = InCamera->CameraMatrices;

	Matrices.ViewMatrix = FLookToMatrix(InCamera->GetLocation(), InCamera->GetRotation().GetForwardVector(), FVector(0, 1, 0));
	Matrices.ProjectionMatrix = FPerspectiveMatrix(FMath::DegreesToRadians(InCamera->GetFOV()), InCamera->GetAspectRatio(), InCamera->GetNearZ(), InCamera->GetFarZ());
	Matrices.ViewProjectionMatrix = Matrices.ViewMatrix * Matrices.ProjectionMatrix;
}

void FScene::BatchAddPrimitivesImpl(TArray<GPrimitiveComponent*> InPrimitives)
{
	for (GPrimitiveComponent* Primitive : InPrimitives)
	{
		check(Primitive);
		//Primitive->RenderMatrix = Primitive->GetRenderMatrix();
		UpdateMatrixPrimitive(Primitive);
		AddedPrimitives.Add(Primitive);
	}
}
