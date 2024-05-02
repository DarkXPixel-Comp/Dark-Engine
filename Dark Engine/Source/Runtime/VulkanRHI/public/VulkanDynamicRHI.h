#pragma once
#include "DynamicRHI.h"



class FVulkanDynamicRHIModule : public IDynamicRHIModule
{
public:
	FVulkanDynamicRHIModule() {}

	virtual FDynamicRHI* CreateRHI() override;
	virtual void Shutdown() override {}

private:
};