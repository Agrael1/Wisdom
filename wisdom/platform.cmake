
if(WISDOM_WINDOWS)
	wisdom_sources_ex(TARGET wisdom-platform
		HEADERS
			"include/wisdom/platform/win32.h"
		SOURCES
			"include/wisdom/platform/impl/win32.cpp"
		DEFINITIONS
			WISDOM_WINDOWS=1
			WIN32_LEAN_AND_MEAN
			VK_USE_PLATFORM_WIN32_KHR=1
	)
	if(WISDOM_VULKAN)
		target_link_libraries(wisdom-platform-headers INTERFACE wisdom-vk-headers)
		target_link_libraries(wisdom-platform ${WISDOM_PUBLIC} wisdom-vk)
	endif()
	if(WISDOM_DX12)
		target_link_libraries(wisdom-platform-headers INTERFACE wisdom-dx12-headers)
		target_link_libraries(wisdom-platform ${WISDOM_PUBLIC} wisdom-dx12)
	endif()
elseif(WISDOM_LINUX)
	wisdom_sources_ex(TARGET wisdom-platform
		HEADERS
			"include/wisdom/platform/linux.h"
		SOURCES
			"include/wisdom/platform/impl/linux.cpp"
		DEFINITIONS
			WISDOM_LINUX=1
			VK_USE_PLATFORM_XCB_KHR=1
			VK_USE_PLATFORM_WAYLAND_KHR=1
	)
	target_link_libraries(wisdom-platform-headers INTERFACE wisdom-vk-headers)
	target_link_libraries(wisdom-platform ${WISDOM_PUBLIC} wisdom-vk)
elseif(WISDOM_MAC)
	wisdom_sources_static(TARGET wisdom-platform
		HEADERS
			"include/wisdom/platform/mac.h"
		SOURCES
			"include/wisdom/platform/impl/mac.cpp"
		DEFINITIONS
			WISDOM_MAC=1
			VK_USE_PLATFORM_MACOS_MVK=1
			VK_USE_PLATFORM_METAL_EXT=1
			VK_ENABLE_BETA_EXTENSIONS=1
	)
endif()

if(WISDOM_FORCE_VULKAN)
	target_compile_definitions(wis-header-only INTERFACE WISDOM_FORCE_VULKAN=1)
endif()
