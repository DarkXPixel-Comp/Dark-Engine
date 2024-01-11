add_library(RHI)

find_library(RHILibs Core Engine)


target_compile_features(RHI PUBLIC cxx_std_20)
target_link_libraries(RHI PUBLIC Core D3D12RHI)
#target_link_libraries(RHI PUBLIC ${RHILibs})

if(MSVC AND WIN32)
	target_sources(RHI PRIVATE "private/Windows/WindowsDynamicRHI.cpp")
endif()

target_sources(RHI PRIVATE "private/DynamicRHI.cpp" "private/RHI.cpp" "private/RHICommandList.cpp")
target_include_directories(RHI PUBLIC "public")

install(TARGETS RHI)
