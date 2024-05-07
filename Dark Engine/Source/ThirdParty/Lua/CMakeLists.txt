add_library(Lua)

target_compile_features(Lua PUBLIC cxx_std_20)


SETTINGFILES(LuaSrc)
target_sources(Lua PRIVATE ${LuaSrc})

target_include_directories(Lua PUBLIC "public")
target_include_directories(Lua PRIVATE "private")


SettingProject(Lua)

#install(TARGETS D3D12RHI)
