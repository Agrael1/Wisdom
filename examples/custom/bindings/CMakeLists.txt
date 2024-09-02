project(bindings-test LANGUAGES C)

add_executable(bindings-test entry_main.c)
target_link_libraries(bindings-test PRIVATE wisdom-bindings)

set_target_properties(
  bindings-test
  PROPERTIES C_STANDARD 17
             C_STANDARD_REQUIRED YES
             C_EXTENSIONS NO)

# copy the shared library to the build directory
add_custom_command(
  TARGET bindings-test
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:wisdom-bindings>
          $<TARGET_FILE_DIR:bindings-test>)
