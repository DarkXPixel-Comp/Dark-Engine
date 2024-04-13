#pragma once
#include "DynamicRHI.h"
#include "vulkan/vulkan.h"


class FVulkanDynamicRHI : public FDynamicRHI
{
public:
	//static FVulkanDynamicRHI& Get() {}

	FVulkanDynamicRHI();

	virtual VkInstance RHIGetVkInstance() const;
	virtual VkDevice RHIGetVkDevice() const;


	virtual void Init() override;
	virtual void PostInit() override;


};


class FVulkanDynamicRHIModule : public IDynamicRHIModule
{
public:
	FVulkanDynamicRHIModule();

	virtual FDynamicRHI* CreateRHI() override;

private:
};