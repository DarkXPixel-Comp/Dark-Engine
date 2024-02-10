add_library(UICore)


target_compile_features(UICore PUBLIC cxx_std_20)
target_link_libraries(UICore PUBLIC Core ApplicationCore IMGUI)

target_sources(UICore PRIVATE "public/UICore.h" "public/Widgets/UIWindow.h" "private/Widgets/UIWindow.cpp" "private/Application/UIApplication.cpp" "public/Application/UIApplication.h" "public/Widgets/UIViewport.h" "public/Rendering/RenderingComon.h" "public/Rendering/UIShaderResource.h" "public/Rendering/UIRenderer.h")
target_include_directories(UICore PUBLIC "public" "private")

SettingProject(UICore)

install(TARGETS UICore)