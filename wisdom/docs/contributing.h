/**
 * @page contributing_page Contributing
 *
 * This page provides a comprehensive guide on how to contribute to the Wisdom project.
 *
 * @section overview_sec Overview
 *
 * We welcome contributions to the Wisdom project! Whether you're fixing bugs, adding features, improving documentation, or helping with testing, your contributions are valuable to the community.
 *
 * Before contributing, please familiarize yourself with our codebase structure and coding standards outlined below.
 *
 * @section getting_started_contributing Getting Started
 *
 * 1. **Fork the repository** on GitHub: https://github.com/Agrael1/Wisdom
 * 2. **Clone your fork** locally: `git clone https://github.com/your-username/Wisdom.git`
 * 3. **Create a feature branch**: `git checkout -b feature/your-feature-name`
 * 4. **Set up the build environment** following the @ref getting_started_page "Getting Started" guide
 * 5. **Make your changes** following our coding standards
 * 6. **Test your changes** thoroughly
 * 7. **Submit a pull request**
 *
 * @section coding_standards_sec Coding Standards and Best Practices
 *
 * @subsection code_style Code Style
 *
 * The Wisdom project uses a **custom clang-format configuration** located in `.clang-format` at the root of the repository. All code should be formatted according to this style:
 *
 * - **Base Style**: WebKit
 * - **Pointer Alignment**: `Type* pointer` (pointer binds to type)
 * - **Brace Style**: Custom with specific wrapping rules
 * - **Template Declarations**: Always break before template declarations
 *
 * **To format your code:**
 * ```bash
 * clang-format -i --style=file your-file.cpp
 * ```
 * Or use your IDE's built-in clang-format support.
 *
 * @note The generator automatically formats generated files using clang-format during the build process.
 * The repo formatting is also enforced in CI using GitHub Actions, so you don't have to worry about it.
 *
 * @subsection naming_conventions Naming Conventions
 *
 * Names follow Windows API conventions where applicable:
 * - **Implementation Classes**: PascalCase (e.g., `ImplDX12Device`, `CommandQueue`, `DescriptorStorage`)
 * - **Functions**: PascalCase (e.g., `CreateFactory`, `GetAdapter`)
 * - **Variables and constants**: snake_case (e.g., `command_list`, `descriptor_heap`)
 * - **Defines**: SCREAMING_SNAKE_CASE (e.g., `WISDOM_VERSION`)
 * - **Enums and enum values**: PascalCase (e.g., `Status`, `Ok`, `ErrorInvalidArgument`)
 * - **Container Classes**: snake_case (e.g., `unique_event`). These are usually templated containers or wrappers around STL containers.
 * - **Namespaces**: snake_case (e.g., `wis`)
 *
 * @note I just prefer this style, it is not strictly enforced.
 *
 * @subsection api_design API Design Principles
 *
 * - **Result-based returns**: Most functions return `wis::Result` as 1st value, that allows RVO and easy error handling
 * - **RAII compliance**: All resources manage their own lifetime, no raw pointers outside of implementation
 * - **Move semantics**: Most classes are move-only to prevent resource duplication
 * - **No virtual functions**: Direct API calls for maximum performance, unless designing extension classes for device or factory
 * - **Platform abstraction**: Code should work on both DirectX 12 and Vulkan backends, unless explicitly stated otherwise
 *
 * @subsection language_standards Language Standards
 *
 * - **Primary**: C++20 (required)
 * - **Supported**: C++23 features where available. If you want to use C++23 features, please ensure they are supported by all target compilers by using feature macros. Don't use `__cplusplus`! It is unreliable on MSVC.
 * - **Legacy**: If you see any C++17 or older constructs, please consider updating them to modern C++20 equivalents.
 * - **Modules**: Experimental C++20 modules support (opt-in via `WISDOM_EXPERIMENTAL_CPP_MODULES`). Modules are not required and the project fully supports traditional headers.
 *
 * @section build_requirements_sec Build Requirements
 *
 * @subsection cmake_requirements CMake Requirements
 *
 * - **Minimum CMake version**: 3.28
 * - **Build system**: Ninja (recommended)
 * - **Presets**: Use CMake presets for consistent builds
 *
 * @subsection compiler_requirements Compiler Requirements
 *
 * - **Windows**: MSVC 2022 or Clang 16+
 * - **Linux**: GCC 13+ or Clang 16+
 * - **C++20 standard library**: Required for `std::format` (or use `WISDOM_USE_FMT=ON`)
 *
 * @section testing_sec Testing
 *
 * @subsection test_framework Test Framework
 *
 * The project uses **Catch2 v3 Unit Test Framework** for testing. Tests are located in the `tests/` directory.
 *
 * @subsection running_tests Running Tests
 *
 * ```bash
 * # Build with tests enabled (default when building as top-level project)
 * cmake --preset x64-debug -DWISDOM_BUILD_TESTS=ON
 * cmake --build --preset x64-debug
 *
 * # Run tests
 * ctest --preset x64-debug
 * ```
 *
 * @subsection writing_tests Writing Tests
 *
 * - Place test files in `tests/basic/` or appropriate subdirectory
 * - Use Catch2 macros: `TEST_CASE`, `REQUIRE`, `CHECK`
 * - Test both DirectX 12 and Vulkan backends when applicable
 * - Include proper cleanup and resource management
 *
 * **Example test structure:**
 * ```cpp
 * #include <catch2/catch_test_macros.hpp>
 * #include <wisdom/wisdom.hpp>
 *
 * TEST_CASE("your_test_name") {
 *     // Setup
 *     auto [result, factory] = wis::CreateFactory();
 *     REQUIRE(result.status == wis::Status::Ok);
 *
 *     // Test logic
 *     // ...
 *
 *     // Assertions
 *     REQUIRE(condition);
 * }
 * ```
 *
 * @section documentation_sec Documentation
 *
 * @subsection doxygen_docs Doxygen Documentation
 *
 * - All public APIs must be documented with Doxygen comments
 * - Follow RFC 2119 guidelines for requirement levels (MUST, SHOULD, MAY)
 * - Use `@param`, `@return`, `@note`, `@code` appropriately
 * - Documentation is automatically generated and deployed to GitHub Pages
 *
 * @subsection doc_generation Documentation Generation
 *
 * ```bash
 * # Enable documentation build
 * cmake --preset x64-release -DWISDOM_BUILD_DOCS=ON
 * cmake --build --preset x64-release --target docs
 * ```
 *
 * @section extension_development_sec Extension Development
 *
 * @subsection creating_extensions Creating Extensions
 *
 * - Place extensions in `wisdom/extensions/your_extension/`
 * - Follow existing extension patterns (see `wisdom/extensions/debug/`, `wisdom/extensions/raytracing/`)
 * - Provide both header-only and library targets
 * - Include CMake integration
 *
 * @subsection extension_structure Extension Structure
 *
 * ```
 * wisdom/extensions/your_extension/
 * ├── CMakeLists.txt
 * ├── wisdom/
 * │   ├── your_extension.hpp
 * │   ├── your_extension.include.h
 * │   ├── dx12_your_extension.hpp
 * │   ├── vk_your_extension.hpp
 * │   └── impl/
 * │       ├── dx12_your_extension.cpp
 * │       └── vk_your_extension.cpp
 * ```
 *
 * @section submission_guidelines_sec Submission Guidelines
 *
 * @subsection pull_requests Pull Requests
 *
 * 1. **Create descriptive commits**: Use clear, concise commit messages
 * 2. **Target the right branch**: Submit PRs against the `master` branch
 * 3. **Include tests**: New features should include appropriate tests
 * 4. **Update documentation**: Update relevant documentation pages
 * 5. **Check CI**: Ensure all GitHub Actions workflows pass
 *
 * @subsection pr_checklist Pull Request Checklist
 *
 * - [ ] Code follows the project's coding standards
 * - [ ] Code is formatted with clang-format (forced in CI)
 * - [ ] All tests pass locally
 * - [ ] New tests added for new functionality
 * - [ ] Documentation updated (if applicable)
 * - [ ] No breaking changes (or clearly documented)
 * - [ ] Vulkan and DirectX 12 backends both work (if applicable)
 *
 * @subsection commit_messages Commit Message Format
 *
 * Use descriptive commit messages that explain what and why:
 *
 * ```
 * feat: add descriptor buffer extension support
 *
 * - Implement descriptor buffer for both DX12 and Vulkan
 * - Add comprehensive tests for descriptor operations
 * - Update documentation with usage examples
 *
 * Fixes #123
 * ```
 *
 * @section ci_cd_sec Continuous Integration
 *
 * @subsection github_actions GitHub Actions
 *
 * The project uses GitHub Actions for CI/CD with the following workflows:
 *
 * - **Linux build**: Ubuntu with Vulkan backend
 * - **Windows DirectX 12**: MSVC with DirectX 12 backend
 * - **Windows Vulkan**: MSVC with Vulkan backend
 * - **Windows Clang**: Clang with Vulkan and C++20 modules
 * - **Documentation**: Auto-generated and deployed to GitHub Pages
 * - **Release**: Automated NuGet and GitHub releases
 *
 * @subsection build_matrix Build Matrix
 *
 * All PRs are automatically tested against:
 * - **Platforms**: Windows, Linux
 * - **Compilers**: MSVC, Clang, GCC
 * - **Graphics APIs**: DirectX 12, Vulkan
 * - **Build types**: Debug, Release
 *
 * @section contribution_types_sec Types of Contributions
 *
 * @subsection bug_fixes Bug Fixes
 *
 * - Report bugs using GitHub Issues
 * - Include minimal reproduction case
 * - Specify platform, compiler, and graphics API
 * - Test fixes on both graphics backends when possible
 *
 * @subsection feature_additions Feature Additions
 *
 * - Discuss major features in GitHub Issues first
 * - Ensure cross-platform compatibility
 * - Follow existing API patterns
 * - Include comprehensive tests and documentation
 *
 * @subsection documentation_improvements Documentation Improvements
 *
 * - Fix typos, clarify explanations
 * - Add usage examples
 * - Improve API documentation
 * - Update getting started guides
 *
 * @subsection platform_support Platform Support
 *
 * - Add support for new platforms
 * - Improve existing platform integrations
 * - Update CMake configuration
 * - Ensure proper CI coverage
 *
 * @section cmake_flags_sec Important CMake Flags for Development
 *
 * - `WISDOM_BUILD_TESTS=ON`: Enable test compilation
 * - `WISDOM_BUILD_EXAMPLES=ON`: Enable example compilation
 * - `WISDOM_BUILD_DOCS=ON`: Enable documentation generation
 * - `WISDOM_FORCE_VULKAN=ON`: Force Vulkan backend on Windows
 * - `WISDOM_DEBUG_LAYER=ON`: Enable debug validation layers
 * - `WISDOM_EXPERIMENTAL_CPP_MODULES=ON`: Enable C++20 modules
 * - `WISDOM_USE_FMT=ON`: Use fmtlib instead of std::format
 *
 * @section community_sec Community and Communication
 *
 * - **Repository**: https://github.com/Agrael1/Wisdom
 * - **Issues**: Use GitHub Issues for bug reports and feature requests
 * - **Discussions**: Use GitHub Discussions for questions and general discussion
 * - **Contact**: agrael@live.ru (for code of conduct violations)
 *
 * @section license_sec License
 *
 * By contributing to Wisdom, you agree that your contributions will be licensed under the same license as the project.
 * Please see the LICENSE.txt file in the repository root for details.
 *
 * @note Thank you for your interest in contributing to Wisdom! Your contributions help make this library better for everyone.
 */
