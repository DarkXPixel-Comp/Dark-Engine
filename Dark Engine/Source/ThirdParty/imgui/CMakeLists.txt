add_library(imgui INTERFACE)

find_package(imgui CONFIG REQUIRED)

target_link_libraries(imgui INTERFACE imgui::imgui)
