#add_subdirectory("Assimp")
#add_subdirectory("DTK12")
#add_subdirectory("D3DCompiler")
#add_subdirectory("DearImGui")
#add_subdirectory("NvidiaNsight")
#add_subdirectory("cmrc-master")
#add_subdirectory("NodeImGui")
#add_subdirectory("WinPix")
#add_subdirectory("Vulkan")




SUBDIRLIST(Subs ${CMAKE_CURRENT_SOURCE_DIR})

foreach(sub ${Subs})
add_subdirectory(${sub})
endforeach()
