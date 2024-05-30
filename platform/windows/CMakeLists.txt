
wisdom_sources(TARGET wisdom-windows
	HEADERS
		"wisdom/wisdom_windows.h"
	SOURCES
		"wisdom/wisdom_windows.cpp"
	DEFINITIONS
		WIS_WINDOWS_PLATFORM=1
	LIBS 
		d3d11
	NO_INSTALL
)


target_link_libraries(wisdom-windows-headers INTERFACE wisdom-dx12-headers)
target_link_libraries(wisdom-windows INTERFACE wisdom-dx12)

if(WISDOM_VULKAN)
	target_link_libraries(wisdom-windows-headers INTERFACE wisdom-vk-headers)
	target_link_libraries(wisdom-windows INTERFACE wisdom-vk)
endif()

target_include_directories(wisdom-windows-headers INTERFACE 
 	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


# Platform
target_link_libraries(wisdom-platform INTERFACE wisdom-windows)
target_link_libraries(wisdom-platform-headers INTERFACE wisdom-windows-headers)

install(TARGETS wisdom-windows wisdom-windows-headers
	EXPORT wisdom-windows-targets
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT wisdom-windows-targets
	FILE wisdom-windows-targets.cmake
	NAMESPACE wis::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})