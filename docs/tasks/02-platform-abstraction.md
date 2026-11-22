# 02 · Provide window, input, clock and path services

**Prerequisite:** Task 01 passes on Linux. **Delegation:** small-model safe with the contracts below. **Deliverable:** the first resizable SDL3 window and independently testable platform behavior. All paths are relative to the repository root. Use a bootstrapped external vcpkg Git checkout in `VCPKG_ROOT`. Keep build products in the preset binary directories.

## Context and file boundary

`Dark Engine/Source/Runtime/ApplicationCore/private/Windows/WindowsApplication.cpp` pumps Win32 messages. `Input/private/InputCore.cpp` under Runtime uses DirectXTK for held movement keys, relative mouse and F11 edge detection. `Core/Timer/GameTimer.cpp` uses QueryPerformanceCounter. `Core/Private/Misc/Paths.cpp` derives paths from the project root. These are behavioral references.

**Allowed:** edit `vcpkg.json` to add SDL3, and the build list in `Dark Engine/Source/Portable/CMakeLists.txt`. Create `Platform/{public,private}/` files under that Portable directory for the four services, and `tests/linux/platform_{events,clock_paths,window}.cpp`. Register their targets in the portable build list. Keep the baseline from Task 01.

**Do not touch:** Runtime, root build dispatch, preset definitions, legacy Input/ApplicationCore, RHI, shaders, UI, scripts and vendor wrappers. Under ThirdParty read only CMakeLists.txt and never edit there. Exclude build, .git, assets, Cache, .cache and .github from source discovery.

## Checklist and required behavior

- [ ] Resolve the pinned `sdl3` port with Wayland/X11 support and link `SDL3::SDL3` privately to `DarkPlatform`. Document host development packages actually needed in the implementation handoff. Compile each new public header on its own. No public include or argument may expose SDL, Vulkan, HWND or DirectX.
- [ ] Provide an owning, noncopyable window with create, set logical size, get pixel extent, pump events, close and set relative mouse mode. Accept a title, initial size and presentation-capable creation flag. Set the SDL Vulkan window flag only when requested. Report creation errors, including the driver. Expose the borrowed SDL window **only through a private integration header** for Task 03. The main thread owns the window lifetime.
- [ ] Translate close, focus, key press/release/repeat, mouse motion/buttons/wheel and drawable-size changes into engine values. Maintain held state and release it on focus loss. A higher-level test consumer must be able to drive WASD/space/shift while held, switch relative mode via F1/F2 and toggle F11 once until release. Expose F3, F4 and Escape as well. Keep action policy out of the SDL event translator.
- [ ] Implement a steady-clock timer with reset, tick, pause, resume, nonnegative delta and elapsed active seconds. Test known time points so scheduler timing cannot cause failures. Implement path resolution for explicit content/config/shader/cache roots with `std::filesystem::path`. Exercise a path containing spaces, a missing input and a writable temporary cache.
- [ ] Register `platform_events` and `platform_clock_paths` as `cpu`. The first feeds deterministic events through the translation path without opening a window. Register `platform_window` as `display`: create a real resizable window, pump, observe positive pixel extent, request a different size, observe its drawable change and close cleanly. Report the actual video driver, fail on mismatch, and set a 30-second timeout.

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
for name in platform_events platform_clock_paths
do
  ctest --preset linux-clang -R "^$name$" --output-on-failure --no-tests=error
done
```

Run each display command in a session exposing that backend. A Wayland session with functional XWayland can supply both. Require the test to report the requested driver. Missing display, driver, required capabilities or validation support leaves the corresponding gate unverified. Use a bounded test timeout and fail on initialization errors.

```sh
SDL_VIDEO_DRIVER=wayland ctest --preset linux-clang -R '^platform_window$' --output-on-failure --no-tests=error
SDL_VIDEO_DRIVER=x11 ctest --preset linux-clang -R '^platform_window$' --output-on-failure --no-tests=error
```

**Done:** all CPU cases pass under both compilers and ASan/UBSan, and both session runs create, resize and close a real window. Next task: [Vulkan bootstrap](03-vulkan-rhi-bootstrap.md).
