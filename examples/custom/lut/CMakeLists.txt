project(LutBenchmark)

find_package(glm QUIET)

add_executable(${PROJECT_NAME} entry_main.cpp app.h app.cpp lut_loader.h
                               lut_loader.cpp)

wis_install_deps(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE wis::wis-header-only
                                              wisdom-platform-headers glm::glm)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
target_include_directories(${PROJECT_NAME} PRIVATE ${fpng_SOURCE_DIR}/src)

wis_compile_shader(TARGET ${PROJECT_NAME} SHADER
                   ${CMAKE_CURRENT_SOURCE_DIR}/lut.vs.hlsl)
wis_compile_shader(TARGET ${PROJECT_NAME} SHADER
                   ${CMAKE_CURRENT_SOURCE_DIR}/lut.ps.hlsl)
wis_compile_shader(
  TARGET
  ${PROJECT_NAME}
  SHADER
  ${CMAKE_CURRENT_SOURCE_DIR}/lut.ps.hlsl
  DEFINITIONS
  "TETRA"
  OUTPUT
  ${CMAKE_CURRENT_BINARY_DIR}/lut_tetra.ps)

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/lut.cube
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/lut.cube)
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/image.png
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/image.png)
