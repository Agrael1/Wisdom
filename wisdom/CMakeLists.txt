# CMakeList.txt : CMake project for Wisdom
project(Wisdom)

include(cmake/documentation.cmake)

add_library(${PROJECT_NAME} STATIC)

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
		"include/wisdom/vulkan/vk_definitions.h"
		"include/wisdom/vulkan/vk_adapter.h"
		"include/wisdom/vulkan/vk_shared_handle.h"
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
	 "include/wisdom/vulkan/vk_xshared_handle.h")
	set(VKSOURCES
		"src/wisdom/vulkan/vk_factory.cpp" 
	)
	target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_VULKAN_FOUND)
endif()

include(cmake/dx12.cmake)

if(CMAKE_SYSTEM_NAME STREQUAL "iOS" OR CMAKE_SYSTEM_NAME STREQUAL "tvOS" AND NOT WISDOM_FORCE_VULKAN)
	message("Platform Library is Metal")
	set(WISDOMMTL TRUE)
	message(FATAL_ERROR "Metal is not implemented")
else()

endif()

set(HEADER_LIST 
	"include/wisdom/wisdom.h"
	"include/wisdom/util/log_layer.h" 
	"include/wisdom/util/flags.h" 
	"include/wisdom/bridge/generator.h" 
	"include/wisdom/global/definitions.h" 
	"include/wisdom/util/exception.h" 
	"include/wisdom/api/api_factory.h" 
	"include/wisdom/api/api_barrier.h" 
	"include/wisdom/api/api_internal.h"
	"include/wisdom/api/api_adapter.h"
	"include/wisdom/api/api_swapchain.h"
	
	"include/wisdom/api/api_barrier.h"
	"include/wisdom/shader_compiler.h"
	"include/wisdom/api/api_shader.h"
	"include/wisdom/api/api_input_layout.h"
	"include/wisdom/util/profile.h"
	"include/wisdom/api/api_common.h"
	"include/wisdom/util/misc.h"
	"include/wisdom/api/api_render_pass.h"
	"include/wisdom/util/small_allocator.h"
	"include/wisdom/bridge/format.h"
	"include/wisdom/bridge/source_location.h"
 )




# Add source to this project's executable.
target_sources(${PROJECT_NAME}
	PUBLIC FILE_SET HEADERS 
		BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
		FILES ${HEADER_LIST} ${VKHEADERS}
	PRIVATE
		${VKSOURCES}
)

#target_precompile_headers(${PROJECT_NAME} PRIVATE "include/wisdom/pch.h")
target_include_directories(${PROJECT_NAME} PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
set_target_properties(${PROJECT_NAME} 
	PROPERTIES
		CXX_STANDARD 23
)
target_compile_definitions(${PROJECT_NAME} 
	PUBLIC 
		DEBUG_MODE=$<IF:$<CONFIG:Debug>,1,0>
		RUNTIME_ASSERTS=$<BOOL:${RUNTIME_ASSERTS}>
		WISDOMDX12=$<BOOL:${WISDOMDX12}>
		WISDOMVK=$<BOOL:${WISDOMVK}>
		WISDOMMTL=$<BOOL:${WISDOMMTL}>
		WISDOM_LOG_LEVEL=${SEV_INDEX}
		NOMINMAX
)

if(WISDOM_BUILD_TYPE STREQUAL "headers")
	target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_HEADER_ONLY WIS_EXPORT=;)# hack, because cmake does not allow empty definitions
elseif(WISDOM_BUILD_TYPE STREQUAL "modules")
	target_compile_definitions(${PROJECT_NAME} PUBLIC WIS_EXPORT=export WISDOM_HEADER_ONLY)
	target_sources(${PROJECT_NAME}
		PUBLIC FILE_SET CXX_MODULES 
			BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/modules
			FILES "modules/wisdom.ixx"
	)
endif()


if(WISDOM_FORCE_VULKAN)
	target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_FORCE_VULKAN)
endif()

if (APPLE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_MAC VK_USE_PLATFORM_METAL_EXT)
elseif (UNIX AND NOT APPLE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_LINUX VK_USE_PLATFORM_XCB_KHR)
endif ()

if(WISDOM_USE_FMT)
	target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_USE_FMT)
	target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)
endif()


target_install(${PROJECT_NAME})
