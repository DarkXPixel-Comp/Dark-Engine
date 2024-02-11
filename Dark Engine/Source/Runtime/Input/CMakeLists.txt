add_library(Input)

find_library(InputLibs Core DTK12)

target_compile_features(Input PUBLIC cxx_std_20)
target_link_libraries(Input PRIVATE Core DTK12)
#target_link_libraries(Input PUBLIC ${InputLibs})

set(Input_Sources ${Input_Sources} "InputCore.cpp")

#GroupSourcesByFolder(Input "${Input_Sources}")


target_sources(Input PRIVATE "private/InputCore.cpp")
target_include_directories(Input PUBLIC "public")

SettingProject(Input)
install(TARGETS Input)
