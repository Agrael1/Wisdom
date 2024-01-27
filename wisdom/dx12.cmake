
if(WISDOM_WINDOWS)
	message("Platform Library is DirectX 12")
	set(WISDOM_DX12 TRUE)
	set(DX12HEADERS
		"include/wisdom/dx12/xdx12_root_signature.h"
		"include/wisdom/dx12/xdx12_shader.h"
		"include/wisdom/dx12/xdx12_convert.h"
		"include/wisdom/dx12/xdx12_info.h" 
		"include/wisdom/dx12/xdx12_checks.h" 
		"include/wisdom/dx12/xdx12_adapter.h" 
		"include/wisdom/dx12/xdx12_device.h" 
		"include/wisdom/dx12/xdx12_views.h" 
		"include/wisdom/dx12/xdx12_fence.h" 
		"include/wisdom/dx12/xdx12_allocator.h"   
		"include/wisdom/dx12/xdx12_command_queue.h" 
		"include/wisdom/dx12/xdx12_pipeline_state.h" 
		"include/wisdom/dx12/xdx12_command_list.h" 
		"include/wisdom/dx12/xdx12_resource.h" 
		"include/wisdom/dx12/xdx12_swapchain.h" 

		"include/wisdom/dx12/impl/dx12_device.h" 
		"include/wisdom/dx12/impl/dx12_swapchain.h"
	)

	target_link_libraries(${PROJECT_NAME}
		PUBLIC
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
		FILES ${DX12HEADERS}
	)

	if(WISDOM_BUILD_TYPE STREQUAL "static")
		target_sources(${PROJECT_NAME}
		PRIVATE
			"src/wisdom/dx12.cpp"
		)
	endif()
endif()
