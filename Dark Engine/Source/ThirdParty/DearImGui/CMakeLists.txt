add_library(IMGUI STATIC)

target_compile_features(IMGUI PUBLIC cxx_std_20)

target_include_directories(IMGUI PUBLIC "public")

target_sources(IMGUI PRIVATE "public/imgui.h" "public/imconfig.h" "public/imgui_internal.h" "public/imstb_rectpack.h" "public/imstb_textedit.h" "public/imstb_truetype.h" "public/imgui_impl_dx12.h" "public/imgui_impl_win32.h" "public/imgui_stdlib.h"
"private/imgui.cpp" "private/imgui_demo.cpp" "private/imgui_draw.cpp" "private/imgui_tables.cpp" "private/imgui_widgets.cpp" "private/imgui_impl_dx12.cpp" "private/imgui_impl_win32.cpp" "private/imgui_stdlib.cpp")

install(TARGETS IMGUI)
