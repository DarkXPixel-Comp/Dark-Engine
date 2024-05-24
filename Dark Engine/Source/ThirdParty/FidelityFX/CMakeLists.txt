add_library(FidelityFX INTERFACE)


target_include_directories(FidelityFX INTERFACE "include")

if(WIN32)
SET(Libs "")
file(GLOB_RECURSE Libs "${CMAKE_CURRENT_SOURCE_DIR}/*.lib")

target_link_libraries(FidelityFX INTERFACE ${Libs})
endif()