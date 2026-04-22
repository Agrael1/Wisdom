project(multisampling-${POSTFIX})

set(C_SOURCES entry_main.c)
set(CPP_SOURCES entry_main.cpp)

add_executable(${PROJECT_NAME}-cpp ${CPP_SOURCES})
target_link_libraries(
  ${PROJECT_NAME}-cpp PUBLIC wis::wisdom-headers wis::wisdom-platform-headers
                             SDL3::SDL3 example_backend_cpp-${POSTFIX})
set_target_properties(
  ${PROJECT_NAME}-cpp PROPERTIES CXX_STANDARD 23 RUNTIME_OUTPUT_DIRECTORY
                                                 ${EXAMPLE_BIN_OUTPUT})
target_compile_definitions(${PROJECT_NAME}-cpp PUBLIC ${ADD_DEFINITIONS})
add_dependencies(${PROJECT_NAME}-cpp copy_sdl wis_test_compile_shaders)

if(POSTFIX STREQUAL "dx12" AND WISDOM_USE_AGILITY_SDK)
  wis_install_agility_win32(TARGET ${PROJECT_NAME}-cpp
    PATCH_EXE ON)
endif()
