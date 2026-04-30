# Each extension will provide an option to build it

# Extensions

# Ray Tracing
option(WISDOM_BUILD_RAYTRACING "Build the Ray Tracing extension." ON)
if (WISDOM_BUILD_RAYTRACING)
  add_subdirectory(raytracing)
endif()

# Video
option(WISDOM_BUILD_VIDEO "Build the Video extension." ON)
if (WISDOM_BUILD_VIDEO)
  add_subdirectory(video)
endif()