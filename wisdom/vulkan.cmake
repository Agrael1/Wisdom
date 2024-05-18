find_package(Vulkan)
# All Vulkan Sources are disabled on UWP
if(Vulkan_FOUND AND NOT WISDOM_WINDOWS_STORE)
	set(WISDOM_VULKAN TRUE)
	message("Vulkan found!")
	wisdom_sources(TARGET wisdom-vk
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
			"include/wisdom/vulkan/impl/vk_create_factory.cpp"
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
			wisdom-shared
			VKAllocator
			Wisdom::WisVk
		NO_INSTALL
	)

install(
  TARGETS wisdom-vk wisdom-vk-headers
  EXPORT wisdom-vk-targets
  FILE_SET HEADERS
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install
install(
  EXPORT wisdom-vk-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom
  NAMESPACE wis::
  FILE wisdom-vk-targets.cmake # Not sure if this is still needed
)


endif()
