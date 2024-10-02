#pragma once
#include "SceneInterface.h"
#include "Engine/World.h"
#include "CoreTypes.h"
#include "PrimitiveSceneProxy.h"
#include "Components/CameraComponent.h"

class FScene : public FSceneInterface
{
public:
	FScene(FWorld* InWorld);


	virtual FScene* GetRenderScene() override { return this; }
	virtual FWorld* GetWorld() { return World; }

	virtual void Update() override;

	virtual void AddPrimitive(class GPrimitiveComponent* Primitive) override;

	virtual void UpdatePrimitiveTransform(class GPrimitiveComponent* Primitive) override;

	void UpdateMatrixPrimitive(class GPrimitiveComponent* Primitive);

	virtual void SetCamera(GCameraComponent* InCamera) override
	{
		Camera = InCamera;
		if (InCamera != nullptr)
		{
			UpdateCamera(InCamera);
		}
	}

	virtual void UpdateCamera(GCameraComponent* InCamera) override;

public:
	void IncrementFrameNumber()
	{
		++SceneFrameNumber;
	}
	int32 GetFrameNumber() const { return SceneFrameNumber; }


	
	void BatchAddPrimitivesImpl(TArray<GPrimitiveComponent*> Primitives);


	TArray<GPrimitiveComponent*> AddedPrimitives;
	TArray<GPrimitiveComponent*> Primitives;
	TArray<GPrimitiveComponent*> UpdatedPrimitives;

	GCameraComponent* Camera = nullptr;

	/*TArray<FPrimitiveSceneInfo*> AddedPrimitives;
	TArray<FPrimitiveSceneInfo*> Primitives;
	TArray<FPrimitiveSceneInfo*> UpdatedPrimitives;*/



private:
	FWorld* World;
	uint32 SceneFrameNumber;



};