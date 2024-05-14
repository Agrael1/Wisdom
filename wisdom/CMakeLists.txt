# CMakeList.txt : CMake project for Wisdom
project(wisdom VERSION 0.1.0)

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

add_library(wis-header-only INTERFACE)
add_library(wis::wis-header-only ALIAS wis-header-only)

add_library(${PROJECT_NAME} INTERFACE)
add_library(wis::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

include(platform.cmake)
target_link_libraries(wis-header-only INTERFACE wisdom-platform-headers)
target_link_libraries(${PROJECT_NAME} INTERFACE wisdom-platform)



# Formatting library
if(WISDOM_USE_FMT)
  target_compile_definitions(wis-header-only INTERFACE WISDOM_USE_FMT)
  target_link_libraries(wis-header-only INTERFACE fmt::fmt-header-only)
endif()

# API generation
if(WISDOM_GENERATE_API)
  add_custom_target(
    GenerateAPI
    COMMAND $<TARGET_FILE:generator>
    DEPENDS generator)
  add_dependencies(wis-header-only GenerateAPI)
endif()

install(TARGETS ${PROJECT_NAME} wis-header-only
			EXPORT wisdom-targets
			FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
			RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
			LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
			ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
			INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
		)

# Install
install(
  EXPORT wisdom-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  NAMESPACE wis::
  FILE wisdom-targets.cmake # Not sure if this is still needed
  )

include(CMakePackageConfigHelpers)
configure_file("../cmake/wisdom-deps.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/wisdom-deps.cmake" @ONLY)

configure_package_config_file( 
  "../cmake/wisdom-config.cmake.in" 
  "wisdom-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
    CMAKE_INSTALL_BINDIR # For agility dlls
  )

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/wisdom-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
  )

### Install Config and ConfigVersion files
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/wisdom-deps.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/wisdom-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/wisdom-config-version.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )
