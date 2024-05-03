#pragma once
#include "vulkan/vulkan_core.h"


class IVulkanLoader
{
public:
	virtual bool LoadVulkanFunctions() = 0;
};


namespace VK
{
	extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
	extern PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
	extern PFN_vkCreateInstance vkCreateInstance;
}