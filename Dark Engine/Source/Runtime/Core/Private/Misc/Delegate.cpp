#include "Misc/Delegate.h"

uint64 Core::Delegate::GenerateContainerId()
{
	static uint64 CurrentId = 1;
	return CurrentId++;
}
