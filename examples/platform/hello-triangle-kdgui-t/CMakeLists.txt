project(hello-triangle-kdgui-t)

find_package(KDUtils CONFIG)
find_package(KDFoundation CONFIG)
find_package(KDGui CONFIG)

add_executable(${PROJECT_NAME} entry_main.cpp "window.h" "window.cpp")

wis_install_deps(${PROJECT_NAME})

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC PlatformExamplesShared
  PRIVATE KDUtils::KDGui KDUtils::KDUtils)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
target_compile_definitions(
  ${PROJECT_NAME} PUBLIC SHADER_DIR="${SHADER_DIR}" GLM_FORCE_SIMD_AVX2
                         GLM_FORCE_ALIGNED_GENTYPES)

if(UNIX)
  target_link_options(${PROJECT_NAME} PRIVATE "-lm")
endif()
