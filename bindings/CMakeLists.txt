project(wisdom-bindings VERSION ${WISDOM_VERSION})

if(WISDOM_SHARED_LIB)
  set(WISDOM_LIB_TYPE SHARED)
else()
  set(WISDOM_LIB_TYPE STATIC)
endif()

add_library(${PROJECT_NAME} ${WISDOM_LIB_TYPE} wisdom.h)
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/wisdom.cpp
                                       "custom.cpp" "wisdom_ext.inl")

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                         $<INSTALL_INTERFACE:include>)
target_link_libraries(${PROJECT_NAME} PRIVATE wis::headers
                                              wis::extension-headers)

include(GenerateExportHeader)
generate_export_header(
  ${PROJECT_NAME}
  BASE_NAME
  WISDOM
  EXPORT_MACRO_NAME
  WISDOM_API
  EXPORT_FILE_NAME
  ${CMAKE_CURRENT_SOURCE_DIR}/wisdom_exports.h
  STATIC_DEFINE
  WISDOM_STATIC)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

if(WISDOM_WINDOWS)
  get_target_property(DX12SDKVER DX12Agility DX12SDKVER)
endif()

include(GNUInstallDirs)
install(
  TARGETS ${PROJECT_NAME}
  EXPORT wisdom-bindings-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/wisdom.h
              ${CMAKE_CURRENT_SOURCE_DIR}/wisdom_exports.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

# Install
install(
  EXPORT wisdom-bindings-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  NAMESPACE wis::
  FILE wisdom-bindings-targets.cmake)

include(CMakePackageConfigHelpers)
configure_file("../cmake/wisdom-deps.cmake.in"
               "${CMAKE_CURRENT_BINARY_DIR}/wisdom-deps.cmake" @ONLY)

configure_package_config_file(
  "../cmake/wisdom-bindings-config.cmake.in" "wisdom-bindings-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  PATH_VARS CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_BINDIR # For agility dlls
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/wisdom-bindings-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

# Install Config and ConfigVersion files
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/wisdom-deps.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/wisdom-bindings-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/wisdom-bindings-config-version.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
