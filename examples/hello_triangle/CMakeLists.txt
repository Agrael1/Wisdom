project(hello_triangle-${POSTFIX})

set(C_SOURCES entry_main.c)
set(CPP_SOURCES entry_main.cpp)

if(WISDOM_BUILD_STATIC)
  add_executable(${PROJECT_NAME}-c ${C_SOURCES})
  target_link_libraries(
    ${PROJECT_NAME}-c PUBLIC wis::wisdom wis::wisdom-platform SDL3::SDL3
                             example_backend_c-${POSTFIX})
  set_target_properties(
    ${PROJECT_NAME}-c PROPERTIES C_STANDARD 11 RUNTIME_OUTPUT_DIRECTORY
                                               ${EXAMPLE_BIN_OUTPUT})
  target_compile_definitions(${PROJECT_NAME}-c PUBLIC ${ADD_DEFINITIONS})
  add_dependencies(${PROJECT_NAME}-c copy_sdl wis_test_compile_shaders)

  add_executable(${PROJECT_NAME}-cpp ${CPP_SOURCES})
  target_link_libraries(
    ${PROJECT_NAME}-cpp PUBLIC wis::wisdom wis::wisdom-platform SDL3::SDL3
                               example_backend_cpp-${POSTFIX})
  set_target_properties(
    ${PROJECT_NAME}-cpp PROPERTIES CXX_STANDARD 20 RUNTIME_OUTPUT_DIRECTORY
                                                   ${EXAMPLE_BIN_OUTPUT})
  target_compile_definitions(${PROJECT_NAME}-cpp PUBLIC ${ADD_DEFINITIONS})
  add_dependencies(${PROJECT_NAME}-cpp copy_sdl wis_test_compile_shaders)
endif()

if(WISDOM_BUILD_SHARED)
  add_executable(${PROJECT_NAME}-c-shared ${C_SOURCES})
  target_link_libraries(
    ${PROJECT_NAME}-c-shared
    PUBLIC wis::wisdom-shared wis::wisdom-platform-shared SDL3::SDL3
           example_backend_c-${POSTFIX})
  set_target_properties(
    ${PROJECT_NAME}-c-shared PROPERTIES C_STANDARD 11 RUNTIME_OUTPUT_DIRECTORY
                                                      ${EXAMPLE_BIN_OUTPUT})
  target_compile_definitions(${PROJECT_NAME}-c-shared PUBLIC ${ADD_DEFINITIONS})
  add_dependencies(${PROJECT_NAME}-c-shared copy_dll copy_sdl
                   wis_test_compile_shaders)

  add_executable(${PROJECT_NAME}-cpp-shared ${CPP_SOURCES})
  target_link_libraries(
    ${PROJECT_NAME}-cpp-shared
    PUBLIC wis::wisdom-shared wis::wisdom-platform-shared SDL3::SDL3
           example_backend_cpp-${POSTFIX})
  set_target_properties(
    ${PROJECT_NAME}-cpp-shared
    PROPERTIES CXX_STANDARD 20 RUNTIME_OUTPUT_DIRECTORY ${EXAMPLE_BIN_OUTPUT})
  target_compile_definitions(${PROJECT_NAME}-cpp-shared
                             PUBLIC ${ADD_DEFINITIONS})
  add_dependencies(${PROJECT_NAME}-cpp-shared copy_dll copy_sdl
                   wis_test_compile_shaders)
endif()

add_executable(${PROJECT_NAME}-cpp-headers ${CPP_SOURCES})
target_link_libraries(
  ${PROJECT_NAME}-cpp-headers
  PUBLIC wis::wisdom-headers wis::wisdom-platform-headers SDL3::SDL3
         example_backend_cpp-${POSTFIX})
set_target_properties(
  ${PROJECT_NAME}-cpp-headers
  PROPERTIES CXX_STANDARD 23 RUNTIME_OUTPUT_DIRECTORY ${EXAMPLE_BIN_OUTPUT})
target_compile_definitions(${PROJECT_NAME}-cpp-headers
                           PUBLIC ${ADD_DEFINITIONS})
add_dependencies(${PROJECT_NAME}-cpp-headers copy_sdl wis_test_compile_shaders)

if(POSTFIX STREQUAL "dx12" AND WISDOM_BUILD_STATIC)
  wis_install_deps(${PROJECT_NAME}-c)
  wis_install_deps(${PROJECT_NAME}-cpp)
endif()
