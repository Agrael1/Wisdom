# CMakeList.txt : CMake project for Wisdom
project(Wisdom)

include(cmake/documentation.cmake)

add_library(${PROJECT_NAME} ${WISDOM_BTYPE})

# load dx12 and vulkan
include(platform.cmake)
include(dx12.cmake)
include(vulkan.cmake)

# Add common sources
target_sources(${PROJECT_NAME}
  ${WISDOM_PUBLIC} 
    FILE_SET HEADERS BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include 
    FILES 
        "include/wisdom/global/definitions.h"
        "include/wisdom/global/internal.h"
        "include/wisdom/util/com_ptr.h"

        "include/wisdom/wisdom.h"
        "include/wisdom/wisdom.hpp"
        "include/wisdom/util/log_layer.h"
        "include/wisdom/util/flags.h"
        "include/wisdom/bridge/generator.h"
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
        "include/wisdom/global/assertions.h"
        "include/wisdom/util/error_messages.h"
        
)

# target_precompile_headers(${PROJECT_NAME} PRIVATE "include/wisdom/pch.h")
target_include_directories(
  ${PROJECT_NAME} ${WISDOM_PUBLIC}
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)


target_compile_definitions(
  ${PROJECT_NAME}
  ${WISDOM_PUBLIC} 
         DEBUG_MODE=$<IF:$<CONFIG:Debug>,1,0>
         DEBUG_LAYER=$<BOOL:${WISDOM_DEBUG_LAYER}>
         WISDOM_DX12=$<BOOL:${WISDOM_DX12}>
         WISDOM_VULKAN=$<BOOL:${WISDOM_VULKAN}>
         WISDOM_METAL=$<BOOL:${WISDOM_METAL}>
         WISDOM_VERSION=${WISDOM_VERSION}
         WISDOM_LOG_LEVEL=${SEV_INDEX}
         WIS_INLINE=$<IF:$<BOOL:${WISDOM_headers}>,inline,>
         NOMINMAX)

# Inline definition
if(WISDOM_headers)
  target_compile_definitions(${PROJECT_NAME} INTERFACE WISDOM_HEADER_ONLY)
endif()

# Formatting library
if(WISDOM_USE_FMT)
  target_compile_definitions(${PROJECT_NAME} ${WISDOM_PUBLIC}  WISDOM_USE_FMT)
  target_link_libraries(${PROJECT_NAME} ${WISDOM_PUBLIC} fmt::fmt)
endif()

# API generation
if(WISDOM_GENERATE_API)
  add_custom_target(
    GenerateAPI
    COMMAND $<TARGET_FILE:generator>
    DEPENDS generator)
  add_dependencies(${PROJECT_NAME} GenerateAPI)
endif()

target_install(${PROJECT_NAME})
