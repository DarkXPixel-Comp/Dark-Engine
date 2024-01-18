add_library(ApplicationCore)

find_library(ApplicationCoreLibs Core Engine Input PATHS)


target_compile_features(ApplicationCore PUBLIC cxx_std_20)
target_link_libraries(ApplicationCore PUBLIC Core Engine Input IMGUI)
#target_link_libraries(ApplicationCore PUBLIC ${ApplicationCoreLibs})

if(MSVC AND WIN32)
	target_sources(ApplicationCore PRIVATE "private/Windows/WindowsWindow.cpp")
endif()

target_include_directories(ApplicationCore PUBLIC "public")

install(TARGETS ApplicationCore)
