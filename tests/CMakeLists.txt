include(Catch)
include(CTest)

function(wis_add_test TARGET SOURCES IMPL)
  set(TEST_TARGET ${TARGET}-${IMPL})
  add_executable(${TEST_TARGET} ${SOURCES})

  if(WISDOM_BUILD_STATIC) # Link against static library if built
    target_link_libraries(${TEST_TARGET} PUBLIC wis::wisdom wis::wisdom-platform wis::wisdom-raytracing
                                                Catch2::Catch2WithMain)
  else()
    target_link_libraries(${TEST_TARGET} PUBLIC wis::wisdom-headers wis::wisdom-platform-headers wis::wisdom-raytracing-headers
                                                Catch2::Catch2WithMain)
  endif()

  set_target_properties(
    ${TEST_TARGET} PROPERTIES CXX_STANDARD 20 RUNTIME_OUTPUT_DIRECTORY
                                              ${CMAKE_BINARY_DIR}/tests/bin)

  if(IMPL STREQUAL "vk")
    target_compile_definitions(${TEST_TARGET} PUBLIC WISDOM_FORCE_VULKAN=1)
  endif()

  catch_discover_tests(${TEST_TARGET} TEST_PREFIX "${IMPL}.${PROJECT_NAME}.")
endfunction(wis_add_test)

function(wis_add_test_suite NAME SOURCES)
  if(WISDOM_DX12)
    wis_add_test(${NAME} "${SOURCES}" "dx12")
  endif()

  if(WISDOM_VULKAN)
    wis_add_test(${NAME} "${SOURCES}" "vk")
  endif()
endfunction(wis_add_test_suite)

add_subdirectory(basic)
