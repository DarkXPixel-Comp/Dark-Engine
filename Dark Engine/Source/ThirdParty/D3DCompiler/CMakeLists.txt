add_library(D3DCompiler INTERFACE)

target_include_directories(D3DCompiler INTERFACE "inc")
target_link_libraries(D3DCompiler INTERFACE "dxcompiler.lib")

#target_link_libraries(D3DCompiler INTERFACE "lib/x64/dxcompiler")