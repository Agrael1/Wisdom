function(target_install_interface TARGET)
	install(TARGETS "${TARGET}" EXPORT ${TARGET}Targets DESTINATION include)
	install(EXPORT ${TARGET}Targets
		FILE "${TARGET}Targets.cmake"
		NAMESPACE Ver::
		DESTINATION "lib/cmake/${TARGET}"
	)

	export(EXPORT ${TARGET}Targets
	   FILE "${TARGET}Targets.cmake"
	   NAMESPACE Ver::
	)
endfunction()

function(target_install TARGET)
	install(TARGETS ${TARGET} EXPORT ${TARGET}Targets
		FILE_SET HEADERS DESTINATION include
		FILE_SET CXX_MODULES DESTINATION modules
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		INCLUDES DESTINATION include
	)
	#install(EXPORT ${TARGET}Targets
	#	 FILE "${TARGET}Targets.cmake"
	#	 NAMESPACE Ver::
	#	 DESTINATION "lib/cmake/${TARGET}"
	#)

	#export(EXPORT ${TARGET}Targets
	#	   FILE "${TARGET}Targets.cmake"
	#	   NAMESPACE Ver::
	#)
endfunction()


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
#                     [LIBS [items...]])
##
function(wisdom_sources)
	set(options OPTIONAL "")
	set(oneValueArgs TARGET)
	set(multiValueArgs HEADERS SOURCES DEFINITIONS LIBS)

	cmake_parse_arguments(wisdom_sources
						  "${options}" "${oneValueArgs}" "${multiValueArgs}"
						  ${ARGN})
	
	if(NOT WISDOM_BUILD_BINARIES)
		target_sources(${wisdom_sources_TARGET}
			INTERFACE FILE_SET HEADERS
				BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
				FILES ${wisdom_sources_HEADERS}
					${wisdom_sources_SOURCES}
		)
		set_source_files_properties(${wisdom_sources_SOURCES}
									PROPERTIES HEADER_FILE_ONLY TRUE)
	else()
		target_sources(${wisdom_sources_TARGET}
			PUBLIC FILE_SET HEADERS
				BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
				FILES ${wisdom_sources_HEADERS}
			PRIVATE
				${wisdom_sources_SOURCES}
		)
	endif()
	if(wisdom_sources_DEFINITIONS)
		target_compile_definitions(${wisdom_sources_TARGET}Headers INTERFACE ${wisdom_sources_DEFINITIONS})
	endif()
	if(wisdom_sources_LIBS)
		target_link_libraries(${wisdom_sources_TARGET}Headers INTERFACE ${wisdom_sources_LIBS})
	endif()
endfunction(wisdom_sources)
