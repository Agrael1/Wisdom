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
## windows store example does not need kdutils
#if (NOT WINDOWS_STORE)
#    set(BUILD_SHARED_LIBS OFF)
#    set(KDUTILS_BUILD_TESTS OFF)
#
#    find_package(KDUtils CONFIG)
#    if(NOT KDUtils_FOUND)
#        FetchContent_Declare(
#            KDUtils
#            GIT_REPOSITORY https://github.com/KDAB/KDUtils.git
#            GIT_TAG        master
#        )
#        FetchContent_MakeAvailable(KDUtils)
#    endif()
#
#    find_package(KDFoundation CONFIG)
#    find_package(KDGui CONFIG)
#endif()
#
add_subdirectory(shaders)
#
#find_package(Vulkan)
#
#if(WISDOM_BUILD_TYPE STREQUAL "modules")
#	add_subdirectory(hello-triangle-modules)
#elseif(WINDOWS_STORE)
#	add_subdirectory(hello-triangle-winrt)
#else()
#	if(WIN32)
		add_subdirectory(hello-triangle-win32-t)
#		add_subdirectory(hello-triangle-win32)
#	endif()
#	if(Vulkan_FOUND)
#		add_subdirectory(hello-triangle-kdgui)
#		add_subdirectory(hello-triangle-kdgui-t)
#		add_subdirectory(uniform-buffers-kdgui)
#	endif()
#endif()

