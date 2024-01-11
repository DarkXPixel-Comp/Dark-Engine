add_library(Editor)

find_library(EditorLibs Core Engine)

target_compile_features(Editor PUBLIC cxx_std_20)
target_link_libraries(Editor PUBLIC Core Engine)
#target_link_libraries(Editor PUBLIC ${EditorLibs})

target_sources(Editor PRIVATE "private/DEditor.cpp")
target_include_directories(Editor PUBLIC "public")
install(TARGETS Editor)


