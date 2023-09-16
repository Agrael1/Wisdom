# Wisdom

![CMake Windows](https://github.com/Agrael1/Wisdom/actions/workflows/cmake.yml/badge.svg)

**Low-level thin Graphics API layer. Easy to learn, easy to extend, high performance enabled, multiplatform!**

# Why?

This library has several tasks to accomplish:

- Make a functional portable library for graphics programming without extra efforts in usage
- Make use of API similarities and provide implementation agnostic interface to the user for seamless experience
- Make transition to this library and making extensions seamless with existing codebase

# Details

The API is designed for game-ready and heavy computations use. It is done in low-level, with customizable logs. Underlying implementation is pure classes with minimal encapsulation.

The API is structured like this:
 - The basic types are defined, depending on platform of choice. They are **Factory**, **Adapter**, **Device** etc. They are directly implemented, this eliminates memory indirection and potential cache misses.
 - The the platform is selected the most suitable to the system: Windows - DirectX 12, MacOS - Metal __[TBD]__
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
- `WISDOM_BUILD_TYPE=headers/static/modules` build type, `headers` makes header only library, `static` builds static library, `modules` builds modules, default is `static`
- `WISDOM_BUILD_DOCS=ON/OFF` build documentation with Doxygen, default is dependent on wether you are building the library as a top project (ON) or as a part/dep for other (OFF)

# Consumption

You may use FetchContent, provided by cmake, to download the library and use it in your project.The library is designed to be header-only, so you can also just copy the header folder at `wisdom/include` and use it.

# Modules

Library comes with beta C++20 module support. You can enable it using `WISDOM_BUILD_TYPE=modules`.
This feature comes mostly untested, and only works on the latest preview of Visual Studio 2022.
If you found a bug, please report via issues. Testing on linux is also appreciated.

# System Requirements

**Windows:**
- Windows 10/11
- DirectX 12 capable video card
- CMake 3.25+

Tested on MSVC v143, Visual Studio 2022

for Vulkan:
- Vulkan 1.3.2xx+ 
 
**Windows Store:**

To Compile for Windows Store, the following requirements are needed:
- CMakeSettings: `-DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0 -DCMAKE_BUILD_TYPE=Debug/Release/RelWithDebInfo/MinSizeRel`
- Windows 10 SDK 10.0.19041.0+
- Visual Studio Generator, tested on Visual Studio 2022 (v143) - Ninja generator is not supported
- Installed UWP SDK

To launch a project find generated .sln in build `out/build/{BuildName}/examples/hello-triangle-winrt` folder and launch it with Visual Studio. This is due to deployment requirements of UWP applications, which is performed with Visual Studio.

After the first launch, the project can be launched from the Start Menu.

This type of project does not support Vulkan, since Vulkan does not have UWP surface.

**Linux**

- CMake 3.25+
- GCC 12+ or Clang 15+ Tested on GCC 12.0.1
- Vulkan 1.3.2xx+ for dynamic vertex buffers and some latest features

KDUils for the example need some packages to be installed:
`sudo apt install libxkbcommon-dev libxcb-xkb-dev libxkbcommon-x11-dev wayland-scanner++ wayland-protocols`

Visit https://github.com/KDAB/KDUtils to see more details.

**MacOS**
- TBD... When I get my hands on a Mac

# Roadmap

The project has trello page. https://trello.com/b/qjrv97Bc/wisdom-veritas
Suggestions are appreciated.


1. C API Generation
2. Reduce Vulkan overhead (Volk/less shared handles?)
3. Profiling and logs 
4. Bind less model.
5. Fallbacks on demand
6. Result