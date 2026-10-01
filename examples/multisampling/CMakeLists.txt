project(multisampling-${POSTFIX})

if(WISDOM_EXAMPLES_BUILD_HEADERS)
  add_executable(${PROJECT_NAME}-cpp entry_main.cpp)
  target_link_libraries(${PROJECT_NAME}-cpp PRIVATE example_backend_cpp-${POSTFIX}-headers)
  wis_setup_example(${PROJECT_NAME}-cpp headers PATCH_EXE)
endif()
