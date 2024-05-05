# CMakeList.txt : CMake project for Wisdom
project(Wisdom)

include(cmake/documentation.cmake)

add_library(${PROJECT_NAME}Headers INTERFACE)
add_library(${PROJECT_NAME}::Headers ALIAS ${PROJECT_NAME}Headers)
target_include_directories(${PROJECT_NAME}Headers INTERFACE
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

if(WISDOM_BUILD_BINARIES)
    add_library(${PROJECT_NAME} STATIC)
    add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}Headers)
    target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_BUILD_BINARIES)
    set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
else()
	add_library(${PROJECT_NAME} INTERFACE)
	add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}Headers)
endif()


# load dx12 and vulkan
include(platform.cmake)
include(dx12.cmake)
include(vulkan.cmake)

# Add common sources
target_sources(
  ${PROJECT_NAME}Headers
  INTERFACE
  FILE_SET
  HEADERS
  BASE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  FILES
  "include/wisdom/global/definitions.h"
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
  "include/wisdom/global/assertions.h"
  "include/wisdom/util/error_messages.h")

set_target_properties(${PROJECT_NAME}Headers PROPERTIES CXX_STANDARD 20)

target_compile_definitions(
  ${PROJECT_NAME}Headers
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
  target_compile_definitions(${PROJECT_NAME}Headers INTERFACE WISDOM_USE_FMT)

  message(STATUS "Using fmt library ${FMT_TARGET}")
  target_link_libraries(${PROJECT_NAME}Headers INTERFACE ${FMT_TARGET})
endif()

# API generation
if(WISDOM_GENERATE_API)
  add_custom_target(
    GenerateAPI
    COMMAND $<TARGET_FILE:generator>
    DEPENDS generator)
  add_dependencies(${PROJECT_NAME}Headers GenerateAPI)
endif()
