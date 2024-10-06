#include "Components/SceneComponent.h"


IMPLEMENT_INTRINSIC_CLASS(GSceneComponent, GBaseComponent);


GSceneComponent::GSceneComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bVisible = true;
}


void GSceneComponent::SetWorldScale(FVector NewScale)
{
	FVector NewRelativeScale = NewScale;
	if (GetAttachParent() != nullptr && IsRelativeScale())
	{
		FTransform ParentToWorld = GetAttachParent()->GetComponentTransform();
		//NewRelativeLocation = ParentToWorld.InverseTransformPosition(NewRelativeLocation);
	}

	RelativeScale = NewRelativeScale;
	ComponentToWorld.SetScale(NewRelativeScale);
}


void GSceneComponent::SetWorldRotation(FRotator NewRoatation)
{
	FRotator NewRelativeRotation = NewRoatation;
	if (GetAttachParent() != nullptr && IsRelativeRotation())
	{
		FTransform ParentToWorld = GetAttachParent()->GetComponentTransform();
		//NewRelativeLocation = ParentToWorld.InverseTransformPosition(NewRelativeLocation);
	}

	RelativeRotation = NewRelativeRotation;
	ComponentToWorld.SetRotation(NewRelativeRotation);
}

void GSceneComponent::SetWorldLocation(FVector NewLocation)
{
	FVector NewRelativeLocation = NewLocation;
	if (GetAttachParent() != nullptr && IsRelativeLocation())
	{
		FTransform ParentToWorld = GetAttachParent()->GetComponentTransform();
		NewRelativeLocation = ParentToWorld.InverseTransformPosition(NewRelativeLocation);
	}

	RelativeLocation = NewRelativeLocation;
	ComponentToWorld.SetLocation(NewRelativeLocation);
}


FVector GSceneComponent::GetEntityPositionForRenderer()	const
{
	/*const GSceneComponent* Top;
	for (Top = this; Top->GetAtt; ++)
	{

	}*/
	return FVector();
}

FVector GSceneComponent::GetFrontVector() const
{
	return GetLocation() + GetRotation().GetForwardVector();
}
