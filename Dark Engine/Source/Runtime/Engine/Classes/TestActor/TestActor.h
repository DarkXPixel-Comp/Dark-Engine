#pragma once


#include "../Actor/Actor.h"
#include "../Components/RotateComponent/RotateComponent.h"

class ATestActor : public AActor
{
	typedef AActor Super;
public:
	ATestActor();

	void BeginPlay() override;
	void Update(float DeltaTime) override;
	void Destroy() override;

public:
	URotateComponent* rotateComponent;



};