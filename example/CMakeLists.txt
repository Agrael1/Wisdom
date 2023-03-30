# CMakeList.txt : CMake project for Wisdom, include source and define
# project specific logic here.
#
project(Example)

# SDK exports
get_property(DX12SDKVER TARGET DX12Agility PROPERTY DX12SDKVER)
configure_file(exports.cpp.in exports.cpp)

# Add source to this project's executable.
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/exports.cpp)
target_sources(${PROJECT_NAME}
	PRIVATE
		"src/entry_main.cpp"
	PUBLIC
)
target_link_libraries(
	${PROJECT_NAME}
		PUBLIC 
			Wisdom
)
set_target_properties(${PROJECT_NAME} 
    PROPERTIES
        CXX_STANDARD 23
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:DX12AgilityCore> $<TARGET_FILE_DIR:${PROJECT_NAME}>/D3D12/$<TARGET_FILE_NAME:DX12AgilityCore>
  COMMAND_EXPAND_LISTS
  COMMENT "Copying DX12 Agility Core..."
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:DX12AgilitySDKLayers> $<TARGET_FILE_DIR:${PROJECT_NAME}>/D3D12/$<TARGET_FILE_NAME:DX12AgilitySDKLayers>
	  COMMAND_EXPAND_LISTS
	  COMMENT "Copying DX12 Agility SDKLayers..."
	)
endif()


# TODO: Add tests and install targets.
