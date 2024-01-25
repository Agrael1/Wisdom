# CMakeList.txt : CMake project for Wisdom
project(Wisdom)

include(cmake/documentation.cmake)

add_library(${PROJECT_NAME} STATIC)

# load dx12 and vulkan
include(dx12.cmake)
include(vulkan.cmake)
include(platform.cmake)

if(CMAKE_SYSTEM_NAME STREQUAL "iOS"
   OR CMAKE_SYSTEM_NAME STREQUAL "tvOS"
   AND NOT WISDOM_FORCE_VULKAN)
  message("Platform Library is Metal")
  set(WISDOMMTL TRUE)
  message(FATAL_ERROR "Metal is not implemented")
endif()

set(HEADER_LIST
    "include/wisdom/wisdom.h"
    "include/wisdom/wisdom.hpp"
    "include/wisdom/util/log_layer.h"
    "include/wisdom/util/flags.h"
    "include/wisdom/bridge/generator.h"
    "include/wisdom/global/definitions.h"
    "include/wisdom/util/exception.h"
    "include/wisdom/api/api_factory.h"
    "include/wisdom/api/api_barrier.h"
    "include/wisdom/api/api_internal.h"
    "include/wisdom/api/api_adapter.h"
    "include/wisdom/api/api_swapchain.h"
    "include/wisdom/api/api_barrier.h"
    "include/wisdom/shader_compiler.h"
    "include/wisdom/api/api_shader.h"
    "include/wisdom/api/api_input_layout.h"
    "include/wisdom/util/profile.h"
    "include/wisdom/api/api_common.h"
    "include/wisdom/util/misc.h"
    "include/wisdom/api/api_render_pass.h"
    "include/wisdom/util/small_allocator.h"
    "include/wisdom/bridge/format.h"
    "include/wisdom/bridge/source_location.h"
    "include/wisdom/global/assertions.h" "include/wisdom/util/error_messages.h" "include/wisdom/util/com_ptr.h" "include/wisdom/api/consts.h")

# Add source to this project's executable.
target_sources(
  ${PROJECT_NAME}
  PUBLIC FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include FILES
         ${HEADER_LIST}
  PRIVATE "src/wisdom/empty.cpp")

if(WISDOM_BUILD_TYPE STREQUAL "modules")
  target_sources(
    ${PROJECT_NAME}
    PUBLIC FILE_SET CXX_MODULES BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/modules
           FILES "modules/api.ixx")
endif()

# target_precompile_headers(${PROJECT_NAME} PRIVATE "include/wisdom/pch.h")
target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include>)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
target_compile_definitions(
  ${PROJECT_NAME}
  PUBLIC DEBUG_MODE=$<IF:$<CONFIG:Debug>,1,0>
         DEBUG_LAYER=$<BOOL:${WISDOM_DEBUG_LAYER}>
         WISDOMDX12=$<BOOL:${WISDOMDX12}>
         WISDOMVK=$<BOOL:${WISDOMVK}>
         WISDOMMTL=$<BOOL:${WISDOMMTL}>
         WISDOM_VERSION=${WISDOM_VERSION}
         WISDOM_LOG_LEVEL=${SEV_INDEX}
         NOMINMAX)

if(WISDOM_BUILD_TYPE STREQUAL "modules")
  target_compile_definitions(
    ${PROJECT_NAME} PUBLIC WIS_EXPORT=export WISDOM_HEADER_ONLY WIS_INLINE=;)
  target_sources(
    ${PROJECT_NAME}
    PUBLIC FILE_SET CXX_MODULES BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/modules
           FILES "modules/wisdom.ixx")
else()
  target_compile_definitions(${PROJECT_NAME} PUBLIC WIS_EXPORT=; WIS_INLINE=;)
endif()

if(WISDOM_BUILD_TYPE STREQUAL "headers")
  target_compile_definitions(
    ${PROJECT_NAME} PUBLIC WISDOM_HEADER_ONLY WIS_EXPORT=; WIS_INLINE=inline
  )# hack, because cmake does not allow empty definitions
endif()

if(WISDOM_FORCE_VULKAN)
  target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_FORCE_VULKAN)
endif()

if(APPLE)
  target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_MAC
                                                    VK_USE_PLATFORM_METAL_EXT)
elseif(UNIX AND NOT APPLE)
  target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_LINUX
                                                    VK_USE_PLATFORM_XCB_KHR)
endif()

if(WISDOM_USE_FMT)
  target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_USE_FMT)
  target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)
endif()

if(WISDOM_GENERATE_API)
  add_custom_target(
    GenerateAPI
    COMMAND $<TARGET_FILE:generator>
    DEPENDS generator)
  add_dependencies(${PROJECT_NAME} GenerateAPI)
endif()

target_install(${PROJECT_NAME})
