add_executable(Launch)


target_link_libraries(Launch PRIVATE Core ApplicationCore DarkUI Package ImGui JSON Optick Agility PhysFS)
target_compile_features(Launch PUBLIC cxx_std_20)


if(WIN32)
	set_target_properties(Launch PROPERTIES OUTPUT_NAME "Dark Engine" SUFFIX ".exe")
	#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:WinMain")
	set(CMAKE_AUTORCC ON)
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTOUIC ON)
	set_target_properties(Launch PROPERTIES WIN32_EXECUTABLE 1)
	if(${CMAKE_GENERATOR} MATCHES Ninja)
		target_sources(Launch PRIVATE "Resources/Windows/app.rc")
	elseif(${CMAKE_GENERATOR} MATCHES "Visual Studio 17 2022")
		target_sources(Launch PRIVATE "Resources/Windows/app.rc")
	endif()

	target_sources(Launch PRIVATE "Private/Windows/LaunchWindows.cpp" "Private/Launch.cpp" "Public/LaunchEngineLoop.h" "Private/LaunchEngineLoop.cpp")
endif()


SETTINGPROJECT(Launch)
target_include_directories(Launch PUBLIC "public")
target_include_directories(Launch PRIVATE "private")