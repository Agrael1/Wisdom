include(cmake/deps.cmake)

function(add_example name)
  if(WISDOM_VULKAN)
    set(POSTFIX "vk")
    add_subdirectory(${name} ${CMAKE_CURRENT_BINARY_DIR}/${name}-${POSTFIX})
  endif()
  if(WISDOM_DX12)
    set(POSTFIX "dx12")
    add_subdirectory(${name} ${CMAKE_CURRENT_BINARY_DIR}/${name}-${POSTFIX})
  endif()
  unset(POSTFIX)
endfunction()

set(EXAMPLE_BIN_OUTPUT ${CMAKE_BINARY_DIR}/bin/examples)

add_example(common)
add_subdirectory(basic)
add_subdirectory(custom)
