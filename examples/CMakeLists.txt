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
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:wisdom-shared>
            ${EXAMPLE_BIN_OUTPUT}
    DEPENDS wisdom-shared)
endif()

add_example_suite(test)
add_example_suite(test_c)

