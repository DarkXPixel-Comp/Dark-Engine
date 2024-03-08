add_executable(Scene PRIVATE "private/main.cpp" "../Launch/Resource/Windows/app.rc") # "Resource/Windows/app.o"




target_compile_features(Scene PUBLIC cxx_std_20)

set_target_properties(Scene PROPERTIES OUTPUT_NAME "Scene" SUFFIX ".exe")


if(MSVC AND WIN32)
	#set(CMAKE_AUTORCC ON)
	#set(CMAKE_AUTOMOC ON)
	#set(CMAKE_AUTOUIC ON)
	set_target_properties(Scene PROPERTIES WIN32_EXECUTABLE 1)
endif()



SettingProject(Scene)

target_include_directories(Scene PUBLIC "public")
target_include_directories(Scene PRIVATE "private")



install(TARGETS Scene)