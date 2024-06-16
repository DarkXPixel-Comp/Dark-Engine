add_library(Assimp INTERFACE)

target_compile_features(Assimp INTERFACE cxx_std_20)

target_link_libraries(Assimp INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/assimp-vc143-mtd.lib")

target_include_directories(Assimp INTERFACE "include")


install(TARGETS Assimp)