add_subdirectory(shared)

# only winrt is supported on windows store apps
if(WISDOM_WINDOWS_STORE)
  # add_subdirectory(hello-triangle-winrt)
else()
  if(NOT WISDOM_EXCLUDE_KDGUI)
    add_subdirectory(hello-triangle-kdgui-t)
  endif()
  if(WISDOM_WINDOWS)
    add_subdirectory(hello-triangle-win32-t)
  endif()
endif()
