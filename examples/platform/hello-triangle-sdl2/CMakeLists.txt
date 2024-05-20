project(hello-triangle-sdl2)

find_package(SDL2 QUIET)

add_executable(${PROJECT_NAME} entry_main.cpp platform.cpp)

wis_install_deps(${PROJECT_NAME})

if(UNIX)
    include(FindX11)
    if(NOT X11_X11_xcb_FOUND)
        target_compile_definitions(${PROJECT_NAME} PRIVATE NO_X11)
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE X11::X11_xcb)
    endif()
    target_link_options(${PROJECT_NAME} PRIVATE "-lm")
endif()

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC PlatformExamplesShared
  PRIVATE SDL2::SDL2)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

target_compile_definitions(
  ${PROJECT_NAME} PUBLIC SHADER_DIR="${SHADER_DIR}" GLM_FORCE_SIMD_AVX2
                         GLM_FORCE_ALIGNED_GENTYPES)
