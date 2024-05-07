add_library(Scripting)


target_compile_features(Scripting PUBLIC cxx_std_20)
target_link_libraries(Scripting PRIVATE Lua Core)
#target_link_libraries(RHI PUBLIC ${RHILibs})

SETTINGFILES(ScriptingSrc)
target_sources(Scripting PRIVATE ${ScriptingSrc})

target_include_directories(Scripting PUBLIC "public")
target_include_directories(Scripting PRIVATE "private")

SettingProject(Scripting)

