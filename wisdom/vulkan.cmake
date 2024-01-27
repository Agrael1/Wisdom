find_package(Vulkan)
# All Vulkan Sources are disabled on UWP
if(Vulkan_FOUND AND NOT WISDOM_WINDOWS_STORE)
	message("Vulkan found!")
	set(WISDOM_VULKAN TRUE)

	target_link_libraries(${PROJECT_NAME}
		PUBLIC VKAllocator wis::wisvk
	)
	target_include_directories(${PROJECT_NAME} PUBLIC
		${Vulkan_INCLUDE_DIRS}
	)

	set(VKHEADERS		
		"include/wisdom/vulkan/xvk_pipeline_state.h"
		"include/wisdom/vulkan/xvk_shader.h"
		"include/wisdom/vulkan/xvk_root_signature.h"
		"include/wisdom/vulkan/xvk_factory.h" 
		"include/wisdom/vulkan/xvk_convert.h" 
		"include/wisdom/vulkan/xvk_checks.h" 
		"include/wisdom/vulkan/xvk_adapter.h" 
		"include/wisdom/vulkan/xvk_device.h" 
		"include/wisdom/vulkan/xvk_views.h" 
		"include/wisdom/vulkan/vk_queue_residency.h" 
		"include/wisdom/vulkan/xvk_swapchain.h" 
		"include/wisdom/vulkan/xvk_fence.h" 
		"include/wisdom/vulkan/xvk_command_queue.h" 
		"include/wisdom/vulkan/xvk_handles.h" 
		"include/wisdom/vulkan/xvk_allocator.h" 
		"include/wisdom/vulkan/xvk_descriptors.h" 
		"include/wisdom/vulkan/xvk_descriptor_heap.h" 
		"include/wisdom/vulkan/xvk_state_builder.h" 
		"include/wisdom/vulkan/xvk_command_list.h" 
		"include/wisdom/vulkan/xvk_resource.h" 

		"include/wisdom/vulkan/impl/vk_allocator.h" 
		"include/wisdom/vulkan/impl/vk_device.h" 
		"include/wisdom/vulkan/impl/vk_factory.h" 
		"include/wisdom/vulkan/impl/vk_swapchain.h"
	)

	target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_VULKAN WISVK_NO_DEFAULT_DELETER)

	# Add source to this project's executable.
	target_sources(${PROJECT_NAME}
	PUBLIC FILE_SET HEADERS
		BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
		FILES ${VKHEADERS}
	)

	if(WISDOM_BUILD_TYPE STREQUAL "static")
		target_sources(${PROJECT_NAME}
		PRIVATE
			"src/wisdom/vulkan.cpp"
		)
	endif()
endif()
