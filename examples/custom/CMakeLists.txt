add_subdirectory(lut)

if(NOT WISDOM_WINDOWS_STORE)
  add_subdirectory(lut_windowed)
endif()
