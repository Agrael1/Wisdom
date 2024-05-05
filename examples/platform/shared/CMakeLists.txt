project(PlatformExamplesShared)

find_package(glm CONFIG REQUIRED)

add_library(${PROJECT_NAME} STATIC)
target_sources(
  ${PROJECT_NAME}
  PUBLIC "app.h"
  PRIVATE "app.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME} PUBLIC Wisdom::Wisdom glm::glm)
add_dependencies(${PROJECT_NAME} compile_shaders_kd)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
target_compile_definitions(${PROJECT_NAME} PUBLIC SHADER_DIR="${SHADER_DIR}")
