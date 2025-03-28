# CMakeList.txt : CMake project for Wisdom
project(wisdom VERSION ${WISDOM_VERSION})

include(cmake/documentation.cmake)

# load dx12 and vulkan
include(shared.cmake)
include(dx12.cmake)
include(vulkan.cmake)

add_library(wisdom-headers INTERFACE)
add_library(wis::headers ALIAS wisdom-headers)
add_library(wis::wisdom-headers ALIAS wisdom-headers)

add_library(wisdom INTERFACE)
add_library(wis::wisdom ALIAS wisdom)

if(WISDOM_CPP_MODULES_SUPPORTED)
  add_library(wisdom-module STATIC)
  add_library(wis::module ALIAS wisdom-module)
  target_link_libraries(wisdom-module 
    PUBLIC 
        wisdom-headers)
  target_sources(
    wisdom-module 
    PUBLIC FILE_SET CXX_MODULES 
    BASE_DIRS include
    FILES "include/wisdom/wisdom.ixx"
                         "include/wisdom/wisdom.fvk.ixx"
  
  )
endif()

if(WISDOM_VULKAN)
  target_link_libraries(wisdom-headers INTERFACE wis::vulkan-headers)
  target_link_libraries(wisdom INTERFACE wis::vulkan)
  if(WISDOM_CPP_MODULES_SUPPORTED)
    target_link_libraries(wisdom-module PUBLIC wisdom-vk-module)
  endif()
endif()

if(WISDOM_DX12)
  target_link_libraries(wisdom-headers INTERFACE wis::dx12-headers)
  target_link_libraries(wisdom INTERFACE wis::dx12)
  if(WISDOM_CPP_MODULES_SUPPORTED)
    target_link_libraries(wisdom-module PUBLIC wisdom-dx12-module)
  endif()
endif()

# API generation
if(WISDOM_GENERATE_API)
  add_custom_target(
    GenerateAPI
    COMMAND $<TARGET_FILE:generator>
    DEPENDS generator)
  add_dependencies(wisdom-headers GenerateAPI)
endif()



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

if(WISDOM_CPP_MODULES_SUPPORTED)
  install(
    TARGETS wisdom-module
    EXPORT wisdom-module-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET CXX_MODULES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(
    EXPORT wisdom-module-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    NAMESPACE wis::
    FILE wisdom-module-targets.cmake # Not sure if this is still needed
  )
endif()

include(CMakePackageConfigHelpers)
configure_file("../cmake/wisdom-functions.cmake"
               "${CMAKE_CURRENT_BINARY_DIR}/wisdom-functions.cmake" @ONLY)

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
  FILES "${CMAKE_CURRENT_BINARY_DIR}/wisdom-functions.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/wisdom-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/wisdom-config-version.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
