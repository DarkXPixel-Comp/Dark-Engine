#include "Actor.h"

void AActor::BeginPlay()
{
	Super::BeginPlay();
}

void AActor::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
	if (m_model)
	{
		m_model->SetPosition(m_position);
		m_model->SetRotation(m_rotation);
		m_model->SetScaling(m_scale);
	}
}

void AActor::Destroy()
{
}
