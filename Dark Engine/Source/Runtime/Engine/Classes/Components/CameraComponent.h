#pragma once


#include "Components/SceneComponent.h"
#include "Math/Matrix.h"


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
			DECLARE_PROPERTY(float, FOV, FOV, PROPERTY_META(DECLARE_SETTER(SetFOV)));
		});

public:

	virtual void RegisterComponentWithWorld(FWorld* InWorld) override;

	virtual void BeginPlay() override;

	virtual void EndPlay() override;

	FCameraMatrices CameraMatrices;

	float GetFOV() const { return FOV; }
	
	IMPLEMENT_SETTER(SetFOV, float)
	void SetFOV(float InFOV) { FOV = InFOV; }  

	float GetAspectRatio() const { return AspectRatio; }

	float GetNearZ() const { return NearZ; }
	float GetFarZ() const { return FarZ; }

protected:
	float FOV = 90.f;
	float AspectRatio = 16.f / 9.f;

	float NearZ = 0.1f;
	float FarZ = 100.f;


private:

};