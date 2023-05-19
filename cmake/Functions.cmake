# Function for installing DirectX SDK
function(wis_install_dx PROJECT)
    message("Installing DirectX Agility SDK Dependency")

	get_property(DX12SDKVER TARGET DX12Agility PROPERTY DX12SDKVER)
	
	set(EXPORT_AGILITY "extern \"C\" { _declspec(dllexport) extern const unsigned D3D12SDKVersion = ${DX12SDKVER}; }
						extern \"C\" { _declspec(dllexport) extern const char* D3D12SDKPath = \".\\\\D3D12\\\\\"; }"
	)

	file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/exports.cpp" "${EXPORT_AGILITY}")

	target_sources(${PROJECT} PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/exports.cpp
	)
    
	add_custom_command(TARGET ${PROJECT} POST_BUILD
	  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:DX12AgilityCore> $<TARGET_FILE_DIR:${PROJECT}>/D3D12/$<TARGET_FILE_NAME:DX12AgilityCore>
	  COMMAND_EXPAND_LISTS
	  COMMENT "Copying DX12 Agility Core..."
	)
	
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		add_custom_command(TARGET ${PROJECT} POST_BUILD
		  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:DX12AgilitySDKLayers> $<TARGET_FILE_DIR:${PROJECT}>/D3D12/$<TARGET_FILE_NAME:DX12AgilitySDKLayers>
		  COMMAND_EXPAND_LISTS
		  COMMENT "Copying DX12 Agility SDKLayers..."
		)
	endif()
endfunction()