project(generator)

include(format.cmake)

find_package(tinyxml2 QUIET)
if(NOT tinyxml2_FOUND AND NOT TARGET tinyxml2::tinyxml2)
  set(tinyxml2_BUILD_TESTING
      OFF
      CACHE INTERNAL "" FORCE)

  # Load tinyxml2
  message("Loading tinyxml2...")
  cpmaddpackage(NAME tinyxml2 GITHUB_REPOSITORY leethomason/tinyxml2 GIT_TAG
                origin/master)
endif()

add_executable(${PROJECT_NAME})
target_sources(
  ${PROJECT_NAME}
  PRIVATE "entry_main.cpp"
          "generator.cpp"
          "generator.hpp"
          "enum.cpp"
          "struct.cpp"
          "types.h"
          "handle.cpp"
          "variant.cpp"
          "function.cpp"
          "validation.cpp" "bitmask.cpp")
target_compile_definitions(
  ${PROJECT_NAME}
  PRIVATE CLANG_FORMAT_EXECUTABLE="${CLANG_FORMAT_EXECUTABLE}"
          WISDOM_VERSION="${WISDOM_VERSION}"
          "INPUT_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/../xml/wis.xml\""
          "OUTPUT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/../bindings\""
          "CPP_OUTPUT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/../src/include/wisdom\""
          "DOC_OUTPUT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/../docs\"")
target_include_directories(
  ${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../wisdom/include")
target_link_libraries(${PROJECT_NAME} PRIVATE tinyxml2::tinyxml2)

if(WISDOM_USE_FMT)
  find_package(fmt CONFIG QUIET)
  target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_USE_FMT)
  target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
target_precompile_headers(${PROJECT_NAME} PRIVATE "pch.hpp")
