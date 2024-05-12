add_library(Assimp INTERFACE)


target_include_directories(Assimp INTERFACE "include")
target_link_libraries(Assimp INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/Debug/assimp-vc143-mtd.lib")
