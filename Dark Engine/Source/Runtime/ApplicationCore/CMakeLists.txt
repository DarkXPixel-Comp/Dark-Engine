add_library(ApplicationCore)


target_compile_features(ApplicationCore PUBLIC cxx_std_20)
target_link_libraries(ApplicationCore PUBLIC Core)

SettingFiles(ApplicationCoreSrc)
target_sources(ApplicationCore PRIVATE ${ApplicationCoreSrc})

target_compile_definitions(ApplicationCore PUBLIC "APPLICATIONCORE_API=DLLEXPORT")

#target_sources(UICore PRIVATE "public/UICore.h" "public/Widgets/UIWindow.h" "private/Widgets/UIWindow.cpp" "private/Application/UIApplication.cpp" "public/Application/UIApplication.h" "public/Widgets/UIViewport.h" "public/Rendering/RenderingCommon.h" "public/Rendering/UIShaderResource.h" "public/Rendering/UIRenderer.h" "public/Rendering/ElementBatcher.h" "public/Rendering/UIRHIRenderer.h" "private/Rendering/UIRHIRenderer.cpp" "public/Rendering/SceneViewport.h" "public/Widgets/UIWidget.h" "private/Widgets/UIViewport.cpp" "private/Rendering/UIRenderer.cpp" "public/Application/UIImgui.h" "public/Widgets/UIMainMenuBar.h" "private/Widgets/UIMainMenuBar.cpp" "public/Widgets/UIMenu.h" "private/Widgets/UIMenu.cpp" "public/Widgets/UIButton.h" "private/Widgets/UIButton.cpp" "private/Widgets/UIWidget.cpp" "public/Widgets/UINodeEditor.h" "private/Widgets/UINodeEditor.cpp" "public/Widgets/UISideWidget.h" "private/Widgets/UISideWidget.cpp" "public/Widgets/UIWidgetTest.h" "private/Widgets/UIWidgetTest.cpp" "public/Widgets/UIDock.h" "private/Widgets/UIDock.cpp" "public/Widgets/UILogs.h" "private/Widgets/UILogs.cpp" "public/Widgets/UILevelEditor.h" "public/Widgets/UIEditorViewport.h")
target_include_directories(ApplicationCore PUBLIC "Public" "Private")

SettingProject(ApplicationCore)

#install(TARGETS UICore)