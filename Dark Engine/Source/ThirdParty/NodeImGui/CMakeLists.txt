#add_library(IMGUI_NODE STATIC)

#target_compile_features(IMGUI_NODE PUBLIC cxx_std_20)

#target_include_directories(IMGUI_NODE PUBLIC "../NodeImGui")

#target_link_libraries(IMGUI_NODE PUBLIC IMGUI)

#set(IMGUI_Sources ${IMGUI_Sources} "public/imgui.h" "public/imconfig.h" "public/imgui_internal.h" "public/imstb_rectpack.h" "public/imstb_textedit.h" "public/imstb_truetype.h" "public/imgui_impl_dx12.h" "public/imgui_impl_win32.h" "public/imgui_stdlib.h"
#"private/imgui.cpp" "private/imgui_demo.cpp" "private/imgui_draw.cpp" "private/imgui_tables.cpp" "private/imgui_widgets.cpp" "private/imgui_impl_dx12.cpp" "private/imgui_impl_win32.cpp" "private/imgui_stdlib.cpp")

#target_sources(IMGUI_NODE PRIVATE "crude_json.cpp" "crude_json.h" "imgui_bezier_math.h" "imgui_bezier_math.inl"
#"imgui_canvas.cpp" "imgui_canvas.h" "imgui_extra_math.h" "imgui_extra_math.inl" "imgui_node_editor.cpp" "imgui_node_editor.h"
#"imgui_node_editor_api.cpp" "imgui_node_editor_internal.h" "imgui_node_editor_internal.inl")

#SettingProject(IMGUI_NODE)

#install(TARGETS IMGUI_NODE)
