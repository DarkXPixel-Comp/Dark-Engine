add_library(D3D12RHI)

find_library(D3D12RHILibs Core RHI)

target_compile_features(D3D12RHI PUBLIC cxx_std_20)
target_link_libraries(D3D12RHI PUBLIC Core RHI)
#target_link_libraries(D3D12RHI PUBLIC ${D3D12RHILibs})

if(MSVC AND WIN32)
target_sources(D3D12RHI PRIVATE "private/Windows/WindowsD3D12Device.cpp")
endif()

target_sources(D3D12RHI PRIVATE "private/D3D12Adapter.cpp" "private/D3D12RHIPrivate.cpp")
target_include_directories(D3D12RHI PUBLIC "public")

install(TARGETS D3D12RHI)
