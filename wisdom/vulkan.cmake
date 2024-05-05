find_package(Vulkan)
# All Vulkan Sources are disabled on UWP
if(Vulkan_FOUND AND NOT WISDOM_WINDOWS_STORE)
	message("Vulkan found!")
	set(WISDOM_VULKAN TRUE)

	wisdom_sources(TARGET ${PROJECT_NAME}
		HEADERS
			"include/wisdom/vulkan/vk_factory.h"
			"include/wisdom/vulkan/vk_views.h"
			"include/wisdom/vulkan/vk_handles.h"
			"include/wisdom/vulkan/vk_adapter.h"
			"include/wisdom/vulkan/vk_convert.h"
			"include/wisdom/vulkan/vk_fence.h"
			"include/wisdom/vulkan/vk_device.h"
			"include/wisdom/vulkan/vk_command_queue.h"
			"include/wisdom/vulkan/vk_command_list.h"
			"include/wisdom/vulkan/vk_root_signature.h"
			"include/wisdom/vulkan/vk_descriptor_buffer.h"
			"include/wisdom/vulkan/vk_resource.h"
			"include/wisdom/vulkan/vk_shader.h"
			"include/wisdom/vulkan/vk_allocator.h"
			"include/wisdom/vulkan/vk_swapchain.h"
			"include/wisdom/vulkan/vk_debug.h"
		SOURCES
			"include/wisdom/vulkan/impl/vk_factory.cpp"
			"include/wisdom/vulkan/impl/vk_allocator.cpp"
			"include/wisdom/vulkan/impl/vk_adapter.cpp"
			"include/wisdom/vulkan/impl/vk_fence.cpp"
			"include/wisdom/vulkan/impl/vk_device.cpp"
			"include/wisdom/vulkan/impl/vk_command_queue.cpp"
			"include/wisdom/vulkan/impl/vk_command_list.cpp"
			"include/wisdom/vulkan/impl/vk_swapchain.cpp"

		DEFINITIONS
			WISDOM_VULKAN=1
			WISVK_NO_DEFAULT_DELETER=1
		LIBS
			VKAllocator
			Wisdom::WisVk
	)
endif()
