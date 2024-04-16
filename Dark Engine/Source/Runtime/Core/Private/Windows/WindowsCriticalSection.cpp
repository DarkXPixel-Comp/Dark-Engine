#include "Windows/WindowsCriticalSection.h"
#include "Containers/DarkString.h"



FWindowsSystemWideCriticalSection::FWindowsSystemWideCriticalSection(const FString& InName)
{
	FString MutexName = InName;
	MutexName.Replace(TEXT('\\'), TEXT('/'));

	Mutex = CreateMutex(NULL, true, *MutexName);

	if (Mutex != 0 && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(Mutex);
		Mutex = 0;
	}
}

FWindowsSystemWideCriticalSection::~FWindowsSystemWideCriticalSection()
{
	Release();
}

bool FWindowsSystemWideCriticalSection::IsValid() const
{
	return Mutex != 0;
}

void FWindowsSystemWideCriticalSection::Release()
{
	if (IsValid())
	{
		ReleaseMutex(Mutex);
		CloseHandle(Mutex);
		Mutex = 0;
	}
}
