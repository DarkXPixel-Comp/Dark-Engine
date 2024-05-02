add_library(VulkanRHI)

target_compile_features(VulkanRHI PUBLIC cxx_std_20)
target_link_libraries(VulkanRHI PRIVATE Core RHI IMGUI NvidiaStreamline)
target_link_libraries(VulkanRHI PRIVATE Vulkan)



SETTINGFILES(VulkanRHI)
target_sources(VulkanRHI PRIVATE ${VulkanRHI})

target_include_directories(VulkanRHI PUBLIC "public")
target_include_directories(D3D12RHI PRIVATE "private")


target_compile_definitions(VulkanRHI PRIVATE VULKAN_VERSION=VK_API_VERSION_1_3)
target_compile_definitions(VulkanRHI PRIVATE VK_ENGINE_VERSION_MAJOR=${PROJECT_VERSION_MAJOR})
target_compile_definitions(VulkanRHI PRIVATE VK_ENGINE_VERSION_MINOR=${PROJECT_VERSION_MINOR})
target_compile_definitions(VulkanRHI PRIVATE VK_ENGINE_VERSION_PATCH=${PROJECT_VERSION_PATCH})
target_compile_definitions(VulkanRHI PRIVATE VK_ENGINE_VERSION_TWEAK=${PROJECT_VERSION_TWEAK})

SettingProject(VulkanRHI)

#install(TARGETS D3D12RHI)
