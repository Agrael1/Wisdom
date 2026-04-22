cmake_minimum_required(VERSION 3.20)
project(WisdomZipIntegrationTest)

# This is what a standard user does after extracting your ZIP
find_package(wisdom REQUIRED)

add_executable(TestAppShared entry_main.cpp)
add_executable(TestApp entry_main.cpp)
add_executable(TestAppHeaders entry_main.cpp)

# Test dynamic linkage to ensure the DLLs load correctly
target_link_libraries(TestAppShared PRIVATE wis::wisdom-shared)
target_link_libraries(TestApp PRIVATE wis::wisdom)
target_link_libraries(TestAppHeaders PRIVATE wis::wisdom-headers)

set_target_properties(TestAppHeaders PROPERTIES CXX_STANDARD 20)