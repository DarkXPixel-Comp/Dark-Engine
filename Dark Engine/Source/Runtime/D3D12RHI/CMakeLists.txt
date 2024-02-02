add_library(D3D12RHI)

find_library(D3D12RHILibs Core RHI)

target_compile_features(D3D12RHI PUBLIC cxx_std_20)
target_link_libraries(D3D12RHI PRIVATE Core RHI)
#target_link_libraries(D3D12RHI PUBLIC ${D3D12RHILibs})

if(MSVC AND WIN32)
target_sources(D3D12RHI PRIVATE "private/Windows/WindowsD3D12Device.cpp" "public/Windows/WindowsD3D12ThirdParty.h")
#set(D3D12RHI_Sources ${D3D12RHI_Sources} "private/Windows/WindowsD3D12Device.cpp" "public/Windows/WindowsD3D12ThirdParty.h")
endif()

#set(D3D12RHI_Sources ${D3D12RHI_Sources} "private/D3D12Adapter.cpp" "private/D3D12RHIPrivate.cpp" "private/D3D12Resources.h"
#"private/D3D12Device.h" "private/D3D12Device.cpp" "private/D3D12RHICommon.h" "private/D3D12Descriptors.h" 
#"private/D3D12Util.h" "public/d3dx12.h" "public/D3D12Defines.h" "public/D3D12DynamicRHI.h" "public/D3D12RHI.h"
#"public/D3D12RootSignatureDefinitions.h")

#GroupSourcesByFolder(D3D12RHI "${D3D12RHI_Sources}")

target_sources(D3D12RHI PRIVATE "private/D3D12Adapter.cpp" "private/D3D12RHIPrivate.cpp" "private/D3D12Resources.h"
"private/D3D12Device.h" "private/D3D12Device.cpp" "private/D3D12RHICommon.h" "private/D3D12Descriptors.h" 
"private/D3D12Util.h" "public/d3dx12.h" "public/D3D12Defines.h" "public/D3D12DynamicRHI.h" "public/D3D12RHI.h"
"public/D3D12RootSignatureDefinitions.h")
target_include_directories(D3D12RHI PUBLIC "public")

SettingProject(D3D12RHI)

#install(TARGETS D3D12RHI)
