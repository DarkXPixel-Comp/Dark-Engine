# 04 · Clear, present and resize one window

**Prerequisite:** Task 03 creates a presentable device and passes validation. **Delegation:** graphics-experienced implementer or reviewer for submission and teardown. **Deliverable:** a bounded Linux frame loop with recoverable resize. All paths are relative to the repository root. Use a bootstrapped external vcpkg Git checkout in `VCPKG_ROOT`. Keep build products in the preset binary directories.

## Context and file boundary

`Dark Engine/Source/Runtime/D3D12RHI/private/D3D12Viewport.cpp` gives the reference responsibilities. `FD3D12Viewport::Resize` drops and reacquires back buffers. `Present` transitions the image and flushes commands. Its completion hook is empty. Express the frame lifetime explicitly in the Vulkan path.

**Allowed:** edit the portable `RHI/public/` device/frame contract, `VulkanBackend/private/` implementation, and `Dark Engine/Source/Portable/CMakeLists.txt`. Create portable `Launch/main.cpp` for the Linux smoke executable and `tests/linux/vulkan_frame_state.cpp` for CPU state tests. Register the smoke executable itself with CTest.

**Do not touch:** Task 02 Platform, legacy Runtime (including DX12), shader files, manifest, presets and existing tests. Under ThirdParty read only CMakeLists.txt and never edit there. Exclude build, .git, assets, Cache, .cache and .github from source discovery.

## Checklist and required behavior

- [ ] Add acquire, clear/record and submit/present operations to RHI. A frame token borrows the current drawable. Distinguish rendered, suspended, recreate and fatal outcomes. Launch owns the window and device, pumps events, supplies pixel extent, handles close and reports failures. Keep queue, command buffer and semaphore handles private.
- [ ] Query surface capabilities, formats and present modes. Use a supported color attachment format, image count within limits, supported composite alpha, FIFO and a clamped drawable extent. Handle fixed extents and independent graphics/present families (concurrent sharing is sufficient). Defer swapchain creation at zero extent.
- [ ] Allocate the swapchain, views, a command pool/buffer and synchronization for **one frame in flight**. Record a color attachment clear using dynamic rendering and appropriate layout transitions. Wait on acquisition at the first consuming stage. Use a submission fence and one render-finished semaphore per swapchain image so presentation has consumed a signal before reuse. Track the acquired index, which can differ on each frame.
- [ ] Wait before recycling command resources. Reset the submission fence only when a submission will signal it. Handle acquire-out-of-date without submitting; consume a successful/suboptimal acquisition before replacing the chain. Requery on resize/out-of-date and finish queued work before releasing old objects. Surface or device loss yields a diagnosed fatal result and clean teardown. Pause on minimized/zero extent while continuing event processing.
- [ ] Register `vulkan_frame_state` under `cpu` in all presets, including the Vulkan-disabled sanitizer build. Exercise zero extent before acquisition, restore, acquire-out-of-date with no fence reset, suboptimal after acquisition and fatal-error handling using the same frame decisions as the backend.
- [ ] Register `vulkan_present_resize` under `gpu` against the real executable, with validation and a 30-second timeout. For the test, present at least three frames, request a distinct logical size through Platform, wait for changed nonzero pixel extent, recreate and present at least three more frames. Report old/new drawable extents, generations, actual driver, number of successful presents and validation errors. Close and drain with the last submission outstanding. An unchanged extent, endless retry or early exit fails the test.

## Verification

```sh
set -eu
for preset in linux-gcc linux-clang linux-sanitize
do
  cmake --preset "$preset"
  cmake --build --preset "$preset"
  ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
    ctest --preset "$preset" -L cpu --output-on-failure --no-tests=error
done
ctest --preset linux-sanitize -R '^vulkan_frame_state$' --output-on-failure --no-tests=error
```

Run each display command in a session exposing that backend. A Wayland session with functional XWayland can supply both. Require the test to report the requested driver. Missing display, driver, required capabilities or validation support leaves the corresponding gate unverified. Use a bounded test timeout and fail on initialization errors.

```sh
SDL_VIDEO_DRIVER=wayland ctest --preset linux-clang -R '^vulkan_present_resize$' --output-on-failure --no-tests=error
SDL_VIDEO_DRIVER=x11 ctest --preset linux-clang -R '^vulkan_present_resize$' --output-on-failure --no-tests=error
```

**Done:** both drivers present and resize with zero validation errors, and deterministic CPU tests cover transitions that a compositor cannot reliably induce. Next task: [textured draw and readback](05-rhi-resources-and-draw.md).
