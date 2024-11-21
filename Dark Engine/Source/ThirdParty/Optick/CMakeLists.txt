add_library(Optick INTERFACE)

target_compile_features(Optick INTERFACE cxx_std_20)

set(OPTICK_FOLDER_STATIC_LIBS "")

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	set(OPTICK_FOLDER_STATIC_LIBS "debug")
	target_compile_definitions(Optick INTERFACE USE_OPTICK=1)
elseif(${CMAKE_BUILD_TYPE} MATCHES "RelWithDebInfo")
	set(OPTICK_FOLDER_STATIC_LIBS "release")
	target_compile_definitions(Optick INTERFACE USE_OPTICK=1)
elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
	set(OPTICK_FOLDER_STATIC_LIBS "release")
	target_compile_definitions(Optick INTERFACE USE_OPTICK=0)
endif()

if(WIN32)
target_link_libraries(Optick INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/${OPTICK_FOLDER_STATIC_LIBS}/OptickCore.lib")
endif()

target_include_directories(Optick INTERFACE "include")
