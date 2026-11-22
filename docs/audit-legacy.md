# Legacy engine audit

Inspected the working tree at commit `373d7143`. The interesting split is inside modules. Core contains a flag utility that already compiles on Linux, alongside public headers that require WRL. The D3D12 viewport contains real presentation logic, alongside an empty completion hook. Porting an entire CMake target would bring both kinds of code across at once.

**Evidence standard.** “Working” refers to the specific behavior exercised. “Partial” marks implemented paths with coupling, gaps or unverified runtime behavior. “Stub/draft” marks declarations, no-op bodies or an incomplete creation path. The Windows executable was not run. Two C++23 syntax-only probes checked flag union and membership in `Core/Public/Misc/EnumClassFlags.h` with GCC and Clang. The module statuses below otherwise come from source inspection. References of the form `Core/…` or `RHI/…` are relative to `Dark Engine/Source/Runtime/`.

## Directory and build map

| Location | What participates in the build or runtime |
|---|---|
| Root | `CMakeLists.txt` defines `SettingProject`, `SettingFiles` and `SUBDIRLIST`, requests CMake 3.8, globs the entire Source tree, sets `UBT_COMPILED_PLATFORM=Windows` and `ROOT_DIR` globally, and writes runtime output under the checkout. `CMakeSettings.json` has Ninja configurations that inherit MSVC. The repository lacks a Linux preset, vcpkg manifest and portable CTest entry point. |
| `Dark Engine/CMakeLists.txt`, `Dark Engine/Source/CMakeLists.txt` | Enter Source, then Runtime, ThirdParty and Scripts. MSVC/Win32 gets `/FI Definitions.h`. Both Runtime and ThirdParty enumerate immediate directories and call `add_subdirectory`. Running the existing configure would enter vendor integrations. |
| `Dark Engine/Source/Windows/Definitions.h` | Forced macros include `WIN32`, `PLATFORM_WINDOWS`, `__UNREAL__`, `WITH_VERSE`, `GIsEditor` and `IMGUI`. Several definitions repeat. Their presence is evidence of build assumptions, not of implemented features. |
| `Dark Engine/Source/Runtime/` | First-party modules in the next table. Several have historical `.module.sharpmake.cs` files. The CMake graph is the entry point inspected here. |
| `Dark Engine/Source/ThirdParty/` | Wrapper inventory below. Only CMakeLists.txt content was inspected. Local source and binary availability cannot be established from wrapper references. |
| `Dark Engine/Config/`, `Dark Engine/Shaders/`, `Dark Engine/Scripts/` | Engine/UI configuration, HLSL programs and runtime script examples respectively. Separate `Dark Engine/Source/Scripts/` holds Python scripts and a CMake entry. `Doxyfile` is at the root. |

Two build hazards precede compilation. `Core/CMakeLists.txt` configures `Private/Misc/EngineVersion.cpp.var` into the source directory. `Launch/CMakeLists.txt` names `Private/Launch.cpp` while the file is `private/Launch.cpp`. It also forces an `.exe` suffix, uses Windows resources and specifies `/WHOLEARCHIVE:Engine.lib`. A new Linux route must branch before the root source globs and before either subtree is added.

## Status by module

