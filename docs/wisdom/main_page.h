/**
 * @mainpage Wisdom Documentation
 * @image html bar.png
 *
 *
 * @section intro_sec Introduction
 *
 * Welcome to the Wisdom project documentation. This library provides minimal abstraction
 * for a low level graphics. It is designed to be lightweight, efficient and extensible focusing on
 * modern C++ standards and best practices with a lot of optimizations for best performance.
 * Wisdom is built with a strong emphasis on extensibility and maintainability, making it suitable for a wide range of
 * applications.
 *
 * @note DOCUMENTATION IS IN PROGRESS. Would appreciate any feedback or contributions to improve it.
 *
 * @section rules_sec Coding Standards and Documentation
 * Wisdom follows a set of coding standards that is derived from complexitioes of implementation.
 * The library is divided into core and extensions. Core library follows a strict procedural style, with focus on
 * supporting C99 and C++11 standards, ensuring wide compatibility and adoption. The interfaces are generated with a
 * custom generator, which ensures consistency across the API and allows for easy maintenance and updates. The code is
 * documented using customized Doxygen, and we strive to keep the documentation up-to-date with the codebase.
 * Generaor also creates documentation headers, writes inline documentation and provides boilerplate for API functions
 * on C++. Basically it generates outer layer of the library, while the inner implementation is written by hand and can
 * follow any style, as long as it is consistent within the implementation file.
 *
 * Extension part is free of standards and is provided to extend functionality without affecting the core library.
 * This allows for a lot of flexibility and creativity in the implementation of extensions, while still maintaining a
 * clear separation of concerns between the core library and extensions.
 *
 * @note Please refer to the @ref contributing_page "Contributing" page for guidelines on contributing to the project.
 *
 * Documentation from 25.8.2025 will adhere to the RFC 2119 guidelines for requirement levels.
 * Latest statement can be found here: https://www.rfc-editor.org/rfc/rfc2119.html
 *
 * The documentation is written in a way that explains behavior and not the state. It mimics the documentation style of
 * Vulkan, where the focus is on describing the behavior of the library and its components, rather than just listing the
 * features or capabilities. This approach allows for a more comprehensive understanding of how the each part of library
 * works and how to use it effectively.
 *
 * @section navigation_sec Quick Navigation
 *
 * - @ref getting_started_page "Getting Started" - Learn how to build the library and integrate it into your project
 * - @ref setup_page "Consumption" - Installation and configuration instructions
 * - @ref library_structure "Library Structure" - Overview of the library structure and components
 * - @ref examples_page "Initialization" - Basic examples of how to use the Wisdom library
 * - [API reference](topics.html) - Detailed reference for all API functions, types, and constants
 *
 * Some additional pages:
 * - @ref contributing_page "Contributing" - Contribution guidelines and how to get involved
 * - @ref why_page "Why Wisdom?" - Explanation of the motivation and goals behind the project
 *
 * @section features_sec Key Features
 *
 * - Modern C++ support (C++20/C++23) while maintaining compatibility with C99 and C++11 for the core library
 * - CMake integration and installation support
 * - Cross-platform compatibility
 * - Open-end extensibility
 * - High performance with minimal overhead
 *
 * @section requirements_sec Build Requirements
 *
 * - CMake 3.22 or higher
 * - C++20 compatible compiler
 * - Vulkan SDK or Vulkan Headers &lt;optional for Windows&gt;
 *
 * @section contact_sec Contact & Support
 *
 * For questions, issues, or contributions, please refer to the project repository at https://github.com/Agrael1/Wisdom
 */

