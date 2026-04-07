<p align="center">
  <img src="https://raw.githubusercontent.com/Agrael1/Wisdom/feat/0.7.0/docs/assets/bar.png" alt="Wisdom Logo" />
</p>

# Wisdom

<p align="center">
  <a href="https://github.com/Agrael1/Wisdom/actions/workflows/cmake.yml"><img src="https://github.com/Agrael1/Wisdom/actions/workflows/cmake.yml/badge.svg" alt="CMake Windows"/></a>
  <a href="https://www.nuget.org/packages/Wisdom/"><img src="https://img.shields.io/nuget/v/Wisdom.svg" alt="NuGet Version"/></a>
  <a href="https://github.com/Agrael1/Wisdom/blob/main/LICENSE.txt"><img src="https://img.shields.io/badge/License-MIT-blue.svg" alt="License"/></a>
</p>

**Low-level Direct Translation Graphics API. Easy to learn, easy to extend, highly performant, multiplatform!**

## Quick Links

**NuGet** https://www.nuget.org/packages/Wisdom/

**GitHub** https://github.com/Agrael1/Wisdom

**Documentation [WIP]** https://agrael1.github.io/Wisdom/

# Features

- [x] Compute pipeline, Basic rendering and Multiview with stereoscopic rendering support.
- [x] Embedded DXC shader compiler and standard HLSL language.
- [x] Inline API with no virtual functions, featuring heavy inlining for C++ and C with static linkage and IPO.
- [x] Extensibility with unparalleled internal state access, no direct code change required.
- [x] Compatibility mode allowing C++11 and C99 API interfaces for easy legacy codebase conversion.
- [x] Advanced memory allocations and direct memory writes, eliminating the need for staging buffers and reducing CPU overhead.
- [x] Header only mode for C++20+.

# Roadmap

- [ ] Raytracing support (temporarily unavailable, planned for future release)
- [ ] Mesh Shaders
- [ ] Extended documentation and tutorials
- [ ] Vcpkg support

# Why?

A lot of old OpenGL solutions are scratching the ceiling of OpenGL potential, and Vulkan is too low-level for most of the tasks. DirectX 12 is a good alternative, but it's not cross-platform.
Wisdom is designed to be a direct translation layer on top of DirectX 12 and Vulkan, with a simple API, that is easy to learn and extend.
It is still low level, yet more user friendly. It uses a lot of advanced graphics features, like Descriptor Buffer and Direct GPU Upload.

Library has transparent API. All classes have their own internal state, that can be accessed and modified. This allows for easy extension of the library, without the need to rewrite the whole API.

# Details

The API is structured like this:

- The basic types are defined, depending on platform of choice. They are **Factory**, **Adapter**, **Device** etc. They are directly implemented, this eliminates memory indirection and potential cache misses.
- The platform selects the most suitable implemetation to the system: Windows - DirectX 12, Linux - Vulkan. This is done at compile time.
- You can override the implementation selection with `WISDOM_FORCE_VULKAN` option on CMake configuration. This will force the library to use Vulkan as a base API. This is useful for debugging Vulkan extensions.
- All calls are done directly, without usage of interfaces/virtual functions. This eliminates call indirection and the code is inlined as if you wrote the code directly inside your functions.
- Underlying accessibility, all of the internals are accessible using `GetInternal()` and can be used to bridge functionality or to create extensions. All the internal state is immutable for the stability of work between library and extensions. However it's not advised to use internal state directly, since it is platform dependent.

Vulkan is compiled on compatible systems and used as default only if there is no other alternative. Vulkan can still be used under supported operating system with explicit types `wis::VKFactory`, `wis::VKDevice` etc.

[Changelog](Changelog.md)

# Platforms

Supported platforms are:

- Windows API (Win32) - DirectX 12 and Vulkan
- Linux (X11, XCB and Wayland) - Vulkan only
- Windows Store (UWP) - Microsoft Store applications. DirectX 12 only.

New platform extensions can be added by implementation using extensibility API without the need to rewrite the whole library.

