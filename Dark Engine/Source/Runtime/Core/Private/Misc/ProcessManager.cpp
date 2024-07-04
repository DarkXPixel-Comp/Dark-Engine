#include "Misc/ProcessManager.h"




TArray<TSharedPtr<FProcess>> FProcessManager::Processes;
FCriticalSection FProcessManager::CriticalSection;