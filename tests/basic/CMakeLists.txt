set(TEST_SOURCES "relaxed_destruction_order.cpp" "rt_basic.cpp" "rt_primitives.cpp" "video.cpp")

if(WISDOM_DX12)
  wis_add_test(test-basic "${TEST_SOURCES}" "dx12")
endif()

if(WISDOM_VULKAN)
  wis_add_test(test-basic "${TEST_SOURCES}" "vk")
endif()

target_sources(test-basic-vk PRIVATE "platform_check.cpp")