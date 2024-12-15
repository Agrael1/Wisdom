add_library(wisdom-windows-headers INTERFACE)
add_library(wis::windows-headers ALIAS wisdom-windows-headers)
target_link_libraries(wisdom-windows-headers INTERFACE wis::header-only d3d11)
target_compile_definitions(wisdom-windows-headers
                           INTERFACE WIS_WINDOWS_PLATFORM=1)
target_include_directories(
  wisdom-windows-headers
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

if(WISDOM_BUILD_BINARIES)
  add_library(wisdom-windows STATIC "wisdom/wisdom_windows.hpp"
                                    "wisdom/wisdom_windows.cpp")
  add_library(wis::windows ALIAS wisdom-windows)
  target_link_libraries(wisdom-windows PUBLIC wis::wisdom d3d11)
  target_compile_definitions(wisdom-windows PUBLIC WIS_WINDOWS_PLATFORM=1)
  target_include_directories(
    wisdom-windows PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                          $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
else()
  add_library(wisdom-windows ALIAS wisdom-windows-headers)
  add_library(wis::windows ALIAS wisdom-windows-headers)
endif()

target_link_libraries(wisdom-platform INTERFACE wis::windows)
target_link_libraries(wisdom-platform-headers INTERFACE wis::windows-headers)

install(
  TARGETS wisdom-windows wisdom-windows-headers
  EXPORT wisdom-windows-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

install(
  EXPORT wisdom-windows-targets
  FILE wisdom-windows-targets.cmake
  NAMESPACE wis::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom
  )

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${PLATFORM_CONFIG_DIR}/wisdom-windows-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/wisdom-windows-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom)

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/wisdom-windows-config.cmake"
  
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/wisdom")

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wisdom
  )
