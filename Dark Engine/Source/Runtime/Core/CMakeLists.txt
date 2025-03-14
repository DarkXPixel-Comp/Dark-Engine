add_library(Core)


target_compile_features(Core PUBLIC cxx_std_20)


set(VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(VERSION_TWEAK ${PROJECT_VERSION_TWEAK})
set(VERSION_PREFIX ${PROJECT_PREFIX})
set(BUILD_TYPE_PREFIX ${CMAKE_BUILD_TYPE})


configure_file("Private/Misc/EngineVersion.cpp.var" "${CMAKE_CURRENT_SOURCE_DIR}/Private/Misc/EngineVersion.cpp" @ONLY)

target_compile_definitions(Core PUBLIC "UNICODE=1")
target_compile_definitions(Core PUBLIC "_UNICODE=1")
#target_compile_definitions(Core PUBLIC "CORE_API=DLLEXPORT")
target_compile_definitions(Core PUBLIC "_CRT_SECURE_NO_WARNINGS")


if(WIN32)
target_compile_definitions(Core PUBLIC "PLATFORM_HEADER_NAME=Windows")
target_link_libraries(Core PRIVATE "Pathcch.lib")
endif()
SETTINGFILES(CoreSrc)
target_sources(Core PRIVATE ${CoreSrc})

#target_include_directories(Core PUBLIC "Public")
SettingProject(Core)
#message(${CMAKE_INSTALL_PREFIX})