#include "VulkanInterfaces.h"
#include "vulkan/vulkan_core.h"

namespace VK
{
	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
	PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
	PFN_vkCreateInstance vkCreateInstance;
}