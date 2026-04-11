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
- [x] Conan integration.

# Roadmap

- [ ] Raytracing support (temporarily unavailable, planned for future release)
- [ ] Mesh Shaders
- [ ] Extended documentation and tutorials
- [ ] Interoperability with other graphics libraries

# Why?

A lot of old graphics APIs are still widely used, but the modern ones are not as widely adopted,
since they are more complex and often require a lot of boilerplate code to get started.
This library aims to provide a low-level, high-performance graphics API that is easy to learn and use,
while still being extensible and compatible with existing codebases.

The design is simple - provide a low-level API that is as close to the underlying graphics API as possible,
while still being easy to use and extend. This allows developers to get the most out of their hardware,
while still being able to write clean and maintainable code without digging too deep into foreign API.

Wisdom acknowledges that a lot of special use cases require writing extensions or using platform specific features,
so it provides unparalleled access to the internal state of the library,
allowing developers to write extensions without needing to modify the core library code.
Internal extension resolver allows getting access to the state without worrying about
collisions of extensions or internal structures.

# Details

The API is structured like this:

- The basic types are defined at compile time with selected backend. They are **Instance**, **Device** etc. They are stack based and do not require dynamic memory allocation.
- The platform selects the most suitable implemetation to the system: Windows - DirectX 12, Linux - Vulkan. This is done at compile time.
- You can override the implementation selection with `WISDOM_FORCE_VULKAN` option on CMake configuration or global definition. This will force the library to use Vulkan as a base API. This is useful for debugging Vulkan extensions.
- All calls are done directly, without usage of interfaces/virtual functions. This eliminates call indirection and the code is inlined as if you wrote the code directly inside your functions.
- Underlying accessibility, all of the internals are accessible using `GetInternal()` in C++ and can be used to bridge functionality or to create extensions.
- Internal state is considered sealed, but the types that are at offset 0 are guaranteed to be stable and can be used for extensions. This allows you to write extensions without worrying about the internal state changes, as long as you don't rely on the internal state that is not at offset 0.

Vulkan is compiled on compatible systems and used as default only if there is no other alternative. Vulkan can still be used under supported operating system with explicit types `wis::VKInstance`, `wis::VKDevice` etc.

# Platforms

Supported platforms are:

- Windows API (Win32) - DirectX 12 and Vulkan.
- Linux (X11, XCB and Wayland) - Vulkan only.
- Windows Store (UWP) - Microsoft Store applications. DirectX 12 only. Can be used with NuGet package.

New platform extensions can be added by implementation using extensibility API.

# Build

This is a CMake project, all the plugins are ensured to download beforehand, so it's enough to just configure the project, everything is going to be downloaded with respect to platform.

The library uses minimal dependencies, so the build is going to be fast and easy.

If you don't have Vulkan SDK installed on Windows the library will still provide you with DX12 implementation, that comes with Windows system.
There is also an option to directly point to Vulkan headers with `WISDOM_VULKAN_HEADER_PATH` CMake option, so you can use the library with custom Vulkan headers or without Vulkan SDK.
Vulkan library is loaded dynamically, so it is not required to have Vulkan SDK installed to run the library.

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

There is also Conan package available for consumption, it can't be loaded to Conan Center yet,
but you can add it manually by downloading the repo and executing `conan create .` command in the root of the repository.
This will build the library and create a local package that you can use in your projects. The package name is `wisdom/0.7.0` and it can be used in your `conanfile.txt` or `conanfile.py` with `requires = wisdom/0.7.0`.

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

- Core features demand Descriptor Heap, requiring Vulkan 1.3 with VK_EXT_descriptor_heap.
- Tested with RTX A4000 with latest drivers on Ubuntu 24.04.

**Windows Store:**

You can install a NuGet package to any Visual studio project.

After the first launch, the project can be launched from the Start Menu.

This type of project does not support Vulkan, since Vulkan does not have UWP surface, but the API is the same as for any other platform. Useful when you want to deploy your application to Microsoft Store without too much code rewriting.

# API

The library has a simple API, that is defined in xml/\*.xml files. Those files can be used to generate bindings for other languages, or to generate documentation. The API is defined in a way that is easy to use and understand, while still being powerful and flexible.
