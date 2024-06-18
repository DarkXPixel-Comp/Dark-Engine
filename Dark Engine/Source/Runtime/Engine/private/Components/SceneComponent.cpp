#include "Components/SceneComponent.h"


IMPLEMENT_INTRINSIC_CLASS(GSceneComponent, GBaseComponent);


GSceneComponent::GSceneComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bVisible = true;
}

void GSceneComponent::SetWorldLocation(FVector NewLocation)
{
	RelativeLocation = NewLocation;
}

void GSceneComponent::SetWorldRotation(FRotator NewRoatation)
{
	RelativeRotation = NewRoatation;
}
