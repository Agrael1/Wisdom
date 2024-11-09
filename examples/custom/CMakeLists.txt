if(WISDOM_WINDOWS AND NOT WISDOM_WINDOWS_STORE)
  add_subdirectory(bindings)

  if(WISDOM_VULKAN)
    add_subdirectory(cross_device)
    add_subdirectory(multimon)
  endif()
endif()
