
if(WISDOM_WINDOWS)
	message("Platform Library is DirectX 12")
	set(WISDOM_DX12 TRUE)

	wisdom_sources(TARGET wisdom
		HEADERS
			"include/wisdom/dx12/dx12_factory.h"
			"include/wisdom/dx12/dx12_views.h"
			"include/wisdom/dx12/dx12_adapter.h"
			"include/wisdom/dx12/dx12_info.h"
			"include/wisdom/dx12/dx12_checks.h"
			"include/wisdom/dx12/dx12_convert.h"
			"include/wisdom/dx12/dx12_fence.h"
			"include/wisdom/dx12/dx12_device.h"
			"include/wisdom/dx12/dx12_command_queue.h"
			"include/wisdom/dx12/dx12_command_list.h"
			"include/wisdom/dx12/dx12_descriptor_buffer.h"
			"include/wisdom/dx12/dx12_root_signature.h"
			"include/wisdom/dx12/dx12_resource.h"
			"include/wisdom/dx12/dx12_shader.h"
			"include/wisdom/dx12/dx12_allocator.h"
			"include/wisdom/dx12/dx12_swapchain.h"
			"include/wisdom/dx12/dx12_debug.h"
		SOURCES
			"include/wisdom/dx12/impl/dx12_allocator.cpp"
			"include/wisdom/dx12/impl/dx12_factory.cpp"
			"include/wisdom/dx12/impl/dx12_info.cpp"
			"include/wisdom/dx12/impl/dx12_fence.cpp"
			"include/wisdom/dx12/impl/dx12_device.cpp"
			"include/wisdom/dx12/impl/dx12_swapchain.cpp"
			"include/wisdom/dx12/impl/dx12_command_list.cpp"
		LIBS
			DX12Agility
			DX12Allocator
			d3d12
			d3d11
			DXGI
			DXGUID
	)
endif()
