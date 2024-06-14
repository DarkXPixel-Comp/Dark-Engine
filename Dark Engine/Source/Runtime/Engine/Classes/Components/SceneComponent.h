#pragma once

#include "Components/BaseComponent.h"
#include "Math/MathFwd.h"


class GSceneComponent : public GBaseComponent
{
	DECLARE_CLASS_INTINSIC_NO_CTOR_WITH_PROPERTIES(GSceneComponent, GBaseComponent,
		{
			DECLARE_PROPERTY(uint8, bVisible, Visible);
			DECLARE_PROPERTY(FVector, RelativeLocation, Location);
			DECLARE_PROPERTY(FVector, RelativeScale, Scale);
			DECLARE_PROPERTY(FRotator, RelativeRotation, Rotation);
		});

public:
	GSceneComponent(const FObjectInitializer& ObjectInitalizer = FObjectInitializer::Get());

	const FRotator& GetRotation() const { return RelativeRotation; }
	const FVector& GetLocation() const { return RelativeLocation; }
	const FVector& GetScale() const { return RelativeScale; }

	uint8 bVisible;


private:
	FVector RelativeLocation;
	FRotator RelativeRotation;
	FVector RelativeScale;

};