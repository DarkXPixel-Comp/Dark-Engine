#include "Components/SceneComponent.h"


IMPLEMENT_INTRINSIC_CLASS(GSceneComponent, GBaseComponent);


GSceneComponent::GSceneComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bVisible = true;
}
