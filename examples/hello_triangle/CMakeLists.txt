project(hello_triangle-${POSTFIX})

foreach(MODE ${WISDOM_EXAMPLES_MODES})
  if(MODE STREQUAL "static")
    set(SUFFIX "")
  else()
    set(SUFFIX -${MODE})
  endif()

  if(NOT MODE STREQUAL "headers")
    set(C_TARGET ${PROJECT_NAME}-c${SUFFIX})
    add_executable(${C_TARGET} entry_main.c)
    target_link_libraries(${C_TARGET}
                          PRIVATE example_backend_c-${POSTFIX}-${MODE})
    wis_setup_example(${C_TARGET} ${MODE})
  endif()

  set(CPP_TARGET ${PROJECT_NAME}-cpp${SUFFIX})
  add_executable(${CPP_TARGET} entry_main.cpp)
  target_link_libraries(${CPP_TARGET}
                        PRIVATE example_backend_cpp-${POSTFIX}-${MODE})
  wis_setup_example(${CPP_TARGET} ${MODE})
endforeach()
