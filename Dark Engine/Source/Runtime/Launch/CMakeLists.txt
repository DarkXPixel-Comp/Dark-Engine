add_executable(Launch)

find_library(LaunchLibs Core Engine)




#set(CMAKE_CXX_STANDARD 20)
target_link_libraries(Launch PRIVATE Core Engine IMGUI)
#target_link_libraries(Launch PUBLIC ${LaunchLibs})
target_compile_features(Launch PUBLIC cxx_std_20)

set_target_properties(Launch PROPERTIES OUTPUT_NAME "Dark Engine" SUFFIX ".exe")


if(MSVC AND WIN32)
	set_target_properties(Launch PROPERTIES WIN32_EXECUTABLE 1)
	target_sources(Launch PRIVATE "private/Windows/LaunchWindows.cpp")
endif()


target_sources(Launch PRIVATE "Private/Launch.cpp")
target_include_directories(Launch PUBLIC "public")

install(TARGETS Launch)