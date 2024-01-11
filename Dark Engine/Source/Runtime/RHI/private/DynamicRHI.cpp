#include "RHI.h"
#include "DynamicRHI.h"
#include "Windows/WindowsDynamicRHI.cpp"
#include "RHICommandList.h"


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

void RHIPostInit();



void RHIExit();

