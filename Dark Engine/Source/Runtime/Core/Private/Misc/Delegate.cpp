#include "Misc/Delegate.h"

IContainer::IContainer()
{
	static uint64 IdCounter = 0;
	Id = IdCounter++;
}

uint64 IContainer::GetID() const
{
	return Id;
}
