find_package(XCB COMPONENTS XCB)
if(NOT XCB_FOUND)
	return()
endif()

add_library(wisdom-xcb-headers INTERFACE)
add_library(wis::xcb-headers ALIAS wisdom-xcb-headers)
target_link_libraries(wisdom-xcb-headers INTERFACE wis::header-only)
target_compile_definitions(wisdom-xcb-headers INTERFACE WIS_XCB_PLATFORM=1)
target_include_directories(wisdom-xcb-headers INTERFACE ${XCB_INCLUDE_DIRS})
target_include_directories(wisdom-xcb-headers INTERFACE 
 	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(WISDOM_BUILD_BINARIES)
	add_library(wisdom-xcb STATIC 
		"wisdom/wisdom_xcb.h"
		"wisdom/wisdom_xcb.cpp"
	)
	add_library(wis::xcb ALIAS wisdom-xcb)
	target_link_libraries(wisdom-xcb PUBLIC wis::wisdom)
	target_compile_definitions(wisdom-xcb PUBLIC WIS_WINDOWS_PLATFORM=1)
	target_include_directories(wisdom-xcb PUBLIC 
	 	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	)
else()
	add_library(wisdom-xcb ALIAS wisdom-xcb-headers)
	add_library(wis::xcb ALIAS wisdom-xcb-headers)
endif()

target_link_libraries(wisdom-platform INTERFACE wis::xcb)
target_link_libraries(wisdom-platform-headers INTERFACE wis::xcb-headers)

install(TARGETS wisdom-xcb wisdom-xcb-headers
	EXPORT wisdom-xcb-targets
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	COMPONENT platform
)

install(EXPORT wisdom-xcb-targets
	FILE wisdom-xcb-targets.cmake
	NAMESPACE wis::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom
	COMPONENT platform
)

configure_package_config_file(
  "${PLATFORM_CONFIG_DIR}/wisdom-xcb-config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/wisdom-xcb-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom
)

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/wisdom-xcb-config.cmake"
  COMPONENT platform
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/wisdom")


install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
		COMPONENT platform
)