#include "DynamicRHI.h"

#include "D3D12RHI.h"


IDynamicRHIModule* GLoadedRHIModule = nullptr;



enum class EWindowsRHI
{
	D3D12,
	count
};



FDynamicRHI* PlatformCreateDynamicRHI()
{
	FDynamicRHI* DynamicRHI = nullptr;
	GLoadedRHIModule = new FD3D12DynamicRHIModule();

	if (GLoadedRHIModule)
	{
		DynamicRHI = GLoadedRHIModule->CreateRHI();
	}
	return DynamicRHI;
}

void DynamicRHIShutdown()
{
	if (GLoadedRHIModule)
	{
		GLoadedRHIModule->Shutdown();
		delete GLoadedRHIModule;
	}
}


