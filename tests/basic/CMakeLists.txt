project("test-basic")


set(TEST_SOURCES 
  "relaxed_destruction_order.cpp"
)

add_executable(${PROJECT_NAME}-dx12 ${TEST_SOURCES})
add_executable(${PROJECT_NAME}-vk ${TEST_SOURCES})

WIS_INSTALL_DEPS(${PROJECT_NAME}-dx12)
target_link_libraries(${PROJECT_NAME}-dx12 PUBLIC Catch2::Catch2WithMain wis::platform-headers)
WIS_INSTALL_DEPS(${PROJECT_NAME}-vk)
target_link_libraries(${PROJECT_NAME}-vk PUBLIC Catch2::Catch2WithMain wis::platform-headers)
target_compile_definitions(${PROJECT_NAME}-vk PRIVATE WISDOM_FORCE_VULKAN)


catch_discover_tests(${PROJECT_NAME}-vk
  TEST_PREFIX "vk.${PROJECT_NAME}."
)
catch_discover_tests(${PROJECT_NAME}-dx12
  TEST_PREFIX "dx12.${PROJECT_NAME}."
)