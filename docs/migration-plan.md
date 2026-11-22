# Linux and Vulkan migration plan

**First milestone:** compile real Core code on Linux. **First graphics milestone:** present a clear, resize the SDL3 window and present again. **First rendering milestone:** verify a known textured draw by reading its pixels. Each milestone has an executable check. The [audit](audit-legacy.md) identifies existing code to carry forward and the [architecture](architecture-target.md) defines its destination.

The baseline checkout has no Linux presets or CTest graph. The commands below are **acceptance commands for future implementation**, starting with Task 01. The documentation pass did not configure the legacy project. Its root globs enter ThirdParty and its Core configure step can modify a tracked file.

## Dependency order and phase gates

Run the **CPU matrix** below for every phase, followed by the exact named checks in the next section. Keep checks from earlier phases registered. Each test name below is part of the proposed build interface.

| Phase and prerequisites | Objective and affected modules | Principal risk | Definition of done and delegation |
|---|---|---|---|
| **0 · Build**, first | Isolate the Linux root CMake route, pin vcpkg, introduce presets and test Core's actual flag header. | The legacy graph can enumerate vendor sources, overwrite EngineVersion.cpp, select Windows globally or succeed without compiling meaningful code. | Three presets configure, build and pass `core_flags`, sanitizer compile and link flags are present, and Linux commands contain no legacy vendor/Windows inputs. **Small-model safe.** [Task 01](tasks/01-build-and-deps.md) |
| **1 · Platform**, after 0 | Add SDL3, portable window/input, monotonic timing and explicit path roots. Use ApplicationCore/Input/Core as behavioral references. | Losing the distinction between held and pressed keys, logical and pixel sizes, or assuming a relative checkout path. | Event and clock/path tests pass without a display. Both native video drivers create, resize and close a real window. **Small-model safe.** [Task 02](tasks/02-platform-abstraction.md) |
| **2 · Device**, after 1 | Add neutral RHI device/error ownership, factory in the composition root, private SDL3 surface adapter and Vulkan backend bootstrap. | A graphics queue may lack present support. Validation, feature selection or partial construction may fail. | Selection tests cover missing capability and split families. Two create/destroy cycles pass on each driver with zero validation errors. **Graphics review** for selection and lifetime. [Task 03](tasks/03-vulkan-rhi-bootstrap.md) |
| **3 · Present**, after 2 | Add swapchain, recording, submission, present and recreation. Introduce the bounded Linux Launch path. | Reusing a semaphore still in use by presentation, resetting a fence without submitting, or spinning on zero extent. | Present before and after a real resize on both drivers. CPU tests exercise zero extent, restore, out-of-date and suboptimal results. **Graphics review required.** [Task 04](tasks/04-vulkan-present.md) |
| **4 · Draw**, after 3 | Implement resource ownership, upload, offline shader compilation, pipeline, textured draw and readback. Use legacy RHI descriptors and ClearQuad as references. | Incorrect layout/visibility or DX12 binding assumptions can produce a clear screen despite successful submission. | Readback verifies sampled red and clear blue pixels; previous resize test passes; validation is clean. **Delegate fixtures and assertions** separately from resource synchronization. [Task 05](tasks/05-rhi-resources-and-draw.md) |
| **5 · Import**, after 4 for integration (CPU extraction needs only 0) | Move Assimp traversal from `FSceneResourceBuilder::BuildGraphScene` into a CPU-only path. Preserve the node, face, vertex, normal, UV and material-index outputs consumed by `FSceneResourceImporter`. Add the Assimp manifest port now. | Implicit content-root concatenation and Assimp-owned memory may outlive the import operation. | `scene_import` loads a small generated mesh fixture from an explicit root, verifies copied geometry/attributes after release, and reports a missing input. No display, GPU or GObject setup required. **Delegate fixture and conversion tests.** |
| **6 · Scene render**, after 4 and 5 | Connect `GStaticMesh::BuildFromGraphSceneMesh` data, vertex/index uploads and camera/view transforms to a single indexed scene pass. Renderer consumes scene values. Engine retains the world. | DirectXMath conventions, Vulkan clip coordinates and row/column layout can silently flip the view; Engine ↔ Renderer link cycle. | `camera_transform` checks known point-to-clip results and `scene_mesh_draw` checks imported indexed geometry and camera motion via pixel readback. Run the Linux loop with that scene. **Delegate CPU fixtures**, review projection and GPU integration. |
| **7 · World**, after 6 | Port one level/entity/component lifecycle from `FWorld`, `EEntity` and `GLevel` into Linux Launch. Retain registration behaviors used by that slice. | `FWorld::InitWorld` allocates both renderer and PhysX state, and object construction relies on global registration/class defaults. | `world_lifecycle` spawns an entity, preserves its component ownership and observes BeginPlay, ticks and EndPlay exactly as scheduled. CPU test runs without editor/physics. `scene_mesh_draw` still passes after world integration. **Senior-owned boundary**, delegate focused lifecycle tests. |
| **8 · Editor**, after 6 and 7 | Introduce optional stock ImGui SDL3/Vulkan integration for one viewport and console action from UICore. Keep platform and core runtime independently usable. | Stock packages may lack imgui_spectrum/imdark_extension. Existing UI rendering and window hooks assume DX12/Win32. | `editor_viewport` renders and resizes one viewport, dispatches a known console action and shuts down with validation clean. Document custom widget substitutions at their callers. **Delegate one widget at a time.** |
| **9 · CPU physics**, after 7 | Admit optional PhysX and port `FPhysicsCore::CreateScene`, world simulate/fetch and per-body transform propagation. Can proceed independently of 8. | Cleanup repeats and `onAdvance` uses the first pose for multiple actors. CUDA and SDK types leak through public headers. | `physics_two_bodies` verifies two distinct poses, simulate/fetch and a second independent scene lifetime under sanitizers, with CPU physics and no CUDA requirement. **Review ownership**, delegate test data. |
| **10 · Lua**, after 7 | Admit an optional compatible Lua/sol2 pair. Exercise one gameplay callback through `FScriptManager` and the class-registration code that binds it. Can proceed independently of 8 and 9. | Removing registration macros prematurely can break inherited script methods and default objects. Legacy wrapper pins a Windows Lua 5.1 library. | `lua_begin_play` verifies a registered callback runs for an entity, observes one invocation, reports a script error and releases VM state cleanly. **Delegate the fixture.** Reassess reflection with these consumer tests in place. |

