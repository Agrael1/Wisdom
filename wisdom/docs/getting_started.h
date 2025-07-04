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
 * NuGet package is available at: https://www.nuget.org/packages/Wisdom/
 * 
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
 * @ref setup_page "Consumption" provides more information on how to consume the library in your project.
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
 * - wis::wisdom 
 * - wis::wisdom-debug 
 * - wis::wisdom-extended-allocation 
 * - wis::wisdom-descriptor-buffer 
 * - wis::wisdom-raytracing 
 * - wis::wisdom-extension 
 * 
 * Header targets are also available for header-only usage:
 * - wis::wisdom-headers 
 * - wis::wisdom-debug-headers 
 * - wis::wisdom-extended-allocation-headers 
 * - wis::wisdom-descriptor-buffer-headers 
 * - wis::wisdom-raytracing-headers 
 * - wis::wisdom-extension-headers
 * 
 * The library also provides targets for c++ modules, but they are not recommended for production use yet.
 * - wis::wisdom-module 
 * - wis::wisdom-debug-module 
 * - wis::wisdom-extended-allocation-module 
 * - wis::wisdom-descriptor-buffer-module 
 * - wis::wisdom-raytracing-module 
 * - wis::wisdom-extension-module
 * 
 * There are also platform-specific targets available:
 * - wis::wisdom-platform /-headers /-module
 * 
 * or directly 
 * - wis::wisdom-windows /-headers /-module
 * - wis::wisdom-x11 /-headers /-module
 * - wis::wisdom-wayland /-headers /-module
 * - wis::wisdom-linux /-headers /-module
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
 */

/**
 * @page examples_page Usage Examples
 * 
 * 
 */