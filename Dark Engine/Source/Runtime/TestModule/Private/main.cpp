#include "Modules/ModuleManager.h"
#include "Platform/PlatformMisc.h"

class FTestModule : IModule
{
public:
	virtual void StartModule()
	{
		FPlatformMisc::CreateMessageBoxError(TEXT("HUY"), TEXT("hYU"));
	}

	virtual void ShutdownModule() {}
};

IMPLEMENT_DYNAMIC_MODULE(FTestModule, TestModule);
	

void test()
{

}