For phases 5–10, write one constrained implementation spec per slice before assigning engine integration. Keep the CPU importer, camera math and world tests independently executable even when a later integration needs a GPU. Build optional editor, physics and Lua dependencies through manifest features consumed only by the corresponding enabled targets. When its phase lands, enable the new feature in the verification presets that exercise it (GCC/Clang for all, sanitizer for CPU features). Keep a core Linux executable available without optional systems.

## Commands and expected results

Run from the repository root with a bootstrapped official vcpkg Git checkout in `VCPKG_ROOT`, CMake 3.25+, Ninja, GCC and Clang. Task 02 adds SDL's Linux development prerequisites, Task 03 adds the system Vulkan 1.3 loader/headers, ICD and validation layers, and Task 05 adds system glslc. Tests use small generated or checked-in test fixtures, independent of local assets. Preserve the pinned manifest across these additions.

**CPU matrix for every phase.** The normal presets build all targets admitted by that phase. The sanitizer preset enables ASan + UBSan at compile/link time and builds CPU tests with GPU integration disabled from Phase 2 onward. Every named CPU test below must carry the `cpu` label. From Phase 2, `vulkaninfo --summary` is a host diagnostic; the integration test determines support for the actual SDL surface.

```sh
set -eu
for preset in linux-gcc linux-clang linux-sanitize
do
  cmake --preset "$preset"
  cmake --build --preset "$preset"
  ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
    ctest --preset "$preset" -L cpu --output-on-failure --no-tests=error
done
```

**Per-phase acceptance.** Define these two shell functions in the same shell as the selected row's commands. `cpu_case` requires a named test in each preset. `display_case` requires the requested driver in each available session and the Clang Debug build. If Wayland and X11 are on different machines or logins, execute the two invocations there after the CPU matrix. A backend that could not be exercised stays unverified. Each Vulkan test must request validation and fail on error messages through teardown.

```sh
set -eu
cpu_case() {
  for preset in linux-gcc linux-clang linux-sanitize
  do
    ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
      ctest --preset "$preset" -R "^$1$" --output-on-failure --no-tests=error || return 1
  done
}
display_case() {
  SDL_VIDEO_DRIVER=wayland ctest --preset linux-clang -R "^$1$" --output-on-failure --no-tests=error || return 1
  SDL_VIDEO_DRIVER=x11 ctest --preset linux-clang -R "^$1$" --output-on-failure --no-tests=error || return 1
}
```

| Phase | Exact checks after the CPU matrix |
|---|---|
| 0 | `cpu_case core_flags` plus the manifest and build-command inspection in Task 01 |
| 1 | `cpu_case platform_events && cpu_case platform_clock_paths && display_case platform_window` |
| 2 | `vulkaninfo --summary && cpu_case vulkan_device_selection && display_case vulkan_bootstrap` |
| 3 | `cpu_case vulkan_frame_state && display_case vulkan_present_resize` |
| 4 | `glslc --version && cpu_case rhi_resource_validation && display_case vulkan_textured_draw && display_case vulkan_present_resize` |
| 5 | `cpu_case scene_import` |
| 6 | `cpu_case camera_transform && display_case scene_mesh_draw` |
| 7 | `cpu_case world_lifecycle && display_case scene_mesh_draw` |
| 8 | `display_case editor_viewport && display_case vulkan_present_resize` |
| 9 | `cpu_case physics_two_bodies && cpu_case world_lifecycle` |
| 10 | `cpu_case lua_begin_play && cpu_case world_lifecycle` |

A completed phase reports the build preset, named test outcomes, GPU/driver and compositor when applicable, and any unverified configuration. Begin with [01 · Build and dependencies](tasks/01-build-and-deps.md).
