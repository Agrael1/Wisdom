# CMakeList.txt : CMake project for Wisdom
project(wisdom VERSION 0.1.0)

include(cmake/documentation.cmake)

add_library(wis-header-only INTERFACE)
add_library(wis::wis-header-only ALIAS wis-header-only)
target_include_directories(wis-header-only INTERFACE
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

if(WISDOM_BUILD_BINARIES)
    add_library(${PROJECT_NAME} STATIC)
    add_library(wis::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME} PUBLIC wis-header-only)
    target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_BUILD_BINARIES)
    set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
else()
	add_library(${PROJECT_NAME} INTERFACE)
	add_library(wis::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME} INTERFACE wis-header-only)
    set(WISDOM_PUBLIC INTERFACE)
endif()

# load dx12 and vulkan
include(platform.cmake)
include(dx12.cmake)
include(vulkan.cmake)

# Add common sources
wisdom_sources( TARGET wisdom
    HEADERS
        "include/wisdom/global/definitions.h"
        "include/wisdom/global/internal.h"
        "include/wisdom/util/com_ptr.h"
        "include/wisdom/wisdom.hpp"
        "include/wisdom/util/log_layer.h"
        "include/wisdom/util/flags.h"
        "include/wisdom/bridge/generator.h"
        "include/wisdom/util/exception.h"
        "include/wisdom/util/shader_compiler.h"
        "include/wisdom/util/profile.h"
        "include/wisdom/util/misc.h"
        "include/wisdom/util/small_allocator.h"
        "include/wisdom/bridge/format.h"
        "include/wisdom/bridge/source_location.h"
        "include/wisdom/util/error_messages.h")

set_target_properties(wis-header-only PROPERTIES CXX_STANDARD 20)

target_compile_definitions(
  wis-header-only
  INTERFACE
  DEBUG_MODE=$<IF:$<CONFIG:Debug>,1,0>
  DEBUG_LAYER=$<BOOL:${WISDOM_DEBUG_LAYER}>
  WISDOM_DX12=$<BOOL:${WISDOM_DX12}>
  WISDOM_VULKAN=$<BOOL:${WISDOM_VULKAN}>
  WISDOM_METAL=$<BOOL:${WISDOM_METAL}>
  WISDOM_VERSION=${WISDOM_VERSION}
  WISDOM_LOG_LEVEL=${SEV_INDEX}
  NOMINMAX
  _CRT_SECURE_NO_WARNINGS)

# Formatting library
if(WISDOM_USE_FMT)
  find_package(fmt QUIET)
  target_compile_definitions(wis-header-only INTERFACE WISDOM_USE_FMT)

  message(STATUS "Using fmt library ${FMT_TARGET}")
  target_link_libraries(wis-header-only INTERFACE ${FMT_TARGET})
endif()

# API generation
if(WISDOM_GENERATE_API)
  add_custom_target(
    GenerateAPI
    COMMAND $<TARGET_FILE:generator>
    DEPENDS generator)
  add_dependencies(wis-header-only GenerateAPI)
endif()

# Install
install(TARGETS wis-header-only ${PROJECT_NAME} WisVk
    EXPORT WisdomTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  EXPORT WisdomTargets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  NAMESPACE wis::
  FILE WisdomTargets.cmake # Not sure if this is still needed
  )

include(CMakePackageConfigHelpers)
configure_package_config_file( 
  "../cmake/wisdom-config.cmake.in" 
  "wisdom-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
  )

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
  )

### Install Config and ConfigVersion files
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/wisdom-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )
