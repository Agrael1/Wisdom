project(test_c-${POSTFIX})

set(HEADERS)
set(SOURCES entry_main.c)

if(WISDOM_BUILD_STATIC)
  add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
  target_link_libraries(${PROJECT_NAME} PUBLIC wis::wisdom)
  set_target_properties(
    ${PROJECT_NAME} PROPERTIES C_STANDARD 11 RUNTIME_OUTPUT_DIRECTORY
                                             ${EXAMPLE_BIN_OUTPUT})

  target_compile_definitions(${PROJECT_NAME} PUBLIC ${ADD_DEFINITIONS})
  add_dependencies(${PROJECT_NAME} wis_test_compile_shaders)
endif()

if(WISDOM_BUILD_SHARED)
  add_executable(${PROJECT_NAME}-shared ${HEADERS} ${SOURCES})
  target_link_libraries(${PROJECT_NAME}-shared PUBLIC wis::wisdom-shared)
  set_target_properties(
    ${PROJECT_NAME}-shared PROPERTIES C_STANDARD 11 RUNTIME_OUTPUT_DIRECTORY
                                                    ${EXAMPLE_BIN_OUTPUT})

  target_compile_definitions(${PROJECT_NAME}-shared PUBLIC ${ADD_DEFINITIONS})

  # Copy dlls to output directory on Windows
  add_dependencies(${PROJECT_NAME}-shared copy_dll wis_test_compile_shaders)
endif()

if(POSTFIX STREQUAL "dx12")
    wis_install_deps(${PROJECT_NAME})
endif()
