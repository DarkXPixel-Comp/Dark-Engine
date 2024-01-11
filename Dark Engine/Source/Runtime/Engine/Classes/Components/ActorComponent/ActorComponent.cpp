#include "ActorComponent.h"

#include <Actor/Actor.h>


UActorComponent::UActorComponent()
{
}

void UActorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UActorComponent::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
}


void UActorComponent::Destroy()
{
	Super::Destroy();
}

//AActor* UActorComponent::GetOwner()
//{
//	return m_Owner;
//}
