#include "DynamicRHI.h"

#include "RHI.h"
#include "RHICommandList.h"
//#include "Windows/WindowsDynamicRHI.cpp"




FDynamicRHI* GDynamicRHI = nullptr;


void RHIInit()
{
	if (!GDynamicRHI)
	{
		GRHICommandList.LatchByPass();

		GDynamicRHI = PlatformCreateDynamicRHI();


		if (GDynamicRHI)
		{
			GDynamicRHI->Init();
		}


	}
	else
	{


	}


}

void RHIPostInit()
{
	GDynamicRHI->PostInit();
}

void RHIExit()
{
	GDynamicRHI->Shutdown();
	DynamicRHIShutdown();
}



void RHIExit();

TSharedPtr<FRHIViewport> CreateRHIViewport(void* WindowHandle, uint32 SizeX, uint32 SizeY, bool bIsFullscreen)
{
	return GDynamicRHI->RHICreateViewport(WindowHandle, SizeX, SizeY, bIsFullscreen);
}

