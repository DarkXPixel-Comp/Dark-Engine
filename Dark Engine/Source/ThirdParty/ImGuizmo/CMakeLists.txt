add_library(ImGuizmo STATIC)

target_compile_features(ImGuizmo PUBLIC cxx_std_20)
target_link_libraries(ImGuizmo PRIVATE IMGUI)

target_include_directories(ImGuizmo PUBLIC "public")

target_sources(ImGuizmo PRIVATE "ImGuizmo.h" "ImGuizmo.cpp")
target_include_directories(ImGuizmo PUBLIC "../ImGuizmo")
SettingProject(ImGuizmo)

install(TARGETS ImGuizmo)
