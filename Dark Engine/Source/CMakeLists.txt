if(WIN32)
	include_directories("Windows")
	add_definitions(/FI "Definitions.h")
	add_compile_definitions(USING_IMGUI)
endif()



add_subdirectory("Runtime")
add_subdirectory("ThirdParty")
add_subdirectory("Scripts")