| Status | Module and source evidence | Migration disposition |
|---|---|---|
| Working, compile-time probe | Core flags. `Core/Public/Misc/EnumClassFlags.h` defines `ENUM_CLASS_FLAGS` and `EnumHasAnyFlags`. Both compilers evaluated positive and negative static assertions without a forced include. | Use the actual header for the first build and CTest gate. |
| Partial | Core. `FConfigCache::InitConfigFiles` reads Engine.ini and registers defaults. `FPaths::CombineDir` concatenates strings. `FGameTimer::Tick` uses QPC, and `Stop` is declared without a definition in its implementation. `Core/Memory/TUniquePtr.h` includes WRL even for standard smart-pointer aliases. `FModuleManager` in `Core/Public/Modules/ModuleManager.h` has declarations with no first-party definitions found. | Preserve config values, timer behavior and call sites as they are needed. Give paths, time and ownership independent tests. Defer the module manager. |
| Partial | ApplicationCore and Input. `FWindowsApplication::ProcessMessage` / `PumpMessages` and `FWindowsWindow::Initialize` implement Win32 dispatch and window creation. `FGenericApplication` and `FGenericWindow` provide many empty defaults and the latter exposes ImGui hooks. `FInputCore::InputCoreProc` feeds DirectXTK Keyboard/Mouse. `Update` handles held movement keys, F1/F2 relative mode and an F11 edge latch. | Keep event/held-state semantics. Move native dispatch to SDL3, and let an editor adapter handle UI integration. |
| Partial | RHI. `RHI/public/DynamicRHI.h` describes buffers, textures, shader creation, contexts and viewports. `RHI/private/DynamicRHI.cpp` includes `Windows/WindowsDynamicRHI.cpp`, which constructs `FD3D12DynamicRHIModule`. `ERHIInterfaceType` lists Hidden, Null and D3D12. `FPipelineStateCache::GetAndOrCreateGraphicsPipelineState` inserts on a miss but always returns null. | Preserve creation and presentation contracts. Replace global selection, COM-shaped handles and the unfinished cache in the Linux path. |
| Partial, substantive implementation | D3D12RHI. `FD3D12DynamicRHI::RHICreateBuffer` / `RHICreateTexture`, `FD3D12CommandContext` and `FD3D12Viewport::Resize` / `Present` show allocation, recording, back-buffer release/reacquisition and a transition to presentation. `WaitForFrameEventCompletion` has an empty body, and `Present` returns true even without a `SwapChain4`. | Use the observable operations to define tests. Vulkan needs its own synchronization and error semantics. |
| Stub/draft | VulkanRHI. In `VulkanRHI/private/VulkanDynamicRHI.cpp`, `FVulkanDynamicRHIModule::CreateRHI` selects a loader only under WIN32 and returns null. `FVulkanDynamicRHI::Init` requests an instance with zero extensions and layers. `private/Windows/WindowsVulkanLoader.h` loads vulkan-1.dll. The class stores only an instance, and `LogVkError` is empty. Its CMake links bare `Vulkan` and adds a private include directory to **D3D12RHI**. | Linux loader integration, physical/logical device, surface, swapchain, submission and teardown are missing in this engine backend. A vendor SDK's nested Vulkan CMake file does not supply them. |
| Partial | Renderer and Windows/D3DShaderFormat. `FRenderer::BeginRenderingView` drives `FSceneRender::Render`, which records a pass, selects a test/fractal/seascape/quad path and optionally applies FXAA. `Renderer/private/ShaderCompiler.cpp` includes `D3DShaderFormat.h` and a BS thread pool. `CompileD3DShader` constructs DXC COM interfaces. `Shaders/ClearQuad.hlsl` under `Dark Engine/` uses `ResourceDescriptorHeap`. | Preserve pass behavior and inputs. Start with offline SPIR-V and explicit bindings. Characterize each legacy shader before moving it. |
| Partial | Engine. `FSceneResourceBuilder::BuildGraphScene` imports via Assimp and copies nodes, vertices, faces and attributes. `FSceneResourceImporter::ProcessStaticMesh` constructs a `GStaticMesh`; `GStaticMesh::BuildFromGraphSceneMesh` converts mesh data. `FEngineResourceImporter::ImportObjects` dispatches by extension on a thread pool. `FWorld::BeginPlay` / `Tick`, `EEntity::PostSpawnInitialize` and `DEditorEngine::NewMap` contain lifecycle behavior. `FPrimitiveSceneProxy::SetTransform` is empty. | Keep CPU import and entity behavior. Separate GPU upload, class registration and global world access at their actual consumers. |
| Partial | CoreGObject. `GClass::CreateDefaultObject`, `StaticAllocateObject` and `FGObjectArray::AllocateObjectIndex` implement class defaults, allocation and indexing. `GStruct::RegisterAllFunctions` walks parent classes for scripting. `GField::GetOwnerStruct` returns null. | Test class ancestry, spawned entities and script registration before shrinking this machinery. Keep it out of Core, Platform and RHI. |
| Partial | UICore. `UIApplication::Tick` drives platform and widget work. `FUIRHIRenderer::DrawWindows` opens a render pass and runs ImGui hooks. `UIApplication.cpp` includes DXGI/D3D12 and imgui_spectrum. `UIContentBrowser.cpp` includes nfd, `UIDock.cpp` uses imdark_extension, and `UIWidgetTest.cpp` includes ImGuizmo. | Preserve editor workflows individually. Stock ImGui integration needs a substitute or a deliberate deferral for each custom extension. |
| Stub/draft | Editor. `Editor/public/DEditor.h` gives `DEditor::Init`, `Update` and `Shutdown` empty bodies. Actual editor work appears in Engine's `DEditorEngine`, UICore and Launch's `FEditorLayout`. | Port those consumers. The empty Editor class provides no migration milestone. |
| Partial | PhysicsCore. `FPhysicsCore::Init` creates PhysX foundation, physics and a CPU dispatcher. `CreateScene` supports optional CUDA. `FWorld::Tick` / `FetchPhysic` drive simulation and fetch. `PhysicsCore/public/PhysicsCore.h` exposes PhysX and CUDA types. | Preserve CPU simulation in an optional adapter. Audit cleanup and pose indexing with a two-body test before enabling it. |
| Partial | Scripting. `FScriptManager` opens Lua base, package and table libraries through `sol2/sol.hpp`. CoreGObject and Engine consume script state. Launch includes `Python.h` and links Python and Boost. That alone does not establish a Python gameplay runtime. | Keep a tested Lua gameplay hook. Defer Python/Boost until a concrete runtime consumer needs them. |
| Partial | Launch. `WinMain` calls `LaunchWindowsStartup` and `GuardedMain`. `FEngineLoop::PreInit` initializes RHI, shaders, physics and editor UI. `Tick` updates UI, Engine and world. `Exit` tears services down. | Reuse the lifecycle ordering where applicable. A small Linux executable can exercise the window and GPU before constructing an editor or world. |

