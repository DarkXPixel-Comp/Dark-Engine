#pragma once
#include "SceneInterface.h"
#include "Engine/World.h"
#include "CoreTypes.h"

class FScene : public FSceneInterface
{
public:
	virtual FScene* GetRenderScene() override final { return this; }
	virtual FWorld* GetWorld() final { return World; }

public:
	void IncrementFrameNumber()
	{
		++SceneFrameNumber;
	}
	int32 GetFrameNumber() const { return SceneFrameNumber; }





private:
	FWorld* World;
	uint32 SceneFrameNumber;



};