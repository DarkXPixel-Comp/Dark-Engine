cmake_minimum_required(VERSION 3.8)

#Dark Engine
project("Dark Engine" VERSION 0.2.18.0)

include("${CMAKE_SOURCE_DIR}/cmake/Settings.cmake")

add_subdirectory("Dark Engine")

install(EXPORT DarkEngineConfig
    NAMESPACE DE::
    DESTINATION lib/DarkEngine
)

message(STATUS "INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")