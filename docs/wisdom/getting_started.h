/**
 *
 * @page getting_started_page Getting Started
 * This page provides a quick path to build and consume Wisdom with modern CMake.
 * It covers requirements, common install flows, and the current public targets.
 *
 * @section install_sec Installation
 *
 * Wisdom can be consumed in three common ways:
 * - CMake dependency (FetchContent / add_subdirectory)
 * - Installed package with `find_package(wisdom CONFIG REQUIRED)`
 * - NuGet package for Visual Studio projects
 *
 * @note NuGet package is available at: https://www.nuget.org/packages/Wisdom/
 * Release archives are available at: https://github.com/Agrael1/Wisdom/releases/
 *
 * One of the easiest integration options is CMake FetchContent:
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
 * - A supported backend environment:
 *   - Windows: DirectX 12 (default) and Vulkan
 *   - Linux: Vulkan
 *
 * On Windows, Vulkan SDK is optional unless you explicitly force Vulkan.
 * Most third-party dependencies are resolved by CMake during configure.
 * 
 * Consuming the library does not require C++20, as interface is C++11 compliant and C99 compatible.
 * However building or header only usage may require C++20 for `std::format` an other advanced features.
 * @subsection build_steps Build Steps
 *
 * Basic local build:
 *
 * @code{.bash}
 * mkdir build
 * cd build
 * cmake -G Ninja ..
 * cmake --build .
 * @endcode
 *
 * To build only libraries (no tests/examples), configure options explicitly:
 *
 * @code{.bash}
 * mkdir build
 * cd build
 * cmake -G Ninja .. -DWISDOM_BUILD_EXAMPLES=OFF -DWISDOM_BUILD_TESTS=OFF
 * cmake --build .
 * @endcode
 *
 * You can also use project presets when available for your platform/configuration.
 *
 * @subsection quick_consume Quick consume example
 *
 * After install, use `find_package` and link one of the public targets:
 *
 * @code{.cmake}
 * find_package(wisdom CONFIG REQUIRED)
 * target_link_libraries(my_app PRIVATE wis::wisdom)
 * @endcode
 *
 * Four binary targets are currently provided:
 * - `wis::wisdom`
 * - `wis::wisdom-shared`
 * - `wis::wisdom-platform`
 * - `wis::wisdom-platform-shared`
 *
 * Header interface targets are also available (`wis::wisdom-headers`, `wis::wisdom-platform-headers`).
 *
 * @ref library_structure "Library Structure" provides an overview of the library structure and its components.
 * @ref setup_page "Consumption" provides more details for integration and build flags.
 */

