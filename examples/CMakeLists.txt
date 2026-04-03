set(EXAMPLE_BIN_OUTPUT ${CMAKE_BINARY_DIR}/bin/examples)

function(add_example_suite NAME)
  if(WISDOM_VULKAN)
    set(POSTFIX "vk")
    set(ADD_DEFINITIONS -DWISDOM_FORCE_VULKAN=1)
    add_subdirectory(${NAME} ${CMAKE_CURRENT_BINARY_DIR}/${NAME}-${POSTFIX})
  endif()
  if(WISDOM_DX12)
    set(POSTFIX "dx12")
    set(ADD_DEFINITIONS)
    add_subdirectory(${NAME} ${CMAKE_CURRENT_BINARY_DIR}/${NAME}-${POSTFIX})
  endif()
  unset(POSTFIX)
  unset(ADD_DEFINITIONS)
endfunction()

# Add custom target to copy dlls if needed
if(WISDOM_BUILD_SHARED)
  add_custom_target(
    copy_dll
    COMMAND ${CMAKE_COMMAND} -E echo "Copying DLLs to example binaries..."
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:wisdom-shared> ${EXAMPLE_BIN_OUTPUT}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:wisdom-platform-shared> ${EXAMPLE_BIN_OUTPUT}
    DEPENDS wisdom-shared wisdom-platform-shared )
endif()

# compile shaders from folder shaders and put them in the binary output directory

add_custom_target(wis_test_compile_shaders)
file (GLOB SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*)
foreach(SHADER ${SHADERS})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    WIS_COMPILE_SHADER(
        TARGET wis_test_compile_shaders
        SHADER ${SHADER}
        OUTPUT ${EXAMPLE_BIN_OUTPUT}/${SHADER_NAME}
    )
endforeach()

add_custom_target(
    copy_sdl
    COMMAND ${CMAKE_COMMAND} -E echo "Copying DLLs to example binaries..."
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL3-shared> ${EXAMPLE_BIN_OUTPUT}
    DEPENDS SDL3-shared )

include(cmake/deps.cmake)

add_example_suite(backend)
add_example_suite(compute_particles_c)
add_example_suite(hello_triangle)
add_example_suite(multisampling)

