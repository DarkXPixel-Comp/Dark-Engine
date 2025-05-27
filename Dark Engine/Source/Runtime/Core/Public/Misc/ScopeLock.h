#pragma	once
#include "Platform/PlatformCriticalSection.h"
#include "Misc/AssertionMacros.h"



class FScopeLock
{
public:
	FScopeLock(FCriticalSection* InCriticalSection) :
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