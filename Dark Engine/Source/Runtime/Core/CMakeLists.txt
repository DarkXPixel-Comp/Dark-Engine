add_library(Core SHARED)


target_compile_features(Core PUBLIC cxx_std_20)


set(VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(VERSION_TWEAK ${PROJECT_VERSION_TWEAK})
set(VERSION_PREFIX alpha)
set(BUILD_TYPE_PREFIX ${CMAKE_BUILD_TYPE})


configure_file("Private/Misc/EngineVersion.cpp.var" "${CMAKE_CURRENT_SOURCE_DIR}/Private/Misc/EngineVersion.cpp" @ONLY)

target_compile_definitions(Core PUBLIC "UNICODE=1")
target_compile_definitions(Core PUBLIC "_UNICODE=1")

if(WIN32)
target_compile_definitions(Core PUBLIC "PLATFORM_HEADER_NAME=Windows")
endif()
SETTINGFILES(CoreSrc)
target_sources(Core PRIVATE ${CoreSrc})

target_include_directories(Core PUBLIC "Public")
target_include_directories(Core PRIVATE "Private")
SettingProject(Core)