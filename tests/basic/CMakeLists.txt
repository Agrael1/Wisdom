project("test-basic")

set(TEST_SOURCES "relaxed_destruction_order.cpp" "extension_tests.cpp"
                 "copy_tests.cpp")

if(WISDOM_DX12)
  add_executable(${PROJECT_NAME}-dx12 ${TEST_SOURCES})
  wis_install_deps(${PROJECT_NAME}-dx12)
  target_link_libraries(
    ${PROJECT_NAME}-dx12
    PUBLIC Catch2::Catch2WithMain wis::platform-headers wis::debug-headers
           wis::extended-allocation-headers)
  catch_discover_tests(${PROJECT_NAME}-dx12 TEST_PREFIX "dx12.${PROJECT_NAME}.")
endif()

if(WISDOM_VULKAN)
  add_executable(${PROJECT_NAME}-vk ${TEST_SOURCES} "vulkan_tests.cpp")
  wis_install_deps(${PROJECT_NAME}-vk)
  target_link_libraries(
    ${PROJECT_NAME}-vk
    PUBLIC Catch2::Catch2WithMain wis::platform-headers wis::debug-headers
           wis::extended-allocation-headers)
  target_compile_definitions(${PROJECT_NAME}-vk PRIVATE WISDOM_FORCE_VULKAN)
  catch_discover_tests(${PROJECT_NAME}-vk TEST_PREFIX "vk.${PROJECT_NAME}.")
endif()
