#pragma once
#include "CoreTypes.h"
#include "HAL/PlatformMemory.h"
#include <Microsoft/MinimalWindowsApi.h>
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Misc/AssertionMacros.h"



class FWindowsProcess
{
public:
	FWindowsProcess(const TCHAR* Path, const TCHAR* CmdLine, bool bInScopedClose = false) : bAutoClose(bInScopedClose)
	{
		StartupInfo.cb = sizeof(StartupInfo);

		bCreated = CreateProcess(NULL, *FString::PrintF(TEXT("%s %s"), Path, CmdLine), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE, NULL, NULL, &StartupInfo, &ProcessInfo);
	}

	FORCEINLINE bool CheckProcess()	const
	{
		if (bCreated)
		{
			DWORD Code = WaitForSingleObject(ProcessInfo.hProcess, 0);
			if (Code == WAIT_FAILED || Code == WAIT_OBJECT_0 || Code == WAIT_ABANDONED)
			{
				return false;
			}
		}
		return true;
	}

	FORCEINLINE bool IsValid() const
	{
		return bCreated;
	}

	FORCEINLINE void WaitProcess(uint32 Milliseconds = INFINITE)
	{
		if (bCreated)
		{
			WaitForSingleObject(ProcessInfo.hProcess, Milliseconds);
		}
	}


	FORCEINLINE bool CloseProcess(uint32* OutExitCode = nullptr)
	{
		if (bCreated)
		{
			DWORD ExitCode = 0;
			if (GetExitCodeProcess(ProcessInfo.hProcess, &ExitCode) && OutExitCode)
			{
				*OutExitCode = ExitCode;
			}
			bCreated = false;
			CloseHandle(ProcessInfo.hProcess);
			CloseHandle(ProcessInfo.hThread);
			CloseHandle(StartupInfo.hStdError);
			CloseHandle(StartupInfo.hStdInput);
			CloseHandle(StartupInfo.hStdOutput);
			return true;
		}
		return false;
	}

	~FWindowsProcess()
	{
		if (bAutoClose)
		{
			CloseProcess();
		}
	}



private:
	bool bAutoClose = false;
	bool bCreated = false;
	STARTUPINFO StartupInfo = {};
	PROCESS_INFORMATION ProcessInfo = {};
};

typedef FWindowsProcess FProcess;