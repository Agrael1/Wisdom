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

function(add_dependent_target target_name visibility condition)
    # Shift the arguments to get the list of libraries
    set(libraries ${ARGN})

    # Check if the condition is met
    if(${condition})
        # Add each library as a dependency with the specified visibility
        foreach(lib IN LISTS libraries)
            target_link_libraries(${target_name} ${visibility} ${lib})
        endforeach()
    endif()
endfunction()
