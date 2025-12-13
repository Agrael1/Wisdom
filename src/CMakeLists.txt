

wis_make_target_bundle(
  TARGET shared
  HEADERS
    "include/wisdom/global/definitions.h"
    "include/wisdom/global/internal.hpp"

    "include/wisdom/generated/c_api.h"
    "include/wisdom/generated/cpp_api.hpp"

    "include/wisdom/bridge/span.hpp"
    "include/wisdom/bridge/source_location.hpp"
    "include/wisdom/bridge/format.hpp"

    "include/wisdom/util/com_ptr.hpp"
    "include/wisdom/util/string_literal.hpp"
    "include/wisdom/util/error_messages.hpp"
    "include/wisdom/util/allocation.hpp"
  INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
  HEADER_ONLY
)

if(WISDOM_WINDOWS)
  target_compile_definitions(wisdom-shared-headers INTERFACE WISDOM_WINDOWS=1)
endif()
if(WISDOM_LINUX)
  target_compile_definitions(wisdom-shared-headers INTERFACE WISDOM_LINUX=1)
endif()

wis_make_target_bundle(
  TARGET dx12
  HEADERS
    "include/wisdom/impl/dx12/dx12_types.hpp"
    "include/wisdom/impl/dx12/dx12_utils.hpp"
    "include/wisdom/generated/dx12_cpp_api.hpp"
    "include/wisdom/generated/dx12_api.h"
  SOURCES
    "include/wisdom/impl/dx12/dx12_instance.cpp"
  LINK_TARGETS
    wisdom-shared-headers
    DXGI
    DXGUID
  COMPILE_DEFINITIONS
    WISDOM_DX12=1
)

wis_make_target_bundle(
  TARGET vk
  HEADERS
    "include/wisdom/impl/vulkan/vk_types.hpp"
    "include/wisdom/impl/vulkan/vk_utils.hpp"
    "include/wisdom/impl/vulkan/vk_loader.hpp"
    "include/wisdom/generated/vk_cpp_api.hpp"
    "include/wisdom/generated/vk_api.h"
  SOURCES
    "include/wisdom/impl/vulkan/vk_instance.cpp"
  LINK_TARGETS
    wisdom-shared-headers
    vkma
  COMPILE_DEFINITIONS
    WISDOM_VULKAN=1
)

# Unified targets
# Create a convenience target for all headers
add_library(wisdom-headers INTERFACE)
add_library(wis::wisdom-headers ALIAS wisdom-headers)
target_link_libraries(wisdom-headers INTERFACE wisdom-shared-headers)
if(WISDOM_DX12)
  target_link_libraries(wisdom-headers INTERFACE wisdom-dx12-headers)
endif()
if(WISDOM_VULKAN)
  target_link_libraries(wisdom-headers INTERFACE wisdom-vk-headers)
endif()

# Create a convenience target for all static libraries
if(WISDOM_BUILD_STATIC)
  add_library(wisdom STATIC)
  add_library(wis::wisdom ALIAS wisdom)
  if(WISDOM_DX12)
    target_link_libraries(wisdom PUBLIC wisdom-dx12-headers)
    target_sources(wisdom PRIVATE $<TARGET_OBJECTS:wisdom-dx12-objects>)
  endif()
  if(WISDOM_VULKAN)
    target_link_libraries(wisdom PUBLIC wisdom-vk-headers)
    target_sources(wisdom PRIVATE $<TARGET_OBJECTS:wisdom-vk-objects>)
  endif()
  target_compile_definitions(wisdom PUBLIC WISDOM_BUILD_BINARIES=1)
  set_target_properties(wisdom PROPERTIES 
    CXX_STANDARD 20
    POSITION_INDEPENDENT_CODE ON
  )
endif()

# Create a convenience target for all dynamic libraries
if(WISDOM_BUILD_SHARED)
  include(GenerateExportHeader)
  
  add_library(wisdom-shared SHARED)
  add_library(wis::wisdom-shared ALIAS wisdom-shared)
  if(WISDOM_DX12)
    target_link_libraries(wisdom-shared PUBLIC wisdom-dx12-headers)
    target_sources(wisdom-shared PRIVATE $<TARGET_OBJECTS:wisdom-dx12-objects-shared>)
  endif()
  if(WISDOM_VULKAN)
    target_link_libraries(wisdom-shared PUBLIC wisdom-vk-headers)
    target_sources(wisdom-shared PRIVATE $<TARGET_OBJECTS:wisdom-vk-objects-shared>)
  endif()
  target_compile_definitions(wisdom-shared PUBLIC WISDOM_BUILD_BINARIES=1 WISDOM_SHARED_LIBRARY=1)
  set_target_properties(wisdom-shared PROPERTIES 
    CXX_STANDARD 20
    POSITION_INDEPENDENT_CODE ON
  )

  generate_export_header(
    wisdom-shared
    BASE_NAME WISDOM
    EXPORT_MACRO_NAME WISDOM_API
    EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/wisdom/generated/wisdom_exports.h
    STATIC_DEFINE WISDOM_STATIC
    INCLUDE_GUARD_NAME WISDOM_EXPORTS_H
  )
endif()