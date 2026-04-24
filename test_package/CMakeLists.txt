cmake_minimum_required(VERSION 3.22)

project(TestApp)
find_package(Wisdom REQUIRED)

add_executable(test_app main.cpp)
if(WISDOM_IS_SHARED)
    target_link_libraries(test_app PRIVATE wis::wisdom-shared)
else()
    target_link_libraries(test_app PRIVATE wis::wisdom)
endif()
