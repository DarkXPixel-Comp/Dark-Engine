add_library(Renderer)


target_compile_features(Renderer PUBLIC cxx_std_20)
target_link_libraries(Renderer PUBLIC Core RHI Engine)
#target_link_libraries(RHI PUBLIC ${RHILibs})


target_sources(Renderer PRIVATE "public/RenderResource.h" "private/SceneRendering.h"
"private/RenderResource.cpp" "public/RendererInterface.h" "private/RenderGlobals.cpp" "private/SceneRenderer.h" "private/SceneRenderer.cpp" "private/SceneRendering.cpp")

SettingProject(Renderer)

target_include_directories(Renderer PUBLIC "public")

install(TARGETS Renderer)
