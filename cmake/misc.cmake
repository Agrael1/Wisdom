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
# This function always adds sources to target, but when "WHEN" condition is not meet
# source is excluded from build process.
# This doesn't break build, but source is always visible for the project, what is 
# very handy when working with muti-platform project with sources needed
# only for specific platform
#
# Usage:
#      target_optional_sources(WHEN <condition> 
#                              TARGET <target>
#                              <INTERFACE|PUBLIC|PRIVATE> [items2...]
#                              [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
##
function(target_optional_sources)
	set(options OPTIONAL "")
	set(oneValueArgs WHEN TARGET)
	set(multiValueArgs PUBLIC PRIVATE INTERFACE)

	cmake_parse_arguments(target_optional_sources 
						  "${options}" "${oneValueArgs}" "${multiValueArgs}"
						  ${ARGN})

	target_sources(${target_optional_sources_TARGET}
				   PUBLIC ${target_optional_sources_PUBLIC}
				   PRIVATE ${target_optional_sources_PRIVATE}
				   INTERFACE ${target_optional_sources_INTERFACE})

	if (${target_optional_sources_WHEN})
		set_source_files_properties(${target_optional_sources_PUBLIC}
									PROPERTIES HEADER_FILE_ONLY TRUE)
		set_source_files_properties(${target_optional_sources_PRIVATE}
									PROPERTIES HEADER_FILE_ONLY TRUE)
		set_source_files_properties(${target_optional_sources_INTERFACE}
									PROPERTIES HEADER_FILE_ONLY TRUE)
	endif(NOT ${target_optional_sources_WHEN})
endfunction(target_optional_sources)

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
	if(${WISDOM_headers})
		target_sources(${wisdom_sources_TARGET}
			${WISDOM_PUBLIC} FILE_SET HEADERS
				BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
				FILES ${wisdom_sources_HEADERS} 
					${wisdom_sources_SOURCES}
		)
		set_source_files_properties(${wisdom_sources_SOURCES}
									PROPERTIES HEADER_FILE_ONLY TRUE)
	else()
		target_sources(${wisdom_sources_TARGET}
			${WISDOM_PUBLIC} FILE_SET HEADERS
				BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
				FILES ${wisdom_sources_HEADERS}
			${WISDOM_PRIVATE}
				${wisdom_sources_SOURCES}
		)
	endif()

	if(wisdom_sources_DEFINITIONS)
		target_compile_definitions(${wisdom_sources_TARGET} ${WISDOM_PUBLIC} ${wisdom_sources_DEFINITIONS})
	endif()
	if(wisdom_sources_LIBS)
		target_link_libraries(${wisdom_sources_TARGET} ${WISDOM_PUBLIC} ${wisdom_sources_LIBS})
	endif()
endfunction(wisdom_sources)
