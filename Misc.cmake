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

