add_library(ImGui INTERFACE)

CPMAddPackage("gh:ocornut/imgui@1.91.5-docking")


target_link_libraries(ImGui INTERFACE imgui::imgui)
