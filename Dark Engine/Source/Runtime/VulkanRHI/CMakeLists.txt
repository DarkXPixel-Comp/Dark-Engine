add_library(VulkanRHI)

target_compile_features(VulkanRHI PUBLIC cxx_std_20)
target_link_libraries(VulkanRHI PRIVATE Core RHI IMGUI Vulkan)



SETTINGFILES(VulkanRHI)
target_sources(VulkanRHI PRIVATE ${VulkanRHI})

target_include_directories(VulkanRHI PUBLIC "public")
target_include_directories(D3D12RHI PRIVATE "private")

SettingProject(VulkanRHI)

#install(TARGETS D3D12RHI)
