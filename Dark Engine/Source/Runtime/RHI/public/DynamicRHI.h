#pragma once
#include "RHIDefines.h"




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


	virtual ERHIInterfaceType GetInterfaceType() const { return ERHIInterfaceType::Hidden; }

};


class IDynamicRHI
{
public:
	virtual FDynamicRHI* CreateRHI() = 0;
};


FDynamicRHI* PlatformCreateDynamicRHI();