## Windows and graphics coupling outside the backend

| Boundary | Evidence and consequence |
|---|---|
| Compiler and type system | `Core/Public/HAL/Platform.h` expands `COMPILED_PLATFORM_HEADER(Platform.h)` and misspells a fallback as `PLATFORRM_WINDOWS`. `Core/Public/GenericPlatform/GenericPlatform.h` defines its “generic” types inside `_WIN64`. `Core/Public/MSVC/MSVCPlatform.h` checks `_MSC_VER`. `Core/CorePlatform.h` uses `__declspec`. Faking Windows macros would select these implementations. |
| Memory, strings and services | `Core/Public/HAL/DarkMemory.h` includes Windows platform memory directly. `Core/Private/Containers/DarkString.cpp` uses `MultiByteToWideChar`. `Core/CommandLine/CommandLine.cpp` uses Windows CRT handle binding. `Core/Public/Windows/WindowsProcess.h` calls `CreateProcess`. Logging, app setup and settings also include Windows headers. `Core/Public/Windows/WindowsSystemIncludes.h` requests Pathcch.lib. |
| Math and ownership | `Core/Public/Math/Transform.h`, `LookToMatrix.h`, `Core/Private/Math/MathSSE.cpp` and `Renderer/private/ScenePrivate.cpp` use DirectXMath. `TRefCountPtr` aliases `Microsoft::WRL::ComPtr` while `FRefCountedObject` implements intrusive AddRef/Release. Migrating the shared header would pull COM into resource users. Test matrix layout, handedness and projection before changing math. |
| Window, messages and user interface | `Input/public/InputCore.h` exposes HWND, LRESULT and DirectXTK types. `ApplicationCore/private/Windows/WindowsWindow.cpp` contains Win32 ImGui hooks and COM drag/drop methods. Launch sets Windows DPI awareness. `UICore/private/Rendering/UIRHIRenderer.cpp` passes an OS window handle to RHI. |
| GPU tools, linking and shader ABI | `D3D12RHI/private/Windows/WindowsD3D12ThirdParty.h` includes DXGI, PIX and conditional allocator/DirectSR headers and requests D3D12.lib, DXGI.lib and dxcompiler.lib via pragmas. The backend CMake disables Streamline and DirectSR. Renderer includes its D3D shader format directly. A portable backend switch alone cannot change compiled shader format or descriptor layout. |

## ThirdParty integration ledger

All 25 rows below correspond to direct children of `Dark Engine/Source/ThirdParty/` with a **CMakeLists.txt**. “Declared” describes build metadata. Engine usage comes only from the first-party sources and link declarations identified here.

