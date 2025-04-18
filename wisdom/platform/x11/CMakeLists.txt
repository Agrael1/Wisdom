find_package(X11)
if(NOT X11_FOUND)
  return()
endif()

add_library(wisdom-x11-headers INTERFACE)
add_library(wis::x11-headers ALIAS wisdom-x11-headers)
target_link_libraries(wisdom-x11-headers INTERFACE wis::wisdom-headers)
target_compile_definitions(wisdom-x11-headers INTERFACE WIS_X11_PLATFORM=1)
target_include_directories(wisdom-x11-headers INTERFACE ${X11_INCLUDE_DIR})
target_include_directories(
  wisdom-x11-headers
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/platform/x11>)

if(WISDOM_BUILD_BINARIES)
  add_library(wisdom-x11 STATIC "wisdom/wisdom_x11.hpp" "wisdom/wisdom_x11.cpp")
  add_library(wis::x11 ALIAS wisdom-x11)
  target_link_libraries(wisdom-x11 PUBLIC wis::wisdom)
  target_compile_definitions(wisdom-x11 PUBLIC WIS_X11_PLATFORM=1)
  target_include_directories(
    wisdom-x11
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/platform/x11>)
else()
  add_library(wisdom-x11 ALIAS wisdom-x11-headers)
  add_library(wis::x11 ALIAS wisdom-x11-headers)
endif()

target_link_libraries(wisdom-platform INTERFACE wis::x11)
target_link_libraries(wisdom-platform-headers INTERFACE wis::x11-headers)

if(WISDOM_CPP_MODULES_SUPPORTED)
  add_library(wisdom-x11-module STATIC "wisdom/x11.include.h")
  add_library(wis::x11-module ALIAS wisdom-x11-module)
  target_sources(wisdom-x11-module PUBLIC FILE_SET CXX_MODULES FILES
                                          "wisdom/wisdom_x11.ixx")
  target_link_libraries(wisdom-x11-module PUBLIC wisdom-vk-module
                                                 wisdom-x11-headers)
  set_target_properties(wisdom-x11-module PROPERTIES CXX_STANDARD 20
                                                     CXX_SCAN_FOR_MODULES ON)

  target_link_libraries(wisdom-platform-module PUBLIC wis::x11-module)
endif()

install(
  TARGETS wisdom-x11 wisdom-x11-headers
  EXPORT wisdom-x11-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  EXPORT wisdom-x11-targets
  FILE wisdom-x11-targets.cmake
  NAMESPACE wis::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom)

if(WISDOM_CPP_MODULES_SUPPORTED)
  install(
    TARGETS wisdom-x11-module
    EXPORT wisdom-x11-module-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET CXX_MODULES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/platform/x11)

  install(
    EXPORT wisdom-x11-module-targets
    FILE wisdom-x11-module-targets.cmake
    NAMESPACE wis::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom)
endif()

configure_package_config_file(
  "${PLATFORM_CONFIG_DIR}/wisdom-x11-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/wisdom-x11-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/wisdom-x11-config.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/wisdom")

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/platform/x11/wisdom)
