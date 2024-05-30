find_package(Vulkan)
# All Vulkan Sources are disabled on UWP
if(NOT Vulkan_FOUND OR WISDOM_WINDOWS_STORE)
	return()
endif()

set(WISDOM_VULKAN TRUE CACHE BOOL "Enable Vulkan Support" FORCE)
message(STATUS "Vulkan found!")


add_library (wisdom-vk-headers INTERFACE)
add_library (wis::vulkan-headers ALIAS wisdom-vk-headers)
target_link_libraries(wisdom-vk-headers INTERFACE wisdom-shared VKAllocator Wisdom::WisVk)
target_compile_definitions(wisdom-vk-headers INTERFACE WISDOM_VULKAN=1 WISVK_NO_DEFAULT_DELETER=1)

if(WISDOM_BUILD_BINARIES)
	add_library(wisdom-vk STATIC
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
	 "include/wisdom/vulkan/vk_factory_ext.h")
	add_library(wis::vulkan ALIAS wisdom-vk)
	target_sources(wisdom-vk
		PRIVATE
 			"include/wisdom/vulkan/impl/vk_descriptor_buffer.cpp"
			"include/wisdom/vulkan/impl/vk_factory.cpp"
			"include/wisdom/vulkan/impl/vk_create_factory.cpp"
			"include/wisdom/vulkan/impl/vk_allocator.cpp"
			"include/wisdom/vulkan/impl/vk_adapter.cpp"
			"include/wisdom/vulkan/impl/vk_fence.cpp"
			"include/wisdom/vulkan/impl/vk_device.cpp"
			"include/wisdom/vulkan/impl/vk_command_queue.cpp"
			"include/wisdom/vulkan/impl/vk_command_list.cpp"
			"include/wisdom/vulkan/impl/vk_swapchain.cpp"
	)
	target_link_libraries(wisdom-vk PUBLIC wisdom-vk-headers)
	target_compile_definitions(wisdom-vk PUBLIC WISDOM_BUILD_BINARIES=1)
else()
	add_library(wisdom-vk ALIAS wisdom-vk-headers)
	add_library(wis::vulkan ALIAS wisdom-vk)
endif()

install(
  TARGETS wisdom-vk wisdom-vk-headers
  EXPORT wisdom-vk-targets
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
  FILE wisdom-vk-targets.cmake
)