| Folder | Declared target and integration | Consumer or disposition |
|---|---|---|
| Assimp | `Assimp` interface, assimp-vc143-mtd.lib | Engine calls `aiImportFile` in SceneResourceBuilder. First later import dependency. |
| BSThreadPool | `BSThreadPool` interface include path | Engine importer and Renderer include BS_thread_pool.hpp. Preserve asynchronous behavior when a consumer needs it. |
| Boost | `Boost` interface links versioned Boost.Python VC143 library | Launch links it. Defer. |
| D3D12 | `D3D12` interface headers | D3D12RHI private integration. Windows reference only. |
| D3DCompiler | `D3DCompiler` interface links dxcompiler.lib | D3DShaderFormat uses dxcapi.h. |
| DTK12 | `DTK12` interface names an absolute developer-machine path to DirectXTK12.lib | Input uses Keyboard/Mouse, and Engine, UICore and D3D12RHI link the target. |
| DearImGui | `IMGUI` static with Win32/DX12 backends and project extensions | ApplicationCore, UICore and D3D12RHI use ImGui. Port editor incrementally. |
| DirectXMath | `DirectXMath`, sources selected by SettingFiles | Core links it and exposes it through math headers. Characterize transforms. |
| FidelityFX | `FidelityFX` interface collects .lib files on Windows | Nested component and Vulkan backend CMake files exist. No engine Vulkan implementation follows from this declaration. Defer. |
| ImGuizmo | `ImGuizmo` static links IMGUI | UICore rendering and widget code include ImGuizmo.h. |
| JSON | `JSON` interface include path | No verified initial-phase consumer. |
| Lua | `Lua` interface links lua51.lib | Scripting uses sol2 and a Lua VM. Verify API/ABI with a script fixture when ported. |
| NativeFileDialog | `NativeFileDialog` interface selects debug/release nfd.lib | Editor settings and content browser include nfd.h. |
| NodeImGui | Top-level declarations commented out | Its nested example projects are separate from the engine graph. No active wrapper target. |
| NvidiaNsight | `NvidiaNsight` interface links Aftermath .lib | Optional diagnostic integration metadata. |
| NvidiaStreamline | `NvidiaStreamline` interface links sl.interposer.lib on Windows | D3D12RHI sets its Streamline feature macro to zero. Defer. |
| OpenFrameworks | Empty wrapper | No target. |
| Optick | `Optick` interface sets USE_OPTICK by configuration and links OptickCore.lib | Public dependency of Core. Launch, Renderer, Engine and UI contain instrumentation. |
| PhysX | `PhysX` interface selects debug/profile/release static .lib files, including snippets | PhysicsCore creates real scenes. Package only for the CPU physics milestone. |
| Python | `Python` interface links python312.lib, python3.lib and _tkinter.lib | Launch includes Python.h. Defer. |
| Stb | `STB` interface, but target_sources uses UICoreSrc and include directories are applied to UICore | Engine scene viewport and UI include stb_image.h. Use an explicit package dependency for a real consumer. |
| WinPix | `WinPix` interface links WinPixEventRuntime.lib | Public dependency of D3D12RHI, which includes pix3.h. |
| d3dx12 | `d3dx12` links D3D12, lists helper source and headers | D3D12RHI uses the helpers privately. |
| glm | `glm` interface, conditional `glm_static` / `glm_shared` | The wrapper alone does not establish use as engine math. |
| nvngx_dlss | Empty wrapper | No target. |

## Refactoring priorities

1. **Break dependency cycles at the caller.** The CMakeLists.txt files for Engine ↔ Renderer, RHI ↔ D3D12RHI, Renderer ↔ D3DShaderFormat and Engine ↔ CoreGObject contain reciprocal links. ApplicationCore also links Engine, which links ApplicationCore. Select backends in Launch, pass scene data into Renderer and keep callbacks pointed at small interfaces.
2. **Make failure and ownership observable.** RHI startup writes `GDynamicRHI`; subsequent calls dereference it. `FD3D12Viewport::Present` masks failure. `FPhysicsCore` calls `Destroy` both on explicit shutdown and from its destructor. Its cleanup leaves the CUDA pointer set and does not release the CPU dispatcher there. Test partial initialization, normal release and a second lifetime before porting those paths.
3. **Retain data and behavior first.** Assimp traversal, input actions, delta-time measurement and scene pass selection all have consumers. In the physics path, `FWorldPhysicsCallback::onAdvance` indexes the body array but reads each pose through `PoseBuffer->p`, and `FWorld::GetGravity` can fall through without returning. Write behavior tests before carrying these implementations into Linux.
4. **Reduce UE-style machinery at bounded seams.** Prefer explicit service ownership to `GWorld`, `GDynamicRHI` and static registration for the bootstrap. `Core/Delegate/Delegate.h` allocates type-erased call containers in `Bind`; `TArray` and the string/trait layers carry platform dependencies into public APIs. Use standard-library types in new contracts and keep adapters at existing consumers. Preserve class-default-object and script behavior until tests cover it.

The [target architecture](architecture-target.md) gives the replacement interfaces. The [migration plan](migration-plan.md) assigns each risk to a phase and acceptance test.
