add_library(d3dx12)

target_compile_features(d3dx12 PUBLIC cxx_std_20)

target_link_libraries(d3dx12 PUBLIC D3D12)
target_include_directories(d3dx12 PUBLIC "include")

target_sources(d3dx12 PRIVATE "src/d3dx12_property_format_table.cpp")
target_sources(d3dx12 PRIVATE "include/d3dx12/d3dx12.h" "include/d3dx12/d3dx12_barriers.h" "include/d3dx12/d3dx12_check_feature_support.h" 
"include/d3dx12/d3dx12_core.h" "include/d3dx12/d3dx12_default.h" "include/d3dx12/d3dx12_pipeline_state_stream.h"
"include/d3dx12/d3dx12_property_format_table.h" "include/d3dx12/d3dx12_render_pass.h" "include/d3dx12/d3dx12_resource_helpers.h"
"include/d3dx12/d3dx12_root_signature.h" "include/d3dx12/d3dx12_state_object.h")

SettingProject(d3dx12)

install(TARGETS d3dx12)
