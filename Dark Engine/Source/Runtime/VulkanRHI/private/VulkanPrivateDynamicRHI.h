#pragma once
#include "Vulkan/vulkan.h"
#include "VulkanDynamicRHI.h"



class FVulkanDynamicRHI : public FDynamicRHI
{
public:
	//static FVulkanDynamicRHI& Get() {}

	FVulkanDynamicRHI() {}


	virtual void Init();
	//virtual void PostInit();

private:
	VkInstance Instance = NULL;

};