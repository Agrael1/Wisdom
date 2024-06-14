set(AUX_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../cmake)
set(PLATFORM_CONFIG_DIR "${AUX_CMAKE_DIR}/platform")

add_library(wisdom-platform-headers INTERFACE "wisdom/wisdom_platform.h")
add_library(wis::platform-headers ALIAS wisdom-platform-headers)
target_include_directories(
  wisdom-platform-headers
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
add_library(wisdom-platform INTERFACE "wisdom/wisdom_platform.h")
add_library(wis::platform ALIAS wisdom-platform)
target_include_directories(
  wisdom-platform INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

if(WISDOM_WINDOWS)
  add_subdirectory("windows")
elseif(WISDOM_LINUX)
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                        "${AUX_CMAKE_DIR}/ECM")

  add_subdirectory("xcb")
  add_subdirectory("wayland")

  install(
    DIRECTORY "${AUX_CMAKE_DIR}/ECM"
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

configure_package_config_file(
  "${PLATFORM_CONFIG_DIR}/wisdom-platform-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/wisdom-platform-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom)

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/wisdom-platform-config.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/wisdom")

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wisdom)
