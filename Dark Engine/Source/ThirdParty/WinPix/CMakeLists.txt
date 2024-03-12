add_library(WinPix INTERFACE)


target_include_directories(WinPix INTERFACE "inc")
target_link_libraries(WinPix INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/WinPixEventRuntime.lib")