add_library(Engine)

find_library(EngineLibs Core Render Input ApplicationCore World Editor RHI)

target_compile_features(Engine PUBLIC cxx_std_20)
target_link_libraries(Engine PUBLIC Input ApplicationCore World Editor RHI Core Render)
#target_link_libraries(Engine PUBLIC ${EngineLibs})

target_sources(Engine PRIVATE "private/DEngine.cpp" "Classes/Actor/Actor.cpp"  "Classes/Camera/CameraActor.cpp" "Classes/Components/ActorComponent/ActorComponent.cpp"
"" "Classes/Components/RotateComponent/RotateComponent.cpp" "Classes/Components/PrimitiveComponent/PrimitiveComponent.cpp"
"Classes/Components/SceneComponent/USceneComponent.cpp" "Classes/TestActor/TestActor.cpp" "Classes/UObject/UObject.cpp"
 "Classes/Engine/GameViewportClient.h" "Classes/Engine/SceneViewport.h" "public/ViewportClient.h" "public/DarkClient.h")
target_include_directories(Engine PUBLIC "public" "Classes")

install(TARGETS Engine)
