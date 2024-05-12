add_library(DTK12 INTERFACE)


target_link_libraries(DTK12 INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/DirectXTK12.lib")
target_include_directories(DTK12 INTERFACE "../DTK12")
