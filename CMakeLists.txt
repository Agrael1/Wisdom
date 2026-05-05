cmake_minimum_required(VERSION 3.22)

include(cmake/version.cmake)
project(
  wisdom
  VERSION ${WISDOM_VERSION}
  LANGUAGES C CXX)

include(GNUInstallDirs)
include(GenerateExportHeader)
include(cmake/functions.cmake)
wisdom_detect_platform()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(WTOP ON)
else()
  set(WTOP OFF)
endif()

# Options
option(WISDOM_FORCE_VULKAN "Force Vulkan support" OFF)
option(WISDOM_BUILD_EXAMPLES "Build the example project." ${WTOP})
option(WISDOM_BUILD_TESTS "Build the tests." ${WTOP})
option(WISDOM_BUILD_STATIC "Build the static lib." ON)
option(WISDOM_BUILD_SHARED "Build the dynamic lib." ON)
option(WISDOM_BUILD_PLATFORM "Build unified platform extension library." ON)
option(WISDOM_BUILD_DOCS "Build the documentation." OFF)
option(WISDOM_USE_AGILITY_SDK "Download and use DirectX 12 Agility SDK." ON)
option(WISDOM_USE_CONAN "Use Conan to manage dependencies. Only for library builds." OFF)

# DXC deployment options
set(WISDOM_VULKAN_HEADER_PATH
    ""
    CACHE PATH "Path to custom Vulkan Headers (optional)")

# Conan includes Vulkan Headers.
if (WISDOM_USE_CONAN)
    set(WISDOM_VULKAN ON)
endif()

# Load all dependencies
include(cmake/deps.cmake)
include(cmake/doc.cmake)

message(
  STATUS
    "Wisdom Configuration:
    WISDOM_VULKAN: ${WISDOM_VULKAN}
    WISDOM_VULKAN_VERSION: ${WISDOM_VULKAN_VERSION}
    WISDOM_DX12: ${WISDOM_DX12}
    WISDOM_VERSION: ${WISDOM_VERSION}
    WISDOM_PLATFORM: ${WISDOM_PLATFORM}

    WISDOM_BUILD_DOCS: ${WISDOM_BUILD_DOCS}
    WISDOM_BUILD_EXAMPLES: ${WISDOM_BUILD_EXAMPLES}
    WISDOM_BUILD_TESTS: ${WISDOM_BUILD_TESTS}
    WISDOM_BUILD_STATIC: ${WISDOM_BUILD_STATIC}
    WISDOM_BUILD_SHARED: ${WISDOM_BUILD_SHARED}
    WISDOM_BUILD_PLATFORM: ${WISDOM_BUILD_PLATFORM}
    WISDOM_USE_AGILITY_SDK: ${WISDOM_USE_AGILITY_SDK}
    WISDOM_USE_CONAN: ${WISDOM_USE_CONAN}

    WISDOM_VULKAN_HEADER_PATH: ${WISDOM_VULKAN_HEADER_PATH}")

if(WISDOM_BUILD_EXAMPLES AND WISDOM_BUILD_TESTS)
  add_subdirectory(generator)
endif()
add_subdirectory(src) # add main libraries

if(WISDOM_BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(WISDOM_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

install(FILES README.md DESTINATION .)
install(FILES LICENSE.txt DESTINATION .)
install(FILES version/VERSION DESTINATION version)
install(FILES docs/assets/favicon.png DESTINATION .)
install(FILES cmake/functions.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/wisdom-config-version.cmake
  VERSION ${WISDOM_VERSION}
  COMPATIBILITY SameMajorVersion)
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/install/wisdom-config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/wisdom-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/wisdom-config-version.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/wisdom-config.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom)

# Set up package metadata
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "Agrael")
set(CPACK_NUGET_PACKAGE_AUTHORS "Agrael")
set(CPACK_PACKAGE_DESCRIPTION "A Low-level thin multiplatform and extensible Graphics API layer over Vulkan and DX12")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://agrael1.github.io/Wisdom/")
set(CPACK_NUGET_PACKAGE_REPOSITORY_URL "https://github.com/Agrael1/Wisdom.git")
set(CPACK_NUGET_PACKAGE_ICON "favicon.png") # pulled from installed files
set(CPACK_NUGET_PACKAGE_REPOSITORY_TYPE git)
set(CPACK_NUGET_PACKAGE_LICENSE_EXPRESSION "MIT")
set(CPACK_NUGET_PACKAGE_README "README.md") # pulled from installed files
set(CPACK_PROJECT_CONFIG_FILE "${CMAKE_CURRENT_LIST_DIR}/cmake/install/cpack-options.cmake")

include(CPack)