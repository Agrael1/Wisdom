/**
 *
 * @page getting_started_page Getting Started
 * 
 * This page provides a quick guide to get started with the Wisdom library. The Wisdom library is designed to be lightweight and efficient, focusing on modern C++ standards and best practices. This guide will help you set up the library, understand its basic usage, and provide resources for further exploration.
 * 
 * @section install_sec Installation
 * 
 * There are several ways to install the Wisdom library. The recommended method is to use CMake, which allows for easy integration into your project.
 * However, you can also clone the repository directly or download a release package. If you are using a Visual Studio project, you can also use a public NuGet package to install Wisdom.
 * 
 * @note NuGet package is available at: https://www.nuget.org/packages/Wisdom/
 * The latest version of release for Windows can be downloaded from the GitHub releases page: https://github.com/Agrael1/Wisdom/releases/tag/Latest
 * 
 * One of the easiest ways to install Wisdom is to use CMake FetchContent or CMake Package Manager.
 * 
 * @code{.cmake}
 * include(FetchContent)
 * FetchContent_Declare(
 *   Wisdom
 *   GIT_REPOSITORY https://github.com/Agrael1/Wisdom.git
 *   GIT_TAG        master
 * )
 * FetchContent_MakeAvailable(Wisdom)
 * @endcode
 * 
 * @subsection requirements Requirements
 *
 * - CMake 3.22 or higher
 * - C++20 compatible compiler
 * - Ninja build system (recommended)
 * 
 * On Windows you are not required to install Vulkan SDK, but it is recommended if you are working with Vulkan.
 * Other requirements are downloaded automatically by CMake.
 *
 * @subsection build_steps Build Steps
 * 
 * There is a simple way to build the Wisdom library using CMake. Follow these steps to build the library:
 *
 * @code{.bash}
 * mkdir build
 * cd build
 * cmake -G Ninja ..
 * ninja
 * @endcode
 * 
 * However that will build examples and tests as well. If you want to build only the library, you can use presets.
 * 
 * The library can be built using the following command:
 * 
 * @code{.bash}
 * mkdir build
 * cd build
 * cmake --preset x64-release-only-lib ..
 * cmake --build --preset x64-release-only-lib
 * @endcode
 * 
 * This will build the library in release mode for windows. You can also use other presets to build the library in different configurations.
 * The shown configuration is used for NuGet package.
 * 
 * As much as Wisdom would like to be header-only library, it is not possible due to the nature of the dependencies. But it is still possible to use it as a header-only library from CMake.
 * For that you can use cmake targets that have suffix `-headers`.
 * 
 * @ref library_structure "Library Structure" provides an overview of the library structure and its components.
 * @ref setup_page "Initialization" provides more information on how to use the library in your project.
 */


