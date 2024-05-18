file(GLOB UTIL_HEADERS "include/wisdom/util/*.h")
file(GLOB GLOBAL_HEADERS "include/wisdom/global/*.h")
file(GLOB BRIDGE_HEADERS "include/wisdom/bridge/*.h")
file(GLOB API_HEADERS "include/wisdom/generated/api/*.h")

source_group("util" FILES ${UTIL_HEADERS})
source_group("global" FILES ${GLOBAL_HEADERS})
source_group("bridge" FILES ${BRIDGE_HEADERS})
source_group("api" FILES ${API_HEADERS})

add_library(wisdom-shared INTERFACE ${UTIL_HEADERS} ${GLOBAL_HEADERS} ${BRIDGE_HEADERS} ${API_HEADERS})

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

target_compile_definitions(
  wisdom-shared
  INTERFACE
  DEBUG_MODE=$<IF:$<CONFIG:Debug>,1,0>
  DEBUG_LAYER=$<BOOL:${WISDOM_DEBUG_LAYER}>
  WISDOM_VERSION=${WISDOM_VERSION}
  WISDOM_LOG_LEVEL=${SEV_INDEX}
  NOMINMAX
  _CRT_SECURE_NO_WARNINGS)

install (TARGETS wisdom-shared EXPORT wisdom-targets)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
