add_library(NvidiaNsight INTERFACE)


target_include_directories(NvidiaNsight INTERFACE "include")
target_link_libraries(NvidiaNsight INTERFACE "lib/x64/GFSDK_Aftermath_Lib.x64.lib")