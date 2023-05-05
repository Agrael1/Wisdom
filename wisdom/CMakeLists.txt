# CMakeList.txt : CMake project for Wisdom, include source and define
# project specific logic here.
#
project(Wisdom)

set(SEVERITY_LEVELS "debug;trace;info;warn;error;critical" CACHE INTERNAL "List of possible severities for logging")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CURR_LEVEL "debug")
else()
	set(CURR_LEVEL "warn")
endif()

set(LOG_LEVEL ${CURR_LEVEL} CACHE STRING "Wisdom library logging level")
set_property(CACHE LOG_LEVEL PROPERTY STRINGS ${SEVERITY_LEVELS})
list(FIND SEVERITY_LEVELS "${CURR_LEVEL}" SEV_INDEX)
message("[Wisdom] Log Level is set to ${LOG_LEVEL} [${SEV_INDEX}]")

option(RUNTIME_ASSERTS "Turn on/off runtime assertions (turned on might slightly affect performance)" ON)
option(FORCE_VULKAN "Forces the default device to be vulkan, instead of platform specific" OFF)

find_package(Vulkan)
add_library(${PROJECT_NAME} STATIC)


if(Vulkan_FOUND)
	message("Vulkan found!")
	set(WISDOMVK TRUE)
		target_link_libraries(${PROJECT_NAME} 
		PUBLIC
			VKAllocator
	)
	set(VKHEADERS
	
	 "include/wisdom/vulkan/vk_factory.h" "include/wisdom/vulkan/vk_definitions.h" "include/wisdom/vulkan/vk_adapter.h" "include/wisdom/vulkan/vk_shared_handle.h"  "include/wisdom/vulkan/vk_dynamic_loader.h" "include/wisdom/vulkan/vk_device.h" "include/wisdom/vulkan/vk_allocator.h"  "include/wisdom/vulkan/vk_allocator_handles.h" "include/wisdom/vulkan/vk_command_queue.h" "include/wisdom/vulkan/vk_fence.h" "include/wisdom/vulkan/vk_checks.h" "include/wisdom/vulkan/vk_swapchain.h" "include/wisdom/vulkan/vk_format.h" "include/wisdom/vulkan/vk_resource.h" "include/wisdom/vulkan/vk_rtv.h" "include/wisdom/vulkan/vk_command_list.h" "include/wisdom/vulkan/vk_render_pass.h" "include/wisdom/vulkan/vk_pipeline_state.h" "include/wisdom/vulkan/vk_state_builder.h" "include/wisdom/vulkan/vk_shader.h" "include/wisdom/vulkan/vk_root_signature.h" "include/wisdom/vulkan/vk_buffer_views.h")
	set(VKSOURCES
	
	 "src/wisdom/vulkan/vk_factory.cpp" )
endif()


if(WIN32)
	message("Platform Library is DirectX 12")
	if(NOT FORCE_VULKAN)
		set(WISDOMDX12 TRUE)
	endif()
	set(DX12HEADERS 
		"include/wisdom/dx12/dx12_definitions.h" 
		"include/wisdom/dx12/dx12_factory.h" 
		"include/wisdom/dx12/dx12_checks.h" 
		"include/wisdom/dx12/dx12_info.h" 
		"include/wisdom/dx12/dx12_adapter.h" 
		"include/wisdom/dx12/dx12_device.h" 
		"include/wisdom/dx12/dx12_command_queue.h" 
		"include/wisdom/dx12/dx12_swapchain.h" 
		"include/wisdom/dx12/dx12_resource.h" 
		"include/wisdom/dx12/dx12_command_list.h" 
		"include/wisdom/dx12/dx12_fence.h" 
		"include/wisdom/dx12/dx12_rtv.h" 
		"include/wisdom/dx12/dx12_pipeline_state.h" 
		"include/wisdom/dx12/dx12_root_signature.h" 
		"include/wisdom/dx12/dx12_shader.h" 
		"include/wisdom/dx12/dx12_allocator.h" 
		"include/wisdom/dx12/dx12_state_builder.h" "include/wisdom/dx12/dx12_buffer_views.h" "include/wisdom/dx12/dx12_render_pass.h" "include/wisdom/dx12/dx12_format.h")
	set(DX12SOURCES
		"src/wisdom/dx12/dx12_checks.cpp"
		"src/wisdom/dx12/dx12_info.cpp"
	)
	target_link_libraries(${PROJECT_NAME} 
		PUBLIC 
			WinRT
			DX12Agility
			DX12Allocator
			Wil
			d3d12
			DXGI
			DXGUID
	)
	target_compile_definitions(${PROJECT_NAME} 
		PUBLIC 
			VK_USE_PLATFORM_WIN32_KHR
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS" OR CMAKE_SYSTEM_NAME STREQUAL "tvOS" AND NOT FORCE_VULKAN)
#	message("Platform Library is Metal")
#    set(WISDOMMTL TRUE)
#	target_compile_definitions(${PROJECT_NAME} 
#		PUBLIC 
#			VK_USE_PLATFORM_METAL_EXT
#	)
else()
	#	target_compile_definitions(${PROJECT_NAME} 
	#		PUBLIC 
	#			VK_USE_PLATFORM_METAL_EXT
	#	)
endif()


set(HEADER_LIST 
	"include/wisdom/wisdom.h"
	"include/wisdom/util/log_layer.h" 
	"include/wisdom/util/flags.h" 
	"include/wisdom/util/generator.h" 
	"include/wisdom/global/definitions.h" 
	"include/wisdom/util/exception.h" 
	"include/wisdom/api/api_factory.h" 
	"include/wisdom/api/api_barrier.h" 
	"include/wisdom/api/api_internal.h"
 "include/wisdom/api/api_adapter.h"  "include/wisdom/api/api_swapchain.h" "include/wisdom/util/winsetup.h"  "include/wisdom/api/api_barrier.h"  "include/wisdom/shader_compiler.h" "include/wisdom/api/api_shader.h"  "include/wisdom/api/api_input_layout.h"   "include/wisdom/util/profile.h"  "include/wisdom/api/api_common.h" "include/wisdom/util/misc.h" "include/wisdom/api/api_render_pass.h" "include/wisdom/util/small_allocator.h")

# Add source to this project's executable.
target_sources(${PROJECT_NAME}
	PUBLIC FILE_SET HEADERS 
		BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
		FILES ${HEADER_LIST} ${DX12HEADERS} ${VKHEADERS}
	PRIVATE 
		${DX12SOURCES}
		${VKSOURCES}
		"src/x.cpp"
		"src/wisdom/util/exception.cpp"

)

target_precompile_headers(${PROJECT_NAME} PRIVATE "include/wisdom/pch.h")
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
		WISDOMDX12=$<BOOL:${WISDOMDX12}>
		RUNTIME_ASSERTS=$<BOOL:${RUNTIME_ASSERTS}>
		WISDOMVK=$<BOOL:${WISDOMVK}>
		WISDOMMTL=$<BOOL:${WISDOMMTL}>
		WISDOM_LOG_LEVEL=${SEV_INDEX}
		NOMINMAX
)

target_install(${PROJECT_NAME})
# TODO: Add tests.