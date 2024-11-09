project(common-${POSTFIX})

find_package(glm QUIET)

set(HEADERS "wis_helper.h" "window.h" "keyboard.h" "mouse.h" "window_event.h" "wis_swapchain.h")
set(SOURCES "wis_helper.cpp" "window.cpp" "keyboard.cpp" "mouse.cpp")

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
if(POSTFIX STREQUAL "vk")
  target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_FORCE_VULKAN=1)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC wis::debug wis::wisdom wis::platform SDL3::SDL3 glm::glm)
target_include_directories(${PROJECT_NAME} PUBLIC ${fpng_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

# copy SDL3.dll to the output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  $<TARGET_FILE:SDL3::SDL3>
  ${EXAMPLE_BIN_OUTPUT}/)

if (POSTFIX STREQUAL "dx12")
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:DX12AgilityCore> ${EXAMPLE_BIN_OUTPUT}/D3D12/$<TARGET_FILE_NAME:DX12AgilityCore>
    COMMAND_EXPAND_LISTS
    COMMENT "Copying DX12 Agility Core..."
  )
  
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:DX12AgilitySDKLayers> ${EXAMPLE_BIN_OUTPUT}/D3D12/$<TARGET_FILE_NAME:DX12AgilitySDKLayers>
    COMMAND_EXPAND_LISTS
    COMMENT "Copying DX12 Agility SDKLayers..."
  )
endif()