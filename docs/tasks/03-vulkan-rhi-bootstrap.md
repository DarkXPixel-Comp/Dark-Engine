# 03 · Create a presentable Vulkan device behind RHI

**Prerequisite:** Tasks 01–02, including a real SDL3 window. **Delegation:** bounded implementation with graphics review of ownership and queue selection. **Deliverable:** a GPU integration executable creates and tears down a presentable device. All paths are relative to the repository root. Use a bootstrapped external vcpkg Git checkout in `VCPKG_ROOT`. Keep build products in the preset binary directories.

## Context and file boundary

`Dark Engine/Source/Runtime/RHI/private/DynamicRHI.cpp` includes the Windows factory directly. `Runtime/VulkanRHI/private/VulkanDynamicRHI.cpp` under Source returns null from `FVulkanDynamicRHIModule::CreateRHI`. Its instance path enables zero extensions/layers and its loader is Windows-only. Start from the minimal lifetime contract. A complete implementation of legacy `FDynamicRHI` is outside this task.

**Allowed:** within `Dark Engine/Source/Portable/`, edit `CMakeLists.txt` and create `RHI/public/` device/error contracts and `VulkanBackend/{public,private}/` factory, platform bridge and backend files. Consume the private SDL window accessor from Task 02. Edit `CMakePresets.json` solely to disable the Vulkan slice in the CPU sanitizer preset. Create `tests/linux/vulkan_bootstrap.cpp` and `tests/linux/vulkan_device_selection.cpp`.

**Do not touch:** Runtime (including legacy VulkanRHI), shaders, SDL Platform implementation, DX12, legacy CMake and vcpkg manifest. Under ThirdParty only CMakeLists.txt may be read and nothing may be edited. Exclude build, .git, assets, Cache, .cache and .github from source discovery.

## Checklist and required behavior

- [ ] Add `DARK_BUILD_VULKAN` to the portable build, on for normal presets. Use system `find_package(Vulkan 1.3 REQUIRED)` and privately link `Vulkan::Vulkan`. Confirm resolved headers/loader come from the system. Keep SDK types and headers out of public RHI and factory signatures. Preserve the CPU-only configure path when this option is off.
- [ ] Define an owning device interface with deterministic teardown and a result carrying operation and error detail. The executable/test acts as composition root and explicitly calls the Vulkan factory with a borrowed platform window and validation setting. Keep the generic RHI target independent of the factory. The window outlives the returned device and surface.
- [ ] In the private bridge obtain SDL's platform extensions and create/destroy the window surface with SDL3. Request Vulkan 1.3. When validation is requested, require `VK_LAYER_KHRONOS_validation` and debug utils, install an error collector covering initialization and teardown, and propagate creation errors. The smoke test always requests validation.
- [ ] Enumerate devices and queue families against the **actual surface**. Require usable formats/modes, swapchain extension, graphics and present queues and the Vulkan 1.3 dynamic-rendering and synchronization2 features used in the next tasks. Enable the selected features on the device. Prefer one common queue family, support distinct families, and create unique queue requests. A compatible software ICD qualifies.
- [ ] Release partially initialized objects on each error. A successful cycle releases the device, surface, messenger and instance before the window. Register `vulkan_device_selection` as a CPU test over capability records covering common/split queues, missing extension/feature and no present family. Build the pure decision test even when Vulkan is disabled. Avoid SDL/Vulkan types in its input.
- [ ] Register `vulkan_bootstrap` as a 30-second `gpu` test. Open a presentation-capable window, create and destroy the device twice with independent ownership, print requested/actual SDL driver, GPU and queue families, and fail on mismatch or any validation error. No swapchain, frame loop, shaders, global singleton or editor.

## Verification

Host prerequisites for normal presets: Linux Vulkan headers/loader, Vulkan 1.3-capable ICD, validation layers, vulkaninfo and an active display. The sanitizer preset needs only Task 02's host dependencies.

```sh
set -eu
for preset in linux-gcc linux-clang linux-sanitize
do
  cmake --preset "$preset"
  cmake --build --preset "$preset"
  ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
    ctest --preset "$preset" -L cpu --output-on-failure --no-tests=error
done
ctest --preset linux-sanitize -R '^vulkan_device_selection$' --output-on-failure --no-tests=error
vulkaninfo --summary
```

Run each display command in a session exposing that backend. A Wayland session with functional XWayland can supply both. Require the test to report the requested driver. Missing display, driver, required capabilities or validation support leaves the corresponding gate unverified. Use a bounded test timeout and fail on initialization errors.

```sh
SDL_VIDEO_DRIVER=wayland ctest --preset linux-clang -R '^vulkan_bootstrap$' --output-on-failure --no-tests=error
SDL_VIDEO_DRIVER=x11 ctest --preset linux-clang -R '^vulkan_bootstrap$' --output-on-failure --no-tests=error
```

**Done:** selection failure produces an actionable result, both sessions pass two device lifetimes, validation reports zero errors and the CPU sanitizer build passes without Vulkan. Next task: [present and resize](04-vulkan-present.md).
