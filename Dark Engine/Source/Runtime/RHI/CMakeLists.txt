add_library(RHI)

find_library(RHILibs Core Engine D3D12RHI)


target_compile_features(RHI PUBLIC cxx_std_20)
target_link_libraries(RHI PRIVATE Core D3D12RHI IMGUI Engine VulkanRHI)
#target_link_libraries(RHI PUBLIC ${RHILibs})

if(MSVC AND WIN32)
	target_sources(RHI PRIVATE "private/Windows/WindowsDynamicRHI.cpp")
	#set(RHI_Sources ${RHI_Sources} "private/Windows/WindowsDynamicRHI.cpp")
endif()

#set(RHI_Sources ${RHI_Sources} "private/DynamicRHI.cpp" "private/RHI.cpp" "private/RHICommandList.cpp" "public/RHIResources.h")
target_sources(RHI PRIVATE "private/DynamicRHI.cpp" "private/RHI.cpp" "private/RHICommandList.cpp" "public/RHIResources.h" "public/PixelFormat.h" "public/RHIContext.h" "public/RHIPipeline.h" "private/RHIResources.cpp" "private/PipelineStateCache.cpp" "public/PipelineStateCache.h" "public/RHIShaderParameters.h")
target_include_directories(RHI PUBLIC "public")

SettingProject(RHI)



install(TARGETS RHI)
