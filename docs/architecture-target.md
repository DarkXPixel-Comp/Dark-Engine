# Target architecture for Linux and Vulkan

**First runnable slice:** a Linux process creates one SDL3 window, selects a Vulkan 1.3 device that can present to it, draws a frame, handles resize and releases every object. The next slice draws a known textured primitive and verifies its pixels. Import, scene rendering and editor integration join this path as individual capabilities. These are design decisions for the migration. Current implementations and their limits are recorded in the [legacy audit](audit-legacy.md).

## Targets and allowed dependencies

Use a narrow first-party build entry at `Dark Engine/Source/Portable/`. Keep the runtime modules in place and compile existing portable files by explicit path. This avoids enrolling new translation units in the recursive source lists used by several legacy modules. Once a legacy feature passes its acceptance test, switch its Linux caller to the ported implementation. Avoid maintaining a second complete engine.

| Target or layer | May consume | Owns |
|---|---|---|
| `DarkCore` | C++ standard library, explicitly selected portable Core headers | Value types and tested utilities. Fixed-width integers, UTF-8 text, spans, standard containers and smart pointers at new boundaries. Initially an interface target with the flag helper. |
| `DarkPlatform` | DarkCore, SDL3 privately | Window and event pump, keyboard/mouse state, drawable extent, clock and filesystem roots. Its public headers include only standard or portable engine headers. |
| `DarkRHI` | DarkCore | Backend-neutral descriptors, move-only resource ownership, frame operations and error results. Neither Platform, Renderer nor a backend is a link dependency. |
| `DarkVulkan` | DarkRHI, system Vulkan, a private SDL3/Platform surface adapter | Instance, debug messenger, surface, physical/logical device, queues and all child GPU objects. A private bridge takes a borrowed Platform window. Only the bridge includes both SDL platform integration and Vulkan headers. |
| Renderer / Systems / Engine | Renderer consumes DarkRHI and scene data. Engine owns simulation and supplies that data. Input consumers use DarkPlatform events. | Mesh upload, view constants, scene passes and world tick. Put shared scene inputs in a lower-level value contract so Renderer never links back to Engine. |
| Launch / editor | Selected implementations and their public contracts | Composition, error reporting, run loop and reverse-order shutdown. Editor widgets and optional physics/scripting adapters enter here or through their system consumer. |

Use target-scoped include directories and package targets. A public header must compile as the first include in a consumer translation unit. Keep SDL_Window, X11/Wayland handles, Vk types, DXGI/D3D types, WRL, ImGui and PhysX out of public Core/Platform/RHI contracts. The application selects the Vulkan factory explicitly. A Vulkan-specific factory declaration may forward-declare the engine window type, return an RHI device owner and live outside the generic RHI target. Keep platform selection, renderer selection and device lifetime explicit.

## Platform contract

| Capability | Contract and retained behavior |
|---|---|
| Window | Create a single resizable window with UTF-8 title, logical size and a “presentation capable” flag. The owning thread pumps events and destroys the window. Query pixel extent separately from logical size. Zero pixels or a minimized window suspends rendering. Only private integration code can borrow the SDL handle. `FGenericWindow` and `FWindowsWindow` identify the callers that need show, resize and close. |
| Input | Deliver ordered key transitions, pointer motion, buttons, wheel, focus, resize and close. Provide held-key state for continuous movement and an edge for one-shot actions. Clear held state on focus loss. Preserve F1/F2 relative/absolute mode, relative deltas and F11's one-toggle-per-press behavior from `FInputCore::Update`. Game/editor action mappings sit above SDL translation. |
| Time | Monotonic clock measured in seconds. Reset, tick, pause and resume give nonnegative delta and elapsed active time. Use a controllable time source in tests. `FGameTimer` supplies the intent, including a zero delta while paused. |
| Files | Accept explicit read-only content/config/shader roots and a writable cache root as `std::filesystem::path`. Resolve children with path operations and report missing files and I/O failures with their path. Preserve category lookup from `FPaths`. Remove dependence on compiled checkout paths, a trailing slash or the launch working directory. |

## RHI contract

