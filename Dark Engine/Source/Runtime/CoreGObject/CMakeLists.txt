add_library(CoreGObject)


target_compile_features(CoreGObject PUBLIC cxx_std_20)
target_link_libraries(CoreGObject PUBLIC Core Engine Lua)
#target_link_libraries(Editor PUBLIC ${EditorLibs})



SETTINGFILES(CoreGObjectSrc)
target_sources(CoreGObject PRIVATE ${CoreGObjectSrc})


target_include_directories(CoreGObject PUBLIC "public")
target_include_directories(CoreGObject PRIVATE "private")
SettingProject(CoreGObject)

install(TARGETS CoreGObject)


