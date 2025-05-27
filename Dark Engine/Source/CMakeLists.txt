#if(MSVC AND WIN32)
#	include_directories("Windows")
#	add_definitions(/FI "Definitions.h")
#	add_compile_definitions(USING_IMGUI)
#endif()

SUBDIRLIST(Subs ${CMAKE_CURRENT_SOURCE_DIR})

#foreach(sub ${Subs})
#add_subdirectory(${sub})
#endforeach()

add_subdirectory("Runtime")
add_subdirectory("ThirdParty")
#add_subdirectory("Scripts")