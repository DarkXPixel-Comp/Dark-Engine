add_library(PhysFS INTERFACE)

CPMAddPackage(
	NAME PhysFS
	GITHUB_REPOSITORY icculus/physfs
	GIT_TAG main
#	DOWNLOAD_ONLY YES
)

target_link_libraries(PhysFS INTERFACE PhysFS::PhysFS)

