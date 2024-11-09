# Function for installing DirectX SDK for UWP
function(wis_install_dx_uwp PROJECT)
    #message("Installing DirectX Agility SDK Dependency")

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

	get_target_property(dxalayersdll DX12AgilitySDKLayers IMPORTED_LOCATION)
	message("DX12AgilitySDKLayers: ${dxalayersdll}")
	set_property(SOURCE ${dxalayersdll} PROPERTY VS_DEPLOYMENT_CONTENT 1)
	set_property(SOURCE ${dxalayersdll} PROPERTY VS_DEPLOYMENT_LOCATION "D3D12")
	target_sources(${PROJECT} PRIVATE ${dxalayersdll})
endfunction()

# Function for installing DirectX SDK
function(wis_install_dx_win32 PROJECT)
	#message("Installing DirectX Agility SDK Dependency")

	get_property(DX12SDKVER TARGET DX12Agility PROPERTY DX12SDKVER)

	set(EXPORT_AGILITY "_declspec(dllexport) extern const unsigned D3D12SDKVersion = ${DX12SDKVER};
						_declspec(dllexport) extern const char* D3D12SDKPath = \".\\\\D3D12\\\\\";"
	)

	file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/exports.c" "${EXPORT_AGILITY}")

	target_sources(${PROJECT} PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}/exports.c
	)

	add_custom_command(TARGET ${PROJECT} POST_BUILD
	  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:DX12AgilityCore> $<TARGET_FILE_DIR:${PROJECT}>/D3D12/$<TARGET_FILE_NAME:DX12AgilityCore>
	  COMMAND_EXPAND_LISTS
	  COMMENT "Copying DX12 Agility Core..."
	)


	add_custom_command(TARGET ${PROJECT} POST_BUILD
	  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:DX12AgilitySDKLayers> $<TARGET_FILE_DIR:${PROJECT}>/D3D12/$<TARGET_FILE_NAME:DX12AgilitySDKLayers>
	  COMMAND_EXPAND_LISTS
	  COMMENT "Copying DX12 Agility SDKLayers..."
	)
endfunction()

# Function for installing Wisdom Dependencies
function(wis_install_deps PROJECT)
    if(WISDOM_WINDOWS AND NOT WISDOM_WINDOWS_STORE)
		wis_install_dx_win32(${PROJECT})
	elseif(WISDOM_WINDOWS_STORE)
		wis_install_dx_uwp(${PROJECT})
	endif(WISDOM_WINDOWS AND NOT WISDOM_WINDOWS_STORE)
endfunction()


