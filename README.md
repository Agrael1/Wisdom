# Wisdom

![CMake Windows](https://github.com/Agrael1/Wisdom/actions/workflows/cmake.yml/badge.svg)

**Low-level thin layer header only Graphics API. Easy to learn, easy to extend, highly performant, multiplatform!**

## NuGet Link
https://www.nuget.org/packages/Wisdom/

# Why?

A lot of old OpenGL solutions are scratching the ceiling of OpenGL potential, and Vulkan is too low-level for most of the tasks. DirectX 12 is a good alternative, but it's not cross-platform.
Wisdom is designed to be a thin layer on top of DirectX 12 and Vulkan, with a simple API, that is easy to learn and extend. It's designed to be used in games, simulations, and other heavy computations.
It's not designed to be a rendering engine, but a tool to create one.

The library is designed to be used in a modern C++20 environment, with a lot of modern C++ features, like concepts, modules, and ranges. It's designed to be used with CMake or NuGet, and it's easy to integrate with your project.

# Details

The API is structured like this:

- The basic types are defined, depending on platform of choice. They are **Factory**, **Adapter**, **Device** etc. They are directly implemented, this eliminates memory indirection and potential cache misses.
- The the platform is selected the most suitable to the system: Windows - DirectX 12, MacOS - Metal **[TBD]**
- You can override the platform selection with `WISDOM_FORCE_VULKAN` option on CMake configuration. This will force the library to use Vulkan as a base API. This is useful for debugging Vulkan extensions.
- All calls are done directly, without usage of interfaces/virtual functions. This eliminates call indirection and the projection is direct as if you wrote the code directly inside your functions.
- Underlying accessibility, all of the internals are accessible using `GetInternal()` and can be used to bridge functionality or to create extensions. All the internal state is immutable for the stability of work between library and extensions. However it's not advised to use internal state directly, since it is platform dependent.

Vulkan is compiled on compatible systems and used as default only if there is no other alternative. Vulkan can still be used under supported operating system with explicit types `wis::VKFactory`, `wis::VKDevice` etc.

[Changelog](Changelog.md)

# Platforms

Supported platforms are:

- Windows API (Win32) - DirectX 12 and Vulkan
- Windows Store (UWP) - Microsoft Store certified applications. DirectX 12 only.
- Linux (X11 and Wayland) - Vulkan only

# Build

This is a CMake project, all the plugins are ensured to download beforehand, so it's enough to just configure the project, everything is going to be downloaded with respect to platform.
The later reconfigurations are not reloading the plugins for easy expansion of the library, but if the plugin reload is required, the cache deletion should be done, or change `PLUGINS_LOADED` CMakeCache entry to `FALSE`.

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

# Consumption

You may use FetchContent, provided by cmake, to download the library and use it in your project. The library is designed to be header-only, so you can also just copy the header folder at `wisdom/include` and use it. Install script with Vcpkg is coming next updates.

There is also a NuPkg available for NuGet consumption in release artifacts.

To link library simply use `target_link_libraries(${YOUR_TARGET} PUBLIC wis::wisdom)`. Alternatively if you wish for header only target, there is also `target_link_libraries(${YOUR_TARGET} PUBLIC wis::wisdom-headers)`.

# System Requirements

**Windows:**

- Windows 10/11
- CMake 3.22+

Tested on MSVC v143, Visual Studio 2022.
Video card must support DirectX 12.0+ and Enchanced Barriers.

for Vulkan:

- Vulkan 1.3.2xx+

Tested on Windows with NVIDIA GeForce GTX 1070 and Linux with RTX A4000 with latest drivers.

**Windows Store:**

To Compile for Windows Store with CMake, the following is required:

- CMakeSettings: `-DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0 -DCMAKE_BUILD_TYPE=Debug/Release/RelWithDebInfo/MinSizeRel`
- Windows 10 SDK 10.0.19041.0+
- Visual Studio Generator, tested on Visual Studio 2022 (v143) - Ninja generator is not supported
- Installed UWP SDK

To launch a project find generated .sln in build `out/build/{BuildName}/examples/hello-triangle-winrt` folder and launch it with Visual Studio. This is due to deployment requirements of UWP applications, which is performed with Visual Studio. 

Or you can install a NuGet package to any Visual studio project.

After the first launch, the project can be launched from the Start Menu.

This type of project does not support Vulkan, since Vulkan does not have UWP surface.

**Linux**

- CMake 3.22+
- Vulkan 1.3.2xx+

Video card driver should have Descriptor buffer support. Tested on NVIDIA RTX A4000.

KDUils for the example need some packages to be installed:
`sudo apt install libxkbcommon-dev libxcb-xkb-dev libxkbcommon-x11-dev wayland-scanner++ wayland-protocols`

Visit https://github.com/KDAB/KDUtils to see more details.
Alternatively you can disable the example with `WISDOM_EXCLUDE_KDGUI=ON` option.

**MacOS**

- TBD... When I get my hands on a Mac

# Roadmap

The project has Gitub projects enabled, so you can see the progress on the project.
For the roadmap, the following features are planned:

- [ ] SDL2 integration
- [ ] UWP example
- [ ] Elaborate documentation
- [ ] C API generation
- [ ] Debugging tools
- [ ] Small game engine
- [x] Lower CMake version requirement to 3.22
