add_library(Renderer)


target_compile_features(Renderer PUBLIC cxx_std_20)


target_link_libraries(Renderer PUBLIC Core RHI Engine D3DShaderFormat)
target_link_libraries(Renderer PRIVATE BSThreadPool)
#target_link_libraries(RHI PUBLIC ${RHILibs})
#target_link_options(Renderer PUBLIC "LINKER:/WHOLEARCHIVE")


#target_sources(Renderer PRIVATE "public/RenderResource.h" "private/SceneRendering.h"
#"private/RenderResource.cpp" "public/RendererInterface.h" "private/RenderGlobals.cpp" "private/SceneRenderer.h" "private/SceneRenderer.cpp" "private/SceneRendering.cpp")

SETTINGFILES(RendererSrc)
target_sources(Renderer PRIVATE ${RendererSrc} "private/FractalRender.cpp")

SettingProject(Renderer)
target_compile_definitions(Renderer PRIVATE ASYNC_SHADER_COMPILING=1)

target_include_directories(Renderer PUBLIC "public")

install(TARGETS Renderer)
