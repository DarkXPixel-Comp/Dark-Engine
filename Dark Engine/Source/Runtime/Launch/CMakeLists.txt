add_executable(Launch) # "Resource/Windows/app.o"




#set(CMAKE_CXX_STANDARD 20)
target_link_libraries(Launch PRIVATE Core Engine IMGUI RHI ApplicationCore UICore)
#target_link_libraries(Launch PUBLIC ${LaunchLibs})
target_compile_features(Launch PUBLIC cxx_std_20)

set_target_properties(Launch PROPERTIES OUTPUT_NAME "Dark Engine" SUFFIX ".exe")
#set_target_properties(Launch PROPERTIES
#	ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Dark Engine/bin"
#	LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Dark Engine/bin"
#	RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Dark Engine/bin")


if(MSVC AND WIN32)
	set(CMAKE_AUTORCC ON)
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTOUIC ON)
	set_target_properties(Launch PROPERTIES WIN32_EXECUTABLE 1)

	set(LAUNCH_RESOURCES "Resource/Windows/app.rc" "Resource/Windows/Logo.ico" "Resource/Windows/resource.h")
	target_sources(Launch PRIVATE "private/Windows/LaunchWindows.cpp")
	target_sources(Launch PRIVATE "Resource/Windows/app.o")
	set(Launch_Sources ${Launch_Sources} "private/Windows/LaunchWindows.cpp")
endif()


#foreach(item IN LISTS LAUNCH_RESOURCES)
#message(STATUS ${item})
#add_custom_command(OUTPUT "${CMAKE}")
#endforeach()

#add_custom_target(Launch_Resources ALL DEPENDS LAUNCH_RESOURCES)

set(Launch_Sources ${Launch_Sources} "private/Launch.cpp")

#GroupSourcesByFolder(Launch "${Launch_Sources}")

target_sources(Launch PRIVATE "Private/Launch.cpp" "private/LaunchEngineLoop.cpp")


SettingProject(Launch)

target_include_directories(Launch PUBLIC "public")
target_include_directories(Launch PRIVATE "private")



install(TARGETS Launch)