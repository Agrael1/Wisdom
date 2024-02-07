
if(WISDOM_WINDOWS)
	message("Platform Library is DirectX 12")
	set(WISDOM_DX12 TRUE)
	
	wisdom_sources(TARGET ${PROJECT_NAME}
		HEADERS 
			"include/wisdom/xdx12/dx12_factory.h"
			"include/wisdom/xdx12/dx12_views.h"
			"include/wisdom/xdx12/dx12_adapter.h"
			"include/wisdom/xdx12/dx12_info.h"
			"include/wisdom/xdx12/dx12_checks.h"
			"include/wisdom/xdx12/dx12_convert.h"
		SOURCES
			"include/wisdom/xdx12/impl/dx12_factory.cpp"
			"include/wisdom/xdx12/impl/dx12_info.cpp"
		LIBS
			DX12Agility
			DX12Allocator
			d3d12
			d3d11
			DXGI
			DXGUID
	)
endif()
