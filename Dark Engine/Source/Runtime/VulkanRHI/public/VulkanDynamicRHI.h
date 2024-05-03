#pragma once
#include "DynamicRHI.h"



class FVulkanDynamicRHIModule : public IDynamicRHIModule
{
public:
	FVulkanDynamicRHIModule() {}

	virtual FDynamicRHI* CreateRHI() override;

private:
};