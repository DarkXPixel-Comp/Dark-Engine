add_library(ApplicationCore)

find_library(ApplicationCoreLibs Core Engine Input PATHS)


target_compile_features(ApplicationCore PUBLIC cxx_std_20)
target_link_libraries(ApplicationCore PRIVATE Core Engine Input IMGUI)
#target_link_libraries(ApplicationCore PUBLIC ${ApplicationCoreLibs})

if(WIN32)
	target_sources(ApplicationCore PRIVATE "private/Windows/WindowsWindow.cpp" "public/Windows/WindowsApplication.h" "public/GenericPlatform/GenericWindow.h" "public/GenericPlatform/GenericApplication.h" "private/Windows/WindowsApplication.cpp" "public/GenericPlatform/GenericWindowDefinition.h" "public/GenericPlatform/GenericApplicationMisc.h" "public/Windows/WindowsApplicationMisc.h" "private/Windows/WindowsApplicationMisc.cpp" "public/HAL/PlatformApplicationMisc.h" "public/GenericPlatform/GenericApplicationHandler.h")
	#set(ApplicationCore_Sources ${ApplicationCore_Sources} "private/Windows/WindowsWindow.cpp")
endif()

SettingProject(ApplicationCore)

target_compile_definitions(ApplicationCore PRIVATE APPLICATIONCORE_API=DLLEXPORT)

target_include_directories(ApplicationCore PUBLIC "public")

install(TARGETS ApplicationCore)