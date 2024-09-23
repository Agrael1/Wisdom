add_subdirectory(lut)
add_subdirectory(bindings)

if(WISDOM_WINDOWS AND NOT WISDOM_WINDOWS_STORE)
  add_subdirectory(lut_windowed)

  if(WISDOM_VULKAN)
    add_subdirectory(cross_device)
  endif()
endif()
