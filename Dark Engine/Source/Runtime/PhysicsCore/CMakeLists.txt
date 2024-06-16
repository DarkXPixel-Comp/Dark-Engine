add_library(PhysicsCore)


target_compile_features(PhysicsCore PUBLIC cxx_std_20)


target_link_libraries(PhysicsCore PUBLIC PhysX Core)


SETTINGFILES(PhysicsCoreSrc)
target_sources(PhysicsCore PRIVATE ${PhysicsCoreSrc})

SettingProject(PhysicsCore)

target_include_directories(PhysicsCore PUBLIC "public")

install(TARGETS PhysicsCore)
