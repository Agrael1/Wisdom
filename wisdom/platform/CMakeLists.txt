set(AUX_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../cmake)
set(PLATFORM_CONFIG_DIR "${AUX_CMAKE_DIR}/platform")

add_library(wisdom-platform-headers INTERFACE "wisdom/wisdom_platform.hpp")
add_library(wis::platform-headers ALIAS wisdom-platform-headers)
target_include_directories(
  wisdom-platform-headers
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/platform>)
add_library(wisdom-platform INTERFACE "wisdom/wisdom_platform.hpp"
            "wisdom/linux/interop_device.hpp")
add_library(wis::platform ALIAS wisdom-platform)
target_include_directories(
  wisdom-platform INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/platform>)

if(WISDOM_CPP_MODULES_SUPPORTED)
  add_library(wisdom-platform-module STATIC "wisdom/platform.include.h")
  add_library(wis::platform-module ALIAS wisdom-platform-module)
  target_sources(
    wisdom-platform-module
    PUBLIC FILE_SET CXX_MODULES FILES "wisdom/wisdom_platform.ixx"
           "wisdom/wisdom_platform.fvk.ixx")
  target_link_libraries(wisdom-platform-module PUBLIC wisdom-module
                                                      wisdom-platform-headers)
  set_target_properties(wisdom-platform-module
                        PROPERTIES CXX_STANDARD 20 CXX_SCAN_FOR_MODULES ON)
endif()

if(WISDOM_WINDOWS)
  add_subdirectory("windows")
elseif(WISDOM_LINUX)
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${AUX_CMAKE_DIR}/ECM")

  add_subdirectory("xcb")
  add_subdirectory("x11")
  add_subdirectory("wayland")

  install(DIRECTORY "${AUX_CMAKE_DIR}/ECM"
          DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/wisdom")
endif()

install(
  TARGETS wisdom-platform wisdom-platform-headers
  EXPORT wisdom-platform-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  EXPORT wisdom-platform-targets
  FILE wisdom-platform-targets.cmake
  NAMESPACE wis::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom)

if(WISDOM_CPP_MODULES_SUPPORTED)
  install(
    TARGETS wisdom-platform-module
    EXPORT wisdom-platform-module-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET CXX_MODULES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/platform)
  install(
    EXPORT wisdom-platform-module-targets
    FILE wisdom-platform-module-targets.cmake
    NAMESPACE wis::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom)
endif()

configure_package_config_file(
  "${PLATFORM_CONFIG_DIR}/wisdom-platform-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/wisdom-platform-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/wisdom-platform-config.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/wisdom")

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/platform/wisdom)
