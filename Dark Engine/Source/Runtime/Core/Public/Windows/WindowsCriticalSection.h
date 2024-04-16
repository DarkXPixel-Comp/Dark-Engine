#pragma once
#include "Windows/MinimalWindowsApi.h"


class FWindowsCriticalSection
{
public:
	FWindowsCriticalSection()
	{
		InitializeCriticalSectionAndSpinCount(&CriticalSection, 4000);
	}

	~FWindowsCriticalSection()
	{
		DeleteCriticalSection(&CriticalSection);
	}

	void Lock()
	{
		EnterCriticalSection(&CriticalSection);
	}

	void Unlock()
	{
		LeaveCriticalSection(&CriticalSection);
	}

	bool TryLock()
	{
		return TryEnterCriticalSection(&CriticalSection);
	}


private:
	CRITICAL_SECTION CriticalSection;
};


class FWindowsSystemWideCriticalSection
{
public:
	FWindowsSystemWideCriticalSection(const class FString& InName);

	~FWindowsSystemWideCriticalSection();

	bool IsValid() const;

	void Release();

private:
	HANDLE Mutex;
};


typedef FWindowsCriticalSection FCriticalSection;
typedef FWindowsSystemWideCriticalSection FSystemWideCriticalSection;
