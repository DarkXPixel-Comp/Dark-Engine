#include "Components/BaseComponent.h"



GBaseComponent::GBaseComponent(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	Owner = GetTypedOuter<EEntity>();

	ComponentTick.bCanEverTick = false;
	ComponentTick.bStartWithTickEnable = true;
	ComponentTick.TickGroup = TG_DuringPhysics;
	ComponentTick.Target = this;







}

void GBaseComponent::BeginPlay()
{
	check(!bBeginPlay);
	bBeginPlay = true;
}

void GBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, GBaseComponentTickFunction* ThisTickFunction)
{
	check(bBeginPlay);
}

void GBaseComponent::EndPlay()
{
	check(bBeginPlay);
	bBeginPlay = false;
}