/**
 * @page setup_page Consumption
 * 
 * This page provides information on how to consume the Wisdom library in your project. It covers how to link the library, include headers, use CMake flags, and use the provided features.
 * 
 * @section cmake_usage CMake Usage
 * 
 * @subsection find Finding the Library
 * 
 * Either you have downloaded the library as a package or you have built it from source, you can link it to your project using CMake.
 * 
 * The recommended way to link the library from releases or installation is to use the `find_package` command. This will automatically find the library and its dependencies.
 * 
 * The targets file is installed to the `lib/cmake/Wisdom` directory. You can add this directory to your CMake project using the `CMAKE_PREFIX_PATH` variable.
 * 
 * @subsection link Linking the Library
 * 
 * Once you have found the library, you can link it to your project using the `target_link_libraries` command.
 * The available targets are:
 * 
 * 
 * @subsection core_targets Core Library Targets
 *
 * | Target | Library | Headers | Module | Description |
 * |--------|---------|---------|--------|-------------|
 * | **Main Library** | `wis::wisdom` | `wis::wisdom-headers` | `wis::wisdom-module` | Main library target |
 * | **Debug Extension** | `wis::wisdom-debug` | `wis::wisdom-debug-headers` | `wis::wisdom-debug-module` | Debug extension with logging |
 * | **Extended Allocation** | `wis::wisdom-extended-allocation` | `wis::wisdom-extended-allocation-headers` | `wis::wisdom-extended-allocation-module` | Extended memory allocation features |
 * | **Descriptor Buffer** | `wis::wisdom-descriptor-buffer` | `wis::wisdom-descriptor-buffer-headers` | `wis::wisdom-descriptor-buffer-module` | Descriptor buffer extension |
 * | **Raytracing** | `wis::wisdom-raytracing` | `wis::wisdom-raytracing-headers` | `wis::wisdom-raytracing-module` | Raytracing support |
 *
 * @subsection platform_targets Platform-Specific Targets
 *
 * | Target | Library | Headers | Module | Description |
 * |--------|---------|---------|--------|-------------|
 * | **Platform (auto)** | `wis::wisdom-platform` | `wis::wisdom-platform-headers` | `wis::wisdom-platform-module` | Auto-detected platform support |
 * | **Windows** | `wis::wisdom-windows` | `wis::wisdom-windows-headers` | `wis::wisdom-windows-module` | Windows-specific implementation + interop |
 * | **X11** | `wis::wisdom-x11` | `wis::wisdom-x11-headers` | `wis::wisdom-x11-module` | X11 windowing system support |
 * | **Wayland** | `wis::wisdom-wayland` | `wis::wisdom-wayland-headers` | `wis::wisdom-wayland-module` | Wayland compositor support |
 * | **Linux** | `wis::wisdom-linux` | `wis::wisdom-linux-headers` | `wis::wisdom-linux-module` | Linux platform interop support |
 *
 * @note The C++ modules targets are experimental and not recommended for production use yet.
 * To enable C++ modules in CMake, you need to set the `WISDOM_EXPERIMENTAL_CPP_MODULES` CMake flag to `ON`.
 * 
 * @subsection flags CMake Flags
 * 
 * The library provides several CMake flags that can be used to customize the build process.
 * 
 * - `WISDOM_LOG_LEVEL=debug/warn` set the log level for the library, values are `debug,trace,info,warn,error,critical` log calls under current level are not compiled
 * - `WISDOM_USE_SYSTEM_DXC=OFF` use system DXC compiler instead of the one provided with the library (default uses the one provided)
 * - `WISDOM_FORCE_VULKAN=OFF` if set `ON` forces base types to be Vulkan, useful for debugging Vulkan extensions
 * - `WISDOM_BUILD_EXAMPLES=ON` enable/disable example compilation
 * - `WISDOM_BUILD_TESTS=ON` enable/disable test compilation
 * - `WISDOM_USE_FMT=ON/OFF` use fmt instead of `std::format` (`ON` for Linux build for GCC<13 and Clang<16)
 * - `WISDOM_BUILD_DOCS=ON/OFF` build documentation with Doxygen, default is dependent on wether you are building the library as a top project (ON) or as a part/dep for other (OFF)
 * - `WISDOM_BUILD_BINARIES=ON/OFF` build static lib. If turned off, the header only version will be the main target.
 * - `WISDOM_EXPERIMENTAL_CPP_MODULES=ON/OFF` enable C++20 modules support.
 * 
 * Most important flag is `WISDOM_FORCE_VULKAN`. There is the same #define in the library code. This flag allows you to force the library to use Vulkan types instead of DirectX 12 types.
 * The library picks the best available API for the platform, but you can force it to use Vulkan types if you want to use the library with Vulkan.
 * This is useful for debugging Vulkan extensions or if you want to use the library with Vulkan to further extend it.
 * 
 * @section nuget NuGet Package
 * 
 * The nuget package is available at https://www.nuget.org/packages/Wisdom/.
 * Its usage is a little different from the CMake usage.
 * There is no need to link libraries manually, as the package will automatically link the library to your project.
 * There is a property page that allows you to configure the library.
 * Options are the same as CMake flags, but with a different naming convention.
 * 
 * @section addendum Addendum
 * 
 * I would like to add more consumption examples, like conan or vcpkg, but I don't have enough experience with them.
 * I would appreciate any contributions to improve that part.
 * 
 * @ref examples_page "Examples" provides usage examples for the library.
 */

