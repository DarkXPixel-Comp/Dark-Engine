#include "Modules/ModuleManager.h"
#include "Platform/PlatformMisc.h"
#include "TestModule.h"

class FTestModule : IModule
{
public:
	virtual void StartModule()
	{
		FPlatformMisc::CreateMessageBoxError(TEXT("HUY"), TEXT("hYUss"));
		testTestModule();
	}

	virtual void ShutdownModule() {}
};

IMPLEMENT_DYNAMIC_MODULE(FTestModule, TestModule);
	

void testTestModule()
{
	FPlatformMisc::CreateMessageBoxError(TEXT("HUY123"), TEXT("hYUss321"));

}