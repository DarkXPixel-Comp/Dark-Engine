add_library(CoreDObject)


target_compile_features(CoreDObject PUBLIC cxx_std_20)
target_link_libraries(CoreDObject PUBLIC Core Engine)
#target_link_libraries(Editor PUBLIC ${EditorLibs})



target_sources(CoreDObject PRIVATE "public/CoreDObject.h" "public/DObject/DObject.h")
target_include_directories(CoreDObject PUBLIC "public")
SettingProject(CoreDObject)
#install(TARGETS CoreDObject)


