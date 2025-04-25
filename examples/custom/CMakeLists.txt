if(WISDOM_WINDOWS AND NOT WISDOM_WINDOWS_STORE)
  if(WISDOM_BUILD_BINDINGS)
    add_subdirectory(bindings)
  endif()

  if(WISDOM_VULKAN)
    add_subdirectory(cross_device)
    add_subdirectory(multimon)
  endif()

  add_example(multimon-single)
  add_example(cross-app)
endif()
