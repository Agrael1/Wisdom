file(GLOB UTIL_HEADERS "include/wisdom/util/*.h")
file(GLOB GLOBAL_HEADERS "include/wisdom/global/*.h")
file(GLOB BRIDGE_HEADERS "include/wisdom/bridge/*.h")
file(GLOB API_HEADERS "include/wisdom/generated/api/*.hpp")

source_group("util" FILES ${UTIL_HEADERS})
source_group("global" FILES ${GLOBAL_HEADERS})
source_group("bridge" FILES ${BRIDGE_HEADERS})
source_group("api" FILES ${API_HEADERS})

add_library(wisdom-shared INTERFACE ${UTIL_HEADERS} ${GLOBAL_HEADERS} ${BRIDGE_HEADERS} ${API_HEADERS})

if (WISDOM_CPP_MODULES_SUPPORTED)
  add_library(wisdom-shared-module STATIC)
  target_sources(wisdom-shared-module
    PUBLIC FILE_SET CXX_MODULES FILES
      "include/wisdom/generated/api/wisdom.api.ixx"

  )
  target_link_libraries(wisdom-shared-module PRIVATE wisdom-shared)
  set_target_properties(wisdom-shared-module PROPERTIES CXX_STANDARD 20 CXX_SCAN_FOR_MODULES ON)
  target_compile_definitions(wisdom-shared-module PUBLIC WISDOM_CPP_MODULE)
  target_compile_options(wisdom-shared-module PUBLIC
    $<$<CXX_COMPILER_ID:Clang>:-Wno-include-angled-in-module-purview -Wno-nullability-completeness>
  )
endif()


target_include_directories(wisdom-shared
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

# Formatting library
if(WISDOM_USE_FMT)
  find_package(fmt CONFIG QUIET)
  target_compile_definitions(wisdom-shared INTERFACE WISDOM_USE_FMT)
  target_link_libraries(wisdom-shared INTERFACE fmt::fmt-header-only)
endif()

set_target_properties(wisdom-shared PROPERTIES CXX_STANDARD 20)
target_compile_features(wisdom-shared INTERFACE cxx_std_20)
if (MSVC)
    # Add the /Zc:__cplusplus flag
    target_compile_options(wisdom-shared INTERFACE "/Zc:__cplusplus")
endif()

target_compile_definitions(
  wisdom-shared
  INTERFACE
  WISDOM_VERSION=${WISDOM_VERSION}
  WISDOM_LOG_LEVEL=${SEV_INDEX}
  NOMINMAX
  _CRT_SECURE_NO_WARNINGS)

install(TARGETS wisdom-shared
  EXPORT wisdom-shared-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  EXPORT wisdom-shared-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  NAMESPACE wis::
  FILE wisdom-shared-targets.cmake # Not sure if this is still needed
)

# install wisdom-shared-module if modules are supported
if (WISDOM_CPP_MODULES_SUPPORTED)
  install(TARGETS wisdom-shared-module
    EXPORT wisdom-shared-module-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET CXX_MODULES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )
  install(
    EXPORT wisdom-shared-module-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    NAMESPACE wis::
    FILE wisdom-shared-module-targets.cmake # Not sure if this is still needed
  )
endif()
