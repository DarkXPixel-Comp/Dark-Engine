#pragma once
#include "RHIDefines.h"
#include <RHIResources.h>
#include "Memory/TUniquePtr.h"




class IDynamicRHIModule
{
public:
	virtual class FDynamicRHI* CreateRHI() = 0;


};



class FDynamicRHI
{
public:
	virtual void Init() = 0;
	virtual void PostInit() {}
	virtual void Shutdown() = 0;
	virtual TSharedPtr<FRHIViewport> RHICreateViewport(void* WindowHandle, uint32 SizeX, uint32 SizeY, bool bIsFullscreen) = 0;
	

	virtual ERHIInterfaceType GetInterfaceType() const { return ERHIInterfaceType::Hidden; }

};

extern FDynamicRHI* GDynamicRHI;


class IDynamicRHI
{
public:
	virtual FDynamicRHI* CreateRHI() = 0;
};


FDynamicRHI* PlatformCreateDynamicRHI();


TSharedPtr<FRHIViewport> CreateRHIViewport(void* WindowHandle, uint32 SizeX, uint32 SizeY, bool bIsFullscreen);
