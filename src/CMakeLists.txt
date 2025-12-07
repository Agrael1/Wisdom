function(wis_make_target_bundle)
  cmake_parse_arguments(BUNDLE "HEADER_ONLY" "TARGET" "LINK_TARGETS;SOURCES;HEADERS;INCLUDE_DIRECTORIES" ${ARGN})
  
  if(NOT BUNDLE_TARGET)
    message(FATAL_ERROR "TARGET is required for wis_make_target_bundle")
  endif()

  source_group("Headers" FILES ${BUNDLE_HEADERS})
  source_group("Sources" FILES ${BUNDLE_SOURCES})

  # Create header-only interface target
  add_library (wisdom-${BUNDLE_TARGET}-headers INTERFACE ${BUNDLE_HEADERS} ${BUNDLE_SOURCES})
  add_library (wis::wisdom-${BUNDLE_TARGET}-headers ALIAS wisdom-${BUNDLE_TARGET}-headers)

  # Link dependencies
  if(BUNDLE_LINK_TARGETS)
    target_link_libraries(wisdom-${BUNDLE_TARGET}-headers INTERFACE ${BUNDLE_LINK_TARGETS})
  endif()
  
  # Include directories
  if(BUNDLE_INCLUDE_DIRECTORIES)
    target_include_directories(wisdom-${BUNDLE_TARGET}-headers
    INTERFACE
      $<BUILD_INTERFACE:${BUNDLE_INCLUDE_DIRECTORIES}>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
  endif()

  set_target_properties(wisdom-${BUNDLE_TARGET}-headers PROPERTIES CXX_STANDARD 20)
  
  # Create static target with sources if not header-only
  if(NOT BUNDLE_HEADER_ONLY AND WISDOM_BUILD_BINARIES AND BUNDLE_SOURCES)
    add_library(wisdom-${BUNDLE_TARGET} STATIC ${BUNDLE_HEADERS})
    add_library(wis::wisdom-${BUNDLE_TARGET} ALIAS wisdom-${BUNDLE_TARGET})
    target_sources(wisdom-${BUNDLE_TARGET}
      PRIVATE ${BUNDLE_SOURCES}
    )

    target_link_libraries(wisdom-${BUNDLE_TARGET} PUBLIC wisdom-${BUNDLE_TARGET}-headers)
    target_compile_definitions(wisdom-${BUNDLE_TARGET} PRIVATE WISDOM_BUILD_BINARIES=1)
    set_target_properties(wisdom-${BUNDLE_TARGET} PROPERTIES CXX_STANDARD 20)
  else()
  	add_library(wisdom-${BUNDLE_TARGET} ALIAS wisdom-${BUNDLE_TARGET}-headers)
  	add_library(wis::wisdom-${BUNDLE_TARGET} ALIAS wisdom-${BUNDLE_TARGET}-headers)
  endif()
endfunction()

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
)

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
    wisdom-shared
    DXGI
    DXGUID
)