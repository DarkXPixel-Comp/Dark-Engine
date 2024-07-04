#pragma once
#include "SceneInterface.h"
#include "Engine/World.h"
#include "CoreTypes.h"
#include "PrimitiveSceneProxy.h"

class FScene : public FSceneInterface
{
public:
	FScene(FWorld* InWorld);


	virtual FScene* GetRenderScene() override final { return this; }
	virtual FWorld* GetWorld() final { return World; }

	virtual void Update() override final;

	virtual void AddPrimitive(class GPrimitiveComponent* Primitive) override;

	virtual void UpdatePrimitiveTransform(class GPrimitiveComponent* Primitive) override;

public:
	void IncrementFrameNumber()
	{
		++SceneFrameNumber;
	}
	int32 GetFrameNumber() const { return SceneFrameNumber; }


	
	void BatchAddPrimitivesImpl(TArray<GPrimitiveComponent*> Primitives);



	TArray<FPrimitiveSceneInfo*> AddedPrimitives;
	TArray<FPrimitiveSceneInfo*> Primitives;
	TArray<FPrimitiveSceneInfo*> UpdatedPrimitives;



private:
	FWorld* World;
	uint32 SceneFrameNumber;



};