project(external_handles)

add_executable(${PROJECT_NAME} test.cpp)
target_link_libraries(
  ${PROJECT_NAME} PRIVATE wis::wisdom wis::platform wis::debug
                          wis::extended-allocation)
target_compile_definitions(${PROJECT_NAME} PRIVATE WISDOM_FORCE_VULKAN=1)

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES CXX_STANDARD 23
             CXX_STANDARD_REQUIRED YES
             CXX_EXTENSIONS NO)
