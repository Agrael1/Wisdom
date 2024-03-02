find_package(Vulkan)
# All Vulkan Sources are disabled on UWP
if(Vulkan_FOUND AND NOT WISDOM_WINDOWS_STORE)
	message("Vulkan found!")
	set(WISDOM_VULKAN TRUE)

	wisdom_sources(TARGET ${PROJECT_NAME}
		HEADERS 
			"include/wisdom/xvulkan/vk_factory.h"
			"include/wisdom/xvulkan/vk_views.h"
			"include/wisdom/xvulkan/vk_handles.h"
			"include/wisdom/xvulkan/vk_adapter.h"
			"include/wisdom/xvulkan/vk_convert.h"
			"include/wisdom/xvulkan/vk_fence.h"
			"include/wisdom/xvulkan/vk_device.h"
			"include/wisdom/xvulkan/vk_command_queue.h"
			"include/wisdom/xvulkan/vk_command_list.h"
			"include/wisdom/xvulkan/vk_root_signature.h"
			"include/wisdom/xvulkan/vk_resource.h"
			"include/wisdom/xvulkan/vk_shader.h"
			"include/wisdom/xvulkan/vk_allocator.h"
			"include/wisdom/xvulkan/vk_swapchain.h"
			"include/wisdom/xvulkan/vk_debug.h"
		SOURCES
			"include/wisdom/xvulkan/impl/vk_factory.cpp"
			"include/wisdom/xvulkan/impl/vk_allocator.cpp"
			"include/wisdom/xvulkan/impl/vk_adapter.cpp"
			"include/wisdom/xvulkan/impl/vk_fence.cpp"
			"include/wisdom/xvulkan/impl/vk_device.cpp"
			"include/wisdom/xvulkan/impl/vk_command_queue.cpp"
			"include/wisdom/xvulkan/impl/vk_command_list.cpp"
			"include/wisdom/xvulkan/impl/vk_swapchain.cpp"
			
		DEFINITIONS 
			WISDOM_VULKAN=1
			WISVK_NO_DEFAULT_DELETER=1
		LIBS
			VKAllocator
			wis::wisvk
	)

	target_include_directories(${PROJECT_NAME} ${WISDOM_PUBLIC}
		${Vulkan_INCLUDE_DIRS}
	)
endif()
