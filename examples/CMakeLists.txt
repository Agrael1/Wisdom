cmake_minimum_required(VERSION 3.22)
project(wisdom-examples LANGUAGES C CXX)

# Load dependencies before detecting the available Wisdom targets.
include(cmake/deps.cmake)

set(WISDOM_EXAMPLES_STATIC_AVAILABLE OFF)
set(WISDOM_EXAMPLES_SHARED_AVAILABLE OFF)
set(WISDOM_EXAMPLES_HEADERS_AVAILABLE OFF)
if(TARGET wis::wisdom AND TARGET wis::wisdom-platform)
  set(WISDOM_EXAMPLES_STATIC_AVAILABLE ON)
endif()
if(TARGET wis::wisdom-shared AND TARGET wis::wisdom-platform-shared)
  set(WISDOM_EXAMPLES_SHARED_AVAILABLE ON)
endif()
if(TARGET wis::wisdom-headers AND TARGET wis::wisdom-platform-headers)
  set(WISDOM_EXAMPLES_HEADERS_AVAILABLE ON)
endif()

# Options
option(WISDOM_EXAMPLES_BUILD_STATIC "Build statically linked examples."
       ${WISDOM_EXAMPLES_STATIC_AVAILABLE})
option(WISDOM_EXAMPLES_BUILD_SHARED "Build dynamically linked examples."
       ${WISDOM_EXAMPLES_SHARED_AVAILABLE})
option(WISDOM_EXAMPLES_BUILD_HEADERS "Build header-only C++ examples."
       ${WISDOM_EXAMPLES_HEADERS_AVAILABLE})
option(WISDOM_EXAMPLES_VULKAN "Build Vulkan examples." ${WISDOM_VULKAN})
option(WISDOM_EXAMPLES_DX12 "Build DirectX 12 examples." ${WISDOM_DX12})

set(WISDOM_EXAMPLES_MODES)
foreach(MODE STATIC SHARED HEADERS)
  if(WISDOM_EXAMPLES_BUILD_${MODE})
    if(NOT WISDOM_EXAMPLES_${MODE}_AVAILABLE)
      message(
        FATAL_ERROR
          "Wisdom does not provide the ${MODE} core and platform targets. "
          "Enable the required libraries and WISDOM_BUILD_PLATFORM, or disable "
          "WISDOM_EXAMPLES_BUILD_${MODE}.")
    endif()
    string(TOLOWER ${MODE} MODE_NAME)
    list(APPEND WISDOM_EXAMPLES_MODES ${MODE_NAME})
  endif()
endforeach()
if(NOT WISDOM_EXAMPLES_MODES)
  message(
    FATAL_ERROR "Enable at least one WISDOM_EXAMPLES_BUILD_* linkage mode.")
endif()
if(NOT WISDOM_EXAMPLES_VULKAN AND NOT WISDOM_EXAMPLES_DX12)
  message(FATAL_ERROR "No Wisdom graphics backend is enabled for examples.")
endif()

set(EXAMPLE_BIN_OUTPUT ${CMAKE_BINARY_DIR}/bin/examples)
file(MAKE_DIRECTORY ${EXAMPLE_BIN_OUTPUT})
include(cmake/shaders.cmake)
include(cmake/functions.cmake)

wis_load_dxc(DOWNLOAD_LATEST)
add_custom_target(wis_test_compile_shaders)
file(GLOB SHADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.hlsl)
foreach(SHADER ${SHADERS})
  get_filename_component(SHADER_NAME ${SHADER} NAME)
  wis_compile_shader(TARGET wis_test_compile_shaders SHADER ${SHADER} OUTPUT
                     ${EXAMPLE_BIN_OUTPUT}/${SHADER_NAME})
endforeach()

add_custom_target(
  wis_examples_copy_assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets
          ${EXAMPLE_BIN_OUTPUT}/assets)

function(add_example_suite NAME)
  if(WISDOM_EXAMPLES_VULKAN)
    set(POSTFIX "vk")
    set(ADD_DEFINITIONS WISDOM_FORCE_VULKAN=1)
    add_subdirectory(${NAME} ${CMAKE_CURRENT_BINARY_DIR}/${NAME}-${POSTFIX})
  endif()
  if(WISDOM_EXAMPLES_DX12)
    set(POSTFIX "dx12")
    set(ADD_DEFINITIONS)
    add_subdirectory(${NAME} ${CMAKE_CURRENT_BINARY_DIR}/${NAME}-${POSTFIX})
  endif()
endfunction()

add_example_suite(backend)
add_example_suite(compute_particles_c)
add_example_suite(hello_triangle)
add_example_suite(multisampling)
