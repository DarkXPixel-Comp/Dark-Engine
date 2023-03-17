#include "RotateComponent.h"
#include "../../Actor/Actor.h"


void URotateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URotateComponent::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	m_Owner->AddRotation({0, 0.5f * DeltaTime, 0});
}

void URotateComponent::Destroy()
{
	Super::Destroy();
}
