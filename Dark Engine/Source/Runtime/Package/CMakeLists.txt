add_library(Package)

ADD_PUBLIC_LIBRARY(Package Core)

set(USE_PACKAGE_CONTENT 1)

target_compile_definitions(Package PUBLIC "USE_PACKAGE_CONTENT=${USE_PACKAGE_CONTENT}")


SettingProject(Package)
