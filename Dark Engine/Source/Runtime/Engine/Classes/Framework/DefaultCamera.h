#pragma once

#include "Entity.h"





class EDefaultCamera : public EEntity
{
	DECLARE_CLASS_INTINSIC_NO_CTOR_WITH_PROPERTIES(EDefaultCamera, EEntity,
		{
		DECLARE_PROPERTY(TObjectPtr, CameraComponent, CameraComponent);
		});
public:
	EDefaultCamera(const FObjectInitializer& ObjectInitalizer = FObjectInitializer::Get());


private:
	TObjectPtr<class GCameraComponent> CameraComponent;
};