# Function for compiling shaders
# Arguments:
#	TARGET: Target to add the shader to
#	ENTRY: Entry point of the shader (default: main)
#	SHADER: Path to the shader file
#	OUTPUT: Path to the output (default: ${CMAKE_CURRENT_BINARY_DIR}/${SHADER}.cso)
#	TYPE: Type of shader (vs, ps, cs, ds, gs, hs, ms, as)
#	SHADER_MODEL: Shader model to compile to (default: 6_1)
#	INCLUDE_DIRS: List of include directories
#	DEFINITIONS: List of preprocessor definitions, e.g. "MY_DEFINE=1"
function(wis_compile_shader)
	set(options )
    set(oneValueArgs TARGET ENTRY SHADER OUTPUT TYPE SHADER_MODEL)
    set(multiValueArgs INCLUDE_DIRS DEFINITIONS)
    cmake_parse_arguments(wis_compile_shader "${options}" "${oneValueArgs}"
                          "${multiValueArgs}" ${ARGN} )

	if(NOT dxc_EXECUTABLE)
		message(FATAL_ERROR "wis_compile_shader: dxc not found")
	endif()

	if(NOT wis_compile_shader_TARGET)
		message(FATAL_ERROR "wis_compile_shader: TARGET not specified")
	endif()

	if(NOT wis_compile_shader_SHADER)
		message(FATAL_ERROR "wis_compile_shader: SHADER not specified")
	endif()

	if(NOT wis_compile_shader_TYPE)
		#try deducing from pattern .x.hlsl
		if(wis_compile_shader_SHADER MATCHES ".*\\.ps\\.hlsl$")
			set(wis_compile_shader_TYPE "ps")
		elseif(wis_compile_shader_SHADER MATCHES ".*\\.vs\\.hlsl$")
			set(wis_compile_shader_TYPE "vs")
		elseif(wis_compile_shader_SHADER MATCHES ".*\\.cs\\.hlsl$")
			set(wis_compile_shader_TYPE "cs")
		elseif(wis_compile_shader_SHADER MATCHES ".*\\.ds\\.hlsl$")
			set(wis_compile_shader_TYPE "ds")
		elseif(wis_compile_shader_SHADER MATCHES ".*\\.gs\\.hlsl$")
			set(wis_compile_shader_TYPE "gs")
		elseif(wis_compile_shader_SHADER MATCHES ".*\\.hs\\.hlsl$")
			set(wis_compile_shader_TYPE "hs")
		elseif(wis_compile_shader_SHADER MATCHES ".*\\.ms\\.hlsl$")
			set(wis_compile_shader_TYPE "ms")
		elseif(wis_compile_shader_SHADER MATCHES ".*\\.as\\.hlsl$")
			set(wis_compile_shader_TYPE "as")
		else()
			message(FATAL_ERROR "wis_compile_shader: TYPE not specified")
		endif()
	endif()

	if(wis_compile_shader_ENTRY)
		set(ENTRY wis_compile_shader_ENTRY)
	else()
		set(ENTRY "main")
	endif()

	if(wis_compile_shader_SHADER_MODEL)
		# parse shader model from pattern x.y to x_y
		string(REGEX REPLACE "\\." "_" SHADER_MODEL ${wis_compile_shader_SHADER_MODEL})
	else()
		set(SHADER_MODEL "6_1")
	endif()

	foreach(INCLUDE_DIR ${wis_compile_shader_INCLUDE_DIRS})
		list(APPEND INCLUDES "-I${INCLUDE_DIR} ")
	endforeach()

	foreach(DEFINITION ${wis_compile_shader_DEFINITIONS})
		list(APPEND DEFINES "-D${DEFINITION} ")
	endforeach()



	set(SHADER ${wis_compile_shader_SHADER})
	set(TARGET ${wis_compile_shader_TARGET})
	set(TYPE ${wis_compile_shader_TYPE})


	get_filename_component(FILE_WE ${SHADER} NAME_WLE)
	if(wis_compile_shader_OUTPUT)
		set(OUTPUT_DXIL ${wis_compile_shader_OUTPUT}.cso)
		set(OUTPUT_SPV ${wis_compile_shader_OUTPUT}.spv)
		set(OUTPUT_PDB ${wis_compile_shader_OUTPUT}.pdb)
	else()
		set(OUTPUT_DXIL ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.cso)
		set(OUTPUT_SPV ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.spv)
		set(OUTPUT_PDB ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.pdb)
	endif()

    if(WISDOM_WINDOWS_STORE)
        file(TOUCH ${OUTPUT_DXIL})
        target_sources(${TARGET} PRIVATE ${OUTPUT_DXIL})
        set_property(SOURCE ${OUTPUT_DXIL} PROPERTY VS_DEPLOYMENT_CONTENT 1)
    endif()

    if(WISDOM_WINDOWS)
        add_custom_command(TARGET ${TARGET}
            COMMAND "${dxc_EXECUTABLE}" -E${ENTRY} -T${TYPE}_${SHADER_MODEL} -Zi $<IF:$<CONFIG:DEBUG>,-Od,-O3> -Wno-ignored-attributes ${INCLUDES} ${DEFINES} -Fo${OUTPUT_DXIL} -Fd${OUTPUT_PDB} ${SHADER}
            MAIN_DEPENDENCY ${SHADER}
            COMMENT "HLSL ${SHADER}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
    endif()

    add_custom_command(TARGET ${TARGET}
        COMMAND "${dxc_EXECUTABLE}" -E${ENTRY} -T${TYPE}_${SHADER_MODEL} -Zi $<IF:$<CONFIG:DEBUG>,-Od,-O3> -spirv -Wno-ignored-attributes -fspv-target-env=vulkan1.3 ${INCLUDES} ${DEFINES} -Fo${OUTPUT_SPV} ${SHADER}
        MAIN_DEPENDENCY ${SHADER}
        COMMENT "SPV ${SHADER}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        VERBATIM)
endfunction()
