#include "UObject.h"

void UObject::BeginPlay()
{
	for (auto& i : m_objects)
	{
		i->BeginPlay();
	}
}

void UObject::Update(float DeltaTime)
{
	for (auto& i : m_objects)
	{
		i->Update(DeltaTime);
	}
}

void UObject::Destroy()
{
	for (auto& i : m_objects)
	{
		if (i)
		{
			i->Destroy();
			delete i;
		}
	}
	m_objects.clear();
}

void UObject::SetOwner(UObject* owner)
{
	m_owner = owner;
}

UObject* UObject::GetOwner()
{
	return m_owner;
}
