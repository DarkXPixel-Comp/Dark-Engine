#pragma once
#include "HAL/CriticalSection.h"
#include "AssertionMacros.h"


class FScopeLock
{
public:
	FScopeLock(FCriticalSection* InCriticalSection):
		CriticalSection(InCriticalSection)
	{
		check(CriticalSection);
		CriticalSection->Lock();
	}
	~FScopeLock()
	{
		Unlock();
	}

	void Unlock()
	{
		if (CriticalSection)
		{
			CriticalSection->Unlock();
			CriticalSection = nullptr;
		}
	}

private:
	FCriticalSection* CriticalSection;

};