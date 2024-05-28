add_library(Lua INTERFACE)

target_compile_features(Lua INTERFACE cxx_std_20)

target_link_libraries(Lua INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/lua5.1.lib")
target_link_libraries(Lua INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/lua51.lib")
target_link_libraries(Lua INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/luajit.lib")
target_link_libraries(Lua INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/minilua.lib")
target_link_libraries(Lua INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/buildvm.lib")



#SETTINGFILES(LuaSrc)
#target_sources(Lua private ${LuaSrc})

target_include_directories(Lua INTERFACE "public")
#target_include_directories(Lua PRIVATE "private")


#SettingProject(Lua)

#install(TARGETS Lua)
