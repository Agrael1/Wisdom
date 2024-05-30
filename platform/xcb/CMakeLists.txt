
find_package(XCB COMPONENTS XCB)

if(NOT XCB_FOUND)
	return()
endif()

wisdom_sources(TARGET wisdom-xcb
	HEADERS
		"wisdom/wisdom_xcb.h"
	SOURCES
		"wisdom/wisdom_xcb.cpp"
	DEFINITIONS
		WIS_XCB_PLATFORM=1
	NO_INSTALL
)


target_link_libraries(wisdom-xcb-headers INTERFACE wisdom-vk-headers)
target_link_libraries(wisdom-xcb INTERFACE wisdom-vk)
target_include_directories(wisdom-xcb-headers INTERFACE ${XCB_INCLUDE_DIRS})
target_include_directories(wisdom-xcb-headers INTERFACE 
 	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Platform
target_link_libraries(wisdom-platform INTERFACE wisdom-xcb)
target_link_libraries(wisdom-platform-headers INTERFACE wisdom-xcb-headers)

install(TARGETS wisdom-xcb wisdom-xcb-headers
	EXPORT wisdom-xcb-targets
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT wisdom-xcb-targets
	FILE wisdom-xcb-targets.cmake
	NAMESPACE wis::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})