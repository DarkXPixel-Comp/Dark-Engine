add_library(Optick INTERFACE)

CPMAddPackage(
	NAME OptickPackage
	GITHUB_REPOSITORY bombomby/optick
	GIT_TAG master
#	DOWNLOAD_ONLY YES
)

target_link_libraries(Optick INTERFACE OptickCore)

#CPMAddPackage("gh:bombomby/optick@1.1.2")

#if(OptickPackage_ADDED)
#	FILE(GLOB OptickPackage_sources ${OptickPackage_SOURCE_DIR}/src/*.cpp)
#	target_sources(Optick PRIVATE ${OptickPackage_sources})
#	target_include_directories(Optick PUBLIC ${OptickPackage_SOURCE_DIR}/src)
#endif()