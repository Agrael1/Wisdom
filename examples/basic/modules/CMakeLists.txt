project(modules-${POSTFIX})

set(HEADERS)
set(SOURCES entry_main.cpp window.h window.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

# include the modules in seperate, since we are building dx and vk
# implementation at the same time
target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC wisdom-module
         wisdom-debug-module
         wisdom-platform-module
         wisdom-descriptor-buffer-module
         wisdom-extended-allocation-module
         wisdom-raytracing-module
         SDL3::SDL3)
add_dependencies(${PROJECT_NAME} compile_shaders_basic)
set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES CXX_STANDARD 20
             CXX_STANDARD_REQUIRED ON
             RUNTIME_OUTPUT_DIRECTORY ${EXAMPLE_BIN_OUTPUT}
             CXX_SCAN_FOR_MODULES ON)

if(POSTFIX STREQUAL "dx12")
  wis_install_deps(${PROJECT_NAME}) # install the d3d12 agility sdk, for
  # examples this is enough for the main project, use
  # wis_installdeps(${PROJECT_NAME}) instead
else()
  target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_FORCE_VULKAN=1)
endif()

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL3::SDL3>
          ${EXAMPLE_BIN_OUTPUT}/)
