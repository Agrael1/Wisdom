add_subdirectory(include)

# Platform extensions
if(WISDOM_BUILD_PLATFORM)
  add_subdirectory(platform)
endif()

add_subdirectory(extensions)
