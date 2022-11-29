#pragma once
#include <Windows.h>


class FWindowsCriticalSection
{
	CRITICAL_SECTION CriticalSection;

public:
	FWindowsCriticalSection()
	{
		InitializeCriticalSection(&CriticalSection);
		SetCriticalSectionSpinCount(&CriticalSection, 4000);
	}

	~FWindowsCriticalSection()
	{
		DeleteCriticalSection(&CriticalSection);
	}


	void Lock()
	{
		EnterCriticalSection(&CriticalSection);
	}
	
	bool TryLock()
	{
		if (TryEnterCriticalSection(&CriticalSection))
			return true;
		return false;
	}

	void Unlock()
	{
		LeaveCriticalSection(&CriticalSection);
	}

};


typedef FWindowsCriticalSection FCriticalSection;



class FScopeLock
{
public:

	FScopeLock(FCriticalSection* InSynchObject)
		: SynchObject(InSynchObject)
	{
		SynchObject->Lock();
	}

	~FScopeLock()
	{
		Unlock();
	}

	void Unlock()
	{
		if (SynchObject)
		{
			SynchObject->Unlock();
			SynchObject = nullptr;
		}
	}


private:
	FCriticalSection* SynchObject;
};