/**
 * @page setup_page Consumption
 * This page documents how to consume Wisdom from CMake packages and source builds.
 *
 * @section cmake_usage CMake Usage
 *
 * @subsection find Finding the Library
 *
 * If Wisdom is installed (or provided via package manager), use:
 *
 * @code{.cmake}
 * find_package(wisdom CONFIG REQUIRED)
 * @endcode
 *
 * The config package is installed under `lib/cmake/wisdom`.
 * Add its prefix to `CMAKE_PREFIX_PATH` if CMake cannot locate it automatically.
 *
 * @subsection link Linking the Library
 *
 * Link one binary target with `target_link_libraries`:
 *
 * | Target | Type | Description |
 * |--------|------|-------------|
 * | `wis::wisdom` | Static library | Core API |
 * | `wis::wisdom-shared` | Shared library | Core API (DLL/.so) |
 * | `wis::wisdom-platform` | Static library | Platform interop layer |
 * | `wis::wisdom-platform-shared` | Shared library | Platform interop layer (DLL/.so) |
 *
 * Header interface targets:
 * - `wis::wisdom-headers`
 * - `wis::wisdom-platform-headers`
 *
 * @subsection flags CMake Flags
 *
 * Common configuration options:
 * - `WISDOM_FORCE_VULKAN=ON/OFF` force Vulkan type selection where supported
 * - `WISDOM_USE_FMT=ON/OFF` use fmtlib instead of `std::format`
 * - `WISDOM_BUILD_STATIC=ON/OFF` build static targets
 * - `WISDOM_BUILD_SHARED=ON/OFF` build shared targets
 * - `WISDOM_BUILD_PLATFORM=ON/OFF` build platform interop targets
 * - `WISDOM_BUILD_EXAMPLES=ON/OFF` build examples
 * - `WISDOM_BUILD_TESTS=ON/OFF` build tests
 * - `WISDOM_BUILD_DOCS=ON/OFF` build Doxygen documentation
 * - `WISDOM_DXC_PATH=<path>` custom DXC location
 * - `WISDOM_VULKAN_HEADER_PATH=<path>` custom Vulkan-Headers location
 *
 * @section nuget NuGet Package
 *
 * The nuget package is available at https://www.nuget.org/packages/Wisdom/.
 * Its usage is a little different from the CMake usage.
 * There is no need to link libraries manually, as the package will automatically link the library to your project.
 * There is a property page that allows you to configure the library.
 * Options are the same as CMake flags, but with a different naming convention.
 *
 * @section conan Conan Package
 *
 * The project now provides a Conan 2 recipe (`conanfile.py`) for source-based packaging.
 *
 * @code{.bash}
 * conan create . --build=missing
 * @endcode
 *
 * The recipe maps to library-only CMake configuration and keeps existing build logic.
 * At the moment dependencies are still resolved by project CMake using CPM and NuGet.
 * This works for private Conan remotes, but Conan Center usually requires those dependencies
 * to be available as Conan requirements or vendored in the recipe sources.
 *
 * @section addendum Addendum
 *
 * Conan support is currently focused on private/internal package flows.
 * For Conan Center submission, dependency sourcing should be migrated from CPM/NuGet downloads
 * to Conan packages (or vendored sources) to make builds reproducible in the center CI.
 *
 * @ref examples_page "Examples" provides usage examples for the library.
 */

/**
 * @page library_structure Library Structure
 * This page explains how Wisdom is organized and how that maps to the targets used in examples.
 *
 * @section start_overview_sec Overview
 *
 * Wisdom provides a single API surface with backend-specific implementations under it.
 * The public interfaces are generated and kept consistent across C and C++ bindings.
 *
 * In typical projects, you link one core target and one platform target:
 * - Static: `wis::wisdom` + `wis::wisdom-platform`
 * - Shared: `wis::wisdom-shared` + `wis::wisdom-platform-shared`
 *
 * This is the same linking pattern used by examples such as `hello_triangle`.
 *
 * @section how_it_works How It Works
 *
 * Wisdom sits above DirectX 12 and Vulkan with a minimal abstraction layer.
 * Backend code is split into two families:
 * - `wisdom/dx12` for DirectX 12
 * - `wisdom/vulkan` for Vulkan
 *
 * High-level aliases in `<wisdom/wisdom.hpp>` map to backend-prefixed types:
 * - `wis::DX12*` on DirectX 12 builds
 * - `wis::VK*` on Vulkan builds
 *
 * The main include is `<wisdom/wisdom.hpp>` for core objects and API entry points.
 * Platform extensions are exposed through `<wisdom/wisdom_platform.hpp>`.
 *
 * On Windows, DirectX 12 is the default backend. Vulkan can be forced with
 * `WISDOM_FORCE_VULKAN` (CMake option or preprocessor define).
 *
 * @section using_types Types
 *
 * Use public `.hpp` headers in C++ and `.h` C headers for C/C99 integration.
 *
 * For regular usage, prefer generic aliases (`wis::Instance`, `wis::Device`, `wis::Buffer`, ...).
 * If explicit backend control is needed, use prefixed types (`wis::DX12*` / `wis::VK*`) directly.
 *
 * The C++ API uses `wis::span` in many places. It is intentionally similar to C++20 `std::span`,
 * but remains compatible with C++11. This keeps call sites consistent and improves safety through
 * explicit bounds-aware range passing.
 *
 * The examples mostly use generic aliases and target-based linking, which is the recommended default.
 *
 * Objects follow RAII lifetime management and are move-oriented for low-overhead ownership transfer.
 *
 * @section api_api API
 *
 * Most operations report status with `wis::Result`.
 * C++ helpers keep call sites compact while preserving explicit error handling.
 *
 * Example:
 * @code{.cpp}
 * wis::Result result = wis::success;
 * auto instance = wis::CreateInstance(nullptr, {}, result);
 * @endcode
 *
 * @section extension Extending the Library
 *
 * Extension points are intentionally open-ended.
 * Internal handles can be queried via `GetInternal` / `GetMutableInternal` when integrating
 * custom backend-specific functionality.
 *
 * @note Internals are provided for advanced scenarios and may change between versions.
 * Prefer public APIs unless direct interop is required. The first members of the implementation structures are the
 * guaranteed to represent underlying logical type and guaranteed to stay unless major version bump, so you can safely use them in the extensions.
 * 
 *
 */

