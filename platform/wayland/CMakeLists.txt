find_package(Wayland QUIET)
if(NOT Wayland_FOUND)
	return()
endif()


add_library(wisdom-wayland-headers INTERFACE)
add_library(wis::wayland-headers ALIAS wisdom-wayland-headers)
target_link_libraries(wisdom-wayland-headers INTERFACE wis::header-only)
target_compile_definitions(wisdom-wayland-headers INTERFACE WIS_WAYLAND_PLATFORM=1)
target_include_directories(wisdom-wayland-headers INTERFACE ${WAYLAND_INCLUDE_DIRS})
target_include_directories(wisdom-wayland-headers INTERFACE 
 	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(WISDOM_BUILD_BINARIES)
	add_library(wisdom-wayland STATIC 
		"wisdom/wisdom_wayland.h"
		"wisdom/wisdom_wayland.cpp"
	)
	add_library(wis::wayland ALIAS wisdom-wayland)
	target_link_libraries(wisdom-wayland PUBLIC wis::wisdom)
	target_compile_definitions(wisdom-wayland PUBLIC WIS_WINDOWS_PLATFORM=1)
	target_include_directories(wisdom-wayland PUBLIC 
	 	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	)
else()
	add_library(wisdom-wayland ALIAS wisdom-wayland-headers)
	add_library(wis::wayland ALIAS wisdom-wayland-headers)
endif()

target_link_libraries(wisdom-platform INTERFACE wis::wayland)
target_link_libraries(wisdom-platform-headers INTERFACE wis::wayland-headers)

install(TARGETS wisdom-wayland wisdom-wayland-headers
	EXPORT wisdom-wayland-targets
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	COMPONENT platform
)

install(EXPORT wisdom-wayland-targets
	FILE wisdom-wayland-targets.cmake
	NAMESPACE wis::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom
	COMPONENT platform
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${PLATFORM_CONFIG_DIR}/wisdom-wayland-config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/wisdom-wayland-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom
)

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/wisdom-wayland-config.cmake"
  COMPONENT platform
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/wisdom")


install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
		COMPONENT platform
)