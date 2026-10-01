# Wisdom examples

Examples can be built as part of Wisdom or as a separate CMake project using an installed Wisdom package. The examples directory owns its SDL3 dependency, DXC shader tools, shaders, and assets; it can be copied to another repository without the library's source tree.

## Build from an installed package

First build and install Wisdom, with the platform extensions enabled. From the Wisdom repository root:

```sh
cmake -S . -B build/wisdom -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$PWD/install" \
  -DWISDOM_BUILD_EXAMPLES=OFF \
  -DWISDOM_BUILD_TESTS=OFF
cmake --build build/wisdom --parallel 4
cmake --install build/wisdom

cmake -S examples -B build/examples -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH="$PWD/install"
cmake --build build/examples --parallel 4
```

After copying this directory elsewhere, use `cmake -S .` from its new location and point `CMAKE_PREFIX_PATH` at the same installed package. No Wisdom source directory is required.

On Windows, configure in a shell with the C/C++ compiler environment initialized. On Linux, the Wisdom Vulkan backend requires Vulkan headers; provide `WISDOM_VULKAN_HEADER_PATH` to both configurations when the headers are not discoverable through the Vulkan SDK. Full examples builds currently support the same Windows/Linux platforms as Wisdom.

## Build from the Wisdom source tree

The existing root build remains available:

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DWISDOM_BUILD_EXAMPLES=ON \
  -DWISDOM_BUILD_TESTS=OFF
cmake --build build --parallel 4
```

## Dependencies and build modes

An installed `SDL3::SDL3` target is used when available; otherwise the examples download SDL3 through their own pinned CPM bootstrap. `CPM_SOURCE_CACHE` can be provided to reuse downloaded sources.

Set `DXC_EXECUTABLE` to an existing compiler executable, or `WISDOM_DXC_PATH` to a directory containing `bin/dxc`, `bin/dxc.exe`, or `bin/x64/dxc.exe`. Without an explicit compiler, examples download DXC on Windows/Linux. On macOS an explicit compiler is required; shader compilation can be checked there, but the current Wisdom backend cannot be built or run there.

The following options select examples, independently of the library's build settings:

- `WISDOM_EXAMPLES_BUILD_STATIC`: C/C++ examples using the static core and platform libraries.
- `WISDOM_EXAMPLES_BUILD_SHARED`: C/C++ examples using the shared core and platform libraries.
- `WISDOM_EXAMPLES_BUILD_HEADERS`: C++ examples using the header-only core and platform targets.
- `WISDOM_EXAMPLES_VULKAN`: Vulkan examples; defaults to the detected Wisdom backend setting.
- `WISDOM_EXAMPLES_DX12`: DirectX 12 examples; defaults to the detected Wisdom backend setting.

Linkage modes default to enabled when the corresponding public core and platform targets exist. For example, configure with `-DWISDOM_EXAMPLES_BUILD_STATIC=OFF -DWISDOM_EXAMPLES_BUILD_HEADERS=OFF` to build only shared examples. Each mode uses its own SDL backend and does not pull in another Wisdom linkage mode.

Runtime libraries, compiled shaders, and assets are placed in `bin/examples` under the build directory. DirectX 12 examples copy available Agility SDK binaries from the Wisdom package into its `D3D12` runtime directory. C++ header-only examples still use Wisdom's transitive dependencies, such as Vulkan Memory Allocator.

To compile only shaders or copy assets:

```sh
cmake --build build/examples --target wis_test_compile_shaders
cmake --build build/examples --target wis_examples_copy_assets
```

Examples are interactive GPU workloads. A successful build checks API consumption and packaging; behavioral conformance additionally requires executing workloads and checking their results.
