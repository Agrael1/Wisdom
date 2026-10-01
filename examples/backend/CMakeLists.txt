project(example_backend-${POSTFIX})

foreach(MODE ${WISDOM_EXAMPLES_MODES})
  wis_example_libraries(${MODE})
  set(BACKEND_CPP_TARGET example_backend_cpp-${POSTFIX}-${MODE})

  if(MODE STREQUAL "headers")
    add_library(${BACKEND_CPP_TARGET} STATIC sdl_backend_headers.cpp)
  else()
    set(BACKEND_C_TARGET example_backend_c-${POSTFIX}-${MODE})
    add_library(${BACKEND_C_TARGET} STATIC sdl_backend_c.c)
    target_include_directories(${BACKEND_C_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${BACKEND_C_TARGET} PUBLIC SDL3::SDL3
      ${WISDOM_EXAMPLE_CORE} ${WISDOM_EXAMPLE_PLATFORM})
    target_compile_definitions(${BACKEND_C_TARGET} PUBLIC ${ADD_DEFINITIONS})
    set_target_properties(${BACKEND_C_TARGET} PROPERTIES C_STANDARD 11)

    add_library(${BACKEND_CPP_TARGET} STATIC sdl_backend_cpp.cpp)
    target_link_libraries(${BACKEND_CPP_TARGET} PUBLIC ${BACKEND_C_TARGET})
  endif()

  target_include_directories(${BACKEND_CPP_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(${BACKEND_CPP_TARGET} PUBLIC SDL3::SDL3
    ${WISDOM_EXAMPLE_CORE} ${WISDOM_EXAMPLE_PLATFORM})
  target_compile_definitions(${BACKEND_CPP_TARGET} PUBLIC ${ADD_DEFINITIONS})
  set_target_properties(${BACKEND_CPP_TARGET} PROPERTIES CXX_STANDARD 20)
endforeach()
