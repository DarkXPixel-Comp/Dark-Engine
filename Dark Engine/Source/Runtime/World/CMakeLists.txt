add_library(World)

find_library(WorldLibs Core Engine)

target_compile_features(World PUBLIC cxx_std_20)
target_link_libraries(World PUBLIC Core Engine)


#target_link_libraries(World PUBLIC ${WorldLibs})

target_sources(World PRIVATE "World.cpp")
target_include_directories(World PUBLIC "../World")

install(TARGETS World)