# Build

This is a CMake project, all the plugins are ensured to download beforehand, so it's enough to just configure the project, everything is going to be downloaded with respect to platform.

The library does not contain any extra dependencies.

If you don't have Vulkan SDK installed on Windows the library will still provide you with DX12 implementation, that comes with Windows system. No administrative rights are required to build or use the library.

# CMake Options

- `WISDOM_USE_FMT=OFF` use fmt instead of `std::format` (`ON` for Linux build for GCC<13 and Clang<16)
- `WISDOM_FORCE_VULKAN=OFF` if set `ON` forces base types to be Vulkan, useful for debugging Vulkan extensions. Vulkan is not required to run on Windows and is selected as a fallback or best-platform automatically.
- `WISDOM_BUILD_EXAMPLES=ON` enable/disable example compilation. `ON` for top-level project, off for subproject by default.
- `WISDOM_BUILD_TESTS=ON` enable/disable test compilation. `ON` for top-level project, off for subproject by default.
- `WISDOM_BUILD_STATIC=ON` build static library version.
- `WISDOM_BUILD_SHARED=ON` build shared/dynamic library version.
- `WISDOM_BUILD_PLATFORM=ON` build unified platform extension library.
- `WISDOM_BUILD_DOCS=OFF` build documentation with Doxygen, default is dependent on whether you are building the library as a top project (ON) or as a part/dep for other (OFF)

- `WISDOM_DXC_PATH="Path/to/dxc"` use system DXC compiler instead of the one provided with the library (default uses the one provided)
- `WISDOM_VULKAN_HEADER_PATH="Path/to/vulkan/Headers"` Path to custom Vulkan Headers (optional). If not set, will use the ones provided by Vulkan SDK or system.

# Consumption

Wisdom library uses CPM for dependency management, so it is enough to add `FetchContent_Declare` in your CMakeLists.txt and link the library.

There is also a NuPkg available for NuGet consumption in release artifacts.

Install target is also provided with .zip archive in the release artifacts, that contains the library and all the headers for Windows.
To consume the library, add the path to the library /lib/cmake/wisdom to your CMake configuration and call `find_package(wisdom REQUIRED)`.

To link library simply use `target_link_libraries(${YOUR_TARGET} PUBLIC wis::wisdom)`. Alternatively if you wish for header only target, there is also `target_link_libraries(${YOUR_TARGET} PUBLIC wis::wisdom-headers)`.

Available targets are:

- `wis::wisdom | wis::wisdom-headers` - functional library
- `wis::platform | wis::wisdom-platform-headers` - platform specific extensions (Surface)

# System Requirements

**Windows:**

- Windows 10/11
- CMake 3.22+

Tested on MSVC v143, Visual Studio 2022.
Video card must support DirectX 12.1+ and Enchanced Barriers.

for Vulkan:

- Vulkan 1.3+ minimum
- Core features demand Descriptor Heap, requiring Vulkan 1.4+ or 1.3 with VK_EXT_descriptor_heap.

Functionality is tested on NVIDIA GeForce GTX 1070 and RTX A4000 with latest drivers. AMD cards were tested, but with limited functionality.

Best performance is achieved with NVIDIA cards later than GTX 1650 series, because of the descriptor heap support.
Does not require Vulkan to run on Windows, offering a native translation directly via DirectX 12.

Tested on Windows with NVIDIA GeForce GTX 1070 and Linux with RTX A4000 with latest drivers.

**Linux**

- CMake 3.22+
- Vulkan 1.3.+ minimum

- Core features demand Descriptor Heap, requiring Vulkan 1.4+ or 1.3 with VK_EXT_descriptor_heap.
- Tested with RTX A4000 with latest drivers on Ubuntu 24.04.

**Windows Store:**

You can install a NuGet package to any Visual studio project.

After the first launch, the project can be launched from the Start Menu.

This type of project does not support Vulkan, since Vulkan does not have UWP surface, but the API is the same as for any other platform. Useful when you want to deploy your application to Microsoft Store without too much code rewriting.
