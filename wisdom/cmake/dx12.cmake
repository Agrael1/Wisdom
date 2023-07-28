
if(WIN32)
	message("Platform Library is DirectX 12")
	set(WISDOMDX12 TRUE)
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
		"include/wisdom/dx12/dx12_state_builder.h"
		"include/wisdom/dx12/dx12_buffer_views.h"
		"include/wisdom/dx12/dx12_render_pass.h"
		"include/wisdom/dx12/dx12_format.h" 
		"include/wisdom/dx12/dx12_views.h"
		"include/wisdom/dx12/dx12_descriptor_heap.h")
	set(DX12INL
		"include/wisdom/dx12/impl/dx12_adapter.inl"
		"include/wisdom/dx12/impl/dx12_allocator.inl"
		"include/wisdom/dx12/impl/dx12_checks.inl"
		"include/wisdom/dx12/impl/dx12_device.inl"
		"include/wisdom/dx12/impl/dx12_factory.inl"
		"include/wisdom/dx12/impl/dx12_info.inl"
	)
	target_link_libraries(${PROJECT_NAME} 
		PUBLIC 
			WinRT
			DX12Agility
			DX12Allocator
			d3d12
			d3d11
			DXGI
			DXGUID
	)

	target_sources(${PROJECT_NAME}
	PUBLIC FILE_SET HEADERS 
		BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
		FILES ${DX12HEADERS} ${DX12INL}
	)

	if(WISDOM_BUILD_TYPE STREQUAL "static")
		target_sources(${PROJECT_NAME}
		PRIVATE 
			"src/wisdom/dx12.cpp"
		)
	elseif(WISDOM_BUILD_TYPE STREQUAL "modules")
		target_sources(${PROJECT_NAME}
		PUBLIC FILE_SET CXX_MODULES 
			BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/modules
			FILES "modules/dx12.ixx"
		)
	endif()

	if(WINDOWS_STORE)
		target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_UWP)
	endif()
    target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_WINDOWS VK_USE_PLATFORM_WIN32_KHR)
endif()