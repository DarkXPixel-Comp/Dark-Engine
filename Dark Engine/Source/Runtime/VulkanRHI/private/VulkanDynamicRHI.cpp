#include "VulkanPrivateDynamicRHI.h"
#include "VulkanInterfaces.h"
#include "Windows/WindowsVulkanLoader.h"
#include "VulkanUtil.h"

TSharedPtr<IVulkanLoader> GVulkanLoader = nullptr;


FDynamicRHI* FVulkanDynamicRHIModule::CreateRHI()
{
#ifdef WIN32
	GVulkanLoader = MakeShareble(new FWindowsVulkanLoader());
#endif
	checkAssert(GVulkanLoader && "Your OS not support vulkan api");
	checkAssert(GVulkanLoader->LoadVulkanFunctions());


	return nullptr;
}

void FVulkanDynamicRHI::Init()
{
	VkApplicationInfo ApplicationInfo = {};
	ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	//ApplicationInfo.apiVersion = VK_API_VERSION_1_3;
	ApplicationInfo.apiVersion = VULKAN_VERSION;
	//ApplicationInfo.engineVersion = VK_MAKE_API_VERSION(0, 0, 47, 0);
	ApplicationInfo.engineVersion = VK_MAKE_API_VERSION(VK_ENGINE_VERSION_MAJOR, VK_ENGINE_VERSION_MINOR, VK_ENGINE_VERSION_PATCH, 0);
	ApplicationInfo.pEngineName = "Dark Engine";



	VkInstanceCreateInfo CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	CreateInfo.pApplicationInfo = &ApplicationInfo;
	CreateInfo.enabledExtensionCount = 0;
	CreateInfo.enabledLayerCount = 0;

	VkCall(VK::vkCreateInstance(&CreateInfo, nullptr, &Instance));
}
