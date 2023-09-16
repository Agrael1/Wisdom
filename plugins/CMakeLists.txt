include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

if(WISDOM_USE_FMT)
	message("Loading fmtlib...")
    FetchContent_Declare(
		fmt
		GIT_REPOSITORY https://github.com/fmtlib/fmt.git
		GIT_TAG origin/master
    )
    FetchContent_MakeAvailable(fmt)
endif()


# Win32 plugins
if(WIN32)
	find_program(NUGET_EXE NAMES nuget PATHS ${CMAKE_CURRENT_SOURCE_DIR}/NuGet)
	if(NOT NUGET_EXE)
		message("NUGET.EXE not found.")
		message(FATAL_ERROR "Please install this executable, and run CMake again.")
	endif()

	add_subdirectory("DX12Allocator")
	add_subdirectory("WinRT")
	add_subdirectory("DX12Agility")
	#add_subdirectory("Wil")
endif()


find_package(Vulkan)

# UWP not supported
if(Vulkan_FOUND AND NOT WINDOWS_STORE)
	add_subdirectory("VKAllocator")
	add_subdirectory("Volk")
endif()

add_subdirectory("DXC")

set(PLUGINS_LOADED TRUE CACHE BOOL "Plugins Loaded switch")
