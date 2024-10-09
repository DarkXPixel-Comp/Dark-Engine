#include "Framework/DefaultCamera.h"
#include "Components/CameraComponent.h"

IMPLEMENT_INTRINSIC_CLASS(EDefaultCamera, EEntity);

EDefaultCamera::EDefaultCamera(const FObjectInitializer& ObjectInitializer) : EEntity(ObjectInitializer)
{
	CameraComponent = CreateDefaultSubobject<GCameraComponent>(TEXT("CameraComponent"));
}
