# 05 · Draw a known texture and read back its pixels

**Prerequisite:** Task 04 presents and resizes on both display backends. **Delegation:** shader fixtures and pixel assertions are small-model safe against the contract below. Resource ownership, barriers and host coherency need graphics review. **Deliverable:** the first verified textured draw through the portable RHI. All paths are relative to the repository root. Use a bootstrapped external vcpkg Git checkout in `VCPKG_ROOT`. Keep build products in the preset binary directories.

## Context and file boundary

`Dark Engine/Source/Runtime/RHI/public/DynamicRHI.h` describes buffers, textures, shaders and pipelines. `Renderer/private/ClearQuad.cpp` under Runtime uses a textured quad. `Dark Engine/Shaders/ClearQuad.hlsl` samples through `ResourceDescriptorHeap`. Use an explicit binding for the first image test and preserve this HLSL for later comparison.

**Allowed:** edit `Dark Engine/Source/Portable/CMakeLists.txt`, portable `RHI/public/` contracts and `VulkanBackend/private/` implementation. Create `tests/linux/rhi_resource_validation.cpp`, `tests/linux/vulkan_textured_draw.cpp` and `tests/linux/shaders/textured.{vert,frag}`. Consume the existing public Vulkan factory for composition. Keep all shader binaries in the build directory.

**Do not touch:** existing HLSL, legacy Renderer/RHI, Task 04's Launch executable, Platform, package manifest, prior tests or wrappers. Under ThirdParty read only CMakeLists.txt and never edit there. Exclude build, .git, assets, Cache, .cache and .github from source discovery.

## Checklist and required behavior

- [ ] Add move-only buffer/texture/shader/pipeline owners, descriptors and recording operations sufficient to upload vertices and RGBA8 texels, bind one texture, draw triangles into an RGBA8 UNORM color target and copy it to a readback buffer. Keep sizes, usage, extents, vertex layout, format and shader stage explicit. Reject zero extents, out-of-range upload sizes and malformed SPIR-V input with an operation and reason.
- [ ] Compile two new GLSL shaders with the system `glslc --target-env=vulkan1.3`, using build-time CMake custom commands with declared input/output dependencies. Find the compiler only for the Vulkan build. Load the binary from the configured build output regardless of working directory. Vertex inputs are position and UV. Fragment stage samples one combined image sampler at set 0, binding 0. Use nearest filtering.
- [ ] Record staging copies, image transitions, a dynamic-rendering color pass, a draw and readback. Match pipeline color format and vertex stride, use a known viewport/scissor and disable blending, multisampling and culling for the fixture. Resolve host/device visibility and flush/invalidate noncoherent mappings when needed. Retain staging, descriptors and resources through the fence. Destroy resources before the owning device.
- [ ] Use a 64×64 offscreen target, blue clear, a one-texel opaque red source texture and a centered quad covering the middle half of the target. The position/UV buffer feeds two triangles. Assert center RGBA bytes are (255,0,0,255) and a corner outside the quad is (0,0,255,255), accounting for copy stride. This avoids compositor formats, edge coverage and sRGB variation.
- [ ] Register `rhi_resource_validation` as `cpu` in every preset, including sanitizer, for size/overflow and shader-input checks. Register `vulkan_textured_draw` as a bounded `gpu` test (30 seconds). Create the existing presentable device with validation, render offscreen, wait for completion, compare readback and collect errors through destruction. Fail on mismatched pixels or any validation error. Retain the previous real-window presentation gate.

## Verification

```sh
glslc --version
set -eu
for preset in linux-gcc linux-clang linux-sanitize
do
  cmake --preset "$preset"
  cmake --build --preset "$preset"
  ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
    ctest --preset "$preset" -L cpu --output-on-failure --no-tests=error
done
ctest --preset linux-sanitize -R '^rhi_resource_validation$' --output-on-failure --no-tests=error
```

Run each display command in a session exposing that backend. A Wayland session with functional XWayland can supply both. Require the test to report the requested driver. Missing display, driver, required capabilities or validation support leaves the corresponding gate unverified. Use a bounded test timeout and fail on initialization errors.

```sh
set -eu
for driver in wayland x11
do
  for name in vulkan_textured_draw vulkan_present_resize
  do
    SDL_VIDEO_DRIVER="$driver" ctest --preset linux-clang -R "^$name$" --output-on-failure --no-tests=error
  done
done
```

**Done:** both texel assertions pass on both session paths, validation stays clear, shader rebuilds track source changes and presentation/resize still passes. The [migration plan](../migration-plan.md) takes the next slice through the existing scene importer.
