add_library(Engine)

find_library(EngineLibs Core Render Input ApplicationCore World Editor RHI)

target_compile_features(Engine PUBLIC cxx_std_20)
target_link_libraries(Engine PUBLIC Input Editor Core RHI Renderer UICore STB)
target_link_libraries(Engine PRIVATE ApplicationCore CoreGObject Lua)
#target_link_libraries(Engine PUBLIC ${EngineLibs})

#set(Engine_Sources ${Engine_Sources} "private/DEngine.cpp" "Classes/Actor/Actor.cpp"  "Classes/Camera/CameraActor.cpp" "Classes/Components/ActorComponent/ActorComponent.cpp"
#"Classes/Components/RotateComponent/RotateComponent.cpp" "Classes/Components/PrimitiveComponent/PrimitiveComponent.cpp"
#"Classes/Components/SceneComponent/USceneComponent.cpp" "Classes/TestActor/TestActor.cpp" "Classes/UObject/UObject.cpp"
# "Classes/Engine/GameViewportClient.h" "Classes/Engine/SceneViewport.h" "public/ViewportClient.h" "public/DarkClient.h" "Classes/Engine/EditorViewportClient.h")


SettingFiles(EngineSrc)
target_sources(Engine PRIVATE ${EngineSrc})



#target_sources(Engine PRIVATE "private/DEngine.cpp" "Classes/Actor/Actor.cpp"  "Classes/Camera/CameraActor.cpp" "Classes/Components/ActorComponent/ActorComponent.cpp"
#"Classes/Components/RotateComponent/RotateComponent.cpp" "Classes/Components/PrimitiveComponent/PrimitiveComponent.cpp"
#"Classes/Components/SceneComponent/USceneComponent.cpp" "Classes/TestActor/TestActor.cpp" "Classes/UObject/UObject.cpp"
#"Classes/Engine/GameViewportClient.h" "Classes/Engine/SceneViewport.h" "public/ViewportClient.h" "public/DarkClient.h"
#"Classes/Engine/EditorViewportClient.h" "private/Canvas.cpp" 
#"Classes/Engine/EditorEngine.h" "Classes/Engine/Engine.h" "private/EditorEngine.cpp" "private/EditorViewportClient.cpp"
#"private/DarkClient.cpp" "public/Canvas.h" "public/SceneInterface.h" "private/SceneView.cpp" "Classes/Camera/CameraTypes.h" "private/CameraTypes.cpp" "private/Engine.cpp" "Classes/Engine/LevelEditorViewport.h")
target_include_directories(Engine PUBLIC "public" "Classes")

SettingProject(Engine)

#install(TARGETS Engine)
