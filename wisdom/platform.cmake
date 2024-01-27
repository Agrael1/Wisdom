
if(WISDOM_WINDOWS)
set(PLATFORM_HEADERS "include/wisdom/platform/win32.h" "include/wisdom/platform/impl/win32.h")
target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_WINDOWS=1 VK_USE_PLATFORM_WIN32_KHR=1)
if(WISDOM_WINDOWS_STORE)
target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_WINDOWS_STORE=1)
endif()
elseif(WISDOM_LINUX)
set(PLATFORM_HEADERS "include/wisdom/platform/posix.h")
target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_LINUX=1)
elseif(WISDOM_MAC)
set(PLATFORM_HEADERS "include/wisdom/platform/mac.h")
target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_MAC=1)
endif()

if(WISDOM_FORCE_VULKAN)
target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_FORCE_VULKAN=1)
endif()


target_sources(${PROJECT_NAME}
	PUBLIC FILE_SET HEADERS
		BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
		FILES ${PLATFORM_HEADERS}
	)

if(WISDOM_BUILD_TYPE STREQUAL "static")
	target_sources(${PROJECT_NAME}
	PRIVATE
		"src/wisdom/platform.cpp"
	)
endif()