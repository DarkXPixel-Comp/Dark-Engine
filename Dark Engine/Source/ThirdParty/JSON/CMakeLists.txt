add_library(JSON INTERFACE)

target_compile_features(JSON INTERFACE cxx_std_20)

target_include_directories(JSON INTERFACE "include")


install(TARGETS JSON)