#pragma once
#include <Components/SceneComponent/USceneComponent.h>
#include <SceneTypes.h>
#include <HAL/Platform.h>


class UPrimitiveComponent : public USceneComponent
{
	typedef USceneComponent Super;

public:
	UPrimitiveComponent();
	virtual void BeginPlay() override;
	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;
public:
	FPrimitiveComponentId* GetPrimitiveId() { return &ComponentId; }



private:
	FPrimitiveComponentId ComponentId;

};