project(lut-${POSTFIX})

set(HEADERS lut_loader.h)
set(SOURCES entry_main.cpp lut_loader.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC common-${POSTFIX})
add_dependencies(${PROJECT_NAME} compile_shaders_basic)
set_target_properties(${PROJECT_NAME} 
  PROPERTIES 
	CXX_STANDARD 20
	RUNTIME_OUTPUT_DIRECTORY ${EXAMPLE_BIN_OUTPUT}
)

if (POSTFIX STREQUAL "dx12")
	wis_make_exports_dx(${PROJECT_NAME}) # install the d3d12 agility sdk, for examples this is enough
	# for the main project, use wis_installdeps(${PROJECT_NAME}) instead
endif()

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/lut.cube
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/lut.cube)
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/image.png
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/image.png)

