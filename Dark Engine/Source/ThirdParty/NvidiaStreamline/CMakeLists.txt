add_library(NvidiaStreamline INTERFACE)


target_include_directories(NvidiaStreamline INTERFACE "include")
if(WIN32)
	target_link_libraries(NvidiaStreamline INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/sl.interposer.lib")
endif()
