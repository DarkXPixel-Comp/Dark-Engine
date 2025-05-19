add_library(TestModule SHARED)

ADD_PUBLIC_LIBRARY(TestModule Core)

SettingProject(TestModule)

#install(TARGETS UICore)