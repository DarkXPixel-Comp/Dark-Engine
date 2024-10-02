#pragma once

#include "Components/BaseComponent.h"
#include "Math/MathFwd.h"
#include "Math/Transform.h"


class GSceneComponent : public GBaseComponent
{
	DECLARE_CLASS_INTINSIC_NO_CTOR_WITH_PROPERTIES(GSceneComponent, GBaseComponent,
		{
			DECLARE_PROPERTY(uint8, bVisible, Visible);
			DECLARE_PROPERTY(FVector, RelativeLocation, Location, PROPERTY_META(DECLARE_SETTER(SetWorldLocation)));
			DECLARE_PROPERTY(FVector, RelativeScale, Scale);
			DECLARE_PROPERTY(FRotator, RelativeRotation, Rotation);
		});

public:
	GSceneComponent(const FObjectInitializer& ObjectInitalizer = FObjectInitializer::Get());

	IMPLEMENT_SETTER(SetWorldLocation, FVector);
	virtual void SetWorldLocation(FVector NewLocation);
	void SetWorldRotation(FRotator NewRoatation);


	//void SetWorldLocation(void* Data, uint64)
	//{
	//	FVector* Vector = (FVector*)Data;
	//	SetWorldLocation(*Vector);
	//}

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


private:
	FVector RelativeLocation;
	FRotator RelativeRotation;
	FVector RelativeScale;
	FTransform ComponentToWorld;
	TObjectPtr<GSceneComponent>	AttachParent;


																	   
};