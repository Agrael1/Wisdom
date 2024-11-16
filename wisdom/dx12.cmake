
if(NOT WISDOM_WINDOWS)
	return()
endif()

message("Platform Library is DirectX 12")
set(WISDOM_DX12 TRUE CACHE BOOL "Enable DirectX 12 Support" FORCE)


add_library (wisdom-dx12-headers INTERFACE)
add_library (wis::dx12-headers ALIAS wisdom-dx12-headers)
target_link_libraries(wisdom-dx12-headers INTERFACE
		wisdom-shared
		DX12Agility
		DX12Allocator
		d3d12
		DXGI
		DXGUID)
target_compile_definitions(wisdom-dx12-headers INTERFACE WISDOM_DX12=1)

if(WISDOM_BUILD_BINARIES)
	add_library(wisdom-dx12 STATIC
		"include/wisdom/dx12/dx12_factory.h"
		"include/wisdom/dx12/dx12_views.h"
		"include/wisdom/dx12/dx12_adapter.h"
		"include/wisdom/dx12/dx12_info.h"
		"include/wisdom/dx12/dx12_checks.h"
		"include/wisdom/dx12/dx12_convert.h"
		"include/wisdom/dx12/dx12_fence.h"
		"include/wisdom/dx12/dx12_memory.h"
		"include/wisdom/dx12/dx12_device.h"
		"include/wisdom/dx12/dx12_command_queue.h"
		"include/wisdom/dx12/dx12_command_list.h"
		"include/wisdom/dx12/dx12_descriptor_storage.h"
		"include/wisdom/dx12/dx12_root_signature.h"
		"include/wisdom/dx12/dx12_resource.h"
		"include/wisdom/dx12/dx12_shader.h"
		"include/wisdom/dx12/dx12_allocator.h"
		"include/wisdom/dx12/dx12_swapchain.h"
		"include/wisdom/dx12/dx12_debug.h"
		"include/wisdom/dx12/dx12_factory_ext.h"
		"include/wisdom/dx12/dx12_device_ext.h"
	 "include/wisdom/dx12/dx12_unique_event.h")
	target_sources(wisdom-dx12
		PRIVATE
			"include/wisdom/dx12/impl/dx12_allocator.cpp"
			"include/wisdom/dx12/impl/dx12_command_list.cpp"
			"include/wisdom/dx12/impl/dx12_device.cpp"
			"include/wisdom/dx12/impl/dx12_factory.cpp"
			"include/wisdom/dx12/impl/dx12_fence.cpp"
			"include/wisdom/dx12/impl/dx12_swapchain.cpp"
			"include/wisdom/dx12/impl/dx12_info.cpp"
	)
	add_library(wis::dx12 ALIAS wisdom-dx12)
	target_link_libraries(wisdom-dx12 PUBLIC wisdom-dx12-headers)
	target_compile_definitions(wisdom-dx12 PUBLIC WISDOM_BUILD_BINARIES=1)
else()
	add_library(wisdom-dx12 ALIAS wisdom-dx12-headers)
	add_library(wis::dx12 ALIAS wisdom-dx12)
endif()

install(
  TARGETS wisdom-dx12 wisdom-dx12-headers
  EXPORT wisdom-dx12-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install
install(
  EXPORT wisdom-dx12-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom
  NAMESPACE wis::
  FILE wisdom-dx12-targets.cmake
)