| Operation | What the caller can rely on |
|---|---|
| Create/destroy device | A factory returns an owning device or an error naming the failing operation. For a presentable device, the private bridge obtains SDL's required instance extensions and creates a surface before queue selection. Select graphics and presentation queues against that surface. Require necessary features and swapchain support. Keep the window alive until surface destruction. |
| Frame lifecycle | Acquire a drawable, record, submit, present. Represent ready, suspended/recreate and fatal failure distinctly. Use framebuffer pixels and query surface capabilities on recreation. The caller never assumes that the acquired image index is a frame number. A frame token borrows the image until submission/presentation. |
| Resources | Explicit buffer size/usage and texture extent/format/usage. Data uploads copy caller bytes or retain staging until completion. Resource owners release only after GPU work using them finishes. Model the few required operations: vertex input, texture sampling, a color attachment and transfer/readback. Retain the intent of `FRHIBufferDesc` and `FRHITextureCreateDesc`. |
| Shaders and pipeline | Supply SPIR-V words, stage and entry point. A graphics pipeline declares a vertex layout, texture binding and color attachment format. Start with vertex + fragment stages and one ordinary sampled texture. Port HLSL passes individually with layout and image checks. `ClearQuad.hlsl` currently indexes a D3D descriptor heap. |
| Commands and synchronization | Record a color pass, clear, bind pipeline/vertex buffer/texture, draw, end, submit. The backend owns layouts, queue submission dependencies, acquire/present semaphores and fences. One frame in flight is sufficient initially. Quiesce the device on swapchain replacement/shutdown. Keep normal rendering free of global idle waits. |
| Errors and diagnostics | Carry a stable operation name and readable detail through factory and frame calls. Validation tests request the Khronos layer and debug messenger, collect error-severity messages through teardown and fail on any error. Missing required capability is an explicit failed prerequisite for that milestone. |

Acquire/present work precedes resource generalization. Skip bindless descriptors, pipeline disk caches, async compilation, compute queues and super-resolution during bootstrap. `FDynamicRHI` already combines these concerns. Expand the portable interface when a ported pass demonstrates the need.

At shutdown stop submission, complete pending work, release command buffers, their referenced resources, frame synchronization and swapchain objects, then device and surface, messenger and instance, window and SDL. Release partial construction in reverse acquisition order. `FD3D12Viewport::Resize` and `FEngineLoop::Exit` are useful evidence for the responsibility split.

## Dependency sourcing

- **Build.** Use CMake + Ninja with C++23 required, extensions disabled and compile commands exported for clangd. Task 01 checks in a concrete vcpkg registry commit as `builtin-baseline`. Use the external toolchain through `VCPKG_ROOT` before CMake's first project call. Keep generated sources, shaders and executables in the binary directory.
- **Windowing.** Add the `sdl3` manifest port in Task 02 and consume its CMake package privately through `SDL3::SDL3`. Build SDL with both Wayland and X11 support at the pinned revision. Linux supplies the compositor, display libraries/development prerequisites and session. Test each video driver explicitly.
- **Vulkan.** Use Linux system loader and headers through `find_package(Vulkan 1.3 REQUIRED)` and `Vulkan::Vulkan`. The host supplies a Vulkan ICD (a compatible software implementation is acceptable), validation layers and diagnostics. Task 05 requires the host's `glslc` for offline shaders. Keep these separate from vcpkg's windowing package and from third-party SDK wrappers. Verify the resolved loader/header paths.
- **Later consumers.** Introduce `assimp` for import. Introduce image decoding, ImGui with SDL3/Vulkan integration, CPU PhysX and compatible Lua/sol2 only in their respective slices. Check exact port features and imported targets against the pinned revision when admitting them. The DearImGui wrapper names project-specific extensions, and the Lua wrapper links lua51.lib. Exercise their actual consumers before changing those integrations.
- **Legacy-only.** DXC/DX12, DirectXTK, PIX and vendor SDKs belong to the Windows reference. Optick instrumentation, Python/Boost and unused wrappers supply no first-frame requirement. Keep their transitive links out of the Linux graph. Never modify the wrappers to accomplish this.

Follow the [phase gates](migration-plan.md) for the first executable that exercises each interface.
