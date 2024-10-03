add_library(BSThreadPool INTERFACE)

target_compile_features(BSThreadPool INTERFACE cxx_std_20)

target_include_directories(BSThreadPool INTERFACE "include")


install(TARGETS BSThreadPool)