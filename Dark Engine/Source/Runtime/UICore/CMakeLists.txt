add_library(UICore)


target_compile_features(UICore PUBLIC cxx_std_20)
target_link_libraries(UICore PUBLIC Core ApplicationCore IMGUI DTK12 Engine STB ImGuizmo NativeFileDialog)
#target_link_libraries(UICore PRIVATE ImGuizmo)


SettingFiles(UICoreSrc)
target_sources(UICore PRIVATE ${UICoreSrc})


#target_sources(UICore PRIVATE "public/UICore.h" "public/Widgets/UIWindow.h" "private/Widgets/UIWindow.cpp" "private/Application/UIApplication.cpp" "public/Application/UIApplication.h" "public/Widgets/UIViewport.h" "public/Rendering/RenderingCommon.h" "public/Rendering/UIShaderResource.h" "public/Rendering/UIRenderer.h" "public/Rendering/ElementBatcher.h" "public/Rendering/UIRHIRenderer.h" "private/Rendering/UIRHIRenderer.cpp" "public/Rendering/SceneViewport.h" "public/Widgets/UIWidget.h" "private/Widgets/UIViewport.cpp" "private/Rendering/UIRenderer.cpp" "public/Application/UIImgui.h" "public/Widgets/UIMainMenuBar.h" "private/Widgets/UIMainMenuBar.cpp" "public/Widgets/UIMenu.h" "private/Widgets/UIMenu.cpp" "public/Widgets/UIButton.h" "private/Widgets/UIButton.cpp" "private/Widgets/UIWidget.cpp" "public/Widgets/UINodeEditor.h" "private/Widgets/UINodeEditor.cpp" "public/Widgets/UISideWidget.h" "private/Widgets/UISideWidget.cpp" "public/Widgets/UIWidgetTest.h" "private/Widgets/UIWidgetTest.cpp" "public/Widgets/UIDock.h" "private/Widgets/UIDock.cpp" "public/Widgets/UILogs.h" "private/Widgets/UILogs.cpp" "public/Widgets/UILevelEditor.h" "public/Widgets/UIEditorViewport.h")
target_include_directories(UICore PUBLIC "public" "private")

SettingProject(UICore)

#install(TARGETS UICore)