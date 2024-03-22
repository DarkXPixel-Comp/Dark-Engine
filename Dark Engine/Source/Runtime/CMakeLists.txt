SUBDIRLIST(Subs ${CMAKE_CURRENT_SOURCE_DIR})
foreach(sub ${Subs})
add_subdirectory(${sub})
endforeach()

