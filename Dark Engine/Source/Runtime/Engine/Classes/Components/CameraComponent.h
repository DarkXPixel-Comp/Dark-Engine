#pragma once


#include "Components/SceneComponent.h"


struct FCameraMatrices
{
	FMatrix ViewMatrix;
	FMatrix ProjectionMatrix;
	FMatrix ViewProjectionMatrix;
};

class GCameraComponent : public GSceneComponent
{
	DECLARE_CASETED_CLASS_INTRINSIC_WITH_API_WITH_PROPERTIES(GCameraComponent, GSceneComponent, 
		{
			DECLARE_PROPERTY(float, FOV, FOV);
		});

public:

	virtual void RegisterComponentWithWorld(FWorld* InWorld) override;

	virtual void BeginPlay() override;

	virtual void EndPlay() override;

	FCameraMatrices CameraMatrices;

protected:
	float FOV = 90.f;


private:

};