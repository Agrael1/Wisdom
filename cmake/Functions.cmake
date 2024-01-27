# Function for installing DirectX SDK for UWP
function(wis_install_dx_uwp PROJECT)
    message("Installing DirectX Agility SDK Dependency")

	get_property(DX12SDKVER TARGET DX12Agility PROPERTY DX12SDKVER)
	
	set(EXPORT_AGILITY "extern \"C\" { _declspec(dllexport) extern const unsigned D3D12SDKVersion = ${DX12SDKVER}; }
						extern \"C\" { _declspec(dllexport) extern const char* D3D12SDKPath = \".\\\\D3D12\\\\\"; }"
	)

	file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/exports.cpp" "${EXPORT_AGILITY}")

	target_sources(${PROJECT} PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/exports.cpp
	)

	get_target_property(dxadll DX12AgilityCore IMPORTED_LOCATION)
	message("DX12AgilityCore: ${dxadll}")
	set_property(SOURCE ${dxadll} PROPERTY VS_DEPLOYMENT_CONTENT 1)
	set_property(SOURCE ${dxadll} PROPERTY VS_DEPLOYMENT_LOCATION "D3D12")
	target_sources(${PROJECT} PRIVATE ${dxadll})	

	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		get_target_property(dxalayersdll DX12AgilitySDKLayers IMPORTED_LOCATION)
		message("DX12AgilitySDKLayers: ${dxalayersdll}")
		set_property(SOURCE ${dxalayersdll} PROPERTY VS_DEPLOYMENT_CONTENT 1)
		set_property(SOURCE ${dxalayersdll} PROPERTY VS_DEPLOYMENT_LOCATION "D3D12")
		target_sources(${PROJECT} PRIVATE ${dxalayersdll})	
	endif()
endfunction()

# Function for installing DirectX SDK
function(wis_install_dx_win32 PROJECT)
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

# Function for installing Wisdom Dependencies
function(wis_install_deps PROJECT)
    if(WISDOM_WINDOWS AND NOT WISDOM_WINDOWS_STORE)
		wis_install_dx_win32(${PROJECT})
	elseif(WISDOM_WINDOWS_STORE)
		wis_install_dx_uwp(${PROJECT})
	endif(WISDOM_WINDOWS AND NOT WISDOM_WINDOWS_STORE)
endfunction()