project(example_backend-${POSTFIX})

set(BACKEND_C_TARGET example_backend_c-${POSTFIX})
set(BACKEND_CPP_TARGET example_backend_cpp-${POSTFIX})

add_library(${BACKEND_C_TARGET} STATIC sdl_backend_c.c)
target_include_directories(${BACKEND_C_TARGET}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
  ${BACKEND_C_TARGET}
  PUBLIC SDL3::SDL3
  PRIVATE wis::wisdom wis::wisdom-platform)
target_compile_definitions(${BACKEND_C_TARGET} PUBLIC ${ADD_DEFINITIONS})
set_target_properties(${BACKEND_C_TARGET} PROPERTIES C_STANDARD 11)

add_library(${BACKEND_CPP_TARGET} STATIC sdl_backend_cpp.cpp)
target_include_directories(${BACKEND_CPP_TARGET}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(
  ${BACKEND_CPP_TARGET}
  PUBLIC ${BACKEND_C_TARGET}
  PRIVATE wis::wisdom wis::wisdom-platform)
target_compile_definitions(${BACKEND_CPP_TARGET} PUBLIC ${ADD_DEFINITIONS})
set_target_properties(${BACKEND_CPP_TARGET} PROPERTIES CXX_STANDARD 20)
