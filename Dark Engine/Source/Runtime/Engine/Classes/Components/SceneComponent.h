#pragma once

#include "Components/BaseComponent.h"
#include "Math/MathFwd.h"
#include "Math/Transform.h"


class GSceneComponent : public GBaseComponent
{
	DECLARE_CLASS_INTINSIC_NO_CTOR_WITH_PROPERTIES(GSceneComponent, GBaseComponent,
		{
			DECLARE_PROPERTY(uint8, bVisible, Visible);
			DECLARE_PROPERTY(FVector, RelativeLocation, Location, PROPERTY_META(DECLARE_SETTER(SetWorldLocation); MoveSpeed=0.5f;));
			DECLARE_PROPERTY(FVector, RelativeScale, Scale, PROPERTY_META(DECLARE_SETTER(SetWorldScale); MoveSpeed = 0.5f;));
			DECLARE_PROPERTY(FRotator, RelativeRotation, Rotation, PROPERTY_META(DECLARE_SETTER(SetWorldRotation); MoveSpeed = 0.5f));
		});

public:
	GSceneComponent(const FObjectInitializer& ObjectInitalizer = FObjectInitializer::Get());

	IMPLEMENT_SETTER(SetWorldLocation, FVector);
	virtual void SetWorldLocation(FVector NewLocation);
	IMPLEMENT_SETTER(SetWorldRotation, FRotator);
	virtual void SetWorldRotation(FRotator NewRotation);
	IMPLEMENT_SETTER(SetWorldScale, FVector);
	virtual void SetWorldScale(FVector NewScale);



	const FRotator& GetRotation() const { return RelativeRotation; }
	const FVector& GetLocation() const { return RelativeLocation; }
	const FVector& GetScale() const { return RelativeScale; }

	FVector GetFrontVector() const;

	FVector GetEntityPositionForRenderer() const;

	FORCEINLINE const FTransform& GetComponentTransform() const
	{
		return ComponentToWorld;
	}

	uint8 bVisible;

	FORCEINLINE GSceneComponent* GetAttachParent() const
	{
		return AttachParent;
	}


	bool IsRelativeLocation() const
	{
		return bRelativeLocation;
	}
	bool IsRelativeRotation() const
	{
		return bRelativeRotation;
	}

	bool IsRelativeScale() const
	{
		return bRelataiveScale;
	}

protected:
	

private:
	FVector RelativeLocation;
	FRotator RelativeRotation;
	FVector RelativeScale = {1, 1, 1};
	FTransform ComponentToWorld;
	TObjectPtr<GSceneComponent>	AttachParent;
	TArray<TObjectPtr<GSceneComponent>>	ChildComponents;

	bool bRelativeLocation = true;
	bool bRelativeRotation = true;
	bool bRelataiveScale = true;


																	   
};