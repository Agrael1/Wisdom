project(extended_allocation)

if(WISDOM_VULKAN)
  add_executable(${PROJECT_NAME}-vk test.cpp)
  wis_install_deps(${PROJECT_NAME}-vk)
  target_link_libraries(
    ${PROJECT_NAME}-vk PRIVATE wis::wisdom wis::platform wis::debug
                               wis::extended-allocation)
  target_compile_definitions(${PROJECT_NAME}-vk PRIVATE WISDOM_FORCE_VULKAN=1)
  set_target_properties(
    ${PROJECT_NAME}-vk
    PROPERTIES CXX_STANDARD 23
               CXX_STANDARD_REQUIRED YES
               CXX_EXTENSIONS NO)
endif()
if(WISDOM_DX12)
  add_executable(${PROJECT_NAME}-dx test.cpp)
  wis_install_deps(${PROJECT_NAME}-dx)
  target_link_libraries(
    ${PROJECT_NAME}-dx PRIVATE wis::wisdom wis::platform wis::debug
                               wis::extended-allocation)

  set_target_properties(
    ${PROJECT_NAME}-dx
    PROPERTIES CXX_STANDARD 23
               CXX_STANDARD_REQUIRED YES
               CXX_EXTENSIONS NO)

endif()
