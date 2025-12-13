project(test-${POSTFIX})

set(HEADERS)
set(SOURCES entry_main.cpp)

if(WISDOM_BUILD_STATIC)
  add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
  target_link_libraries(${PROJECT_NAME} PUBLIC wis::wisdom)
  set_target_properties(
    ${PROJECT_NAME} PROPERTIES 
    CXX_STANDARD 11
    RUNTIME_OUTPUT_DIRECTORY ${EXAMPLE_BIN_OUTPUT}
  )
  
  target_compile_definitions(${PROJECT_NAME} PUBLIC
    ${ADD_DEFINITIONS}
  )
endif()


if(WISDOM_BUILD_SHARED)
  add_executable(${PROJECT_NAME}-shared ${HEADERS} ${SOURCES})
  target_link_libraries(${PROJECT_NAME}-shared PUBLIC wis::wisdom-shared)
  set_target_properties(
    ${PROJECT_NAME}-shared PROPERTIES CXX_STANDARD 11
    RUNTIME_OUTPUT_DIRECTORY ${EXAMPLE_BIN_OUTPUT}
  )
  
  target_compile_definitions(${PROJECT_NAME}-shared PUBLIC
    ${ADD_DEFINITIONS}
  )
  
  # Copy dlls to output directory on Windows
  add_dependencies(${PROJECT_NAME}-shared copy_dll)
endif()

add_executable(${PROJECT_NAME}-headers ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME}-headers PUBLIC wis::wisdom-headers)
set_target_properties(
  ${PROJECT_NAME}-headers PROPERTIES CXX_STANDARD 23
  RUNTIME_OUTPUT_DIRECTORY ${EXAMPLE_BIN_OUTPUT}
)

target_compile_definitions(${PROJECT_NAME}-headers PUBLIC
  ${ADD_DEFINITIONS}
)

#if(POSTFIX STREQUAL "dx12")
#  wis_make_exports_dx(${PROJECT_NAME}) # install the d3d12 agility sdk, for
#                                       # examples this is enough
#  # for the main project, use wis_installdeps(${PROJECT_NAME}) instead
#endif()