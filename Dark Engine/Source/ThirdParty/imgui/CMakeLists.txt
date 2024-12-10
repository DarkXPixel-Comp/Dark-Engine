add_library(ImGui STATIC)

CPMAddPackage(
	NAME DearImGui
	GITHUB_REPOSITORY ocornut/imgui
	VERSION 1.91.5-docking
	DOWNLOAD_ONLY YES
)

if(DearImGui_ADDED)
	FILE(GLOB DearImGui_sources ${DearImGui_SOURCE_DIR}/*.cpp)
	target_sources(ImGui PRIVATE ${DearImGui_sources})
	target_include_directories(ImGui PUBLIC ${DearImGui_SOURCE_DIR})
endif()