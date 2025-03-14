cmake_minimum_required(VERSION 3.8)

include("${CMAKE_SOURCE_DIR}/cmake/Settings.cmake")

#Dark Engine
project("Dark Engine" VERSION 0.1.11.0)


#add_compile_definitions(UBT_COMPILED_PLATFORM=Windows ROOT_DIR="${PROJECT_SOURCE_DIR}")

#Output folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Dark Engine/Binaires") # 
set(CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/Dark Engine/Binaires")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Dark Engine/Binaires")
add_subdirectory("Dark Engine")

install(EXPORT DarkEngineConfig
    NAMESPACE DE::
    DESTINATION lib/DarkEngine
)