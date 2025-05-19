#include "Modules/ModuleManager.h"
#include "Platform/PlatformMisc.h"
#include "TestModule.h"

class FTestModule : IModule
{
public:
	virtual void StartModule()
	{
		FPlatformMisc::CreateMessageBoxError(TEXT("HUY"), TEXT("hYUss"));
	}

	virtual void ShutdownModule() {}
};

IMPLEMENT_DYNAMIC_MODULE(FTestModule, TestModule);
	

void testTestModule()
{
	FPlatformMisc::CreateMessageBoxError(TEXT("HUY"), TEXT("hYUss"));

}