add_subdirectory(shaders)
add_example(shared)

# only winrt is supported on windows store apps
if(NOT WISDOM_WINDOWS_STORE AND WISDOM_WINDOWS)
  #add_example(hello-triangle-win32)
endif()
