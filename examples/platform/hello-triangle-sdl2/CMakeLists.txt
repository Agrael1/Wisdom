project(hello-triangle-sdl2)

add_executable(${PROJECT_NAME} entry_main.cpp)

wis_install_deps(${PROJECT_NAME})

include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/sdl.cmake)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC PlatformExamplesShared
  PRIVATE SDL2::SDL2-static)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

target_compile_definitions(
  ${PROJECT_NAME} PUBLIC SHADER_DIR="${SHADER_DIR}" GLM_FORCE_SIMD_AVX2
                         GLM_FORCE_ALIGNED_GENTYPES)

if(UNIX)
  target_link_options(${PROJECT_NAME} PRIVATE "-lm")
endif()
