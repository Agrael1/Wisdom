# Wisdom

![CMake Windows](https://github.com/Agrael1/Wisdom/actions/workflows/cmake.yml/badge.svg)

**Low-level Direct Translation Graphics API. Easy to learn, easy to extend, highly performant, multiplatform!**

## Quick Links

**NuGet** https://www.nuget.org/packages/Wisdom/

**GitHub** https://github.com/Agrael1/Wisdom

**Documentation [WIP]** https://agrael1.github.io/Wisdom/

# Features

- [x] Raytracing support
- [x] Compute pipeline, Basic rendering and Multiview
- [x] Embedded DXC shader compiler and standard HLSL language
- [x] Inline API with no virtual functions.
- [x] Extensibility with internal state access
- [x] Advanced memory allocations
- [x] DMA copy support and ability to share memory between APIs
- [x] C++20 modules support and header only mode

# Why?

A lot of old OpenGL solutions are scratching the ceiling of OpenGL potential, and Vulkan is too low-level for most of the tasks. DirectX 12 is a good alternative, but it's not cross-platform.
Wisdom is designed to be a direct translation layer on top of DirectX 12 and Vulkan, with a simple API, that is easy to learn and extend.
It is still low level, yet more user friendly. It uses a lot of advanced graphics features, like Descriptor Buffer and Direct GPU Upload.

Library has transparent API. All classes have their own internal state, that can be accessed and modified. This allows for easy extension of the library, without the need to rewrite the whole API.

# Details

The API is structured like this:

- The basic types are defined, depending on platform of choice. They are **Factory**, **Adapter**, **Device** etc. They are directly implemented, this eliminates memory indirection and potential cache misses.
- The platform selects the most suitable implemetation to the system: Windows - DirectX 12, Linux - Vulkan. This is done in compile time.
- You can override the implementation selection with `WISDOM_FORCE_VULKAN` option on CMake configuration. This will force the library to use Vulkan as a base API. This is useful for debugging Vulkan extensions.
- All calls are done directly, without usage of interfaces/virtual functions. This eliminates call indirection and the code is inlined as if you wrote the code directly inside your functions.
- Underlying accessibility, all of the internals are accessible using `GetInternal()` and can be used to bridge functionality or to create extensions. All the internal state is immutable for the stability of work between library and extensions. However it's not advised to use internal state directly, since it is platform dependent.

Vulkan is compiled on compatible systems and used as default only if there is no other alternative. Vulkan can still be used under supported operating system with explicit types `wis::VKFactory`, `wis::VKDevice` etc.

[Changelog](Changelog.md)

# Platforms

Supported platforms are:

- Windows API (Win32) - DirectX 12 and Vulkan
- Windows Store (UWP) - Microsoft Store applications. DirectX 12 only.
- Linux (X11, XCB and Wayland) - Vulkan only

# Build

This is a CMake project, all the plugins are ensured to download beforehand, so it's enough to just configure the project, everything is going to be downloaded with respect to platform.

The library does not contain any extra dependencies.

If you don't have Vulkan SDK installed on Windows the library will still provide you with DX12 implementation, that comes with Windows system. No administrative rights are required to build or use the library.

# CMake Options

- `WISDOM_LOG_LEVEL=debug/warn` set the log level for the library, values are `debug,trace,info,warn,error,critical` log calls under current level are not compiled
- `WISDOM_RUNTIME_ASSERTS=ON` enable/disable runtime validation checks from compile time
- `WISDOM_USE_SYSTEM_DXC=OFF` use system DXC compiler instead of the one provided with the library (default uses the one provided)
- `WISDOM_FORCE_VULKAN=OFF` if set `ON` forces base types to be Vulkan, useful for debugging Vulkan extensions
- `WISDOM_BUILD_EXAMPLES=ON` enable/disable example compilation
- `WISDOM_BUILD_TESTS=ON` enable/disable test compilation
- `WISDOM_USE_FMT=ON/OFF` use fmt instead of `std::format` (`ON` for Linux build for GCC<13 and Clang<16)
- `WISDOM_BUILD_DOCS=ON/OFF` build documentation with Doxygen, default is dependent on wether you are building the library as a top project (ON) or as a part/dep for other (OFF)
- `WISDOM_BUILD_BINARIES=ON/OFF` build static lib. If turned off, the header only version will be the main target.
- `WISDOM_EXPERIMENTAL_CPP_MODULES=ON/OFF` enable C++20 modules support.

# Consumption

You may use FetchContent, provided by cmake, to download the library and use it in your project. The library is designed to be header-only, so you can also just copy the header folder at `wisdom/include` and use it. Install script with Vcpkg is coming next updates.

There is also a NuPkg available for NuGet consumption in release artifacts.

To link library simply use `target_link_libraries(${YOUR_TARGET} PUBLIC wis::wisdom)`. Alternatively if you wish for header only target, there is also `target_link_libraries(${YOUR_TARGET} PUBLIC wis::wisdom-headers)`.

Available targets are:

- `wis::wisdom | wis::headers | wis::wisdom-headers` - functional library
- `wis::debug | wis::debug-headers` - debug extension
- `wis::extended-allocation | wis::extended-allocation-headers` - extended allocation extension (direct GPU Upload)
- `wis::platform | wis::platform-headers` - platform specific extensions (Swapchain and Interop exports)
- `wis::raytracing | wis::raytracing-headers` - raytracing
- `wis::descriptor-buffer | wis::descriptor-buffer-headers` - descriptor buffer support, requires Vulkan 1.3 and GPU support for `VK_(EXT|VALVE)_mutable_descriptor_type` if used with Vulkan

Since 0.6.7 the library also features C++20 modules support. To use it, you need to enable the `WISDOM_EXPERIMENTAL_CPP_MODULES` option in CMake. The targets are named with postfix `-module` and are not compatible with non-module targets.

Install interface features only full named targets under namespace wis, e.g. `wis::wisdom`, `wis::wisdom-debug`, `wis::wisdom-extended-allocation-module` etc.

# System Requirements

**Windows:**

- Windows 10/11
- CMake 3.22+

Tested on MSVC v143, Visual Studio 2022.
Video card must support DirectX 12.1+ and Enchanced Barriers.

for Vulkan:

- Vulkan 1.3.2xx+

Functionality is tested on NVIDIA GeForce GTX 1070 and RTX A4000 with latest drivers. AMD cards were tested, but with limited functionality.

Best performance is achieved with NVIDIA cards later than GTX 1650 series, because of the descriptor buffer support.

Tested on Windows with NVIDIA GeForce GTX 1070 and Linux with RTX A4000 with latest drivers.

**Windows Store:**

You can install a NuGet package to any Visual studio project.

After the first launch, the project can be launched from the Start Menu.

This type of project does not support Vulkan, since Vulkan does not have UWP surface, but the API is the same as for any other platform. Useful when you want to deploy your application to Microsoft Store without too much code rewriting.

**Linux**

- CMake 3.22+
- Vulkan 1.3.2xx+

Video card driver should have Descriptor buffer support. Tested on NVIDIA RTX A4000.