/**
 * @page library_structure Library Structure
 * 
 * This page provides an overview of the Wisdom library structure and its components.
 * 
 * @section overview_sec Overview
 * 
 * The Wisdom library is structured into several modules, each providing a specific functionality. The provided classes and functions are designed to be easy to use and overhead free.
 * When using the library as a header-only library, all the functions are inline and do not incur any overhead.
 * Most of the classes are done in an independent way, so you only need to store classes that you need.
 * The classes, as well as functions and types and their respective documentation, are generated to maintain consistency of interface and ease of use.
 * Files that generate everything are stored at `xml` directory. Those files can be used to generate the API and its documentation in any other language, providing you have a geneartor and a way to link those functions to C++ code.
 * 
 * @section how_it_works How It Works
 * 
 * The library sits above the DirectX 12 and Vulkan APIs, providing a minimal abstraction layer. The library implementation is split into 2 implementations: one for DirectX 12 and one for Vulkan.
 * Both implementations are designed to be as similar as possible, providing a consistent interface for the user.
 * The library also does not use virtual functions, so it is as fast as possible.
 * 
 * The implementation for DirectX 12 is done in the `wisdom/dx12` directory, while the implementation for Vulkan is done in the `wisdom/vulkan` directory.
 * DirectX 12 implementation is prefixed with `DX12` and Vulkan implementation is prefixed with `VK`.
 * So the classes are named `DX12Factory`, `DX12Adapter`, `VKFactory`, `VKAdapter` and so on.
 * 
 * Then the implementation is wrapped into the `wis::Factory`, `wis::Adapter`, `wis::Device` and so on.
 * File `wisdom/include/wisdom/wisdom.hpp` is the main header file that includes all the necessary headers for the library.
 * It also selects the implementation based on the platform and the API.
 * On Windows, it will use DirectX 12 implementation, while on Linux it will use Vulkan implementation.
 * 
 * @note To use Vulkan implementation on Windows, you can set the `WISDOM_FORCE_VULKAN` CMake flag to `ON` or define `WISDOM_FORCE_VULKAN` in global preprocessor.
 * NuGet package also has this flag in the propertiy page of the project.
 * 
 * The implementation will scan if you have Vulkan SDK installed and if it is available, it will use Vulkan implementation.
 * Otherwise it will produce an error that Vulkan is not available.
 * 
 * @note For modules there are 2 implementations - one uses regular module name `import wisdom` and the other uses `import wisdom.fvk`.
 * This is done due to the fact that preprocessor switches do not affect modules imported from other libraries.
 * You cannot use both imports in the same project, so you should choose one of them. Althogh both modules provide implemntations for both DirectX 12 and Vulkan.
 * 
 * @section using_types Types
 * 
 * Only .hpp files provided to be used in your project. The .h files contain the implementation details and are not meant to be used directly.
 * However types that they provide can be used directly in your project. One use case is to use both DirectX 12 and Vulkan implementations in the same project.
 * In such case you should use prefixed types, like `wis::DX12Factory` and `wis::VKFactory`, to avoid name clashes.
 * Functions that create device and factory are also prefixed with `DX12` or `VK` for that particular reason.
 * 
 * For regular use you can use the `wis::Factory`, `wis::Adapter`, `wis::Device` and so on types, which are the main types of the library.
 * All the extensions and platform-specific addons follow the same naming convention, so you can use them without any issues.
 * 
 * All the examples and documentation will be provided with non-prefixed types to simplify the usage.
 * 
 * All the types have a destructor and independent lifetime, so you can use them without worrying about the lifetime of the underlying resources.
 * For example, if `wis::Device` is destroyed before `wis::Buffer`, the buffer will still be valid and can be used. The underlying resources will be released when the last reference to them is destroyed.
 * The library uses RAII principles to manage the lifetime of the resources, so you don't have to worry about memory leaks or resource management.
 * Most of the classes are move-only, so you can use them without worrying about copying the resources.
 * 
 * @section api_api API
 * 
 * Provided API is designed to be easy to use and understand. It is based on the modern C++ standards and best practices.
 * Most of the API functions return `wis::Result` type, which is a simple wrapper around the error code and the error message.
 * Error messages are brief and compile-time created, and their lifetime is static, so you can use them without worrying about the lifetime of the error message.
 * Since the library is designed to be used with modern C++ standards, output is best handled with structured bindings.
 * 
 * Example of how to use the API:
 * @code{.cpp}
 * auto&& [result, factory] = wis::CreateFactory();
 * @endcode
 * 
 * The `CreateFactory` function will return a `wis::Result` and a `wis::Factory` object. The `wis::Result` object will contain the error code and the error message, while the `wis::Factory` object will contain the factory object.
 * 
 * The library also provides RVO (Return Value Optimization) for the functions that return objects.
 * Those functions will return the object directly, and result will be stored in the first argument of the function.
 * 
 * Example of how to use the API with RVO:
 * @code{.cpp}
 * wis::Result result = wis::success;
 * auto factory = wis::CreateFactory(result);
 * @endcode
 * 
 * Those functions are generated with first argument being `wis::Result&` and returning the object directly. So if you want to squeeze the last bit of performance, you can use this approach, since it will avoid using extra move constructor or move assignment.
 * 
 * 
 * @section extension Extending the Library
 * 
 * The library is designed to be extensible, so you can add your own extensions and functionality to it.
 * Open-ended extensibility is one of the key features of the library. Every class has a corresponding Internal class that provides the implementation details.
 * To query the internal implementation, you can use the `GetInternal` and `GetMutableInternal`. Once you have the internal implementation, you can use it to extend the library with your own functionality.
 * To follow the practice you can see the implementation of any extension from `wisdom/extensions` directory.
 * 
 * You don't have to use the same conventions as the library does, but it is recommended to follow the same naming conventions and structure to keep the code consistent and easy to read.
 * 
 * @note Although the library provides internals, it is not recommended to rely on them in your code.
 * The internals are subject to change and may not be stable across different versions of the library.
 * Although logical components like `wis::DX12Device` having `ID3D12Device*` are guaranteed to exist as long as underlying APIs don't change the logic.
 * 
 */

