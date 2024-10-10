#pragma once

#include "Entity.h"
#include "Framework/Pawn.h"





class EDefaultCamera : public EPawn
{
	DECLARE_CLASS_INTINSIC_NO_CTOR_WITH_PROPERTIES(EDefaultCamera, EPawn,
		{
		DECLARE_PROPERTY(TObjectPtr, CameraComponent, CameraComponent);
		});
public:
	EDefaultCamera(const FObjectInitializer& ObjectInitalizer = FObjectInitializer::Get());


private:
	TObjectPtr<class GCameraComponent> CameraComponent;
};