#include "USceneComponent.h"

USceneComponent::USceneComponent()
{
}

void USceneComponent::BeginPlay()
{	
	Super::BeginPlay();
}

void USceneComponent::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

}

void USceneComponent::Destroy()
{
	Super::Destroy();
}

void USceneComponent::SetPosition(FVector pos)
{
	m_position = pos + m_positionOffset;
	for (auto& i : m_childComponents) i->SetPosition(m_position);
}

void USceneComponent::SetRotation(FVector rot)
{
	m_rotation = rot + m_rotationOffset;
	for (auto& i : m_childComponents) i->SetRotation(m_rotation);
}

void USceneComponent::SetScaling(FVector sc)
{
	m_scale = sc + m_scaleOffset;
	for (auto& i : m_childComponents) i->SetScaling(m_scale);
}

void USceneComponent::SetPositionOffset(FVector pos)
{
	m_positionOffset = pos;
}

void USceneComponent::SetRotationOffset(FVector rot)
{
	m_rotationOffset = rot;
}

void USceneComponent::SetScalingOffset(FVector sc)
{
	m_scaleOffset = sc;
}

void USceneComponent::SetChild(USceneComponent* component)
{
	m_childComponents.push_back(component);
}


