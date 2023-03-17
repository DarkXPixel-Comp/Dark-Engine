#pragma once
#include "../ActorComponent/ActorComponent.h"

class AActor;

class URotateComponent : public UActorComponent
{
	typedef UActorComponent Super;

public:
	URotateComponent(AActor* actor) : Super(actor) {}
	void BeginPlay() override;
	void Update(float DeltaTime) override;
	void Destroy() override;



};