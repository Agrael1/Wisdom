# plugins
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
include(FetchContent)

add_subdirectory(shaders)
add_subdirectory(deps)

add_subdirectory(platform)
add_subdirectory(custom)
