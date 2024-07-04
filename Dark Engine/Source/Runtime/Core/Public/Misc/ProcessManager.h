#pragma once
#include "HAL/Process.h"
#include "Memory/TUniquePtr.h"
#include "Containers/Array.h"
#include "Misc/ScopeLock.h"

class FProcessManager
{
public:
	FORCEINLINE static TSharedPtr<FProcess>	NewProcess(const TCHAR* Path, const TCHAR* CmdLine, bool bInScopedClose = false)
	{
		TSharedPtr<FProcess> Process = MakeShareble(new FProcess(Path, CmdLine, bInScopedClose));

		CriticalSection.Lock();
		Processes.Add(Process);
		CriticalSection.Unlock();
		return Process;
	}

	FORCEINLINE static void UpdateProcesses()
	{
		FScopeLock Lock(&CriticalSection);

		std::erase_if(Processes.GetVector(), [](const TSharedPtr<FProcess>& InProcess)
			{
				if (!InProcess->CheckProcess())
				{
					InProcess->CloseProcess();
					return true;
				}
				return false;
			});
	}


	FORCEINLINE static void CloseAllProcesses()
	{
		FScopeLock Lock(&CriticalSection);
		for (auto& i : Processes)
		{
			i->CloseProcess();
		}
		Processes.Empty();
	}

private:
	static TArray<TSharedPtr<FProcess>>	Processes;
	static FCriticalSection CriticalSection;


};