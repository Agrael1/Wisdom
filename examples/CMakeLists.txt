# plugins
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
include(FetchContent)

# math library
FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG        master
)
FetchContent_MakeAvailable(glm)

add_subdirectory(shaders)

find_package(Vulkan)

if(WINDOWS_STORE)
	add_subdirectory(hello-triangle-winrt)
else()
	if(WIN32)
		add_subdirectory(hello-triangle-win32)
	endif()
	if(Vulkan_FOUND)
		add_subdirectory(hello-triangle-kdgui)
		if(WISDOM_BUILD_TYPE STREQUAL "modules")
			add_subdirectory(hello-triangle-modules)
		endif()
	endif()
endif()

