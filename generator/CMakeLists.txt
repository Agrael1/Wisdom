project(generator)

include(format.cmake)
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

# Load tinyxml2
message("Loading tinyxml2...")
FetchContent_Declare(
  tinyxml2
  GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
  GIT_TAG origin/master)

set(tinyxml2_BUILD_TESTING
    OFF
    CACHE BOOL "" FORCE)
FetchContent_GetProperties(tinyxml2)
if(NOT tinyxml2_POPULATED)
  FetchContent_Populate(tinyxml2)
  # Disable tinyxml2 installation
  add_subdirectory(${tinyxml2_SOURCE_DIR} ${tinyxml2_BINARY_DIR}
                   EXCLUDE_FROM_ALL)
endif()

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE entry_main.cpp generator.h generator.cpp)
target_compile_definitions(
  ${PROJECT_NAME}
  PRIVATE
    CLANG_FORMAT_EXECUTABLE="${CLANG_FORMAT_EXECUTABLE}"
    WISDOM_VERSION="${WISDOM_VERSION}"
    "INPUT_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/../xml/wis.xml\""
    "OUTPUT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/../bindings\""
    "CPP_OUTPUT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/../wisdom/include/wisdom\"")
target_include_directories(
  ${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../wisdom/include")
target_link_libraries(${PROJECT_NAME} PRIVATE tinyxml2::tinyxml2)

if(WISDOM_USE_FMT)
  find_package(fmt CONFIG QUIET)
  target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_USE_FMT)
  target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
