set(TEST_SOURCES "relaxed_destruction_order.cpp")

wis_add_test_suite("test-basic" ${TEST_SOURCES})

target_sources(test-basic-vk PRIVATE "platform_check.cpp")