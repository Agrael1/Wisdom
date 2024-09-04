# CMakeList.txt : CMake project for Wisdom
project(wisdom VERSION ${WISDOM_VERSION})

include(cmake/documentation.cmake)

if(WISDOM_BUILD_BINARIES)
  set(WISDOM_PUBLIC PUBLIC)
else()
  set(WISDOM_PUBLIC INTERFACE)
endif()

# load dx12 and vulkan
include(shared.cmake)
include(dx12.cmake)
include(vulkan.cmake)

add_library(wisdom-headers INTERFACE)
add_library(wis::header-only ALIAS wisdom-headers)

add_library(wisdom INTERFACE)
add_library(wis::wisdom ALIAS wisdom)

if(WISDOM_VULKAN)
  target_link_libraries(wisdom-headers INTERFACE wis::vulkan-headers)
  target_link_libraries(wisdom INTERFACE wis::vulkan)
endif()

if(WISDOM_DX12)
  target_link_libraries(wisdom-headers INTERFACE wis::dx12-headers)
  target_link_libraries(wisdom INTERFACE wis::dx12)
endif()

# API generation
if(WISDOM_GENERATE_API)
  add_custom_target(
    GenerateAPI
    COMMAND $<TARGET_FILE:generator>
    DEPENDS generator)
  add_dependencies(wisdom-headers GenerateAPI)
endif()

# Extensions
add_subdirectory(platform)
add_subdirectory(extensions)

install(
  TARGETS wisdom wisdom-headers
  EXPORT wisdom-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install
install(
  EXPORT wisdom-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  NAMESPACE wis::
  FILE wisdom-targets.cmake # Not sure if this is still needed
)

include(CMakePackageConfigHelpers)
configure_file("../cmake/wisdom-deps.cmake.in"
               "${CMAKE_CURRENT_BINARY_DIR}/wisdom-deps.cmake" @ONLY)

configure_package_config_file(
  "../cmake/wisdom-config.cmake.in" "wisdom-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  PATH_VARS CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_BINDIR # For agility dlls
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/wisdom-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

# Install Config and ConfigVersion files
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/wisdom-deps.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/wisdom-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/wisdom-config-version.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
