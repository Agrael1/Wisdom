function(load_nuget_dependency NUGET PLUGIN_NAME ALIAS OUT_DIR)
	execute_process(COMMAND ${NUGET} install "${PLUGIN_NAME}" -OutputDirectory ${OUT_DIR})

	file(GLOB PLUGIN_DIRS ${OUT_DIR}/${PLUGIN_NAME}.*)
	list(LENGTH PLUGIN_DIRS PLUGIN_DIRS_L)
	if(${PLUGIN_DIRS_L} GREATER 1)
		#Sort directories by version in descending order, so the first dir is top version
		list(SORT PLUGIN_DIRS COMPARE NATURAL ORDER DESCENDING)
		list(GET PLUGIN_DIRS 0 PLUGIN_DIRX)

		#Remove older version
		MATH(EXPR PLUGIN_DIRS_L "${PLUGIN_DIRS_L}-1")
		foreach(I RANGE 1 ${PLUGIN_DIRS_L})
			list(GET PLUGIN_DIRS ${I} OLD)
			file(REMOVE_RECURSE ${OLD})
		endforeach()
	else()
		list(GET PLUGIN_DIRS 0 PLUGIN_DIRX)
	endif()

	set(${ALIAS}_DIR ${PLUGIN_DIRX} CACHE STRING "${PLUGIN_NAME} PATH" FORCE)
endfunction()


##
#
#
# Usage:
#      wisdom_sources(TARGET <target>
#                     [HEADERS [items...]]
#                     [SOURCES [items...]]
#                     [LIBS [items...]]
#                     [DEFINITIONS [items...]]
#					  [NO_INSTALL])
##
function(wisdom_sources)
	set(options NO_INSTALL "")
	set(oneValueArgs TARGET)
	set(multiValueArgs HEADERS SOURCES DEFINITIONS LIBS)

	cmake_parse_arguments(wisdom_sources
						  "${options}" "${oneValueArgs}" "${multiValueArgs}"
						  ${ARGN})

	if(NOT wisdom_sources_TARGET)
		message(FATAL_ERROR "wisdom_sources: TARGET is required")
	endif()

	set(TARGET_NAME ${wisdom_sources_TARGET})
	set(HEADER_TARGET_NAME ${TARGET_NAME}-headers)

	add_library(${HEADER_TARGET_NAME} INTERFACE ${wisdom_sources_HEADERS})
	add_library(wis::${HEADER_TARGET_NAME} ALIAS ${HEADER_TARGET_NAME})

	if(WISDOM_BUILD_BINARIES)
	    add_library(${TARGET_NAME} STATIC)
	    add_library(wis::${TARGET_NAME} ALIAS ${TARGET_NAME})
		target_sources(${TARGET_NAME}
			PRIVATE
				${wisdom_sources_SOURCES}
		)
	    target_link_libraries(${TARGET_NAME} PUBLIC ${HEADER_TARGET_NAME})
	    set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 20)
		target_compile_definitions(${TARGET_NAME} PUBLIC WISDOM_BUILD_BINARIES=1)
	else()
		add_library(${TARGET_NAME} INTERFACE)
		add_library(wis::${TARGET_NAME} ALIAS ${TARGET_NAME})
	    target_link_libraries(${TARGET_NAME} INTERFACE ${HEADER_TARGET_NAME})
	endif()

	if(wisdom_sources_DEFINITIONS)
		target_compile_definitions(${HEADER_TARGET_NAME} INTERFACE ${wisdom_sources_DEFINITIONS})
	endif()
	if(wisdom_sources_LIBS)
		target_link_libraries(${HEADER_TARGET_NAME} INTERFACE ${wisdom_sources_LIBS})
	endif()

	if(NOT wisdom_sources_NO_INSTALL)
		install(TARGETS ${TARGET_NAME} ${HEADER_TARGET_NAME}
			EXPORT wisdom-targets
			RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
			LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
			ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
			INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
		)
	endif()
endfunction(wisdom_sources)
