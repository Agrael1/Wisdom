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
 		"include/wisdom/vulkan/vk_memory.h"
 		"include/wisdom/vulkan/vk_external.h"
 		"include/wisdom/vulkan/vk_adapter.h"
 		"include/wisdom/vulkan/vk_convert.h"
 		"include/wisdom/vulkan/vk_fence.h"
 		"include/wisdom/vulkan/vk_device.h"
 		"include/wisdom/vulkan/vk_command_queue.h"
 		"include/wisdom/vulkan/vk_command_list.h"
 		"include/wisdom/vulkan/vk_root_signature.h"
 		"include/wisdom/vulkan/vk_descriptor_storage.h"
 		"include/wisdom/vulkan/vk_resource.h"
 		"include/wisdom/vulkan/vk_shader.h"
 		"include/wisdom/vulkan/vk_allocator.h"
 		"include/wisdom/vulkan/vk_swapchain.h"
 		"include/wisdom/vulkan/vk_debug.h"
		"include/wisdom/vulkan/vk_factory_ext.h"
		"include/wisdom/vulkan/vk_device_ext.h"
	)
	add_library(wis::vulkan ALIAS wisdom-vk)
	target_sources(wisdom-vk
		PRIVATE
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
	add_library(wisdom-vk INTERFACE)
	target_link_libraries(wisdom-vk INTERFACE wisdom-vk-headers)
	add_library(wis::vulkan ALIAS wisdom-vk)
endif()

if(WISDOM_FORCE_VULKAN)
	target_compile_definitions(wisdom-vk-headers INTERFACE WISDOM_FORCE_VULKAN=1)
endif()

if(WISDOM_GENERATE_FUNCTIONS)
	if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
	    # Generate a shell script for Linux
	    set(LAUNCHER_SCRIPT "#!/bin/sh\nexec \"\$<TARGET_FILE:WisVk_generator>\" -l -i ${CMAKE_CURRENT_SOURCE_DIR}/include/wisdom/vulkan/gen/vk_functions.in -o ${CMAKE_CURRENT_SOURCE_DIR}/include/wisdom/generated/vulkan/vk_functions.hpp \"$@\"\n")
	    file(GENERATE OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/include/wisdom/vulkan/gen/generate_functions.sh" CONTENT "${LAUNCHER_SCRIPT}")
	    execute_process(COMMAND chmod +x ${CMAKE_CURRENT_SOURCE_DIR}/include/wisdom/vulkan/gen/generate_functions.sh)
	elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
	    # Generate a batch file for Windows
	    set(LAUNCHER_SCRIPT "@echo off\nstart /B \"\" \"\$<TARGET_FILE:WisVk_generator>\" -l -i ${CMAKE_CURRENT_SOURCE_DIR}/include/wisdom/vulkan/gen/vk_functions.in -o ${CMAKE_CURRENT_SOURCE_DIR}/include/wisdom/generated/vulkan/vk_functions.hpp %*\n")
	    file(GENERATE OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/include/wisdom/vulkan/gen/generate_functions.bat" CONTENT "${LAUNCHER_SCRIPT}")
	endif()

	if(WISDOM_GENERATE_API)
		add_custom_target(
			GenerateFunctions
			COMMAND $<TARGET_FILE:WisVk_generator> -l -i ${CMAKE_CURRENT_SOURCE_DIR}/include/wisdom/vulkan/gen/vk_functions.in -o ${CMAKE_CURRENT_SOURCE_DIR}/include/wisdom/generated/vulkan/vk_functions.hpp)
		add_dependencies(wisdom-headers GenerateFunctions)
	endif()
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