/**
 * @page why_page Why Wisdom?
 * @section motivation_sec Motivation
 * Wisdom came to life out of a desire to make a graphics API, that is more explicit than OpenGL or DirectX 11, but also
 * more user-friendly and easier to use than Vulkan. The goal was also to make a library that is not just a wrapper
 * around graphics APIs, but an extensible and open-ended solution, that can be used for a wide range of applications,
 * from game development to scientific visualization.
 *
 * Most of the modern graphics libraries, that are wrapping Vulkan and DirectX 12, are too abstracted. Something like
 * WebGPU or Qt hide a lot of the complexity under more frendly API, but that comes at a cost of performance with
 * virtual calls, and also makes it harder to understand how the underlying graphics API works. Those libraries are
 * great for keeping a compatibility and wide range, but when you need to squeeze every bit of performance, or you want
 * to have more control over the rendering process, they will tangle you into abstraction layers or force you to use
 * their own way of doing things, which can be limiting and frustrating. The point of Wisdom in this case was obvious -
 * don't just hide the complexity, but provide a clear API that allow users to have a good functionality itself, but
 * also give a way to extend with new features and optimizations, without forcing them to break into the abstraction and
 * ultimately abandon the library.
 *
 * In foundation, from version 0.1, Wisdom provided a `GetInternal()` function, that allows access to the internal
 * implementation of the library. Version 0.7 took it a step further and guaranteed that first member of the internal
 * implementation will always be an underlying logical type (e.g. `VkDevice` for `Device` in Vulkan implementation).
 *
 * @section performance_sec Performance
 * While extensibility is solved by the design of the library, performance is achieved through a combination of careful
 * design and implementation. The main focus here is to minimize overhead, while providing reasonable abstraction.
 * First of all, there are no virtual calls in the core library. The ultimate questions here were: **"Would it be
 * possible to use Vulkan buffer with DirectX 12 Command list?"** and **"Why would I need to virtualize the interface if
 * I know the implementation from the start?"**. The answer to both questions is **"No"**. The library is designed in a
 * way that allows users to choose the implementation they want to use at compile time, and then use it without any
 * additional overhead. This allows for a ton of inlining and interprocedural optimizations, which, when added together,
 * can result in significant performance improvements. The goal sounds like this: **"Library lets you operate the code
 * as if you were writing the underlying library yourself."**
 *
 * @section extensibility_sec Extensibility
 * Wisdom has `GetInternal()` function, that allows access to the internal implementation of the library. But what if
 * user wants to extend the library with features that are not paired with the underlying graphics API? For example, if
 * user wants to implement a custom memory allocator, or have an NVidia GSync extension? Usually, this would require
 * breaking into the abstraction and writing custom code that is not part of the library, which is fragile by the nature
 * and can lead to maintenance issues. With Wisdom, users can create extensions that are built on top of the core
 * library, and they have access to either device or instance extensions. From there, they can implement any
 * functionality by querying the functions from Vulkan or using DirectX 12 interfaces directly, while still maintaining
 * the benefits of using the library for the core functionality. This allows for a lot of flexibility and creativity in
 * the implementation of extensions, while still maintaining a clear separation of concerns between the core library and
 * extensions. Yes, there is a bit of overhead to this, but it is negligible, and the benefits of having a clear and
 * extensible API outweigh the costs. In Vulkan implementation there are powerful resolvers in place, so even if you
 * already include a vulkan extension and its structures at the initialization in another place, you will still get
 * structures with properties and features filled in! This spares you from tedious work of keeping track of what you
 * have included and what you haven't.
 *
 * @section faq_sec Possible Questions
 * - **Why not just use Vulkan or DirectX 12 directly?** - While using Vulkan or DirectX 12 directly can provide maximum
 * control and performance, it also comes with a steep learning curve and can be overwhelming for developers who are new
 * to graphics programming. Wisdom provides a more user-friendly API that abstracts away some of the complexities of
 * Vulkan and DirectX 12, while still allowing for high performance and extensibility for advanced users who don't want
 * to spend their valuable time fighting the underlying graphics API.
 *
 * - **Why does the library have DirectX 12 implementation event though Vulkan is cross-platform?** - Short answer -
 * compatibility. While Vulkan is indeed cross-platform, the implementation is extremely demanding. It requires a lot of
 * extensions, such as
 * [VK_EXT_descriptor_heap](https://docs.vulkan.org/refpages/latest/refpages/source/VK_EXT_descriptor_heap.html). This
 * extension is said to replace VkDescriptorPool, which is extremely high level abstraction for Vulkan, and it was
 * released in 2026. At a later point I will probably do a backport to Descriptor buffer, but for now Vulkan
 * implementation is something that is supported only on high end graphics cards, and it is not something that can be
 * used on older hardware. DirectX 12, on the other hand, is supported on a wider range of hardware.
 */
