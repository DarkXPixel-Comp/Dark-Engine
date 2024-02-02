if(MSVC AND WIN32)
	include_directories("Windows")
	add_definitions(/FI "Definitions.h")
endif()



add_subdirectory("Runtime")
add_subdirectory("ThirdParty")
add_subdirectory("Scripts")