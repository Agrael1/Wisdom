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

if(WIN32 AND NOT WINDOWS_STORE)
	add_subdirectory(hello-triangle-win32)
endif()

if(WINDOWS_STORE)
	add_subdirectory(hello-triangle-winrt)
endif()


	find_package(Vulkan)
	if(Vulkan_FOUND)
		add_subdirectory(hello-triangle-kdgui)
	endif()

