#pragma once
#include "VulkanInterfaces.h"
#include "Windows.h"  
#include "Misc/Paths.h"


class FWindowsVulkanLoader : public IVulkanLoader
{
public:
	FORCEINLINE virtual bool LoadVulkanFunctions()
	{
		VulkanModule = LoadLibrary(TEXT("sl.interposer.dll"));
		if (!VulkanModule)
			return false;
		VK::vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(VulkanModule, "vkGetInstanceProcAddr");
		if (!VK::vkGetInstanceProcAddr)
			return false;
		VK::vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)GetProcAddress(VulkanModule, "vkGetDeviceProcAddr");
		VK::vkCreateInstance = (PFN_vkCreateInstance)VK::vkGetInstanceProcAddr(0, "vkCreateInstance");








		return true;
	}

private:
	HMODULE VulkanModule = NULL;
};
