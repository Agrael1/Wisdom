# only winrt is supported on windows store apps
if(WISDOM_WINDOWS_STORE)
  # add_subdirectory(hello-triangle-winrt)
else()
  # add_subdirectory(hello-triangle-kdgui-t)
  if(WISDOM_WINDOWS)
    add_subdirectory(hello-triangle-win32-t)
  endif()
endif()
