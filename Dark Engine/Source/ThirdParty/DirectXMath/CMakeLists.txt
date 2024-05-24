add_library(DirectXMath)

target_compile_features(DirectXMath PUBLIC cxx_std_20)

target_include_directories(DirectXMath PUBLIC "Inc" "Extensions" "MatrixStack" "SHMath" "Stereo3D" "XDSP")

SETTINGFILES(DirectXMathSrc)
target_sources(DirectXMath PRIVATE ${DirectXMathSrc})

SettingProject(DirectXMath)

install(TARGETS DirectXMath)
