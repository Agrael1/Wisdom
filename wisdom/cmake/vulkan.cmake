find_package(Vulkan)
# All Vulkan Sources are disabled on UWP
if(Vulkan_FOUND AND NOT WINDOWS_STORE)
	message("Vulkan found!")
	set(WISDOMVK TRUE)

	target_link_libraries(${PROJECT_NAME}
		PUBLIC VKAllocator
	)

	set(VKHEADERS
		"include/wisdom/vulkan/vk_factory.h"

		"include/wisdom/vulkan/vk_adapter.h"
		
		"include/wisdom/vulkan/vk_managed_handles.h"
		"include/wisdom/vulkan/vk_dynamic_loader.h"
		"include/wisdom/vulkan/vk_device.h"
		"include/wisdom/vulkan/vk_allocator.h"
		"include/wisdom/vulkan/vk_allocator_handles.h"
		"include/wisdom/vulkan/vk_command_queue.h"
		"include/wisdom/vulkan/vk_fence.h"
		"include/wisdom/vulkan/vk_checks.h"
		"include/wisdom/vulkan/vk_swapchain.h"
		"include/wisdom/vulkan/vk_format.h"
		"include/wisdom/vulkan/vk_resource.h"
		"include/wisdom/vulkan/vk_rtv.h"
		"include/wisdom/vulkan/vk_command_list.h"
		"include/wisdom/vulkan/vk_render_pass.h"
		"include/wisdom/vulkan/vk_pipeline_state.h"
		"include/wisdom/vulkan/vk_state_builder.h"
		"include/wisdom/vulkan/vk_shader.h"
		"include/wisdom/vulkan/vk_root_signature.h"
		"include/wisdom/vulkan/vk_buffer_views.h"
		"include/wisdom/vulkan/vk_xshared_handle.h"
		"include/wisdom/vulkan/vk_views.h"
		"include/wisdom/vulkan/vk_descriptor_heap.h" "include/wisdom/vulkan/vk_handle_traits.h")

	target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_VULKAN_FOUND VULKAN_HPP_NO_EXCEPTIONS)

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
	elseif(WISDOM_BUILD_TYPE STREQUAL "modules")
		target_sources(${PROJECT_NAME}
		PUBLIC FILE_SET CXX_MODULES
			BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/modules
			FILES "modules/vulkan.ixx"
		)
	endif()
endif()
