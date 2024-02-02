add_library(UICore)


target_compile_features(UICore PUBLIC cxx_std_20)
target_link_libraries(UICore PUBLIC Core ApplicationCore)

target_sources(UICore PRIVATE "public/UICore.h" "public/Widgets/UIWindow.h" "private/UIWindow.cpp")
target_include_directories(UICore PUBLIC "public" "private")

SettingProject(UICore)

install(TARGETS UICore)