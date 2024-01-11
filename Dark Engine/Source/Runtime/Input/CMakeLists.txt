add_library(Input)

find_library(InputLibs Core DTK12)

target_compile_features(Input PUBLIC cxx_std_20)
target_link_libraries(Input PUBLIC Core DTK12)
#target_link_libraries(Input PUBLIC ${InputLibs})

target_sources(Input PRIVATE "InputCore.cpp")
target_include_directories(Input PUBLIC "../Input")

install(TARGETS Input)
