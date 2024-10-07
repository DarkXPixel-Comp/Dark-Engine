add_library(NativeFileDialog INTERFACE)

target_compile_features(NativeFileDialog INTERFACE cxx_std_20)

set(NativeFileDialog_FOLDER_STATIC_LIBS "")


if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	set(NativeFileDialog_FOLDER_STATIC_LIBS "Debug")
elseif(${CMAKE_BUILD_TYPE} MATCHES "RelWithDebInfo")
	set(NativeFileDialog_FOLDER_STATIC_LIBS "Release")
elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
	set(NativeFileDialog_FOLDER_STATIC_LIBS "Release")
endif()



target_link_libraries(NativeFileDialog INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/libs/${NativeFileDialog_FOLDER_STATIC_LIBS}/x64/nfd.lib")


target_include_directories(NativeFileDialog INTERFACE "include")