/**
 * @page examples_page Initialization
 * This page walks through initialization using the `hello_triangle` example.
 * The sample exists in both C++ and C and demonstrates the same startup flow.
 *
 * @section init_cmake_sec CMake Configuration (from hello_triangle)
 *
 * Example target setup links core + platform targets, with static/shared variants.
 *
 * @code{.cmake}
 * # Static variant
 * target_link_libraries(my_app_cpp
 *   PUBLIC wis::wisdom wis::wisdom-platform SDL3::SDL3)
 *
 * # Shared variant
 * target_link_libraries(my_app_cpp_shared
 *   PUBLIC wis::wisdom-shared wis::wisdom-platform-shared SDL3::SDL3)
 *
 * # Header-interface variant
 * target_link_libraries(my_app_cpp_headers
 *   PUBLIC wis::wisdom-headers wis::wisdom-platform-headers SDL3::SDL3)
 * @endcode
 *
 * The real example also builds C and C++ executables side-by-side and reuses shared backend helpers.
 *
 * @section init_flow_sec Initialization Flow
 *
 * High-level startup sequence used by `hello_triangle`:
 * 1. Create platform extension helper (SDL backend wrapper).
 * 2. Create `Instance` with debug settings and platform extension.
 * 3. Create `Surface` from the window.
 * 4. Query adapters and create `Device`.
 * 5. Create queue, swapchain, fence, heaps, and command objects.
 *
 * @note Once `Device` is created, you usually do not need to keep `Instance` alive for frame execution.
 * `Device` keeps the required internal references.
 * Keep `Instance` only when you plan to re-enumerate adapters and rebuild device selection.
 *
 * @section init_cpp_sec C++ initialization snippet
 *
 * @code{.cpp}
 * #include <wisdom/wisdom.hpp>
 * #include <wisdom/wisdom_platform.hpp>
 *
 * wis::DebugDesc debug_desc{ .enable_debug_layer = true, .callback = log_callback };
 * wis::InstanceExtensionHeader* extensions[] = { platform.Extension() }; // From somewhere like SDL or GLFW platform
 * wrapper
 *
 * wis::Result result;
 * wis::Instance instance = wis::CreateInstance(&debug_desc, wis::span{extensions}, result);
 *
 * wis::Surface surface = platform.CreateWindowSurface(window);
 * wis::AdapterQuery adapters = instance.QueryAdapters(wis::AdapterPreference::Performance, result);
 *
 * wis::DeviceRequirements requirements{};
 * requirements.queue_descs = { queue_descs, 1 };
 * wis::Device device = adapters.CreateDevice(0, requirements, result);
 * @endcode
 *
 * @section init_c_sec C initialization snippet
 *
 * @code{.c}
 * #include <wisdom/wisdom.h>
 *
 * WisDebugDesc debug_desc = {
 *   .enable_debug_layer = true,
 *   .callback = log_callback,
 * };
 *
 * WisInstance instance = {0};
 * WisInstanceExtensionHeader* extensions[] = { platform.platform_extension }; // From somewhere like SDL or GLFW
 * platform wrapper
 * WisResult result = wisCreateInstance(&debug_desc, extensions, 1, &instance);
 *
 * WisAdapterQuery adapters = {0};
 * result = wisInstanceQueryAdapters(&instance, WisAdapterPreferencePerformance, &adapters);
 * result = wisAdapterQueryCreateDevice(&adapters, 0, &requirements, &device);
 * @endcode
 *
 * Both paths continue with queue/swapchain/pipeline creation as shown in `examples/hello_triangle`.
 */