/**
 * @page examples_page Initialization
 *
 * This page provides basic examples of how to use the Wisdom library in your project. The examples cover initialization and basic setup.
 * Most of the examples are provided in the `examples` directory of the library.
 *
 * @section init_sec Factory Initialization
 *
 * In order to use the Wisdom library, you need to create the `wis::Factory`. The concept is similar to the `IDXGIFactory` in DirectX 12 and `VkInstance` in Vulkan.
 *
 * First, you need to include the main header file:
 * 
 * @code{.cpp}
 * #include <wisdom/wisdom.hpp>
 * 
 * int main() {return 0;}
 * @endcode
 * 
 * To create the factory, you can use the `wis::CreateFactory` function. This function will create the factory and return it.
 * 
 * @code{.cpp}
 * #include <wisdom/wisdom.hpp>
 * 
 * int main() {
 *   auto&& [result, factory] = wis::CreateFactory();
 *   return 0;
 * }
 * @endcode
 * 
 * // or with RVO (Return Value Optimization)
 * 
 * @code{.cpp}
 * int main() {
 *   wis::Result result = wis::success;
 *   auto factory = wis::CreateFactory(result);
 * }
 * @endcode
 * 
 * Thats it! You have created the factory and can use it to create adapters and devices.
 * 
 * @note The vulkan implemntation uses static storage for some of the objects. However it is thread safe.
 * Though I don't see a reason to create more than one factory in your application.
 * 
 * @section factory_extensions Factory Extensions
 * 
 * `wis::CreateFactory` function will create the factory with the default extensions. However there are some extensions that you can use to extend the factory functionality.
 * One of such extensions is `wis::DebugExtension`, which provides debug functionality for the factory. It is extension, so you need to link it to your project and include the header file:
 * 
 * @code{.cmake}
 * # CMakeLists.txt
 * target_link_libraries(my_project PUBLIC wis::wisdom-debug)
 * @endcode
 * 
 * @code{.cpp}
 * // main.cpp
 * #include <wisdom/wisdom_debug.hpp>
 * @endcode
 * 
 * To use the extension, you need to pass it to the `wis::CreateFactory` function as an argument.
 * The first argument for `wis::CreateFactory` is a debug flag, which must be set to `true` if you want to use the debug extension.
 * Then all the extensions are passed as an array of pointers to the `wis::FactoryExtension` objects.
 * 
 * @code{.cpp}
 * wis::DebugExtension debug_ext;
 * wis::FactoryExtension* extensions[] = { &debug_ext };
 * auto&& [result, factory] = wis::CreateFactory(true, extensions, std::size(extensions));
 * @endcode
 * 
 * That code will create the factory with the debug extension enabled. 
 *
 * @note The extension is initialized in the `CreateFactory` function. 
 * Before that point the extension is not initialized and cannot be used.
 * This is also true for Device extensions.
 * 
 * After the factory is created you can use factory to create adapters and devices. The debug extension can then be used to create debug messenger.
 *
 * You can also use other extensions to extend the factory functionality.
 * 
 * List of factory extensions:
 * 
 * | Extension | Header | Capability |
 * |-----------|--------|------------|
 * | `wis::DebugExtension` | `<wisdom/wisdom_debug.hpp>` | Debug functionality for the factory |
 * | `wis::WindowsExtension` | `<wisdom/wisdom_platform.hpp>` or `<wisdom/wisdom_windows.hpp>` | Windows-specific functionality for factory and swapchain creation |
 * | `wis::X11Extension` | `<wisdom/wisdom_platform.hpp>` or `<wisdom/wisdom_x11.hpp>` | X11-specific functionality for factory and swapchain creation |
 * | `wis::WaylandExtension` | `<wisdom/wisdom_platform.hpp>` or `<wisdom/wisdom_wayland.hpp>` | Wayland-specific functionality for factory and swapchain creation |
 *
 * 
 */