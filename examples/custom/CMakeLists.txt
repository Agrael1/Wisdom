add_subdirectory(lut)

if(WISDOM_WINDOWS AND NOT WISDOM_WINDOWS_STORE)
  add_subdirectory(lut_windowed)
  add_subdirectory(bindings)
endif()
