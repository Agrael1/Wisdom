# plugins
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
include(FetchContent)

# math library
#find_package(glm QUIET)
#
#if (NOT glm_FOUND)
#    message("Loading GLM...")
#    FetchContent_Declare(
#        glm
#        GIT_REPOSITORY https://github.com/g-truc/glm.git
#        GIT_TAG        master
#    )
#    FetchContent_MakeAvailable(glm)
#endif()
#

add_subdirectory(shaders)
add_subdirectory(deps)
add_subdirectory(platform)
add_subdirectory(custom)
