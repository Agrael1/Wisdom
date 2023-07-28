project(VKAllocator)

if(NOT PLUGINS_LOADED)
	message("Setting up Vulkan Allocator...")
	FetchContent_Declare(
	  vkma
	  GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
	  GIT_TAG 3cb5470faabaf1c1ab7b60828ad8f33f22be0424
	)
	FetchContent_GetProperties(vkma)
	if(NOT vkma_POPULATED)
	  FetchContent_Populate(vkma)
	endif()
	set(vkma_SOURCES ${vkma_SOURCE_DIR} CACHE INTERNAL "")

	FetchContent_Declare(
	  vkmahpp
	  GIT_REPOSITORY https://github.com/YaaZ/VulkanMemoryAllocator-Hpp.git
	  GIT_TAG origin/master
      GIT_SUBMODULES "VulkanMemoryAllocator" "Vulkan-Hpp"
	)
	FetchContent_GetProperties(vkmahpp)
	if(NOT vkmahpp_POPULATED)
	  FetchContent_Populate(vkmahpp)
	endif()

	set(vkmahpp_SOURCES ${vkmahpp_SOURCE_DIR} CACHE INTERNAL "")
endif()


add_library(${PROJECT_NAME} STATIC ${vkma_SOURCES}/include/vk_mem_alloc.h)
target_sources(${PROJECT_NAME}        
	PRIVATE
		${vkma_SOURCES}/src/VmaUsage.cpp
		${vkma_SOURCES}/src/VmaUsage.h
)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # TODO: find a better way to do this
    # TODO: find ANY way to do this
    add_compile_definitions(VK_USE_PLATFORM_WAYLAND_KHR)
    target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_WAYLAND_KHR)
endif()

if (MSVC)
	# Provides MSVC users nicer debugging support
	target_sources(${PROJECT_NAME} PRIVATE ${vkma_SOURCES}/src/vk_mem_alloc.natvis)
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)
target_include_directories(${PROJECT_NAME}
	PUBLIC 
		$<BUILD_INTERFACE:${vkma_SOURCES}/include>
		$<BUILD_INTERFACE:${vkmahpp_SOURCES}/include>
		$<INSTALL_INTERFACE:include>
)
set_target_properties(${PROJECT_NAME}
		PROPERTIES
			CXX_STANDARD 23
)
target_install(${PROJECT_NAME})
