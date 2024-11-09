project(multiview-${POSTFIX})

set(HEADERS )
set(SOURCES entry_main.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC common-${POSTFIX})
add_dependencies(${PROJECT_NAME} compile_shaders_basic)
set_target_properties(${PROJECT_NAME} 
  PROPERTIES 
	CXX_STANDARD 20
	RUNTIME_OUTPUT_DIRECTORY ${EXAMPLE_BIN_OUTPUT}
)

if (POSTFIX STREQUAL "dx12")
	wis_install_deps(${PROJECT_NAME}) # install the d3d12 agility sdk
endif()

