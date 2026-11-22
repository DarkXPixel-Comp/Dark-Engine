# 01 · Establish the Linux build and dependency boundary

**Prerequisite:** the documentation pass. **Delegation:** small-model safe. **Deliverable:** a clean Linux configure, native C++23 build and CTest of existing engine code. All paths are relative to the repository root. Use a bootstrapped external vcpkg Git checkout in `VCPKG_ROOT`. Keep build products in the preset binary directories.

## Context and file boundary

Root `CMakeLists.txt` recursively globs Source and adds the entire legacy graph. `Dark Engine/Source/Runtime/Core/CMakeLists.txt` configures a version source into that module. `CMakeSettings.json` targets MSVC. The first dependency-free Linux test can include `Dark Engine/Source/Runtime/Core/Public/Misc/EnumClassFlags.h` directly.

**Allowed:** edit root `CMakeLists.txt` for the Linux dispatch. Create root `CMakePresets.json`, `vcpkg.json`, `Dark Engine/Source/Portable/CMakeLists.txt`, and `tests/linux/core_flags.cpp`. Use this entire allowlist.

**Do not touch:** existing engine headers/sources, legacy subdirectory CMake, CMakeSettings, shader/config files and documentation. Under ThirdParty only CMakeLists.txt may be read. Never edit that tree or run the legacy configure. Exclude build, .git, assets, Cache, .cache and .github from source discovery.

## Checklist and required behavior

- [ ] Select the explicit Linux graph immediately after project initialization and before all legacy glob calls, Windows definitions, output settings and subdirectory execution. Set a CMake minimum of 3.25 for preset/tooling support. Use an explicit top-level `project` and keep existing Windows entry behavior. Enable CTest at the Linux top level so tests are discoverable from the build root.
- [ ] Add configure, build and test presets named `linux-gcc`, `linux-clang` and `linux-sanitize`. Use Ninja, Debug, distinct `build/<preset>` directories, gcc/g++ or clang/clang++, `CMAKE_EXPORT_COMPILE_COMMANDS=ON`, and the vcpkg toolchain at `$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake`. Make C++23 required and disable compiler extensions. Instrument first-party compilation **and executable linking** with address and undefined-behavior sanitizers and frame pointers in the Clang sanitizer preset.
- [ ] Create a manifest with project name, version and literal 40-hex `builtin-baseline`. Choose the commit of the bootstrapped official vcpkg checkout used for this test. Confirm it contains the baseline registry and SDL3 port. Record that commit **in the manifest** so later machines resolve the same versions. Start with an empty dependencies array. Package installation must use manifest mode.
- [ ] Define `DarkCore` as an interface target exposing the necessary existing Core/Public include root. Add one executable and register `core_flags` with label `cpu`. Include the actual header, define a two-bit scoped enum and exercise union, membership, absent bits and compound assignment. A wrong result must fail even with assertions disabled.
- [ ] Verify generated, compiler and package outputs are confined to the build tree or external vcpkg cache. Check that the Linux compile database, build targets and linker commands exclude the Windows SDK, vendor tree, .lib files, forced Definitions.h, UBT_COMPILED_PLATFORM and /WHOLEARCHIVE. A successful configure with no compiled engine behavior is incomplete.

## Verification

From the repository root, using CMake 3.25+, Ninja, Python 3, both compilers and the bootstrapped registry checkout:

```sh
set -eu
test -x "$VCPKG_ROOT/vcpkg"
pin=$(python3 -c 'import json; print(json.load(open("vcpkg.json"))["builtin-baseline"])')
test "$pin" = "$(git -C "$VCPKG_ROOT" rev-parse "$pin^{commit}")"
git -C "$VCPKG_ROOT" cat-file -e "$pin:versions/baseline.json"
git -C "$VCPKG_ROOT" cat-file -e "$pin:ports/sdl3/vcpkg.json"
cmake --list-presets
```

```sh
set -eu
for preset in linux-gcc linux-clang linux-sanitize
do
  cmake --preset "$preset"
  cmake --build --preset "$preset"
  ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
    ctest --preset "$preset" -L cpu --output-on-failure --no-tests=error
done
ctest --preset linux-clang -R '^core_flags$' --output-on-failure --no-tests=error
python3 - <<'PY'
import json
import pathlib
import subprocess
for name in ("linux-gcc", "linux-clang", "linux-sanitize"):
    directory = pathlib.Path("build") / name
    entries = json.loads((directory / "compile_commands.json").read_text())
    assert any(entry["file"].endswith("core_flags.cpp") for entry in entries)
    commands = subprocess.check_output(["ninja", "-C", str(directory), "-t", "commands"], text=True)
    for legacy in ("Source/ThirdParty/", "UBT_COMPILED_PLATFORM", "Definitions.h", "/WHOLEARCHIVE", ".lib "):
        assert legacy.lower() not in commands.lower(), (name, legacy)
    if name == "linux-sanitize":
        assert "address" in commands and "undefined" in commands
PY
git diff --exit-code -- 'Dark Engine/Source/Runtime/Core/Private/Misc/EngineVersion.cpp'
```

Expected: all three builds compile the test and pass it. On a checkout with existing user edits to the version source, compare its before/after checksum instead. Report the baseline, compiler versions and tests run. Next task: [SDL3 Platform](02-platform-abstraction.md).
