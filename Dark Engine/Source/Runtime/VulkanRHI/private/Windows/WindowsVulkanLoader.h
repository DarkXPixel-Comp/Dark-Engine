#pragma once
#include "VulkanInterfaces.h"
#include "Windows.h"


class FWindowsVulkanLoader : public IVulkanLoader
{
public:
	FORCEINLINE virtual bool LoadVulkanFunctions()
	{
		VulkanModule = LoadLibrary(TEXT("vulkan-1.dll"));
		if (!VulkanModule)
			return false;
		VK::vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(VulkanModule, "vkGetInstanceProcAddr");
		if (!VK::vkGetInstanceProcAddr)
			return false;
		VK::vkCreateInstance = (PFN_vkCreateInstance)VK::vkGetInstanceProcAddr(0, "vkCreateInstance");








		return true;
	}

private:
	HMODULE VulkanModule = NULL;
};
