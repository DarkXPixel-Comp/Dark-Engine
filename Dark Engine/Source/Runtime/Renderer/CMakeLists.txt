add_library(Renderer)


target_compile_features(Renderer PUBLIC cxx_std_20)
target_link_libraries(Renderer PRIVATE Core RHI)
#target_link_libraries(RHI PUBLIC ${RHILibs})

if(MSVC AND WIN32)
	target_sources(Renderer PRIVATE "private/Windows/WindowsDynamicRHI.cpp")
endif()

target_sources(Renderer PRIVATE "public/RenderResource.h" "public/SceneRendering.h" "private/RenderResource.cpp")

SettingProject(Renderer)

target_include_directories(Renderer PUBLIC "public")

#install(TARGETS Renderer)
