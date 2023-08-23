#include "ActorComponent.h"

#include <Engine/Classes/Actor/Actor.h>


UActorComponent::UActorComponent(AActor* actor)
{
	m_Owner = actor;
	BeginPlay();
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

AActor* UActorComponent::GetOwner()
{
	return m_Owner;
}
