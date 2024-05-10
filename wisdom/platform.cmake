
if(WISDOM_WINDOWS)
	wisdom_sources(TARGET wisdom
		HEADERS
			"include/wisdom/platform/win32.h"
		SOURCES
			"include/wisdom/platform/impl/win32.cpp"
		DEFINITIONS
			WISDOM_WINDOWS=1
			WIN32_LEAN_AND_MEAN
			VK_USE_PLATFORM_WIN32_KHR=1
	)
	if(WISDOM_WINDOWS_STORE)
		target_compile_definitions(wis-header-only INTERFACE WISDOM_WINDOWS_STORE=1)
	endif()
elseif(WISDOM_LINUX)
	wisdom_sources(TARGET wisdom
		HEADERS
			"include/wisdom/platform/linux.h"
		SOURCES
			"include/wisdom/platform/impl/linux.cpp"
		DEFINITIONS
			WISDOM_LINUX=1
			VK_USE_PLATFORM_XCB_KHR=1
			#VK_USE_PLATFORM_XLIB_KHR=1 # Breaks build
			VK_USE_PLATFORM_WAYLAND_KHR=1
	)
elseif(WISDOM_MAC)
	wisdom_sources(TARGET wisdom
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
