project(compute_particles_c-${POSTFIX})

set(HEADERS)
set(SOURCES entry_main.c)

if(WISDOM_BUILD_STATIC)
  add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
  target_link_libraries(
    ${PROJECT_NAME} PUBLIC wis::wisdom wis::wisdom-platform SDL3::SDL3
                           example_backend_c-${POSTFIX})
  set_target_properties(
    ${PROJECT_NAME} PROPERTIES C_STANDARD 11 RUNTIME_OUTPUT_DIRECTORY
                                             ${EXAMPLE_BIN_OUTPUT})

  target_compile_definitions(${PROJECT_NAME} PUBLIC ${ADD_DEFINITIONS})
  add_dependencies(${PROJECT_NAME} copy_sdl wis_test_compile_shaders)
endif()

if(WISDOM_BUILD_SHARED)
  add_executable(${PROJECT_NAME}-shared ${HEADERS} ${SOURCES})
  target_link_libraries(
    ${PROJECT_NAME}-shared
    PUBLIC wis::wisdom-shared wis::wisdom-platform-shared SDL3::SDL3
           example_backend_c-${POSTFIX})
  set_target_properties(
    ${PROJECT_NAME}-shared PROPERTIES C_STANDARD 11 RUNTIME_OUTPUT_DIRECTORY
                                                    ${EXAMPLE_BIN_OUTPUT})

  target_compile_definitions(${PROJECT_NAME}-shared PUBLIC ${ADD_DEFINITIONS})

  # Copy dlls to output directory on Windows
  add_dependencies(${PROJECT_NAME}-shared copy_dll copy_sdl
                   wis_test_compile_shaders)
endif()

if(POSTFIX STREQUAL "dx12")
  wis_install_deps(${PROJECT_NAME})
endif()
