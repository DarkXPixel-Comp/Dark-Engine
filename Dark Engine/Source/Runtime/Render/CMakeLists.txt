add_library(Render)

target_compile_features(Render PUBLIC cxx_std_20)

target_link_libraries(Render PUBLIC Core Engine DTK12 D3DCompiler Assimp IMGUI ApplicationCore)


#set(Render_Sources )


#GroupSourcesByFolder(Render "${Render_Sources}")

target_sources(Render PRIVATE "D3D12/D3D12Main.cpp" "D3D12/D3D12Camera.cpp"
"D3D12/D3D12Mesh.cpp" "D3D12/D3D12Model.cpp" "D3D12/D3D12PSO.cpp" "D3D12/D3D12Render.cpp"  "D3D12/D3D12Resource.cpp" 
"D3D12/D3D12Scene.cpp" "D3D12/D3D12Shader.cpp" "D3D12/D3D12Types.cpp" "D3D12/D3D12Utils.cpp" "Renderer.cpp")

SettingProject(Render)


target_include_directories(Render PUBLIC "../Render/")





#install(TARGETS Render)
