add_library(Python INTERFACE)

target_compile_features(Python INTERFACE cxx_std_20)



target_link_libraries(Python INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/libs/python312.lib")
target_link_libraries(Python INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/libs/python3.lib")
target_link_libraries(Python INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/libs/_tkinter.lib")


target_include_directories(Python INTERFACE "include")


#target_include_directories(Lua PRIVATE "private")


#SettingProject(Lua)

#install(TARGETS Lua)