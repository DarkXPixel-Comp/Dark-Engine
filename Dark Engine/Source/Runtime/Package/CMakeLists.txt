add_library(Package)


target_compile_features(Package PUBLIC cxx_std_20)
target_link_libraries(Package PUBLIC Core)
target_link_libraries(Package PRIVATE)

SettingFiles(PackageSrc)
target_sources(Package PRIVATE ${PackageSrc})


set(USE_PACKAGE_CONTENT 1)

target_compile_definitions(Package PUBLIC "USE_PACKAGE_CONTENT=${USE_PACKAGE_CONTENT}")


SettingProject(Package)
