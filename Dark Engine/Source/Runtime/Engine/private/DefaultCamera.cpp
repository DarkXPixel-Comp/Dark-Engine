#include "Framework/DefaultCamera.h"
#include "Components/CameraComponent.h"

IMPLEMENT_INTRINSIC_CLASS(EDefaultCamera, EPawn);

EDefaultCamera::EDefaultCamera(const FObjectInitializer& ObjectInitializer) : EPawn(ObjectInitializer)
{
	CameraComponent = CreateDefaultSubobject<GCameraComponent>(TEXT("CameraComponent"));
}
