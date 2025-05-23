project(wisdom-debug)

add_library(wisdom-debug-headers INTERFACE)
add_library(wis::debug-headers ALIAS wisdom-debug-headers)

target_link_libraries(wisdom-debug-headers INTERFACE wisdom-headers)

target_include_directories(
  wisdom-debug-headers
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/extensions/debug_info>)

if(WISDOM_BUILD_BINARIES)
  add_library(wisdom-debug STATIC "wisdom/wisdom_debug.hpp"
                                  "wisdom/wisdom_debug.cpp")
  add_library(wis::debug ALIAS wisdom-debug)
  target_link_libraries(wisdom-debug PUBLIC wis::wisdom)
  target_include_directories(
    wisdom-debug
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/extensions/debug_info>)
else()
  add_library(wisdom-debug INTERFACE)
  add_library(wis::debug ALIAS wisdom-debug)
  target_link_libraries(wisdom-debug INTERFACE wisdom-debug-headers)
endif()

if(WISDOM_CPP_MODULES_SUPPORTED)
  add_library(wisdom-debug-module STATIC "wisdom/debug.include.h")
  add_library(wis::debug-module ALIAS wisdom-debug-module)
  target_sources(
    wisdom-debug-module
    PUBLIC FILE_SET CXX_MODULES FILES "wisdom/wisdom_debug.ixx"
           "wisdom/wisdom_debug.fvk.ixx")
  target_link_libraries(wisdom-debug-module PUBLIC wisdom-module
                                                   wisdom-debug-headers)
  add_dependent_target(wisdom-debug-module PUBLIC WISDOM_VULKAN
                       wisdom-vk-module)
  add_dependent_target(wisdom-debug-module PUBLIC WISDOM_DX12
                       wisdom-dx12-module)
  set_target_properties(wisdom-debug-module PROPERTIES CXX_STANDARD 20
                                                       CXX_SCAN_FOR_MODULES ON)
endif()

# link the debug library to the main wisdom extension library
target_link_libraries(wisdom-extension INTERFACE wisdom-debug)
target_link_libraries(wisdom-extension-headers INTERFACE wisdom-debug-headers)

install(
  TARGETS wisdom-debug wisdom-debug-headers
  EXPORT wisdom-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/extensions/debug_info/wisdom)

if(WISDOM_CPP_MODULES_SUPPORTED)
  install(
    TARGETS wisdom-debug-module
    EXPORT wisdom-module-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET CXX_MODULES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/extensions/debug_info)
endif()
