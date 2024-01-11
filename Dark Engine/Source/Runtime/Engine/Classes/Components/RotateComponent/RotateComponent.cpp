#include "RotateComponent.h"
#include "../../Actor/Actor.h"


void URotateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URotateComponent::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	XMFLOAT3 rot = m_rotation;

	rot.x *= DeltaTime;
	rot.y *= DeltaTime;
	rot.z *= DeltaTime;

	//Cast<URotateComponent>(GetOwner())->SetRotateOnTick({0, 0, 0});

	Cast<AActor>(GetOwner())->AddRotation(rot);
}

void URotateComponent::Destroy()
{
	Super::Destroy();
}
